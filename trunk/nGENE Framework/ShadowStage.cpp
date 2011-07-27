/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShadowStage.cpp
Version:	0.15
---------------------------------------------------------------------------
*/

#include "ShadowStage.h"
#include "Camera.h"
#include "Frustum.h"
#include "ITexture.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "NodeLight.h"
#include "NodeVisible.h"
#include "RenderPass.h"
#include "ScenePartitioner.h"
#include "ScreenOverlay.h"
#include "Shader.h"
#include "ShaderInstance.h"
#include "ShaderConstant.h"
#include "Surface.h"
#include "TextureManager.h"


namespace nGENE
{
namespace Nature
{
	ShadowStage::ShadowStage():
		m_nShadowMapPrevID(0),
		m_pShadowMapMaterial(NULL),
		m_pShadowMaterial(NULL),
		m_pScreenQuad(NULL),
		m_pDepthThreshold(NULL),
		m_pShadowMatrix(NULL),
		m_pTexelSize(NULL),
		m_fRange(20.0f),
		m_fDistance(50.0f)
	{
	}
//----------------------------------------------------------------------
	ShadowStage::~ShadowStage()
	{
	}
//----------------------------------------------------------------------
	void ShadowStage::init()
	{
		m_pShadowMapMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"shadow_map");

		m_pScreenQuad = new ScreenOverlay(0.0f,
										  0.0f,
										  m_pRenderer->getDisplayMode().width,
										  m_pRenderer->getDisplayMode().height);

