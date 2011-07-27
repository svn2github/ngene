/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIControl.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIControl.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	/// Initialize static members
	TypeInfo GUIControl::Type(L"GUIControl", NULL);
	vector <SGUIElement> GUIControl::m_vElements;


	GUIControl::GUIControl():
		m_fHeight(0.0f),
		m_fWidth(0.0f),
		m_fOpacity(1.0f),
		m_pParent(NULL),
		m_nTabIndex(0),
		m_bEnabled(true),
		m_bVisible(true),
		m_bHasFocus(false),
		m_bHasTabStop(true),
		m_State(STATE_NONE),
		onMouseDown(NULL),
		onMouseRollIn(NULL),
		onMouseRollOut(NULL),
		onMouseOver(NULL),
		onClick(NULL),
		onMouseRelease(NULL),
		onPaint(NULL),
		onFocus(NULL),
		onKeyboard(NULL)
	{
	}
//----------------------------------------------------------------------
	GUIControl::~GUIControl()
	{
	}
//----------------------------------------------------------------------
	Vector2 GUIControl::getAbsolutePosition()
	{
		Vector2 result = m_vecPosition;
		GUIControl* pControl = m_pParent;
		while(pControl)
		{
			result += pControl->getPosition();
			pControl = pControl->getParent();
		}

		return result;
	}
//----------------------------------------------------------------------
	void GUIControl::addGUIElement(SGUIElement& _element)
	{
		m_vElements.push_back(_element);
	}
//----------------------------------------------------------------------
	void GUIControl::renderElements(ScreenOverlay* _overlay)
	{
		for(uint i = 0; i < m_vElements.size(); ++i)
		{
			SGUIElement& element = m_vElements[i];
//			_overlay->render(element.position, m_fWidth, height, SRect<Real>(0.0f, 0.0625f, 0.0625f, 0.125f));
		}
	}
//----------------------------------------------------------------------
	void GUIControl::changeResolution(uint _oldWidth, uint _oldHeight,
		uint _width, uint _height)
	{
		Vector2 vecPos = getPosition();
		if(_oldWidth != _width)
		{
			float ratio = (float)_width / (float)_oldWidth;
			setWidth(getWidth() * ratio);
			vecPos.x *= ratio;
		}

		if(_oldHeight != _height)
		{
			float ratio = (float)_height / (float)_oldHeight;
			setHeight(getHeight() * ratio);
			vecPos.y *= ratio;
		}

		if((_oldWidth != _width) || (_oldHeight != _height))
			setPosition(vecPos);
	}
//----------------------------------------------------------------------
}