/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MainMenu.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "MainMenu.h"

using nGENE::byte;

#include "PlayerCharacter.h"
#include "Engine.h"
#include "TextureDX9.h"
#include "TextureManager.h"
#include "App.h"
#include "NodeSound.h"
#include "ISound.h"


MainMenu::MainMenu():
	GUIWindow(WND_TRANSPARENT)
{
	this->setWidth(800);
	this->setHeight(600);
	this->setPosition(0, 0);
	this->setVisible(true);
	this->onPaint.bind(this, &MainMenu::onWindowPaint);
}
//----------------------------------------------------------------------
void MainMenu::addControls()
{
	btnStartGame.setWidth(220);
	btnStartGame.setHeight(50);
	btnStartGame.setCaption(L"Start game");
	btnStartGame.setPosition(290, 220);
	btnStartGame.onClick.bind(this, &MainMenu::onBtnStartGameClick);
	btnStartGame.setVerticalAlignment(CVA_MIDDLE);
	btnStartGame.setTextAlignment(TEXT_CENTRE);
	btnStartGame.onMouseRollIn.bind(this, &MainMenu::onBtnStartGameOver);
	Colour clr;
	clr.setAlpha(0);
	Colour clrBase = Colour::COLOUR_BLACK;
	Colour clrHighlighted = Colour::COLOUR_WHITE;
	btnStartGame.setColour(clr);
	btnStartGame.setMouseOverColour(clr);
	btnStartGame.setMouseTextOverColour(clrHighlighted);
	btnStartGame.setTextColour(clrBase);
	btnStartGame.setFontSize(40);

	btnHiScores.setWidth(220);
	btnHiScores.setHeight(50);
	btnHiScores.setCaption(L"Hi-Scores");
	btnHiScores.setPosition(290, 290);
	btnHiScores.onClick.bind(this, &MainMenu::onBtnHiScoresClick);
	btnHiScores.setVerticalAlignment(CVA_MIDDLE);
	btnHiScores.setTextAlignment(TEXT_CENTRE);
	btnHiScores.setColour(clr);
	btnHiScores.onMouseRollIn.bind(this, &MainMenu::onBtnStartGameOver);
	btnHiScores.setColour(clr);
	btnHiScores.setMouseOverColour(clr);
	btnHiScores.setMouseTextOverColour(clrHighlighted);
	btnHiScores.setTextColour(clrBase);
	btnHiScores.setFontSize(40);

	btnOptions.setWidth(220);
	btnOptions.setHeight(50);
	btnOptions.setCaption(L"Options");
	btnOptions.setPosition(290, 360);
	btnOptions.onClick.bind(this, &MainMenu::onBtnOptionsClick);
	btnOptions.setVerticalAlignment(CVA_MIDDLE);
	btnOptions.setTextAlignment(TEXT_CENTRE);
	btnOptions.setColour(clr);
	btnOptions.onMouseRollIn.bind(this, &MainMenu::onBtnStartGameOver);
	btnOptions.setColour(clr);
	btnOptions.setMouseOverColour(clr);
	btnOptions.setMouseTextOverColour(clrHighlighted);
	btnOptions.setTextColour(clrBase);
	btnOptions.setFontSize(40);

	btnQuitGame.setWidth(220);
	btnQuitGame.setHeight(50);
	btnQuitGame.setCaption(L"Quit game");
	btnQuitGame.setPosition(290, 430);
	btnQuitGame.onClick.bind(this, &MainMenu::onBtnQuitGameClick);
	btnQuitGame.setVerticalAlignment(CVA_MIDDLE);
	btnQuitGame.setTextAlignment(TEXT_CENTRE);
	btnQuitGame.setColour(clr);
	btnQuitGame.onMouseRollIn.bind(this, &MainMenu::onBtnStartGameOver);
	btnQuitGame.setColour(clr);
	btnQuitGame.setMouseOverColour(clr);
	btnQuitGame.setMouseTextOverColour(clrHighlighted);
	btnQuitGame.setTextColour(clrBase);
	btnQuitGame.setFontSize(40);

	imgBackground.setWidth(800);
	imgBackground.setHeight(600);
	imgBackground.setImage(L"background", L"main_menu.png");
	imgBackground.setPosition(0, 0);

	lstHiScores.setWidth(500);
	lstHiScores.setHeight(300);
	lstHiScores.setPosition(100, 150);
	lstHiScores.setHasScrollbar(false);
	clr = Colour::COLOUR_BLUE;
	clr.setAlpha(0);
	lstHiScores.setColour(clr);
	lstHiScores.setMouseOverColour(clr);
	clr = Colour::COLOUR_WHITE;
	lstHiScores.setTextColour(clr);
	lstHiScores.setBorder(false);

	lblList.setCaption(L"Hi-Score");
	lblList.setPosition(Vector2(80, 110));
	lblList.setWidth(200);
	clr = Colour::COLOUR_WHITE;
	lblList.setTextColour(clr);
	lblList.setFontSize(26);


	this->addControl(&imgBackground);
	this->addControl(&btnStartGame);
	this->addControl(&btnHiScores);
	this->addControl(&btnQuitGame);
	this->addControl(&btnOptions);
}
//----------------------------------------------------------------------
void MainMenu::onWindowPaint()
{
}
//----------------------------------------------------------------------
void MainMenu::onBtnStartGameClick(uint _x, uint _y)
{
	App::getSingleton().createMap();
}
//----------------------------------------------------------------------
void MainMenu::onBtnHiScoresClick(uint _x, uint _y)
{
	this->hide();
	App::getSingleton().getHiScores()->show();
}
//----------------------------------------------------------------------
void MainMenu::onBtnQuitGameClick(uint _x, uint _y)
{
	App::getSingleton().closeApplication();
}
//----------------------------------------------------------------------
void MainMenu::onBtnOptionsClick(uint _x, uint _y)
{
	this->hide();
	App::getSingleton().getOptions()->show();
}
//----------------------------------------------------------------------
void MainMenu::onBtnStartGameOver(uint _x, uint _y)
{
	App::getSingleton().getMenuSound()->getSound()->play();
}
//----------------------------------------------------------------------