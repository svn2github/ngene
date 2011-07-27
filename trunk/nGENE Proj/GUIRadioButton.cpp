/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIRadioButton.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIRadioButton.h"
#include "GUIGroupBox.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUIRadioButton::GUIRadioButton():
		m_pGroup(NULL),
		m_bValue(false)
	{
	}
//----------------------------------------------------------------------
	GUIRadioButton::~GUIRadioButton()
	{
	}
//----------------------------------------------------------------------
	void GUIRadioButton::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		Colour clr = m_Colour;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		_overlay->render(_position, m_fWidth, m_fHeight,
						 SRect<Real>(0.2520f, 0.0f, 0.3125f, 0.0625f),
						 clr);

		if(m_bValue)
		{
			clr = Colour::COLOUR_WHITE;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);

			_overlay->render(_position, m_fWidth, m_fHeight,
							 SRect<Real>(0.127f, 0.0625f, 0.1875f, 0.125f),
							 clr);
		}

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
	void GUIRadioButton::mouseClick(uint _x, uint _y)
	{
		GUIControl::mouseClick(_x, _y);
		m_bValue = true;
		
		if(m_pGroup)
			m_pGroup->select(this);
	}
//----------------------------------------------------------------------
}