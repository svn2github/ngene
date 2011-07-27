/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyWindow.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "MyWindow.h"

#include <sstream>


MyWindow::MyWindow():
	GUIWindow(WND_TRANSPARENT)
{
	this->setWidth(700);
	this->setHeight(500);
	this->setPosition(50, 50);
	this->setVisible(true);
	this->onPaint.bind(this, &MyWindow::onWindowPaint);

	addControls();
}
//----------------------------------------------------------------------
void MyWindow::addControls()
{
	lblMessage.setCaption(L"Light Shafts:\n - press 'H' to hide this menu\n - press 'G' to toggle cookie texture\n - press 'S' to toggle shadows\n - press 'N' to toggle noise\n - use mouse wheel to zoom in and out\n - move mouse to rotate around the scene\n - press '1', '2', '3' to select scene\n - press 'T', 'Y' to change constant coefficient\n - press 'U', 'I' to change linear coefficient\n - press 'O', 'P' to change quadratic coefficient\n - press 'R' to reset light and light-shaft");
	lblMessage.setPosition(Vector2(0, 0));
	lblMessage.setFontSize(20);
	lblMessage.setTextColour(Colour::COLOUR_WHITE);

	lblConstant.setPosition(Vector2(450, 0));
	lblConstant.setFontSize(20);
	lblConstant.setTextColour(Colour::COLOUR_WHITE);

	lblLinear.setPosition(Vector2(450, 30));
	lblLinear.setFontSize(20);
	lblLinear.setTextColour(Colour::COLOUR_WHITE);

	lblQuadratic.setPosition(Vector2(450, 60));
	lblQuadratic.setFontSize(20);
	lblQuadratic.setTextColour(Colour::COLOUR_WHITE);

	this->addControl(&lblMessage);
	this->addControl(&lblConstant);
	this->addControl(&lblLinear);
	this->addControl(&lblQuadratic);
}
//----------------------------------------------------------------------
void MyWindow::onWindowPaint()
{
	NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
	float constant = pLight->getConstantAttenuation();
	float linear = pLight->getLinearAttenuation();
	float quadratic = pLight->getQuadraticAttenuation();

	wstringstream temp;
	temp << L"Constant attenuation: " << constant;

	lblConstant.setCaption(temp.str());

	wstringstream temp2;
	temp2 << L"Linear attenuation: " << linear;
	lblLinear.setCaption(temp2.str());

	wstringstream temp3;
	temp3 << L"Quadratic attenuation: " << quadratic;
	lblQuadratic.setCaption(temp3.str());
}
//----------------------------------------------------------------------