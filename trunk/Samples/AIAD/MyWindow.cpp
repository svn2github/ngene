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

using nGENE::byte;

#include "PlayerCharacter.h"
#include "Engine.h"
#include "TextureDX9.h"
#include "TextureManager.h"
#include "App.h"
#include "NodeSound.h"
#include "ISound.h"


MyWindow::MyWindow():
	GUIWindow(WND_TRANSPARENT),
	m_nPassedTime(0),
	m_bMessageSet(false),
	m_bMessageHide(false),
	m_bHiScoreSet(false),
	m_bHiScoreHide(false),
	m_nPrevTime(0),
	m_nSlideInTime(500),
	m_nStayTime(1500),
	m_nSlideOutTime(1000),
	m_nPassedTimeHiScore(0),
	m_nPrevTimeHiScore(0)
{
	this->setWidth(800);
	this->setHeight(600);
	this->setPosition(0, 0);
	this->setVisible(true);
	this->onPaint.bind(this, &MyWindow::onWindowPaint);
}
//----------------------------------------------------------------------
void MyWindow::addControls()
{
	lblMessage.setCaption(L"Messages go here");
	lblMessage.setPosition(Vector2(0, 60));
	lblMessage.setFontSize(24);
	lblMessage.setWidth(800);
	Colour clr = Colour::COLOUR_WHITE;
	clr.setAlpha(0);
	lblMessage.setTextColour(clr);
	lblMessage.setTextAlignment(TEXT_CENTRE);

	lblHiScores.setCaption(L"YOU REACHED HI-SCORE #1");
	lblHiScores.setPosition(Vector2(0, 265));
	lblHiScores.setFontSize(24);
	lblHiScores.setWidth(800);
	lblHiScores.setTextColour(clr);
	lblHiScores.setTextAlignment(TEXT_CENTRE);
	lblHiScores.setFontSize(70);

	lblPoints.setCaption(L"0");
	lblPoints.setPosition(Vector2(700, 20));
	lblPoints.setFontSize(32);
	lblPoints.setTextColour(Colour::COLOUR_WHITE);
	lblPoints.setTextAlignment(TEXT_RIGHT);
	lblPoints.setWidth(80);

	imgWeapon.setWidth(64);
	imgWeapon.setHeight(64);
	imgWeapon.setImage(L"icon_one_shot.png", L"icon_one_shot.png");
	imgWeapon.setPosition(15, 15);

	imgNoWeapon.setWidth(64);
	imgNoWeapon.setHeight(64);
	imgNoWeapon.setImage(L"no_ammo.png", L"no_ammo.png");
	imgNoWeapon.setPosition(15, 15);
	imgNoWeapon.setEnabled(false);
	imgNoWeapon.setVisible(false);

	prgHealth.setWidth(500);
	prgHealth.setHeight(24);
	prgHealth.setPosition(175, 25);
	prgHealth.setMin(0.0f);
	prgHealth.setMax(1.0f);
	prgHealth.setBorder(false);
	Colour clrHealth = Colour::COLOUR_GREEN;
	prgHealth.setColour(clrHealth);


	this->addControl(&prgHealth);
	this->addControl(&lblMessage);
	this->addControl(&lblPoints);
	this->addControl(&lblHiScores);
	this->addControl(&imgWeapon);
	this->addControl(&imgNoWeapon);
}
//----------------------------------------------------------------------
void MyWindow::onWindowPaint()
{
	PlayerCharacter& player = PlayerCharacter::getSingleton();

	float fHealth = (float)player.getCurrentHP() / (float)player.getMaxHP();
	prgHealth.setValue(fHealth);
	fHealth = 1.0f - fHealth;
	Colour clrHealth = Colour::COLOUR_GREEN;
	Colour clrDeath = Colour::COLOUR_RED;
	nGENE::byte r = Maths::lerp(clrHealth.getRed(), clrDeath.getRed(), fHealth);
	nGENE::byte g = Maths::lerp(clrHealth.getGreen(), clrDeath.getGreen(), fHealth);
	nGENE::byte b = Maths::lerp(clrHealth.getBlue(), clrDeath.getBlue(), fHealth);
	clrHealth.setRGBA(r, g, b);
	prgHealth.setColour(clrHealth);

	wstringstream temp;
	temp << player.getPoints();
	lblPoints.setCaption(temp.str());

	uint now = Engine::getSingleton().getTimer().getMilliseconds();
	

	uint diff = now - m_nPrevTimeHiScore;
	if(m_bHiScoreSet && lblHiScores.getTextColour().getAlpha() < 200)
	{
		m_nPassedTimeHiScore += diff;
		if(m_nPassedTimeHiScore > m_nSlideInTime)
			m_nPassedTimeHiScore = m_nSlideInTime;
		nGENE::byte alpha = Maths::lerp <nGENE::byte>(0, 200, (float)m_nPassedTimeHiScore / (float)m_nSlideInTime);
		lblHiScores.getTextColour().setAlpha(alpha);

		if(alpha == 200)
			m_nPassedTimeHiScore = 0;
	}
	else if(m_bHiScoreSet && lblHiScores.getTextColour().getAlpha() == 200 && !m_bHiScoreHide)
	{
		m_nPassedTimeHiScore += diff;
		if(m_nPassedTimeHiScore > m_nStayTime)
		{
			m_nPassedTimeHiScore = 0;
			m_bHiScoreHide = true;
			m_bHiScoreSet = false;
		}
	}
	else if(m_bHiScoreHide)
	{
		m_nPassedTimeHiScore += diff;
		if(m_nPassedTimeHiScore > m_nSlideOutTime)
			m_nPassedTimeHiScore = m_nSlideOutTime;
		nGENE::byte alpha = Maths::lerp <nGENE::byte>(200, 0, (float)m_nPassedTimeHiScore / (float)m_nSlideOutTime);
		lblHiScores.getTextColour().setAlpha(alpha);

		if(alpha == 0)
		{
			m_nPassedTimeHiScore = 0;
			m_bHiScoreHide = false;

			App::getSingleton().showMsgBox();
		}
	}



	diff = now - m_nPrevTime;
	if(m_bMessageSet && lblMessage.getTextColour().getAlpha() < 255)
	{
		m_nPassedTime += diff;
		if(m_nPassedTime > m_nSlideInTime)
			m_nPassedTime = m_nSlideInTime;
		nGENE::byte alpha = Maths::lerp <nGENE::byte>(0, 255, (float)m_nPassedTime / (float)m_nSlideInTime);
		lblMessage.getTextColour().setAlpha(alpha);

		if(alpha == 255)
			m_nPassedTime = 0;
	}
	else if(m_bMessageSet && lblMessage.getTextColour().getAlpha() == 255 && !m_bMessageHide)
	{
		m_nPassedTime += diff;
		if(m_nPassedTime > m_nStayTime)
		{
			m_nPassedTime = 0;
			m_bMessageHide = true;
			m_bMessageSet = false;
		}
	}
	else if(m_bMessageHide)
	{
		m_nPassedTime += diff;
		if(m_nPassedTime > m_nSlideOutTime)
			m_nPassedTime = m_nSlideOutTime;
		nGENE::byte alpha = Maths::lerp <nGENE::byte>(255, 0, (float)m_nPassedTime / (float)m_nSlideOutTime);
		lblMessage.getTextColour().setAlpha(alpha);

		if(alpha == 0)
		{
			m_nPassedTime = 0;
			m_bMessageHide = false;
		}
	}

	m_nPrevTime = now;
	m_nPrevTimeHiScore = now;
}
//----------------------------------------------------------------------
void MyWindow::setMessage(const wstring& _text, Colour _clr)
{
	lblMessage.setText(_text);
	lblMessage.setTextColour(_clr);
	lblMessage.getTextColour().setAlpha(0);

	m_nPassedTime = 0;
	m_nPrevTime = Engine::getSingleton().getTimer().getMilliseconds();
	m_bMessageSet = true;
	m_bMessageHide = false;
}
//----------------------------------------------------------------------
void MyWindow::setWeaponImage(const wstring& _icon)
{
	if(App::getSingleton().getSwitchSound())
		App::getSingleton().getSwitchSound()->getSound()->play();

	imgWeapon.setImage(_icon, _icon);
}
//----------------------------------------------------------------------
void MyWindow::setNoAmmoImageEnabled(bool _value)
{
	imgNoWeapon.setEnabled(_value);
	imgNoWeapon.setVisible(_value);
}
//----------------------------------------------------------------------
void MyWindow::setHiScore(wstring& _hiscore)
{
	lblHiScores.setText(_hiscore);

	m_nPassedTimeHiScore = 0;
	m_nPrevTimeHiScore = Engine::getSingleton().getTimer().getMilliseconds();

	m_bHiScoreSet = true;
	m_bHiScoreHide = false;
}
//----------------------------------------------------------------------