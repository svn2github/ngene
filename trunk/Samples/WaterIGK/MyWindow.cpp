/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyWindow.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "MyWindow.h"

using nGENE::byte;

#include "Renderer.h"
#include "RendererTypes.h"
#include "ITexture.h"
#include "TextureManager.h"


MyWindow::MyWindow():
	GUIWindow(WND_TRANSPARENT)
{
	this->setWidth(750);
	this->setHeight(550);
	this->setPosition(50, 50);
	this->setVisible(true);

	addControls();
}
//----------------------------------------------------------------------
void MyWindow::addControls()
{
	lblMessage.setCaption(L"Tropical Island Demo:\n - press 'H' to hide this menu\n - press 'W', 'S', 'A', 'D' to move camera\n - press 'F1' to 'F12' to apply different scene effects\n - press '+', '-' to change time of day\n - press 'N' to change water preset\n - press 'Z' to toggle shadows\n - press 'X' to toggle HDR\n - press 'G' to toggle grass\n - press 'PG UP' and 'PG DOWN' to move water up and down");
	lblMessage.setPosition(Vector2(0, 0));
	lblMessage.setFontSize(18);
	lblMessage.setTextColour(Colour::COLOUR_WHITE);

	this->addControl(&lblMessage);
}
//----------------------------------------------------------------------
SplashScreen::SplashScreen():
	GUIWindow(WND_TRANSPARENT)
{
	this->setWidth(Renderer::getSingleton().getDisplayMode().width);
	this->setHeight(Renderer::getSingleton().getDisplayMode().height);
	this->setPosition(0, 0);
	this->setVisible(true);

	addControls();
}
//----------------------------------------------------------------------
void SplashScreen::addControls()
{
	ITexture* image = TextureManager::getSingleton().createTextureFromFile(L"Splash", L"LoadingScreen.jpg");
	imgLoading.setWidth(Renderer::getSingleton().getDisplayMode().width);
	imgLoading.setHeight(Renderer::getSingleton().getDisplayMode().height);
	imgLoading.setImage(image);
	imgLoading.setPosition(0, 0);

	this->addControl(&imgLoading);
}
//----------------------------------------------------------------------