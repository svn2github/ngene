/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LoadingScreen.h
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
using nGENE::dword;


class LoadingScreen: public GUIWindow
{
private:
	GUIStatic lblLoading;
	GUIStatic lblTip;
	GUIImage imgScreen;

	dword m_nPrevTime;
	dword m_nPassed;

	bool m_bDir;
	

	void onWindowPaint();

public:
	LoadingScreen();

	void addControls();
};