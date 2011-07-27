/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUICursor.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUICURSOR_H_
#define __INC_GUICURSOR_H_


#include "GUIControl.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** GUICursor is used to override default OS' cursor.
	*/
	class nGENEDLL GUICursor: public GUIControl
	{
	private:

	public:
		GUICursor();
		~GUICursor();

		void render(ScreenOverlay* _overlay, Vector2& _position);
	};
}


#endif