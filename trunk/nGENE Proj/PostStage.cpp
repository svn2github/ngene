/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PostStage.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PostStage.h"
#include "AABB.h"
#include "Camera.h"
#include "Frustum.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "NodeLight.h"
#include "ScreenOverlay.h"
#include "Surface.h"


namespace nGENE
{
	// Used to compare post process materials in sorting them.
	class PostProcessMaterialPointerComparator
	{
	public:
		bool operator()(const SPriorityMaterial* lhs, const SPriorityMaterial* rhs)
		{
			return (*lhs < *rhs);
		}
	};


	bool compPostStageFunction(Surface* _surf1, Surface* _surf2)
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



	PostStage::PostStage():
		m_pPostProcessQuad(NULL)
	{
	}
//----------------------------------------------------------------------
	PostStage::~PostStage()
	{
	}
//----------------------------------------------------------------------
	void PostStage::init()
	{
		m_pPostProcessQuad = new ScreenOverlay(0.0f,
											   0.0f,
											   m_pRenderer->getDisplayMode().width,
											   m_pRenderer->getDisplayMode().height);
	}
//----------------------------------------------------------------------
	void PostStage::cleanup()
	{
		NGENE_DELETE(m_pPostProcessQuad);
	}
//----------------------------------------------------------------------
	void PostStage::render()
	{
		vector <Surface*> vSurfaces = testVisibility();
		//renderGeometry(vSurfaces);


		vector <NodeLight*>* pLights = m_pRenderer->getLights();
		uint count = 0;
		for(uint i = 0; i < pLights->size(); ++i)
		{
			if((*pLights)[i]->isEnabled() && !(*pLights)[i]->isCulled())
				++count;
		}

		
		// No lights!
		Material* pMaterial = NULL;
		if(!m_pRenderer->isLightEnabled() || !count)
		{
			renderGeometry(vSurfaces);

			m_pPostProcessQuad->begin();
				pMaterial = MaterialManager::getSingleton().getLibrary(L"core")->getMaterial(L"diffuse");
				m_pPostProcessQuad->setMaterial(pMaterial);
				m_pPostProcessQuad->render();
			m_pPostProcessQuad->end();
		}


		if(m_pPostProcess.size())
		{
			if(m_pPostProcess[0]->priority)
			{
				renderGeometry(vSurfaces);


				m_pRenderer->setSeparateAlphaBlendEnabled(true);

				// Render actual post process effects
				m_pRenderer->setSeparateAlphaSourceBlending(BLEND_SRCALPHA);
				m_pRenderer->setSeparateAlphaDestinationBlending(BLEND_INVSRCALPHA);

				m_pPostProcessQuad->begin();

				for(uint i = 0; i < m_pPostProcess.size(); ++i)
				{
					pMaterial = m_pPostProcess[i]->material;
					if(pMaterial->isEnabled() && m_pPostProcess[i]->enabled)
					{
						m_pRenderer->updateBackBuffer();

						m_pPostProcessQuad->setMaterial(pMaterial);
						m_pPostProcessQuad->render();
					}
				}

				m_pPostProcessQuad->end();
			}
			else
			{
				m_pRenderer->setSeparateAlphaBlendEnabled(true);

				// Render actual post process effects
				m_pRenderer->setSeparateAlphaSourceBlending(BLEND_SRCALPHA);
				m_pRenderer->setSeparateAlphaDestinationBlending(BLEND_INVSRCALPHA);

				m_pPostProcessQuad->begin();

				uint i = 0;
				for(i = 0; i < m_pPostProcess.size(); ++i)
				{
					if(m_pPostProcess[i]->priority)
						break;

					pMaterial = m_pPostProcess[i]->material;
					if(pMaterial->isEnabled() && m_pPostProcess[i]->enabled)
					{
						m_pRenderer->updateBackBuffer();

						m_pPostProcessQuad->setMaterial(pMaterial);
						m_pPostProcessQuad->render();
					}
				}

				m_pPostProcessQuad->end();


				renderGeometry(vSurfaces);


				m_pRenderer->setSeparateAlphaBlendEnabled(true);

				// Render actual post process effects
				m_pRenderer->setSeparateAlphaSourceBlending(BLEND_SRCALPHA);
				m_pRenderer->setSeparateAlphaDestinationBlending(BLEND_INVSRCALPHA);

				m_pPostProcessQuad->begin();

				for(; i < m_pPostProcess.size(); ++i)
				{
					pMaterial = m_pPostProcess[i]->material;
					if(pMaterial->isEnabled() && m_pPostProcess[i]->enabled)
					{
						m_pRenderer->updateBackBuffer();

						m_pPostProcessQuad->setMaterial(pMaterial);
						m_pPostProcessQuad->render();
					}
				}

				m_pPostProcessQuad->end();
			}

		}
		

		m_pRenderer->setSeparateAlphaBlendEnabled(false);
	}
//----------------------------------------------------------------------
	void PostStage::addToRender(SPriorityMaterial* _mat)
	{
		m_CriticalSection.lock();

		m_pPostProcess.push_back(_mat);

		sortMaterials();

		m_CriticalSection.unlock();
	}
//----------------------------------------------------------------------
	void PostStage::removeFromRender(SPriorityMaterial* _mat)
	{
		uint i = 0;
		for(i = 0; i < m_pPostProcess.size(); ++i)
		{
			if(m_pPostProcess[i] == _mat)
				break;
		}

		// Material not found, so return
		if(i == m_pPostProcess.size())
			return;

		m_pPostProcess.erase(m_pPostProcess.begin() + i);
	}
//----------------------------------------------------------------------
	void PostStage::removeAllMaterials()
	{
		m_pPostProcess.clear();
	}
//----------------------------------------------------------------------
	void PostStage::sortMaterials()
	{
		PostProcessMaterialPointerComparator comparator;
		std::sort(m_pPostProcess.begin(), m_pPostProcess.end(), comparator);
	}
//----------------------------------------------------------------------
	void PostStage::endRender()
	{
		m_PPRenderQueues.clear();
	}
//----------------------------------------------------------------------
	void PostStage::addToRender(const wstring& _name, Surface* _renderable)
	{
		SharedObject sharedObj(&m_CriticalSection);

		m_PPRenderQueues[_name].push_back(_renderable);
	}
//----------------------------------------------------------------------
	void PostStage::renderGeometry(vector <Surface*>& _surfaces)
	{
		if(!_surfaces.size())
			return;

		Material* pMaterial = NULL;
		sort(_surfaces.begin(), _surfaces.end(), compPostStageFunction);


		m_pRenderer->setSeparateAlphaBlendEnabled(true);
		m_pRenderer->setSeparateAlphaSourceBlending(BLEND_ONE);
		m_pRenderer->setSeparateAlphaDestinationBlending(BLEND_ONE);


		vector <Surface*>::iterator iter2;
		for(iter2 = _surfaces.begin(); iter2 != _surfaces.end(); ++iter2)
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
	vector <Surface*> PostStage::testVisibility()
	{
		vector <Surface*> surfaces;
		HashTable <wstring, RenderQueue>::iterator iter;
		for(iter = m_PPRenderQueues.begin(); iter != m_PPRenderQueues.end(); ++iter)
		{
			surfaces.insert(surfaces.begin(),
							iter->second.getSurfaces().begin(),
							iter->second.getSurfaces().end());
		}

		// No transparent surfaces, so we can exit
		if(!surfaces.size())
			return surfaces;

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

		return surfaces;
	}
//----------------------------------------------------------------------
}
