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
	lblMessage.setCaption(L"2D Non Photorealistic Rendering:\n - press 'H' to hide this menu\n - press 'L' to disable lighting\n - press 'M' to switch materials \n - press arrow keys to move camera \n - press '+' to zoom in\n - press '-' to zoom out\n - press 'Z' and 'X' to rotate camera");
	lblMessage.setPosition(Vector2(0, 0));
	lblMessage.setFontSize(24);
	lblMessage.setTextColour(Colour::COLOUR_WHITE);

	this->addControl(&lblMessage);
}
//----------------------------------------------------------------------