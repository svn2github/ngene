/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		OptionsWindow.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "OptionsWindow.h"

using nGENE::byte;

#include "PlayerCharacter.h"
#include "Engine.h"
#include "TextureDX9.h"
#include "TextureManager.h"
#include "App.h"
#include "NodeSound.h"
#include "ISound.h"


OptionsWindow::OptionsWindow():
	GUIWindow(WND_TRANSPARENT)
{
	this->setWidth(800);
	this->setHeight(600);
	this->setPosition(0, 0);
	this->setVisible(true);
	this->onPaint.bind(this, &OptionsWindow::onWindowPaint);
}
//----------------------------------------------------------------------
void OptionsWindow::addControls()
{
	btnBack.setWidth(220);
	btnBack.setHeight(50);
	btnBack.setCaption(L"Back");
	btnBack.setPosition(550, 520);
	btnBack.onClick.bind(this, &OptionsWindow::onBtnBackClick);
	btnBack.setVerticalAlignment(CVA_MIDDLE);
	btnBack.setTextAlignment(TEXT_CENTRE);
	btnBack.onMouseRollIn.bind(this, &OptionsWindow::onBtnBackOver);
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
	imgBackground.setImage(L"background_options", L"options.png");
	imgBackground.setPosition(0, 0);

	lblList.setCaption(L"Player name:");
	lblList.setPosition(Vector2(30, 220));
	lblList.setWidth(800);
	lblList.setTextAlignment(TEXT_LEFT);
	clr = Colour::COLOUR_WHITE;
	lblList.setTextColour(clr);
	lblList.setFontSize(40);
	lblList.setEnabled(false);

	txtPlayerName.setText(L"Player");
	txtPlayerName.setPosition(Vector2(300, 220));
	txtPlayerName.setSize(300, 32);
	txtPlayerName.setMaxCharacters(8);
	txtPlayerName.setMultiline(false);
	txtPlayerName.onChange.bind(this, &OptionsWindow::onTxtPlayerNameChange);
	txtPlayerName.setTextColour(clr);
	clr = Colour::COLOUR_BLACK;
	txtPlayerName.setEditTextColour(clr);
	clr.setAlpha(0);
	txtPlayerName.setColour(clr);
	txtPlayerName.setFontSize(40);


	this->addControl(&imgBackground);
	this->addControl(&btnBack);
	this->addControl(&lblList);
	this->addControl(&txtPlayerName);
}
//----------------------------------------------------------------------
void OptionsWindow::onWindowPaint()
{
}
//----------------------------------------------------------------------
void OptionsWindow::onBtnBackClick(uint _x, uint _y)
{
	this->hide();
	App::getSingleton().getMainMenu()->show();
}
//----------------------------------------------------------------------
void OptionsWindow::onBtnBackOver(uint _x, uint _y)
{
	App::getSingleton().getMenuSound()->getSound()->play();
}
//----------------------------------------------------------------------
void OptionsWindow::onTxtPlayerNameChange()
{
	App::getSingleton().setPlayerName(txtPlayerName.getText());
	App::getSingleton().getKeySound()->getSound()->play();
}
//----------------------------------------------------------------------