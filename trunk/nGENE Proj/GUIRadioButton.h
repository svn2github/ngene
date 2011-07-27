/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIRadioButton.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUIRADIOBUTTON_H_
#define __INC_GUIRADIOBUTTON_H_


#include "GUIControl.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Radio button control.
	*/
	class nGENEDLL GUIRadioButton: public GUIControl
	{
	private:
		GUIGroupBox* m_pGroup;					///< A group this radio button belongs to

		bool m_bValue;							///< Value of the check box
	
	public:
		GUIRadioButton();
		~GUIRadioButton();

		void render(ScreenOverlay* _overlay, Vector2& _position);
		
		void setValue(bool _value);
		bool getValue() const;

		void mouseClick(uint _x, uint _y);

		void setGroup(GUIGroupBox* _group);
	};



	inline void GUIRadioButton::setValue(bool _value)
	{
		m_bValue = _value;
	}
//----------------------------------------------------------------------
	inline bool GUIRadioButton::getValue() const
	{
		return m_bValue;
	}
//----------------------------------------------------------------------
	inline void GUIRadioButton::setGroup(GUIGroupBox* _group)
	{
		m_pGroup = _group;
	}
//----------------------------------------------------------------------
}


#endif