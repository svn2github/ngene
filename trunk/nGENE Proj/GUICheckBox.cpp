/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUICheckBox.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUICheckBox.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUICheckBox::GUICheckBox():
		m_bValue(false)
	{
	}
//----------------------------------------------------------------------
	GUICheckBox::~GUICheckBox()
	{
	}
//----------------------------------------------------------------------
	void GUICheckBox::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		Colour clr = m_Colour;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		_overlay->render(_position, m_fWidth, m_fHeight,
						 SRect<Real>(0.1875f, 0.0f, 0.25f, 0.0625f),
						 clr);

		if(m_bValue)
		{
			clr = Colour::COLOUR_WHITE;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);
			_overlay->render(_position, m_fWidth, m_fHeight,
							 SRect<Real>(0.125f, 0.0f, 0.1875f, 0.0625f),
							 clr);
		}

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
}