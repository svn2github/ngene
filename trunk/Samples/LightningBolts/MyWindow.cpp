/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyWindow.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "MyWindow.h"


MyWindow::MyWindow():
	GUIWindow(WND_TRANSPARENT)
{
	this->setWidth(700);
	this->setHeight(500);
	this->setPosition(50, 50);
	this->setVisible(true);

	addControls();
}
//----------------------------------------------------------------------
void MyWindow::addControls()
{
	lblMessage.setCaption(L"Lightning bolts demo:\n - press 'H' to hide this menu\n - press 'J' to change bolt colour\n - use mouse wheel to zoom in and out\n - move mouse to rotate around the scene\n - press 'Q', 'W' to change bolt speed\n - 'A', 'S' change offset");
	lblMessage.setPosition(Vector2(0, 0));
	lblMessage.setFontSize(24);
	lblMessage.setTextColour(Colour::COLOUR_WHITE);

	this->addControl(&lblMessage);
}
//----------------------------------------------------------------------