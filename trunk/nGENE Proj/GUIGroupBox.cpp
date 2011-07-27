/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIGroupBox.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIGroupBox.h"
#include "GUIRadioButton.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUIGroupBox::GUIGroupBox():
		m_pActive(NULL)
	{
	}
//----------------------------------------------------------------------
	GUIGroupBox::~GUIGroupBox()
	{
	}
//----------------------------------------------------------------------
	void GUIGroupBox::addControl(GUIRadioButton* _control)
	{
		_control->setGroup(this);
		m_vecRadioButtons.push_back(_control);

		if(m_vecRadioButtons.size() == 1)
		{
			_control->setValue(true);
			m_pActive = _control;
		}
		else
			_control->setValue(false);
	}
//----------------------------------------------------------------------
	void GUIGroupBox::select(GUIRadioButton* _control)
	{
		m_pActive = _control;
		for(uint i = 0; i < m_vecRadioButtons.size(); ++i)
		{
			if(m_vecRadioButtons[i] != m_pActive)
				m_vecRadioButtons[i]->setValue(false);
		}
	}
//----------------------------------------------------------------------
}