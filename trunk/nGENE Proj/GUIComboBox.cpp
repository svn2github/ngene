/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIComboBox.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIComboBox.h"
#include "GUIFont.h"
#include "GUIManager.h"
#include "GUIWindow.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUIComboBox::GUIComboBox():
		m_bOpened(false)
	{
		m_VerticalScrollbar.setVisible(false);
	}
//----------------------------------------------------------------------
	GUIComboBox::~GUIComboBox()
	{
	}
//----------------------------------------------------------------------
	void GUIComboBox::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		Colour clr = m_Colour;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		_overlay->render(_position,
						 m_fWidth,
						 28.0f,
						 SRect<Real>(0.0f, 0.0625f, 0.0625, 0.125f),
						 m_Colour);

		if(m_vSelectedItems.size())
		{
			clr = Colour::COLOUR_BLACK;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);
			GUIManager::getSingleton().getFont()->drawText(m_vSelectedItems[0]->text,
														   _position.x,
														   _position.y,
														   24,
														   clr);
		}

		if(m_bOpened)
		{
			m_fHeight -= 28.0;
			Vector2 pos = _position;
			pos.y += 28.0f;
			GUIListBox::render(_overlay, pos);
			m_fHeight += 28.0;
		}
	}
//----------------------------------------------------------------------
	void GUIComboBox::mouseClick(uint _x, uint _y)
	{
		if(m_bOpened)
		{
			GUIListBox::mouseClick(_x, _y - 28.0f);
			if(_x < getAbsolutePosition().x + m_fWidth)
			{
				m_bOpened = false;
				m_VerticalScrollbar.setVisible(false);
			}
		}
		else
		{
			if(_x < getAbsolutePosition().x + m_fWidth && _y < getAbsolutePosition().y + 28.0f)
			{
				m_bOpened = true;
				m_VerticalScrollbar.setVisible(true);
			}
		}
	}
//----------------------------------------------------------------------
	void GUIComboBox::loosingFocus()
	{
		GUIListBox::loosingFocus();

		m_bOpened = false;
		m_VerticalScrollbar.setVisible(false);
	}
//----------------------------------------------------------------------
	void GUIComboBox::setHeight(Real _height)
	{
		GUIListBox::setHeight(_height);
		m_VerticalScrollbar.setHeight(m_fHeight - 28.0f);
	}
//----------------------------------------------------------------------
	void GUIComboBox::setPosition(const Vector2& _position)
	{
		GUIListBox::setPosition(_position);
		m_VerticalScrollbar.setPosition(m_VerticalScrollbar.getPosition().x,
										m_VerticalScrollbar.getPosition().y + 28.0f);
	}
//----------------------------------------------------------------------
	void GUIComboBox::setPosition(Real _x, Real _y)
	{
		GUIListBox::setPosition(_x, _y);
		m_VerticalScrollbar.setPosition(m_VerticalScrollbar.getPosition().x,
										m_VerticalScrollbar.getPosition().y + 28.0f);
	}
//----------------------------------------------------------------------
	void GUIComboBox::keyboardDown(const KeyboardEvent& _evt)
	{
		if(m_bOpened)
		{
			GUIListBox::keyboardDown(_evt);

			if(_evt.isKeyPressed(KC_RETURN))
			{
				m_bOpened = false;
				m_VerticalScrollbar.setVisible(false);
			}
		}
		else
		{
			if(m_vSelectedIndices.size())
			{
				int i = m_vSelectedIndices[0];
				if(_evt.isKeyPressed(KC_DOWN))
					++i;
				else if(_evt.isKeyPressed(KC_UP))
					--i;

				Maths::clamp_roll <int>(i, 0, m_vItems.size() - 1);
				selectItem(i);
			}
			else
			{
				if(m_vItems.size())
					selectItem(0);
			}
		}
	}
//----------------------------------------------------------------------
}