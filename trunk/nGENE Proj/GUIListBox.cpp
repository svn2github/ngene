/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIListBox.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIListBox.h"
#include "GUIFont.h"
#include "GUIManager.h"
#include "GUIWindow.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUIListBox::GUIListBox():
		m_bHasScrollbar(true),
		m_bMultiSelection(false),
		m_nColumnsNum(1),
		m_nFontSize(24),
		m_bBorder(true),
		m_TextColour(Colour::COLOUR_BLACK)
	{
		m_VerticalScrollbar.setWidth(32.0f);
		m_VerticalScrollbar.setSmallStep(28.0f);
		m_VerticalScrollbar.setBigStep(84.0f);
	}
//----------------------------------------------------------------------
	GUIListBox::~GUIListBox()
	{
	}
//----------------------------------------------------------------------
	void GUIListBox::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		SharedObject sharedObj(&m_CriticalSection);


		float x = _position.x;
		float y = _position.y;

		uint start = FastFloat::floatToInt(m_VerticalScrollbar.getValue() / 28.0f);
		float count = m_fHeight;
		if((m_vItems.size() - start) * 28.0f < count)
			count = (m_vItems.size() - start) * 28.0f;
		uint end = FastFloat::floatToInt((m_VerticalScrollbar.getValue() + count) / 28.0f);

		for(uint i = start; i < end; ++i)
		{
			Colour clr = m_Colour;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);

			if(m_vItems[i].selected)
				clr = m_MouseOverColour;
			float width = m_fWidth;
			if(m_bHasScrollbar)
				width -= 32.0f;
			_overlay->render(Vector2(x, y),
							 width,
							 m_nFontSize + 4.0f,
							 SRect<Real>(0.0f, 0.0625f, 0.0625, 0.125f),
							 clr);

			clr = m_TextColour;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);
			GUIManager::getSingleton().getFont()->drawText(m_vItems[i].text,
														   x,
														   y,
														   m_nFontSize,
														   clr);

			y += m_nFontSize + 4.0f;
		}

		float height = m_fHeight;
		if(m_bBorder)
		{
			Colour clr = Colour::COLOUR_BLACK;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);

			_overlay->render(Vector2(_position.x, _position.y),
							 2.0f,
							 height,
							 SRect<Real>(0.0f, 0.125f, 0.00390625f, 0.1875f),
							 clr);
			_overlay->render(Vector2(_position.x + m_fWidth - 2.0f, _position.y),
							 2.0f,
							 height,
							 SRect<Real>(0.0f, 0.125f, 0.00390625f, 0.1875f),
							 clr);
			_overlay->render(Vector2(_position.x + 2.0f, _position.y),
							 m_fWidth - 4.0f,
							 2.0f,
							 SRect<Real>(0.0f, 0.1875f, 0.0625f, 0.19140625f),
							 clr);
			_overlay->render(Vector2(_position.x + 2.0f, _position.y + height - 2.0f),
							 m_fWidth - 4.0f,
							 2.0f,
							 SRect<Real>(0.0f, 0.1875f, 0.0625f, 0.19140625f),
							 clr);
		}

		if(m_bHasScrollbar)
		{
			m_VerticalScrollbar.setVisible(true);

			if((m_nFontSize + 4.0f) * m_vItems.size() > m_fHeight)
				m_VerticalScrollbar.setEnabled(true);
			else
				m_VerticalScrollbar.setEnabled(false);
		}
		else
			m_VerticalScrollbar.setVisible(false);

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
	void GUIListBox::setPosition(const Vector2& _position)
	{
		GUIControl::setPosition(_position);

		m_VerticalScrollbar.setPosition(Vector2(m_vecPosition.x + m_fWidth - 32.0f,
												m_vecPosition.y));
	}
//----------------------------------------------------------------------
	void GUIListBox::setPosition(Real _x, Real _y)
	{
		GUIControl::setPosition(_x, _y);

		m_VerticalScrollbar.setPosition(Vector2(m_vecPosition.x + m_fWidth - 32.0f,
												m_vecPosition.y));
	}
//----------------------------------------------------------------------
	void GUIListBox::addItem(const wstring& _text)
	{
		SharedObject sharedObj(&m_CriticalSection);


		LIST_BOX_ITEM item;
		item.text = _text;
		item.selected = false;

		deselect();
		m_vItems.push_back(item);

		float max = 28.0f * m_vItems.size() - m_fHeight;
		if(max < 0.0)
			max = 0.0f;
		m_VerticalScrollbar.setMax(max);
	}
//----------------------------------------------------------------------
	void GUIListBox::removeItem(uint _index)
	{
		SharedObject sharedObj(&m_CriticalSection);


		if(_index >= m_vItems.size())
			return;

		deselect();
		m_vItems.erase(m_vItems.begin() + _index);

		float max = 28.0f * m_vItems.size() - m_fHeight;
		if(max < 0.0)
			max = 0.0f;
		m_VerticalScrollbar.setMax(max);
	}
