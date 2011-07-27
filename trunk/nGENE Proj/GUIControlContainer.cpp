/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIControlContainer.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIControlContainer.h"


namespace nGENE
{
	class GUIControlTabIndexComparator
	{
	public:
		bool operator()(const GUIControl* _control1, const GUIControl* _control2)
		{
			return (_control1->getTabIndex() < _control2->getTabIndex());
		}
	};



	GUIControlContainer::GUIControlContainer():
		m_pFocusedControl(NULL)
	{
	}
//----------------------------------------------------------------------
	GUIControlContainer::~GUIControlContainer()
	{
	}
//----------------------------------------------------------------------
	void GUIControlContainer::addControl(GUIControl* _control)
	{
		_control->setParent(this);
		_control->setTabIndex(m_vControls.size());

		m_vControls.push_back(_control);

		if(m_vControls.size() == 1)
			setFocusedControl(_control);

		// Sort controls based on their tab index
		sortByTabIndex();
	}
//----------------------------------------------------------------------
	void GUIControlContainer::changeResolution(uint _oldWidth, uint _oldHeight,
		uint _width, uint _height)
	{
		GUIControl::changeResolution(_oldWidth, _oldHeight, _width, _height);

		for(uint i = 0; i != m_vControls.size(); ++i)
		{
			m_vControls[i]->changeResolution(_oldWidth, _oldHeight,
				_width, _height);
		}
	}
//----------------------------------------------------------------------
	void GUIControlContainer::setFocusedControl(GUIControl* _control)
	{
		if(m_pFocusedControl == _control)
			return;

		// Handle events
		if(m_pFocusedControl)
			m_pFocusedControl->loosingFocus();

		m_pFocusedControl = _control;
		if(m_pFocusedControl)
			m_pFocusedControl->gettingFocus();
	}
//----------------------------------------------------------------------
	void GUIControlContainer::sortByTabIndex()
	{
		GUIControlTabIndexComparator comparator;
		std::sort(m_vControls.begin(), m_vControls.end(), comparator);
	}
//----------------------------------------------------------------------
	void GUIControlContainer::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		// Render all child controls
		for(uint i = 0; i < m_vControls.size(); ++i)
		{
			GUIControl* pControl = m_vControls[i];
			if(pControl->isVisible())
			{
				Vector2 temp = _position + pControl->getPosition();

				SRect <uint> rect(temp.x,
								  temp.y,
								  temp.x + pControl->getWidth(),
								  temp.y + pControl->getHeight());

				pControl->render(_overlay, temp);
			}
		}
	}
//----------------------------------------------------------------------
	void GUIControlContainer::setOpacity(float _opacity)
	{
		GUIControl::setOpacity(_opacity);

		for(uint i = 0; i < m_vControls.size(); ++i)
		{
			m_vControls[i]->setOpacity(_opacity);
		}
	}
//----------------------------------------------------------------------
}