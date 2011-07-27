/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIControlContainer.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUICONTROLCONTAINER_H_
#define __INC_GUICONTROLCONTAINER_H_


#include "GUIControl.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** It is control container.
		@par
			Each gui widget which contains other controls
			should derive from this class. These involves
			for instance windows or tabs.
	*/
	class nGENEDLL GUIControlContainer: public GUIControl
	{
	protected:
		GUIControl* m_pFocusedControl;				///< A control having focus.

		vector <GUIControl*> m_vControls;			///< Controls belonging to the window


		void setFocusedControl(GUIControl* _control);

	public:
		GUIControlContainer();
		virtual ~GUIControlContainer();

		/// Adds new control to the container.
		virtual void addControl(GUIControl* _control);

		virtual void changeResolution(uint _oldWidth, uint _oldHeight,
			uint _width, uint _height);

		/** Sets opacity of the control.
			@par
				For control container opacity of all children
				is also changed.
			@remarks
				It doesn't change the control colour.
		*/
		virtual void setOpacity(float _opacity);

		/// Sorts all controls by their tab index value.
		virtual void sortByTabIndex();

		virtual void render(ScreenOverlay* _overlay, Vector2& _position);
	};
}


#endif