//----------------------------------------------------------------------
	void GUIListBox::clear()
	{
		SharedObject sharedObj(&m_CriticalSection);


		m_vItems.clear();
	}
//----------------------------------------------------------------------
	LIST_BOX_ITEM* GUIListBox::getItem(uint _index)
	{
		SharedObject sharedObj(&m_CriticalSection);


		if(_index >= m_vItems.size())
			return NULL;

		return &m_vItems[_index];
	}
//----------------------------------------------------------------------
	void GUIListBox::setParent(GUIControl* _parent)
	{
		GUIControl::setParent(_parent);

		((GUIWindow*)(_parent))->addControl(&m_VerticalScrollbar);
	}
//----------------------------------------------------------------------
	void GUIListBox::setHeight(Real _height)
	{
		m_fHeight = _height;
		uint height = FastFloat::floatToInt(m_fHeight);
		if(height % 28)
		{
			uint temp = height - FastFloat::floatToInt(floor(m_fHeight / 28.0f)) * 28;
			if(temp < 14)
				m_fHeight -= temp;
			else
				m_fHeight += (28.0f - temp);
		}

		m_VerticalScrollbar.setHeight(m_fHeight);
	}
//----------------------------------------------------------------------
	void GUIListBox::keyboardDown(const KeyboardEvent& _evt)
	{
		GUIControl::keyboardDown(_evt);

		if(!m_bMultiSelection)
		{
			if(_evt.isKeyPressed(KC_UP))
			{
				if(m_vSelectedIndices.size())
				{
					uint i = m_vSelectedIndices[0];
					if(i > 0)
						selectItem(--i);
				}
				else
				{
					if(m_vItems.size())
						selectItem(0);
				}
			}
			else if(_evt.isKeyPressed(KC_DOWN))
			{
				if(m_vSelectedIndices.size())
				{
					uint i = m_vSelectedIndices[0];
					if(i < m_vItems.size() - 1)
						selectItem(++i);
				}
				else
				{
					if(m_vItems.size())
						selectItem(0);
				}
			}
		}
	}
//----------------------------------------------------------------------
	void GUIListBox::selectItem(uint _index)
	{
		if(m_bMultiSelection)
		{
			if(m_vItems[_index].selected)
			{
				m_vItems[_index].selected = false;
				uint i = 0;
				for(i = 0; i < m_vSelectedIndices.size(); ++i)
				{
					if(m_vSelectedIndices[i] == _index)
						break;
				}

				if(i < m_vSelectedIndices.size())
				{
					m_vSelectedItems.erase(m_vSelectedItems.begin() + i);
					m_vSelectedIndices.erase(m_vSelectedIndices.begin() + i);
				}
			}
			else
			{
				m_vSelectedIndices.push_back(_index);
				m_vSelectedItems.push_back(&m_vItems[_index]);
				m_vSelectedItems.back()->selected = true;
			}
		}
		else
		{
			if(m_vSelectedItems.size())
				m_vSelectedItems[0]->selected = false;
			m_vSelectedIndices.clear();
			m_vSelectedItems.clear();

			m_vSelectedIndices.push_back(_index);
			m_vSelectedItems.push_back(&m_vItems[_index]);
			m_vSelectedItems.back()->selected = true;
		}

		if(onChange)
			onChange();
	}
//----------------------------------------------------------------------
	void GUIListBox::deselect()
	{
		for(uint i = 0; i < m_vSelectedItems.size(); ++i)
			m_vSelectedItems[i]->selected = false;

		m_vSelectedIndices.clear();
		m_vSelectedItems.clear();
	}
//----------------------------------------------------------------------
	void GUIListBox::mouseClick(uint _x, uint _y)
	{
		GUIControl::mouseClick(_x, _y);

		if(_x < getAbsolutePosition().x + m_fWidth - 32.0f)
		{
			uint index = FastFloat::floatToInt(floor((_y - getAbsolutePosition().y +
													  m_VerticalScrollbar.getValue()) / 28.0f));

			if(index >= m_vItems.size())
				return;

			selectItem(index);
		}
	}
//----------------------------------------------------------------------
	void GUIListBox::setHasScrollbar(bool _value)
	{
		if(_value != m_bHasScrollbar)
		{
			m_bHasScrollbar = _value;
			m_VerticalScrollbar.setValue(0.0f);
		}
	}
//----------------------------------------------------------------------
	void GUIListBox::setVisible(bool _value)
	{
		GUIControl::setVisible(_value);

		m_VerticalScrollbar.setVisible(_value);
	}
//----------------------------------------------------------------------
	void GUIListBox::toggleVisible()
	{
		GUIControl::toggleVisible();

		m_VerticalScrollbar.toggleVisible();
	}
//----------------------------------------------------------------------
	void GUIListBox::show()
	{
		GUIControl::show();

		m_VerticalScrollbar.show();
	}
//----------------------------------------------------------------------
	void GUIListBox::hide()
	{
		GUIControl::hide();

		m_VerticalScrollbar.hide();
	}
//----------------------------------------------------------------------
}