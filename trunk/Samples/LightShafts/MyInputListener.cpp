/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyInputListener.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "MyInputListener.h"
#include "nGENE.h"

#include "App.h"

#include "NodeVolumetric.h"

#include <sstream>

using namespace nGENE;
using nGENE::Nature::NodeVolumetric;


MyInputListener::MyInputListener()
{
	m_Angle1 = 0.0f;
	m_Angle2 = 0.0f;
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const MouseEvent& _evt)
{
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
		Maths::clamp <Real>(val, 5.0f, 150.0f);

		pCamera->setDistanceFromTarget(val);
	}

	m_Prev = Engine::getSingleton().getTimer().getMilliseconds();
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const KeyboardEvent& _evt)
{
	if(_evt.isKeyPressed(KC_ESCAPE))
	{
		m_pApp->closeApplication();
		return;
	}


	// Enable/Disable cookie
	if(_evt.isKeyPressed(KC_G))
	{
		NodeVolumetric* pVol = (NodeVolumetric*)Engine::getSingleton().getSceneManager(0)->getNode(L"Shaft");
		bool use = pVol->isCookieUsed() ? false : true;
		pVol->setCookieUsed(use);
	}

	// Enable/Disable shadows
	if(_evt.isKeyPressed(KC_S))
	{
		NodeVolumetric* pVol = (NodeVolumetric*)Engine::getSingleton().getSceneManager(0)->getNode(L"Shaft");
		bool use = pVol->isShadowUsed() ? false : true;
		pVol->setShadowUsed(use);

		Renderer::getSingleton().getRenderStage(L"Shadows")->setEnabled(use);
	}

	// Enable/Disable noise
	if(_evt.isKeyPressed(KC_N))
	{
		NodeVolumetric* pVol = (NodeVolumetric*)Engine::getSingleton().getSceneManager(0)->getNode(L"Shaft");
		bool use = pVol->isNoiseUsed() ? false : true;
		pVol->setNoiseUsed(use);
	}

	// Change far plane
	if(_evt.isKeyDown(KC_W))
	{
		NodeVolumetric* pVol = (NodeVolumetric*)Engine::getSingleton().getSceneManager(0)->getNode(L"Shaft");
		pVol->setFar(pVol->getFar() - 0.1f);
	}

	if(_evt.isKeyDown(KC_E))
	{
		NodeVolumetric* pVol = (NodeVolumetric*)Engine::getSingleton().getSceneManager(0)->getNode(L"Shaft");
		pVol->setFar(pVol->getFar() + 0.1f);
	}

	// Move light shaft
	if(_evt.isKeyDown(KC_Q))
	{
		NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
		pLight->translate(0.0f, 0.1f, 0.0f);
		Vector3 startPos(0.0f, 3.0f, 10.0f);
		Vector3 dir = startPos - pLight->getPositionLocal();
		dir.normalize();
		pLight->setDirection(dir);
	}

	if(_evt.isKeyDown(KC_A))
	{
		NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
		pLight->translate(0.0f, -0.1f, 0.0f);
		Vector3 startPos(0.0f, 3.0f, 10.0f);
		Vector3 dir = startPos - pLight->getPositionLocal();
		dir.normalize();
		pLight->setDirection(dir);
	}

	if(_evt.isKeyDown(KC_Z))
	{
		NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
		pLight->translate(-0.1f, 0.0f, 0.0f);
		Vector3 startPos(0.0f, 3.0f, 10.0f);
		Vector3 dir = startPos - pLight->getPositionLocal();
		dir.normalize();
		pLight->setDirection(dir);
	}

	if(_evt.isKeyDown(KC_X))
	{
		NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
		pLight->translate(0.1f, 0.0f, 0.0f);
		Vector3 startPos(0.0f, 3.0f, 10.0f);
		Vector3 dir = startPos - pLight->getPositionLocal();
		dir.normalize();
		pLight->setDirection(dir);
	}

	if(_evt.isKeyPressed(KC_R))
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);

		NodeLight* pLight = (NodeLight*)sm->getNode(L"WorldLight");
		pLight->setPosition(0.0f, 3.0f, -4.0f);
		pLight->setDirection(0.0f, 0.0f, 1.0f);
		pLight->setConstantAttenuation(0.1f);
		pLight->setLinearAttenuation(0.05f);
		pLight->setQuadraticAttenuation(0.025f);

		NodeVolumetric* pVol = (NodeVolumetric*)sm->getNode(L"Shaft");
		pVol->setFar(10.0f);
	}

	// Change attenuation coefficients
	if(_evt.isKeyDown(KC_T))
	{
		NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
		pLight->setConstantAttenuation(pLight->getConstantAttenuation() - 0.01f);
		if(pLight->getConstantAttenuation() < 0.0f)
			pLight->setConstantAttenuation(0.0f);
	}

	if(_evt.isKeyDown(KC_Y))
	{
		NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
		pLight->setConstantAttenuation(pLight->getConstantAttenuation() + 0.01f);
	}

	if(_evt.isKeyDown(KC_U))
	{
		NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
		pLight->setLinearAttenuation(pLight->getLinearAttenuation() - 0.001f);
		if(pLight->getLinearAttenuation() < 0.0f)
			pLight->setLinearAttenuation(0.0f);
	}

	if(_evt.isKeyDown(KC_I))
	{
		NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
		pLight->setLinearAttenuation(pLight->getLinearAttenuation() + 0.001f);
	}

	if(_evt.isKeyDown(KC_O))
	{
		NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
		pLight->setQuadraticAttenuation(pLight->getQuadraticAttenuation() - 0.001f);
		if(pLight->getQuadraticAttenuation() < 0.0f)
			pLight->setQuadraticAttenuation(0.0f);
	}

	if(_evt.isKeyDown(KC_P))
	{
		NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"WorldLight");
		pLight->setQuadraticAttenuation(pLight->getQuadraticAttenuation() + 0.001f);
	}


	// Switch scene
	if(_evt.isKeyPressed(KC_1))
	{
		Engine::getSingleton().getSceneManager(0)->getNode(L"Angel")->setEnabled(true);
		Engine::getSingleton().getSceneManager(0)->getNode(L"House")->setEnabled(false);
		Engine::getSingleton().getSceneManager(0)->getNode(L"Palm")->setEnabled(false);
	}

	if(_evt.isKeyPressed(KC_2))
	{
		Engine::getSingleton().getSceneManager(0)->getNode(L"Angel")->setEnabled(false);
		Engine::getSingleton().getSceneManager(0)->getNode(L"House")->setEnabled(true);
		Engine::getSingleton().getSceneManager(0)->getNode(L"Palm")->setEnabled(false);
	}

	if(_evt.isKeyPressed(KC_3))
	{
		Engine::getSingleton().getSceneManager(0)->getNode(L"Angel")->setEnabled(false);
		Engine::getSingleton().getSceneManager(0)->getNode(L"House")->setEnabled(false);
		Engine::getSingleton().getSceneManager(0)->getNode(L"Palm")->setEnabled(true);
	}

	// Hide menu
	if(_evt.isKeyPressed(KC_H))
	{
		if(GUIManager::getSingleton().getWindow(1)->isVisible())
			GUIManager::getSingleton().getWindow(1)->setVisible(false);
		else
			GUIManager::getSingleton().getWindow(1)->setVisible(true);
	}
}
//----------------------------------------------------------------------