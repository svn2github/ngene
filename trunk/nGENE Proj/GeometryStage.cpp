/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GeometryStage.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GeometryStage.h"
#include "Camera.h"
#include "Material.h"
#include "NodeLight.h"
#include "RenderTechnique.h"
#include "ScenePartitioner.h"


namespace nGENE
{
	GeometryStage::GeometryStage()
	{
	}
//----------------------------------------------------------------------
	GeometryStage::~GeometryStage()
	{
	}
//----------------------------------------------------------------------
	void GeometryStage::init()
	{
		m_bIsEmbeddable = true;
	}
//----------------------------------------------------------------------
	void GeometryStage::onBeginFrame()
	{
		testVisibility();
	}
//----------------------------------------------------------------------
	void GeometryStage::render()
	{
		vector <NodeLight*>* pLights = m_pRenderer->getLights();
		uint count = 0;
		for(uint i = 0; i < pLights->size(); ++i)
		{
			if((*pLights)[i]->isEnabled() && !(*pLights)[i]->isCulled())
				++count;
		}


		HashTable <wstring, RenderQueue>::iterator iter1;
		Material* pMat = NULL;
		for(iter1 = m_PreRenderQueues.begin(); iter1 != m_PreRenderQueues.end(); ++iter1)
		{
			pMat = m_pMaterials[iter1->first];
			pMat->bind();

			RenderTechnique* pTechnique = pMat->getActiveRenderTechnique();
			for(uint i = 0; i < pTechnique->getPassesNum(); ++i)
			{
				RenderPass& pass = pTechnique->getRenderPass(i);
				pass.bind();

				// G-Buffer rendering
				HashTable <wstring, RenderQueue>::iterator iter;
				Material* pMaterial = NULL;
				for(iter = m_RenderQueues.begin(); iter != m_RenderQueues.end(); ++iter)
				{
					if(iter->second.getSurfacesNum())
					{
						pMaterial = m_pMaterials[iter->first];
						pMaterial->bind();
							pMaterial->render(&iter->second);
						pMaterial->unbind();
					}
				}

				pass.unbind();
			}

			pMat->unbind();
		}
	}
//----------------------------------------------------------------------
	void GeometryStage::testVisibility()
	{
		Engine& engine = Engine::getSingleton();
		uint managersCount = engine.getSceneManagersNum();
		ScenePartitioner* pPartitioner = NULL;
		SceneManager* pSM = NULL;
		for(uint j = 0; j < managersCount; ++j)
		{
			pSM = engine.getSceneManager(j);
			pPartitioner = pSM->getScenePartitioner();
			if(pPartitioner && engine.getActiveCamera())
				pPartitioner->setFrustum(&engine.getActiveCamera()->getFrustum());
			else if(pPartitioner)
				pPartitioner->setFrustum(NULL);

			pSM->testVisibility();
		}
	}
//----------------------------------------------------------------------
	void GeometryStage::addToRender(Material* _mat, Surface* _renderable)
	{
		wstring stName = _mat->getName();

		m_CriticalSection.lock();

		if((_mat->getRenderingOrder() != DO_POST) && (_mat->getRenderingOrder() != DO_PRE))
			m_RenderQueues[stName].push_back(_renderable);
		else if(_mat->getRenderingOrder() == DO_PRE)
			m_PreRenderQueues[stName].push_back(_renderable);

		m_pMaterials[stName] = _mat;

		m_CriticalSection.unlock();
	}
//----------------------------------------------------------------------
	void GeometryStage::endRender()
	{
		m_RenderQueues.clear();
		m_PreRenderQueues.clear();
	}
//----------------------------------------------------------------------
}
