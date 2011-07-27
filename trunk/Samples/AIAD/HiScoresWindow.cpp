/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		HiScoresWindow.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "HiScoresWindow.h"

using nGENE::byte;

#include "PlayerCharacter.h"
#include "Engine.h"
#include "TextureDX9.h"
#include "TextureManager.h"
#include "App.h"
#include "NodeSound.h"
#include "ISound.h"


HiScoresWindow::HiScoresWindow():
	GUIWindow(WND_TRANSPARENT)
{
	this->setWidth(800);
	this->setHeight(600);
	this->setPosition(0, 0);
	this->setVisible(true);
	this->onPaint.bind(this, &HiScoresWindow::onWindowPaint);
}
//----------------------------------------------------------------------
void HiScoresWindow::addControls()
{
	btnBack.setWidth(220);
	btnBack.setHeight(50);
	btnBack.setCaption(L"Back");
	btnBack.setPosition(550, 520);
	btnBack.onClick.bind(this, &HiScoresWindow::onBtnBackClick);
	btnBack.setVerticalAlignment(CVA_MIDDLE);
	btnBack.setTextAlignment(TEXT_CENTRE);
	btnBack.onMouseRollIn.bind(this, &HiScoresWindow::onBtnBackOver);
	Colour clr;
	clr.setAlpha(0);
	Colour clrBase = Colour::COLOUR_BLACK;
	Colour clrHighlighted = Colour::COLOUR_WHITE;
	btnBack.setColour(clr);
	btnBack.setMouseOverColour(clr);
	btnBack.setMouseTextOverColour(clrHighlighted);
	btnBack.setTextColour(clrBase);
	btnBack.setFontSize(40);

	imgBackground.setWidth(800);
	imgBackground.setHeight(600);
	imgBackground.setImage(L"background_hiscores", L"hiscores.png");
	imgBackground.setPosition(0, 0);

	lstHiScoresPlayer.setWidth(200);
	lstHiScoresPlayer.setHeight(300);
	lstHiScoresPlayer.setPosition(290, 270);
	lstHiScoresPlayer.setHasScrollbar(false);
	clr = Colour::COLOUR_BLUE;
	clr.setAlpha(0);
	lstHiScoresPlayer.setColour(clr);
	lstHiScoresPlayer.setMouseOverColour(clr);
	clr = Colour::COLOUR_WHITE;
	lstHiScoresPlayer.setTextColour(clr);
	lstHiScoresPlayer.setBorder(false);
	lstHiScoresPlayer.setEnabled(false);
	lstHiScoresPlayer.setFontSize(24);

	lstHiScoresPoints.setWidth(300);
	lstHiScoresPoints.setHeight(300);
	lstHiScoresPoints.setPosition(440, 270);
	lstHiScoresPoints.setHasScrollbar(false);
	clr = Colour::COLOUR_BLUE;
	clr.setAlpha(0);
	lstHiScoresPoints.setColour(clr);
	lstHiScoresPoints.setMouseOverColour(clr);
	clr = Colour::COLOUR_WHITE;
	lstHiScoresPoints.setTextColour(clr);
	lstHiScoresPoints.setBorder(false);
	lstHiScoresPoints.setEnabled(false);
	lstHiScoresPoints.setFontSize(24);


	lblList.setCaption(L"Hi-Score");
	lblList.setPosition(Vector2(0, 220));
	lblList.setWidth(800);
	lblList.setTextAlignment(TEXT_CENTRE);
	clr = Colour::COLOUR_WHITE;
	lblList.setTextColour(clr);
	lblList.setFontSize(40);
	lblList.setEnabled(false);

	this->addControl(&imgBackground);
	this->addControl(&btnBack);
	this->addControl(&lstHiScoresPlayer);
	this->addControl(&lstHiScoresPoints);
	this->addControl(&lblList);
}
//----------------------------------------------------------------------
void HiScoresWindow::onWindowPaint()
{
}
//----------------------------------------------------------------------
void HiScoresWindow::onBtnBackClick(uint _x, uint _y)
{
	this->hide();
	App::getSingleton().getMainMenu()->show();
}
//----------------------------------------------------------------------
void HiScoresWindow::addHiScoreItem(const wstring& _player, const wstring& _score)
{
	lstHiScoresPlayer.addItem(_player);
	lstHiScoresPoints.addItem(_score);
}
//----------------------------------------------------------------------
void HiScoresWindow::clearHiScore()
{
	lstHiScoresPlayer.clear();
	lstHiScoresPoints.clear();
}
//----------------------------------------------------------------------
void HiScoresWindow::onBtnBackOver(uint _x, uint _y)
{
	App::getSingleton().getMenuSound()->getSound()->play();
}
//----------------------------------------------------------------------