/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyInputListener.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "MyInputListener.h"
#include "nGENE.h"

#include "App.h"
#include "NodeLightning.h"

#include <sstream>

using namespace nGENE;
using nGENE::Nature::NodeLightning;


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
		Maths::clamp <Real>(val, 5.0f, 275.0f);

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

	if(_evt.isKeyPressed(KC_J))
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		NodeLightning* pBolt = (NodeLightning*)sm->getNode(L"Bolt");

		Colour col = pBolt->getColour();
		if(col == Colour(128, 255, 128))
			col.setRGBA(255, 192, 255);
		else if(col == Colour(255, 192, 255))
			col.setRGBA(192, 255, 255);
		else if(col == Colour(192, 255, 255))
			col.setRGBA(255, 255, 128);
		else if(col == Colour(255, 255, 128))
			col.setRGBA(128, 255, 128);

		pBolt->setColour(col);
	}

	// Change bolt update frequency
	if(_evt.isKeyDown(KC_Q))
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		NodeLightning* pBolt = (NodeLightning*)sm->getNode(L"Bolt");

		long frequency = pBolt->getUpdateFrequency();
		frequency -= 5;
		Maths::clamp <long>(frequency, 30, 2000);
		pBolt->setUpdateFrequency(frequency);
	}

	if(_evt.isKeyDown(KC_W))
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		NodeLightning* pBolt = (NodeLightning*)sm->getNode(L"Bolt");

		long frequency = pBolt->getUpdateFrequency();
		frequency += 5;
		Maths::clamp <long>(frequency, 30, 2000);
		pBolt->setUpdateFrequency(frequency);
	}

	// Change bolt offset
	if(_evt.isKeyDown(KC_A))
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		NodeLightning* pBolt = (NodeLightning*)sm->getNode(L"Bolt");

		float offset = pBolt->getOffset();
		offset -= 0.1f;
		Maths::clamp(offset, 1.0f, 50.0f);
		pBolt->setOffset(offset);
	}

	if(_evt.isKeyDown(KC_S))
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		NodeLightning* pBolt = (NodeLightning*)sm->getNode(L"Bolt");

		float offset = pBolt->getOffset();
		offset += 0.1f;
		Maths::clamp(offset, 1.0f, 50.0f);
		pBolt->setOffset(offset);
	}

	if(_evt.isKeyPressed(KC_H))
	{
		if(GUIManager::getSingleton().getWindow(1)->isVisible())
			GUIManager::getSingleton().getWindow(1)->setVisible(false);
		else
			GUIManager::getSingleton().getWindow(1)->setVisible(true);
	}
}
//----------------------------------------------------------------------