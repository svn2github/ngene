/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LightStage.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "LightStage.h"
#include "ITexture.h"
#include "Material.h"
#include "NodeLight.h"
#include "ScreenOverlay.h"
#include "TextureManager.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"


namespace nGENE
{
	LightStage::LightStage():
		m_pPostProcessQuad(NULL),
		m_pHDR(NULL),
		m_bUseHDR(false)
	{
	}
//----------------------------------------------------------------------
	LightStage::~LightStage()
	{
	}
//----------------------------------------------------------------------
	void LightStage::init()
	{
		m_pPostProcessQuad = new ScreenOverlay(0.0f,
											   0.0f,
											   m_pRenderer->getDisplayMode().width,
											   m_pRenderer->getDisplayMode().height);

		m_pHDR = TextureManager::getSingleton().createTexture(L"HDRBackBuffer",
												  m_pRenderer->getDisplayMode().width,
												  m_pRenderer->getDisplayMode().height,
												  TT_2D,
												  TU_RENDERTARGET,
												  TF_A16B16G16R16F);

		m_bIsEmbeddable = true;
	}
//----------------------------------------------------------------------
	void LightStage::cleanup()
	{
		NGENE_DELETE(m_pPostProcessQuad);
	}
//----------------------------------------------------------------------
	void LightStage::render()
	{
		vector <NodeLight*>* pLights = m_pRenderer->getLights();

		if(!pLights->size() || !m_pRenderer->isLightEnabled())
			return;
			
		uint count = 0;
		for(uint i = 0; i < pLights->size(); ++i)
		{
			if((*pLights)[i]->isEnabled() && !(*pLights)[i]->isCulled())
				++count;
		}

		// No lights so return
		if(!count)
			return;


		if(m_bUseHDR)
			m_pHDR->bind();

		// Now turn on 2D rendering
		m_pPostProcessQuad->begin();

		// Update back buffer prior to any rendering
		m_pRenderer->updateBackBuffer();

		for(uint i = 0; i < pLights->size(); ++i)
		{
			NodeLight* pLight = (*pLights)[i];
			if(pLight->isEnabled() && !pLight->isCulled())
			{
				m_pRenderer->setActiveLight(pLight);
				m_pRenderer->setLightScissor(*pLight);

				m_pPostProcessQuad->setMaterial(pLight->getMaterial());
				m_pPostProcessQuad->render();
			}
		}


		// Disable scissor test
		if(m_pRenderer->isScissorTestEnabled())
			m_pRenderer->setScissorTestEnabled(false);

		if(m_bUseHDR)
		{
			m_pPostProcessQuad->end();

			m_pHDR->unbind();

			m_pPostProcessQuad->begin();

			Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"core")->getMaterial(L"HDR");
			m_pPostProcessQuad->setMaterial(pMaterial);
			m_pPostProcessQuad->render();

			m_pRenderer->updateBackBuffer();
		}

		m_pPostProcessQuad->end();
	}
//----------------------------------------------------------------------
}
