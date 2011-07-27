/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIGroupBox.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUIGROUPBOX_H_
#define __INC_GUIGROUPBOX_H_


#include "GUIControl.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Group of radio buttons.
	*/
	class nGENEDLL GUIGroupBox: public GUIControl
	{
	private:
		///< Vectors of all radio buttons in this group
		vector <GUIRadioButton*> m_vecRadioButtons;
		///< Currently selected radio button
		GUIRadioButton* m_pActive;

	public:
		GUIGroupBox();
		~GUIGroupBox();

		void addControl(GUIRadioButton* _control);
		void select(GUIRadioButton* _control);

		void render(nGENE::ScreenOverlay *,nGENE::Vector2 &) {}
	};
}


#endif