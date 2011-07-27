/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyWindow.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once


#include "NodeLight.h"
#include "GUIStatic.h"
#include "GUIWindow.h"


using namespace nGENE;


class MyWindow: public GUIWindow
{
private:
	GUIStatic lblMessage;

	GUIStatic lblConstant;
	GUIStatic lblLinear;
	GUIStatic lblQuadratic;

	void onWindowPaint();

public:
	MyWindow();

	void addControls();
};