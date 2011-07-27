/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUISlider.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUISlider.h"
#include "GUIFont.h"
#include "GUIManager.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUISlider::GUISlider():
		m_fMin(0.0f),
		m_fMax(1.0f),
		m_fValue(0.0f),
		m_fStep(0.1f),
		m_bDragged(true)
	{
	}
//----------------------------------------------------------------------
	GUISlider::~GUISlider()
	{
	}
//----------------------------------------------------------------------
	void GUISlider::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		Colour clr = m_Colour;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		_overlay->render(Vector2(_position.x + 8.0f, _position.y + 16.0f),
						 m_fWidth - 16.0f,
						 3.0f,
						 SRect<Real>(0.125f, 0.125f, 0.1875f, 0.130859375f),
						 clr);

		float offset = ((m_fValue - m_fMin) / (m_fMax - m_fMin)) * (m_fWidth - 16.0f);

		_overlay->render(Vector2(_position.x + offset, _position.y),
						 16.0f,
						 32.0f,
						 SRect<Real>(0.0625f, 0.125f, 0.09375f, 0.1875f),
						 clr);

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
	void GUISlider::keyboardDown(const KeyboardEvent& _evt)
	{
		if(_evt.isKeyPressed(KC_LEFT))
			setValue(m_fValue - m_fStep);
		else if(_evt.isKeyPressed(KC_RIGHT))
			setValue(m_fValue + m_fStep);
	}
//----------------------------------------------------------------------
	void GUISlider::mouseClick(uint _x, uint _y)
	{
		if(!m_bEnabled || m_bDragged)
			return;

		GUIControl::mouseClick(_x, _y);

		_x -= getAbsolutePosition().x;
		Real offset = ((m_fValue - m_fMin) / (m_fMax - m_fMin)) * m_fWidth;
		Real value = (_x <= offset ? m_fValue - m_fStep : m_fValue + m_fStep);

		setValue(value);
	}
//----------------------------------------------------------------------
	void GUISlider::mouseDown(uint _x, uint _y)
	{
		if(!m_bEnabled)
			return;

		GUIControl::mouseDown(_x, _y);

		_x -= getAbsolutePosition().x;
		Real offset = ((m_fValue - m_fMin) / (m_fMax - m_fMin)) * (m_fWidth - 16.0f) + 8.0f;

		if(_x > offset - 8.0f && _x < offset + 8.0f)
			m_bDragged = true;
	}
//----------------------------------------------------------------------
	void GUISlider::mouseRelease(uint _x, uint _y)
	{
		if(!m_bEnabled)
			return;

		GUIControl::mouseRelease(_x, _y);

		m_bDragged = false;
	}
//----------------------------------------------------------------------
	void GUISlider::mouseOver(uint _x, uint _y)
	{
		if(!m_bEnabled)
			return;

		GUIControl::mouseOver(_x, _y);

		if(m_State != STATE_MOUSE_DOWN)
			m_bDragged = false;

		if(m_bDragged)
		{
			_x -= getAbsolutePosition().x;
			Real value = ((Real)_x / m_fWidth) * (m_fMax - m_fMin) + m_fMin;

			setValue(value);
		}
	}
//----------------------------------------------------------------------
}