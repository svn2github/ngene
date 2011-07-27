/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LevelUpWindow.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "LevelUpWindow.h"

using nGENE::byte;

#include "PlayerCharacter.h"
#include "Engine.h"
#include "TextureDX9.h"
#include "TextureManager.h"
#include "App.h"


LevelUpWindow::LevelUpWindow():
	GUIWindow(WND_TRANSPARENT)
{
	this->setWidth(800);
	this->setHeight(600);
	this->setPosition(0, 0);
	this->setVisible(false);
	this->onPaint.bind(this, &LevelUpWindow::onWindowPaint);
}
//----------------------------------------------------------------------
void LevelUpWindow::addControls()
{
	
}
//----------------------------------------------------------------------
void LevelUpWindow::onWindowPaint()
{
}
//----------------------------------------------------------------------