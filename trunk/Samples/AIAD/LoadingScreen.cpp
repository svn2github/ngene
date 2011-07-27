/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LoadingScreen.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "LoadingScreen.h"

using nGENE::byte;


#include "App.h"
#include "Engine.h"
#include "FileManager.h"
#include "IFile.h"


LoadingScreen::LoadingScreen():
	GUIWindow(WND_TRANSPARENT),
	m_bDir(false)
{
	this->setWidth(800);
	this->setHeight(600);
	this->setPosition(0, 0);
	this->setVisible(true);
	this->onPaint.bind(this, &LoadingScreen::onWindowPaint);
}
//----------------------------------------------------------------------
void LoadingScreen::addControls()
{
	// Load tips
	FileManager& mgr = FileManager::getSingleton();
	IFile* pFile = mgr.openFile(L"tips.txt", OPEN_READ, FT_UNICODE);

	vector <wstring> vTexts;
	while(!pFile->eof())
	{
		wstring stLine = pFile->getLine();
		if(stLine == L"")
			break;

		vTexts.push_back(stLine);
	}

	mgr.closeFile(pFile->getFileName());

	wstringstream tip;
	tip << L"Tip: " << vTexts[rand() % vTexts.size()];
	lblTip.setCaption(tip.str());
	lblTip.setPosition(Vector2(50, 20));
	lblTip.setFontSize(20);
	lblTip.setWidth(700);
	lblTip.setTextAlignment(TEXT_CENTRE);
	Colour clr = Colour::COLOUR_WHITE;
	lblTip.setTextColour(clr);

	lblLoading.setCaption(L"LOADING...");
	lblLoading.setPosition(Vector2(0, 520));
	lblLoading.setWidth(800);
	lblLoading.setTextAlignment(TEXT_CENTRE);
	clr = Colour::COLOUR_WHITE;
	lblLoading.setTextColour(clr);
	lblLoading.setFontSize(32);

	imgScreen.setWidth(700);
	imgScreen.setHeight(400);
	imgScreen.setImage(L"loading_screen_01.jpg", L"loading_screen_01.jpg");
	imgScreen.setPosition(50, 70);

	this->addControl(&lblTip);
	this->addControl(&lblLoading);
	this->addControl(&imgScreen);

	m_nPrevTime = Engine::getSingleton().getTimer().getMilliseconds();
}
//----------------------------------------------------------------------
void LoadingScreen::onWindowPaint()
{
	dword now = Engine::getSingleton().getTimer().getMilliseconds();
	dword diff = now - m_nPrevTime;

	m_nPassed += diff;
	if(m_nPassed > 1000)
	{
		m_nPassed = 0;
		m_bDir = m_bDir ? false : true;
	}
	else
	{
		Colour clr = lblLoading.getTextColour();
		nGENE::byte alpha = 0;
		if(m_bDir)
			alpha = Maths::lerp(0, 255, (float)m_nPassed / 1000.0f);
		else
			alpha = Maths::lerp(255, 0, (float)m_nPassed / 1000.0f);
		clr.setAlpha(alpha);

		lblLoading.setTextColour(clr);
	}

	m_nPrevTime = now;
}
//----------------------------------------------------------------------