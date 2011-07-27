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
		Maths::clamp <Real>(val, 5.0f, 75.0f);

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
		PhysicsWorld* pWorld = Physics::getSingleton().getWorld(L"PhysicsWorld");

		MaterialLibrary* pLibrary = MaterialManager::getSingleton().getLibrary(L"default");
		Material* pMaterial = pLibrary->getMaterial(L"crate");

		wostringstream buffer;
		buffer << L"NewBox_" << m_BoxesNum++;

		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		PrefabBox* pBox = sm->createBox(2.0f, 2.0f, 2.0f);
		pBox->setPosition(0.0f, 40.0f, 0.0f);
		Surface* pSurface = pBox->getSurface(L"Base");
		pSurface->setMaterial(pMaterial);
		sm->getRootNode()->addChild(buffer.str(), pBox);

		PhysicsActor* pActor = pWorld->createPhysicsActor(L"Box", Vector3(2.0, 2.0f, 2.0f), PhysicsActor::BT_DYNAMIC);
		pActor->attachNode(pBox);
		pActor->setShapeMaterial(0, *pWorld->getPhysicsMaterial(L"DefaultMaterial"));
		pWorld->addPhysicsActor(buffer.str(), pActor);
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