/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUICursor.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUICursor.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUICursor::GUICursor()
	{
		m_fWidth = 32.0f;
		m_fHeight= 32.0f;
	}
//----------------------------------------------------------------------
	GUICursor::~GUICursor()
	{
	}
//----------------------------------------------------------------------
	void GUICursor::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		_overlay->begin();

		Colour clr = m_Colour;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		_overlay->render(m_vecPosition,
						 m_fWidth,
						 m_fHeight,
						 SRect<Real>(0.0f, 0.25f, 0.0625, 0.3125f),
						 clr);

		_overlay->end();

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
}