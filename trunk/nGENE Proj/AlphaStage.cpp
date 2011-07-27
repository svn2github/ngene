/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AlphaStage.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "AlphaStage.h"
#include "AABB.h"
#include "Camera.h"
#include "Frustum.h"
#include "Material.h"
#include "Surface.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	bool compFunction(Surface* _surf1, Surface* _surf2)
	{
		if(_surf1->getMaterial().getPriority() != _surf2->getMaterial().getPriority())
			return _surf1->getMaterial().getPriority() < _surf2->getMaterial().getPriority();

		Vector3 vecCamPosition = Engine::getSingleton().getActiveCamera()->getPositionLocal();
		Vector3 vecToSurf1 = _surf1->getBoundingBox()->getCentre() - vecCamPosition;
		Vector3 vecToSurf2 = _surf2->getBoundingBox()->getCentre() - vecCamPosition;

		if(vecToSurf1.getLength() > vecToSurf2.getLength())
			return true;

		return false;
	}



	AlphaStage::AlphaStage():
		m_pPostProcessQuad(NULL)
	{
	}
//----------------------------------------------------------------------
	AlphaStage::~AlphaStage()
	{
	}
//----------------------------------------------------------------------
	void AlphaStage::init()
	{
		m_pPostProcessQuad = new ScreenOverlay(0.0f,
											   0.0f,
											   m_pRenderer->getDisplayMode().width,
											   m_pRenderer->getDisplayMode().height);

		m_bIsEmbeddable = true;
	}
//----------------------------------------------------------------------
	void AlphaStage::cleanup()
	{
		NGENE_DELETE(m_pPostProcessQuad);
	}
//----------------------------------------------------------------------
	void AlphaStage::render()
	{
		Material* pMaterial = NULL;
		vector <Surface*> surfaces;
		HashTable <wstring, RenderQueue>::iterator iter;
		for(iter = m_AlphaRenderQueues.begin(); iter != m_AlphaRenderQueues.end(); ++iter)
		{
			surfaces.insert(surfaces.begin(),
							iter->second.getSurfaces().begin(),
							iter->second.getSurfaces().end());
		}

		// No transparent surfaces, so we can exit
		if(!surfaces.size())
			return;

		// Re-check surfaces visibility
		Frustum& frustum = Engine::getSingleton().getActiveCamera()->getFrustum();
		for(uint i = 0; i < surfaces.size(); ++i)
		{
			Surface* surf = surfaces[i];
			if(frustum.testAABB(*surf->getBoundingBox()) == CR_INVISIBLE)
				surf->setCulled(true);
			else
				surf->setCulled(false);
		}

		m_pRenderer->setSeparateAlphaBlendEnabled(true);
		m_pRenderer->setSeparateAlphaSourceBlending(BLEND_ONE);
		m_pRenderer->setSeparateAlphaDestinationBlending(BLEND_ONE);

		sort(surfaces.begin(), surfaces.end(), compFunction);


		vector <Surface*>::iterator iter2;
		for(iter2 = surfaces.begin(); iter2 != surfaces.end(); ++iter2)
		{
			if((*iter2)->isCulled())
				continue;

			pMaterial = &(*iter2)->getMaterial();
			RenderQueue queue;
			queue.push_back(*iter2);
			
			if(queue.getSurfacesNum())
			{
				pMaterial->bind();
					pMaterial->render(&queue);
				pMaterial->unbind();
			}

			if(pMaterial->getRenderingOrder() == DO_POST)
			{
				m_pRenderer->updateBackBuffer();

				m_pPostProcessQuad->setMaterial(pMaterial);
				m_pPostProcessQuad->begin();
					m_pPostProcessQuad->render();
				m_pPostProcessQuad->end();
			}
		}

		m_pRenderer->setSeparateAlphaBlendEnabled(false);
	}
//----------------------------------------------------------------------
	void AlphaStage::endRender()
	{
		m_AlphaRenderQueues.clear();
	}
//----------------------------------------------------------------------
	void AlphaStage::addToRender(const wstring& _name, Surface* _renderable)
	{
		SharedObject sharedObj(&m_CriticalSection);

		m_AlphaRenderQueues[_name].push_back(_renderable);
	}
//----------------------------------------------------------------------
}
