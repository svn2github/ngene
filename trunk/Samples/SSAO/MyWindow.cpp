/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyWindow.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "MyWindow.h"

using nGENE::byte;

#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "ShaderConstant.h"
#include "ShaderInstance.h"


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
	lblMessage.setCaption(L"Screen Space Ambient Occlusion:\n - press 'H' to hide this menu\n - press 'S' to disable SSAO\n - press 'M' to show only SSAO \n - use mouse wheel to zoom in and out\n - move mouse to rotate around the scene\n - press '1', '2' to select scene\n - press 'U', 'I' to change SSAO radius\n - press 'O', 'P' to change SSAO range");
	lblMessage.setPosition(Vector2(0, 0));
	lblMessage.setFontSize(24);
	lblMessage.setTextColour(Colour::COLOUR_WHITE);

	lblRadius.setPosition(Vector2(450, 0));
	lblRadius.setFontSize(20);
	lblRadius.setTextColour(Colour::COLOUR_WHITE);

	lblRange.setPosition(Vector2(450, 30));
	lblRange.setFontSize(20);
	lblRange.setTextColour(Colour::COLOUR_WHITE);

	this->addControl(&lblMessage);
	this->addControl(&lblRadius);
	this->addControl(&lblRange);
}
//----------------------------------------------------------------------
void MyWindow::onWindowPaint()
{
	Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"ssao");
	ShaderInstance* pShader = pMaterial->getActiveRenderTechnique()->getRenderPass(0).getPixelShader();
	float* radius = NULL;
	ShaderConstant* pConstant = pShader->getConstant("radius");
	if(pConstant)
		radius = (float*)pConstant->getDefaultValues();

	float* range = NULL;
	pConstant = pShader->getConstant("range");
	if(pConstant)
		range = (float*)pConstant->getDefaultValues();

	wstringstream temp;
	temp << L"Radius: " << *radius;

	lblRadius.setCaption(temp.str());

	wstringstream temp2;
	temp2 << L"Range: " << *range;
	lblRange.setCaption(temp2.str());
}
//----------------------------------------------------------------------