/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUICheckBox.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUICHECKBOX_H_
#define __INC_GUICHECKBOX_H_


#include "GUIControl.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Check box control.
	*/
	class nGENEDLL GUICheckBox: public GUIControl
	{
	private:
		bool m_bValue;							///< Value of the check box

	public:
		ON_PAINT_DELEGATE onChange;
	
	public:
		GUICheckBox();
		~GUICheckBox();

		void render(ScreenOverlay* _overlay, Vector2& _position);
		
		void setValue(bool _value);
		bool getValue() const;

		void mouseClick(uint _x, uint _y);
	};



	inline void GUICheckBox::setValue(bool _value)
	{
		m_bValue = _value;
	}
//----------------------------------------------------------------------
	inline bool GUICheckBox::getValue() const
	{
		return m_bValue;
	}
//----------------------------------------------------------------------
	inline void GUICheckBox::mouseClick(uint _x, uint _y)
	{
		GUIControl::mouseClick(_x, _y);

		m_bValue = (m_bValue ? false : true);
		if(onChange)
			onChange();
	}
//----------------------------------------------------------------------
}


#endif