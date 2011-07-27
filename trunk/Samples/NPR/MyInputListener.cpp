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

#include <sstream>

using namespace nGENE;


MyInputListener::MyInputListener()
{
	m_Zoom = 1.0f;
	m_Angle = 0.0f;
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const KeyboardEvent& _evt)
{
	if(_evt.isKeyPressed(KC_ESCAPE))
	{
		m_pApp->closeApplication();
		return;
	}

	Engine& engine = Engine::getSingleton();
	Camera2D* cam = static_cast<Camera2D*>(engine.getActiveCamera());

	// Everything regarding controls in this demo requires existence of the camera.
	// Therefore if it doesn't exist we return.
	if(!cam)
		return;


	Timer& timer = engine.getTimer();
	float fDiff = float(timer.getMilliseconds() - m_Prev) * 0.001f;
	if(_evt.isKeyDown(KC_Z))
		m_Angle -= 0.9f * fDiff;
	if(_evt.isKeyDown(KC_X))
		m_Angle += 0.9f * fDiff;

	// Rotate camera
	Quaternion rot(Vector3(0.0f, 1.0f, 0.0f), m_Angle);
	Quaternion rot2(Vector3(1.0f, 0.0f, 0.0f), Maths::PI / 4.0f);
	Quaternion rotFinal = rot * rot2;
	rotFinal.normalize();
	if(Maths::abs <float>(rotFinal.x - cam->getRotation().x) > 0.001f ||
	   Maths::abs <float>(rotFinal.y - cam->getRotation().y) > 0.001f ||
	   Maths::abs <float>(rotFinal.z - cam->getRotation().z) > 0.001f ||
	   Maths::abs <float>(rotFinal.w - cam->getRotation().w) > 0.001f)
	{
		cam->setRotation(rotFinal);
	}

	// Clamp zoom value to [0.5, 2.0] range
	if(_evt.isKeyDown(KC_MINUS))
		m_Zoom -= 1.0f * fDiff;
	if(_evt.isKeyDown(KC_EQUALS))
		m_Zoom += 1.0f * fDiff;

	Maths::clamp(m_Zoom, 0.5f, 2.0f);
	cam->setZoom(m_Zoom);

	// Move camera
	Vector2 vecDir;
	if(_evt.isKeyDown(KC_UP))
		vecDir.y = 1.0f;
	if(_evt.isKeyDown(KC_DOWN))
		vecDir.y = -1.0f;
	if(_evt.isKeyDown(KC_LEFT))
		vecDir.x = -1.0f;
	if(_evt.isKeyDown(KC_RIGHT))
		vecDir.x = 1.0f;
	cam->scroll(vecDir);

	// Disable light
	bool isLight = Renderer::getSingleton().isLightEnabled();
	if(_evt.isKeyPressed(KC_L))
	{
		if(isLight)
			Engine::getSingleton().getRenderer().setLightEnabled(false);
		else
			Engine::getSingleton().getRenderer().setLightEnabled(true);
	}

	// Switch materials
	if(_evt.isKeyPressed(KC_M))
	{
		if(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"waterColor")->isEnabled())
		{
			MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"waterColor")->setEnabled(false);
			MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"pencilStroke")->setEnabled(true);
		}
		else
		{
			MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"waterColor")->setEnabled(true);
			MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"pencilStroke")->setEnabled(false);
		}
	}

	if(_evt.isKeyPressed(KC_H))
	{
		if(GUIManager::getSingleton().getWindow(1)->isVisible())
			GUIManager::getSingleton().getWindow(1)->setVisible(false);
		else
			GUIManager::getSingleton().getWindow(1)->setVisible(true);
	}


	m_Prev = timer.getMilliseconds();
}
//----------------------------------------------------------------------