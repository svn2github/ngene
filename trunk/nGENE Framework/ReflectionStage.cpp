/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ReflectionStage.cpp
Version:	0.09
---------------------------------------------------------------------------
*/

#include "ReflectionStage.h"

#include "Camera.h"
#include "Frustum.h"
#include "ITexture.h"
#include "LightStage.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "Matrix4x4.h"
#include "ScenePartitioner.h"
#include "ScreenOverlay.h"
#include "Shader.h"
#include "ShaderInstance.h"
#include "ShaderConstant.h"
#include "TextureManager.h"
#include "TextureSampler.h"


namespace nGENE
{
namespace Nature
{
	// Initialize static members
	int ReflectionStage::s_nRequestsNum = 0;


	ReflectionStage::ReflectionStage(uint _textureSize):
		m_nReflectionMapSize(_textureSize),
		m_pReflectionMaterial(NULL),
		m_pScreenQuad(NULL),
		m_fReflectionDistance(1000.0f),
		m_Plane(0.0f, 1.0f, 0.0f, 0.0f),
		m_pReflectionMap(NULL),
		m_pBackBuffer(NULL),
		m_bIsReady(false)
	{
	}
//----------------------------------------------------------------------
	ReflectionStage::~ReflectionStage()
	{
	}
//----------------------------------------------------------------------
	void ReflectionStage::init()
	{
		m_pScreenQuad = new ScreenOverlay(0.0f,
										  0.0f,
										  m_pRenderer->getDisplayMode().width,
										  m_pRenderer->getDisplayMode().height);
	}
//----------------------------------------------------------------------
	void ReflectionStage::prepare()
	{
		m_pReflectionMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"water");

		// Create reflection map
		float width = m_pRenderer->getDisplayMode().width;
		float height = m_pRenderer->getDisplayMode().height;

		m_pReflectionMap = TextureManager::getSingleton().createTexture(L"reflectionMap",
																		width,
																		height,
																		TT_2D,
																		TU_RENDERTARGET,
																		TF_R8G8B8);

		if(m_pReflectionMaterial)
		{
			TextureSampler sampler;
			sampler.setTexture(m_pReflectionMap);
			sampler.setTextureUnit(5);
			sampler.setAddressingMode(TAM_CLAMP);
			m_pReflectionMaterial->getActiveRenderTechnique()->getRenderPass(0).addTextureSampler(sampler);
		}

		m_bIsReady = true;
	}
//----------------------------------------------------------------------
	void ReflectionStage::render()
	{
		// Reflections are not requested
		if(s_nRequestsNum <= 0)
			return;
		else if(!m_bIsReady)
			prepare();

		Engine& engine = Engine::getSingleton();
		Camera* pActiveCamera = engine.getActiveCamera();
		if(pActiveCamera)
		{
			Vector3 oldPos = pActiveCamera->getPositionLocal();

			Matrix4x4 matViewOld = m_pRenderer->getViewMatrix();
			Matrix4x4 matProjOld = m_pRenderer->getProjectionMatrix();

			// Calculate new view and projection matrices
			Vector3 target = oldPos + pActiveCamera->getForward();
			Vector3 pos = oldPos.reflect(m_Plane);


			Vector3 forward = target.reflect(m_Plane) - pos;
			forward.normalize();
			Vector3 up = forward * pActiveCamera->getRight();

			Frustum frustum(pActiveCamera->getFrustum());
			frustum.setVectors(pos, forward, pActiveCamera->getRight(), up);
			frustum.setFar(m_fReflectionDistance);
			frustum.update();
			Matrix4x4 matProj = frustum.getProjectionMatrix();
			m_pRenderer->setProjectionMatrix(matProj);

			Matrix4x4 matView;
			matView.lookAt(pos, pos + forward, up);			
			m_pRenderer->setViewMatrix(matView);

			// Setup a clipping plane
			Matrix4x4 matViewProj = matView * matProj;
			matViewProj.invert();
			matViewProj.transpose();

			// Create and transform reflection plane to clipping space
			Plane plane = m_Plane;
			plane.transform(matViewProj);
			m_pRenderer->setUserClipPlane(0, plane);

			// We change back buffer
			TextureManager& texMgr = TextureManager::getSingleton();
			m_pBackBuffer = texMgr.getTexture(L"BackBuffer");
			texMgr.setTexture(L"BackBuffer", m_pReflectionMap);
			m_pRenderer->setBackBuffer(m_pReflectionMap);
			m_pRenderer->clearBuffer(CB_DEPTH | CB_TARGET);
			m_pRenderer->updateBackBuffer();

			// Enable clipping plane
			m_pRenderer->setClippingPlanesFlag(PLANE_0_ENABLED);
			
			uint managersCount = engine.getSceneManagersNum();
			SceneManager* pSM = NULL;
			ScenePartitioner* pPartitioner = NULL;
			for(uint j = 0; j < managersCount; ++j)
			{
				pSM = engine.getSceneManager(j);
				pPartitioner = pSM->getScenePartitioner();
				if(pPartitioner)
					pPartitioner->setFrustum(&frustum);
				pSM->testVisibility();
			}

			/// Disable adaptation update if HDR is enabled
			bool bHDR = ((LightStage*)Renderer::getSingleton().getRenderStage(L"Light"))->isUsingHDR();
			Material* pHDRMat = MaterialManager::getSingleton().getLibrary(L"core")->getMaterial(L"HDR");
			RenderTechnique* pTechnique = pHDRMat->getActiveRenderTechnique();
			if(bHDR)
			{
				for(uint i = 1; i <= 7; ++i)
					pTechnique->getRenderPass(i).setEnabled(false);
			}


			// Render all stages
			for(uint i = 0; i < m_vEmbeddedStages.size() ; ++i)
				m_vEmbeddedStages[i]->render();

			/// Restore HDR settings
			if(bHDR)
			{
				for(uint i = 1; i <= 7; ++i)
					pTechnique->getRenderPass(i).setEnabled(true);
			}
			
			// Retrieve old view and projection matrices
			m_pRenderer->setViewMatrix(matViewOld);
			m_pRenderer->setProjectionMatrix(matProjOld);

			// Disable clipping plane
			m_pRenderer->setClippingPlanesFlag(PLANE_NONE);

			// Restore original back buffer
			m_pRenderer->updateBackBuffer();
			texMgr.setTexture(L"BackBuffer", m_pBackBuffer);
			m_pRenderer->setBackBuffer(m_pBackBuffer);
			m_pRenderer->clearBuffer(CB_DEPTH | CB_TARGET);
			m_pRenderer->updateBackBuffer();

			for(uint j = 0; j < managersCount; ++j)
			{
				pSM = engine.getSceneManager(j);
				pPartitioner = pSM->getScenePartitioner();
				if(pPartitioner)
					pPartitioner->setFrustum(pActiveCamera->getFrustumPtr());
			}

			pActiveCamera->getFrustum().reset();
			pActiveCamera->update();
		}
	}
//----------------------------------------------------------------------
}
}