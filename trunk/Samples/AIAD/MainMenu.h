/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MainMenu.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once


#include "GUIImage.h"
#include "GUIStatic.h"
#include "GUIWindow.h"
#include "GUIListBox.h"
#include "GUITextField.h"


using nGENE::GUIImage;
using nGENE::GUIStatic;
using nGENE::GUIWindow;
using nGENE::GUIButton;
using nGENE::GUIListBox;
using nGENE::GUITextField;
using nGENE::uint;



class MainMenu: public GUIWindow
{
private:
	GUIButton btnStartGame;
	GUIButton btnHiScores;
	GUIButton btnOptions;
	GUIButton btnQuitGame;
	GUIImage imgBackground;
	GUIListBox lstHiScores;
	GUIStatic lblList;


	void onWindowPaint();

	void onBtnStartGameClick(uint _x, uint _y);
	void onBtnHiScoresClick(uint _x, uint _y);
	void onBtnOptionsClick(uint _x, uint _y);
	void onBtnQuitGameClick(uint _x, uint _y);

	void onBtnStartGameOver(uint _x, uint _y);

public:
	MainMenu();

	void addControls();
};