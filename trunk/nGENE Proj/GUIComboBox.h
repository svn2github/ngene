/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIComboBox.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUICOMBOBOX_H_
#define __INC_GUICOMBOBOX_H_


#include "GUIListBox.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Combo box control.
	*/
	class nGENEDLL GUIComboBox: public GUIListBox
	{
	private:
		bool m_bOpened;
	
	public:
		GUIComboBox();
		~GUIComboBox();

		void render(ScreenOverlay* _overlay, Vector2& _position);

		void setHeight(Real _height);

		void setPosition(const Vector2& _position);
		void setPosition(Real _x, Real _y);

		void mouseClick(uint _x, uint _y);
		/*void mouseDown(uint _x, uint _y);
		void mouseRelease(uint _x, uint _y);
		void mouseOver(uint _x, uint _y);*/

		void keyboardDown(const KeyboardEvent& _evt);

		void loosingFocus();
	};
}


#endif