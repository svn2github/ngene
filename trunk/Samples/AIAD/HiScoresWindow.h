/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		HiScoresWindow.h
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



class HiScoresWindow: public GUIWindow
{
private:
	GUIButton btnBack;
	GUIImage imgBackground;
	GUIListBox lstHiScoresPlayer;
	GUIListBox lstHiScoresPoints;
	GUIStatic lblList;


	void onWindowPaint();

	void onBtnBackClick(uint _x, uint _y);
	void onBtnBackOver(uint _x, uint _y);

public:
	HiScoresWindow();

	void addControls();
	void clearHiScore();
	void addHiScoreItem(const wstring& _item, const wstring& _score);
};