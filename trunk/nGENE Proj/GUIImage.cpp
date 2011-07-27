/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIImage.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIImage.h"
#include "GUIManager.h"
#include "ScreenOverlay.h"
#include "TextureManager.h"

/// @todo Remove this
#include "Material.h"
#include "RenderPass.h"
///


namespace nGENE
{
	GUIImage::GUIImage():
		m_pImage(NULL),
		m_pOverlay(NULL)
	{
	}
//----------------------------------------------------------------------
	GUIImage::~GUIImage()
	{
		NGENE_DELETE(m_pOverlay);
	}
//----------------------------------------------------------------------
	void GUIImage::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		if(!m_pOverlay)
		{
			m_pOverlay = new ScreenOverlay(0.0f, 0.0f, 1.0f, 1.0f);
		}
		m_pOverlay->setMaterial(_overlay->getMaterial());

		RenderPass& pass = _overlay->getMaterial()->getActiveRenderTechnique()->getRenderPass(0);
		TextureSampler& sampler = pass.getTextureSampler(0);
		ITexture* pTexture = sampler.getTexture();
		sampler.setTexture(m_pImage);

		Colour clr = m_Colour;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		SRect <Real> rect(0.0f, 0.0f, 1.0f, 1.0f);
		Vector2 position = _position;
		m_pOverlay->render(position, m_fWidth, m_fHeight, rect, clr);
		
		sampler.setTexture(pTexture);

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
	void GUIImage::setImage(const wstring& _name, const wstring& _filename)
	{
		m_pImage = TextureManager::getSingleton().createTextureFromFile(_name, _filename);
	}
//----------------------------------------------------------------------
}