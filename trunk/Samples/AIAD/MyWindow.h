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


#include "GUIImage.h"
#include "GUIStatic.h"
#include "GUIWindow.h"
#include "GUIProgressBar.h"
#include "Colour.h"


using nGENE::GUIImage;
using nGENE::GUIStatic;
using nGENE::GUIWindow;
using nGENE::GUIProgressBar;
using nGENE::uint;
using nGENE::Colour;



class MyWindow: public GUIWindow
{
private:
	GUIStatic lblMessage;
	GUIStatic lblPoints;
	GUIStatic lblHiScores;
	GUIImage imgWeapon;
	GUIImage imgNoWeapon;
	GUIProgressBar prgHealth;

	uint m_nPassedTime;
	uint m_nPrevTime;
	uint m_nSlideInTime;
	uint m_nStayTime;
	uint m_nSlideOutTime;

	uint m_nPassedTimeHiScore;
	uint m_nPrevTimeHiScore;

	bool m_bMessageSet;
	bool m_bMessageHide;
	bool m_bHiScoreSet;
	bool m_bHiScoreHide;


	void onWindowPaint();

public:
	MyWindow();

	void addControls();

	void setMessage(const wstring& _text, Colour _clr=Colour::COLOUR_WHITE);
	void setWeaponImage(const wstring& _icon);
	void setNoAmmoImageEnabled(bool _value);

	void setHiScore(wstring& _hiscore);
};