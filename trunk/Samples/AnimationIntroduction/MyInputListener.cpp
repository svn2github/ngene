/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyInputListener.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "MyInputListener.h"
#include "nGENE.h"

#include "App.h"

#include <sstream>

using namespace nGENE;


MyInputListener::MyInputListener():
	m_pApp(NULL),
	m_BoxesNum(0),
	m_Angle1(0.0f),
	m_Angle2(0.0f)
{
}
//----------------------------------------------------------------------
MyInputListener::~MyInputListener()
{
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const MouseEvent& _evt)
{
	float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_prev) * 0.001f;

	CameraThirdPerson* pCamera = m_pApp->getCamera();
	if(_evt.type == MET_MOVE_X || _evt.type == MET_MOVE_Y)
	{
		if(_evt.type == MET_MOVE_X)
			m_Angle1 += _evt.fvalue;
		else if(_evt.type == MET_MOVE_Y)
			m_Angle2 += _evt.fvalue;

		Quaternion quatRot1(Vector3(0.0f, 1.0f, 0.0f), m_Angle1);
		Quaternion quatRot2(Vector3(1.0f, 0.0f, 0.0f), m_Angle2);

		pCamera->setRotation(quatRot1 * quatRot2);
	}

	if(_evt.type == MET_MOVE_Z)
	{
		Real val = pCamera->getDistanceFromTarget() - _evt.fvalue * 1.0f;
		Maths::clamp <Real>(val, 5.0f, 475.0f);

		pCamera->setDistanceFromTarget(val);
	}

	m_prev = Engine::getSingleton().getTimer().getMilliseconds();
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const KeyboardEvent& _evt)
{
	if(_evt.isKeyPressed(KC_ESCAPE))
	{
		m_pApp->closeApplication();
		return;
	}

	if(_evt.isKeyPressed(KC_P))
	{
		m_pApp->getSkeletalAnimation()->togglePauseAnimation();
		m_pApp->getPositionController()->togglePauseAnimation();
	}

	static float angle;
	static float pos;

	if(_evt.isKeyDown(KC_Z))
		angle += 0.1f;
	if(_evt.isKeyDown(KC_X))
		angle -= 0.1f;

	/*Vector3 movement;
	if(_evt.isKeyDown(KC_UP))
		movement.z = 1.0f;
	if(_evt.isKeyDown(KC_DOWN))
		movement.z = -1.0f;
	if(_evt.isKeyDown(KC_LEFT))
		movement.x = -1.0f;
	if(_evt.isKeyDown(KC_RIGHT))
		movement.x = 1.0f;

	m_pApp->getCamera()->strafe(movement.x);
	m_pApp->getCamera()->move(movement.z);*/

	// Hide/show menu
	if(_evt.isKeyPressed(KC_H))
	{
		if(GUIManager::getSingleton().getWindow(1)->isVisible())
			GUIManager::getSingleton().getWindow(1)->setVisible(false);
		else
			GUIManager::getSingleton().getWindow(1)->setVisible(true);
	}
}
//----------------------------------------------------------------------