		float fTexOffs = 0.5f + (0.5f / 512.0f);
		m_matTexAdj.set(0.5f,	  0.0f,		0.0f, 0.0f,
						0.0f,	  -0.5f,	0.0f, 0.0f,
						0.0f,	  0.0f,		1.0f, 0.0f,
						fTexOffs, fTexOffs, 0.0f, 1.0f);
	}
//----------------------------------------------------------------------
	void ShadowStage::render()
	{
		m_matViewOld = m_pRenderer->getViewMatrix();

		vector <NodeLight*>* pLights = m_pRenderer->getLights();
		vector <bool> isLightCulled;
		for(uint i = 0; i < pLights->size(); ++i)
			isLightCulled.push_back((*pLights)[i]->isCulled());

		for(uint i = 0; i < pLights->size(); ++i)
		{
			NodeLight* pLight = (*pLights)[i];
			if(!pLight->isCastingShadows() || !pLight->isEnabled() || isLightCulled[i])
				continue;

			m_pRenderer->setActiveLight(pLight);

			if(pLight->getLightType() == LT_DIRECTIONAL)
			{
				Vector3 eyePos;
				Camera* pActiveCamera = Engine::getSingleton().getActiveCamera();
				if(pActiveCamera)
					eyePos = pActiveCamera->getPositionLocal();
				Vector3 dir = pLight->getDirection();
				Vector3 pos = eyePos;

				Frustum frustum = pActiveCamera->getFrustum();
				frustum.setFar(m_fDistance);
				frustum.update();

				Vector4 pointsWorld[8];
				frustum.getFrustumPoints(pointsWorld);

				Vector3 up = Vector3::UNIT_Y;
				Vector3 right = dir * up;
				up = right * dir;

				Vector4 vecCentroid;
				for(uint i = 0; i < 8; ++i)
				{
					vecCentroid += pointsWorld[i];
				}
				vecCentroid.divideByNumber(8);
				Vector3 target(vecCentroid.x, vecCentroid.y, vecCentroid.z);
				pos = target - pLight->getDirection() * m_fDistance + m_vecShadowOffset;

				Matrix4x4 mat;
				mat.lookAt(pos, target, up);
				m_pRenderer->setViewMatrix(mat);

				renderShadow(pos, dir, right, up, pLight);
			}
			else if(pLight->getLightType() == LT_SPOT)
			{
				Vector3 pos = pLight->getPositionLocal() + m_vecShadowOffset;
				Vector3 dir = pLight->getDirection();
				Vector3 up = Vector3::UNIT_Y;
				Vector3 right = dir * up;
				up = right * dir;

				Matrix4x4 mat;
				mat.lookAt(pos, pos + dir, up);
				m_pRenderer->setViewMatrix(mat);

				renderShadow(pos, dir, right, up, pLight);
			}
			else if(pLight->getLightType() == LT_POINT)
			{
				Matrix4x4 mat;
				
				Vector3 pos = pLight->getPositionLocal() + m_vecShadowOffset;
				Vector3 forward = Vector3::UNIT_X;
				Vector3 right = Vector3::NEGATIVE_UNIT_Z;
				Vector3 up = Vector3::UNIT_Y;
				mat.lookAt(pos, pos + forward, up);
				m_pRenderer->setViewMatrix(mat);
				renderShadow(pos, forward, right, up, pLight, 0);

				forward = Vector3::NEGATIVE_UNIT_X;
				right = Vector3::UNIT_Z;
				up = Vector3::UNIT_Y;
				mat.lookAt(pos, pos + forward, up);
				m_pRenderer->setViewMatrix(mat);
				renderShadow(pos, forward, right, up, pLight, 1);

				forward = Vector3::UNIT_Y;
				right = Vector3::UNIT_X;
				up = Vector3::NEGATIVE_UNIT_Z;
				mat.lookAt(pos, pos + forward, up);
				m_pRenderer->setViewMatrix(mat);
				renderShadow(pos, forward, right, up, pLight, 2);

				forward = Vector3::NEGATIVE_UNIT_Y;
				right = Vector3::UNIT_X;
				up = Vector3::UNIT_Z;
				mat.lookAt(pos, pos + forward, up);
				m_pRenderer->setViewMatrix(mat);
				renderShadow(pos, forward, right, up, pLight, 3);

				forward = Vector3::UNIT_Z;
				right = Vector3::UNIT_X;
				up = Vector3::UNIT_Y;
				mat.lookAt(pos, pos + forward, up);
				m_pRenderer->setViewMatrix(mat);
				renderShadow(pos, forward, right, up, pLight, 4);

				forward = Vector3::NEGATIVE_UNIT_Z;
				right = Vector3::UNIT_X;
				up = Vector3::UNIT_Y;
				mat.lookAt(pos, pos + forward, up);
				m_pRenderer->setViewMatrix(mat);
				renderShadow(pos, forward, right, up, pLight, 5);
			}


			// Skip static shadows if update wasn't requested
			if(pLight->hasStaticShadow() && pLight->hasRequestedShadowUpdate())
				pLight->requestShadowUpdate(false);
		}
	}
//----------------------------------------------------------------------
	void ShadowStage::renderShadowMap()
	{
		m_pShadowMapMaterial->bind();

		Engine& engine = Engine::getSingleton();

		RenderQueue tempQueue;
		HashTable <Material*, RenderQueue> customQueues;

		uint count = engine.getSceneManagersNum();
		for(uint i = 0; i < count; ++i)
		{
			HashTable <wstring, Node*>& nodes = engine.getSceneManager(i)->getNodes();
			HashTable <wstring, Node*>::iterator iter;
			for(iter = nodes.begin(); iter != nodes.end(); ++iter)
			{
				NodeVisible* pNode = dynamic_cast<NodeVisible*>(iter->second);
				if(pNode)
				{
					if(pNode->isEnabled())
					{
						for(uint i = 0; i < pNode->getSurfacesNum(); ++i)
						{
							Surface* pSurf = pNode->getSurface(i);
							if(!pSurf->isCulled() && pSurf->isEnabled())
							{
								Material* pMat = &pSurf->getMaterial();
								if(pMat)
								{
									if(pMat->isCastingShadow() && pMat->isEnabled())
									{
										// Is using custom shadowing scheme?
										if(pMat->hasRenderTechnique(L"shadow"))
										{
											if(customQueues.find(pMat) == customQueues.end())
											{
												RenderQueue temp;
												temp.push_back(pSurf);
												customQueues[pMat] = temp;
											}
											else
												customQueues[pMat].push_back(pSurf);
										}
										else
											tempQueue.push_back(pSurf);
									}
								}
							}
						}
					}
				}
			}
		}

		// Process regular material
		m_pShadowMapMaterial->render(&tempQueue);

		// Process custom materials
		HashTable <Material*, RenderQueue>::iterator iter;
		for(iter = customQueues.begin(); iter != customQueues.end(); ++iter)
		{
			Material* pMaterial = iter->first;
			RenderTechnique* pTechnique = pMaterial->getActiveRenderTechnique();

			pMaterial->setActiveRenderTechnique(pMaterial->getRenderTechnique(L"shadow"));
			pMaterial->getActiveRenderTechnique()->getRenderPass(0).getRenderTarget(0).setTexture(m_pShadowMapMaterial->getActiveRenderTechnique()->getRenderPass(0).getRenderTarget(0).getTexture());
			pMaterial->render(&iter->second);

			// Restore previous technique
			pMaterial->setActiveRenderTechnique(pTechnique);
		}

		m_pShadowMapMaterial->unbind();
	}
//----------------------------------------------------------------------
	void ShadowStage::renderShadow(Vector3& _pos, Vector3& _forward,
		Vector3& _right, Vector3& _up, NodeLight* _light, uint _counter)
	{
		Engine& engine = Engine::getSingleton();

		Matrix4x4 matProjOld = m_pRenderer->getProjectionMatrix();
		Matrix4x4 matProj;

		// Shadow map has to be created
		uint res = _light->getShadowResolution();
		uint id = res;
		if(_light->hasIndividualShadow())
			id = (uint)reinterpret_cast<int>((void*)_light) + res + _counter;
		if(m_pShadowMaps.find(id) == m_pShadowMaps.end())
		{
			TEXTURE_DESC desc;
			desc.depth = 1;
			desc.format = TF_R32F;
			desc.width = res;
			desc.height = res;
			desc.type = TT_2D;
			desc.usage = TU_RENDERTARGET;

			wstringstream name;
			name << "shadowDepth" << id;

			m_pShadowMaps[id] = TextureManager::getSingleton().createTexture(name.str(), desc);
		}

		// Avoid switching shadow maps all the time
		if(!m_nShadowMapPrevID || m_nShadowMapPrevID != id)
		{
			m_pShadowMapMaterial->getActiveRenderTechnique()->getRenderPass(0).getRenderTarget(0).setTexture(m_pShadowMaps[id]);
			m_pShadowMaterial->getActiveRenderTechnique()->getRenderPass(0).getTextureSampler(0).setTexture(m_pShadowMaps[id]);

			m_nShadowMapPrevID = id;

			float fTexOffs = 0.5f + (0.5f / (float)res);
			m_matTexAdj.set(0.5f,	  0.0f,		0.0f, 0.0f,
							0.0f,	  -0.5f,	0.0f, 0.0f,
							0.0f,	  0.0f,		1.0f, 0.0f,
							fTexOffs, fTexOffs, 0.0f, 1.0f);

			if(m_pTexelSize)
			{
				float fSize = fTexOffs - 0.5f;
				m_pTexelSize->setDefaultValue(&fSize);
			}
		}

		bool bUpdate = (_light->hasStaticShadow() && _light->hasRequestedShadowUpdate()) || !_light->hasStaticShadow();

		// Update all scene managers
		uint count = engine.getSceneManagersNum();
		for(uint i = 0; i < count; ++i)
		{
			Frustum frustum;
			ScenePartitioner* pPartitioner = engine.getSceneManager(i)->getScenePartitioner();

			frustum.setPerspective(Maths::PI * 0.5f,
								   1.0f,
								   _light->getShadowBias(),
								   _light->getRadius().x);
			frustum.setVectors(_pos, _forward, _right, _up);
			frustum.update();

			if(_light->getLightType() == LT_DIRECTIONAL)
			{
				float doubleRange = 2.0f * m_fRange;
				matProj.orthogonal(doubleRange,
								   doubleRange,
								   _light->getShadowBias(),
								   _light->getRadius().x);
				m_pRenderer->setProjectionMatrix(matProj);
			}
			else
			{
				matProj = frustum.getProjectionMatrix();
				m_pRenderer->setProjectionMatrix(matProj);
			}

			if(pPartitioner)
			{
				pPartitioner->setFrustum(&frustum);
				if(bUpdate)
					engine.getSceneManager(i)->testVisibility();
				pPartitioner->setFrustum(engine.getActiveCamera()->getFrustumPtr());
			}
		}


		// Render
		Matrix4x4 matView = m_pRenderer->getViewMatrix();

		
		if(bUpdate)
		{
			renderShadowMap();
		}
		
		m_pRenderer->setProjectionMatrix(matProjOld);


		Matrix4x4 matLightViewProj = matView * matProj;

		Matrix4x4 viewInv = m_matViewOld;
		viewInv.invert();

		Matrix4x4 matTexture = (viewInv * matLightViewProj) * m_matTexAdj;

		m_pRenderer->setViewMatrix(m_matViewOld);

		// Set shader constants
		m_pShadowMatrix->setDefaultValue(&matTexture[0][0]);
		if(m_pDepthThreshold)
		{
			float fFar = engine.getActiveCamera()->getFrustum().getFar();
			m_pDepthThreshold->setDefaultValue(&fFar);
		}


		m_pScreenQuad->setMaterial(m_pShadowMaterial);
		m_pScreenQuad->begin();

			m_pRenderer->updateBackBuffer();
			m_pScreenQuad->render();

		m_pScreenQuad->end();

		m_pRenderer->setViewMatrix(m_matViewOld);
	}
//----------------------------------------------------------------------
	void ShadowStage::setShadowMaterial(Material* _material)
	{
		m_pShadowMaterial = _material;

		RenderPass& pass = m_pShadowMaterial->getActiveRenderTechnique()->getRenderPass(0);

		m_pShadowMatrix = pass.getPixelShader()->getShader()->getConstant("matTexture");
		if(!m_pShadowMatrix)
		{
			Log::log(LET_ERROR, L"nGENE::Nature", __WFILE__, __WFUNCTION__, __LINE__,
				L"Missing shadow mapping shader constant: matTexture");
			return;
		}

		m_pDepthThreshold = pass.getPixelShader()->getShader()->getConstant("depthThresh");
		if(!m_pDepthThreshold)
		{
			Log::log(LET_ERROR, L"nGENE::Nature", __WFILE__, __WFUNCTION__, __LINE__,
				L"Missing shadow mapping shader constant: depthThresh");
			return;
		}

		m_pTexelSize = pass.getPixelShader()->getShader()->getConstant("texelSize");
		if(!m_pTexelSize)
		{
			Log::log(LET_ERROR, L"nGENE::Nature", __WFILE__, __WFUNCTION__, __LINE__,
				L"Missing shadow mapping shader constant: texelSize");
			return;
		}
	}
//----------------------------------------------------------------------
}
}