/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIScrollbar.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIScrollbar.h"
#include "GUIFont.h"
#include "GUIManager.h"
#include "GUIWindow.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUIScrollbar::GUIScrollbar():
		m_fMin(0.0f),
		m_fMax(1.0f),
		m_fValue(0.0f),
		m_fSmallStep(0.1f),
		m_fBigStep(0.1f),
		m_bHorizontal(false),
		m_bDragged(false)
	{
		m_btnUp.setPosition(Vector2(0, 0));
		m_btnUp.setHeight(32);
		m_btnUp.setWidth(32);
		m_btnUp.setBorder(true);

		m_btnDown.setPosition(Vector2(0, 0));
		m_btnDown.setHeight(32);
		m_btnDown.setWidth(32);
		m_btnDown.setBorder(true);
		
		m_btnUp.onClick.bind(this, &GUIScrollbar::_ButtonUp_Click);
		m_btnDown.onClick.bind(this, &GUIScrollbar::_ButtonDown_Click);
	}
//----------------------------------------------------------------------
	GUIScrollbar::~GUIScrollbar()
	{
	}
//----------------------------------------------------------------------
	void GUIScrollbar::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		Vector2 position;
		float width, height;
		if(m_bHorizontal)
		{
			m_btnUp.setPosition(m_vecPosition);
			m_btnDown.setPosition(m_vecPosition.x + m_fWidth - 32.0f, m_vecPosition.y);

			position = Vector2(32.0f, 0.0f);
			width = m_fWidth - 64.0f;
			height = m_fHeight;
		}
		else
		{
			m_btnUp.setPosition(m_vecPosition);
			m_btnDown.setPosition(m_vecPosition.x, m_vecPosition.y + m_fHeight - 32.0f);

			position = Vector2(0.0f, 32.0f);
			width = m_fWidth;
			height = m_fHeight - 64.0f;
		}

		Vector2 temp = _position + m_btnUp.getPosition();
		//m_btnUp.render(_overlay, temp);
		temp = _position + m_btnDown.getPosition();
		//m_btnDown.render(_overlay, temp);

		Colour clr = m_Colour;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		position += _position;
		_overlay->render(position,
						 width,
						 height,
						 SRect <Real>(0.1875f, 0.125f, 0.25f, 0.125f),
						 clr);

		if(m_bHorizontal)
		{
			clr = Colour::COLOUR_WHITE;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);

			float offset = (m_fWidth - 96.0f) * ((m_fValue - m_fMin) / (m_fMax - m_fMin));
			_overlay->render(Vector2(_position.x + 32.0f + offset, _position.y),
							 32.0f,
							 32.0f,
							 SRect <Real>(0.125f, 0.1875, 0.1875f, 0.21875f),
							 clr);
		}
		else
		{
			Colour clr = Colour::COLOUR_WHITE;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);

			float offset = (m_fHeight - 96.0f) * ((m_fValue - m_fMin) / (m_fMax - m_fMin));
			_overlay->render(Vector2(_position.x, _position.y + 32.0f + offset),
							 32.0f,
							 32.0f,
							 SRect <Real>(0.0625f, 0.1875, 0.09375f, 0.25f),
							 clr);
		}

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
	void GUIScrollbar::_ButtonUp_Click(uint _x, uint _y)
	{
		if(m_bEnabled)
			setValue(m_fValue - m_fSmallStep);
	}
//----------------------------------------------------------------------
	void GUIScrollbar::_ButtonDown_Click(uint _x, uint _y)
	{
		if(m_bEnabled)
			setValue(m_fValue + m_fSmallStep);
	}
//----------------------------------------------------------------------
	void GUIScrollbar::setParent(GUIControl* _parent)
	{
		GUIControl::setParent(_parent);


		((GUIWindow*)(_parent))->addControl(&m_btnUp);
		((GUIWindow*)(_parent))->addControl(&m_btnDown);
	}
//----------------------------------------------------------------------
	void GUIScrollbar::mouseClick(uint _x, uint _y)
	{
		if(!m_bEnabled || m_bDragged)
			return;

		GUIControl::mouseClick(_x, _y);

		Real value = 0.0f;
		if(m_bHorizontal)
		{
			_x -= getAbsolutePosition().x;
			Real offset = ((m_fValue - m_fMin) / (m_fMax - m_fMin)) * (m_fWidth - 96.0f) + 32.0f;
			value = (_x <= offset ? m_fValue - m_fBigStep : m_fValue + m_fBigStep);
		}
		else
		{
			_y -= getAbsolutePosition().y;
			Real offset = ((m_fValue - m_fMin) / (m_fMax - m_fMin)) * (m_fHeight - 96.0f) + 32.0f;
			value = (_y <= offset ? m_fValue - m_fBigStep : m_fValue + m_fBigStep);
		}

		setValue(value);
		if(onChange)
			onChange();
	}
//----------------------------------------------------------------------
	void GUIScrollbar::mouseDown(uint _x, uint _y)
	{
		if(!m_bEnabled)
			return;

		GUIControl::mouseDown(_x, _y);


		if(m_bHorizontal)
		{
			_x -= getAbsolutePosition().x;
			Real offset = ((m_fValue - m_fMin) / (m_fMax - m_fMin)) * (m_fWidth - 96.0f) + 32.0f;
			if(_x > offset && _x < offset + 32.0f)
				m_bDragged = true;
		}
		else
		{
			_y -= getAbsolutePosition().y;
			Real offset = ((m_fValue - m_fMin) / (m_fMax - m_fMin)) * (m_fHeight - 96.0f) + 32.0f;
			if(_y > offset && _y < offset + 32.0f)
			{
				m_bDragged = true;
			}
		}
	}
//----------------------------------------------------------------------
	void GUIScrollbar::mouseRelease(uint _x, uint _y)
	{
		if(!m_bEnabled)
			return;

		GUIControl::mouseRelease(_x, _y);

		m_bDragged = false;
	}
//----------------------------------------------------------------------
	void GUIScrollbar::mouseOver(uint _x, uint _y)
	{
		if(!m_bEnabled)
			return;

		GUIControl::mouseOver(_x, _y);

		if(m_State != STATE_MOUSE_DOWN)
			m_bDragged = false;

		if(m_bDragged)
		{
			Real value = 0.0f;

			if(m_bHorizontal)
			{
				_x -= getAbsolutePosition().x;
				value = ((Real)_x - 32.0f) / (m_fWidth - 96.0f) * (m_fMax - m_fMin) + m_fMin;
			}
			else
			{
				_y -= getAbsolutePosition().y;
				value = ((Real)_y - 32.0f) / (m_fHeight - 96.0f) * (m_fMax - m_fMin) + m_fMin;
			}

			setValue(value);
			if(onChange)
				onChange();
		}
	}
//----------------------------------------------------------------------
	void GUIScrollbar::setVisible(bool _value)
	{
		GUIControl::setVisible(_value);

		m_btnUp.setVisible(_value);
		m_btnDown.setVisible(_value);
	}
//----------------------------------------------------------------------
	void GUIScrollbar::toggleVisible()
	{
		GUIControl::toggleVisible();

		m_btnUp.toggleVisible();
		m_btnDown.toggleVisible();
	}
//----------------------------------------------------------------------
	void GUIScrollbar::show()
	{
		GUIControl::show();

		m_btnUp.show();
		m_btnDown.show();
	}
//----------------------------------------------------------------------
	void GUIScrollbar::hide()
	{
		GUIControl::hide();

		m_btnUp.hide();
		m_btnDown.hide();
	}
//----------------------------------------------------------------------
}