/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LevelUpWindow.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once


#include "GUIImage.h"
#include "GUIStatic.h"
#include "GUIWindow.h"


using nGENE::GUIImage;
using nGENE::GUIStatic;
using nGENE::GUIWindow;
using nGENE::GUIButton;
using nGENE::uint;



class LevelUpWindow: public GUIWindow
{
private:
	void onWindowPaint();

public:
	LevelUpWindow();

	void addControls();
};