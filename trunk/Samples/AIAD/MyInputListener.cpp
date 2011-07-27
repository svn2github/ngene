/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyInputListener.cpp
Version:	0.05
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
	m_Angle2(0.0f),
	m_kfMaxAngle(0.85f),
	m_kfRotationSpeed(1.35f)
{
}
//----------------------------------------------------------------------
MyInputListener::~MyInputListener()
{
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const MouseEvent& _evt)
{
	if(m_pApp->isPlayerDead())
		return;

	CameraThirdPerson* pCamera = m_pApp->getCamera();
	/*if(_evt.type == MET_MOVE_X)
	{
		if(_evt.type == MET_MOVE_X)
			m_Angle1 += _evt.fvalue;

		Quaternion quatRot1(Vector3::UNIT_Y, m_Angle1);
		Quaternion quatRot2(Vector3::UNIT_X, Maths::PI_HALF);

		PlayerCharacter& player = m_pApp->getPlayer();
		//player.setRotation(quatRot1);

		//pCamera->setRotation(quatRot1 * quatRot2);
	}*/

	if(_evt.type == MET_MOVE_Z)
	{
		PlayerCharacter& player = PlayerCharacter::getSingleton();
		if(_evt.fvalue < 0.0f)
			player.selectPrevWeapon();
		else
			player.selectNextWeapon();
	}

	if(_evt.type == MET_BUTTON_LEFT && _evt.nvalue & 0x80)
	{
		PlayerCharacter& player = m_pApp->getPlayer();
		Weapon* pWeapon = player.getActiveWeapon();
		if(pWeapon)
			pWeapon->use(player.getVisual()->getPositionLocal(), false);
	}
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const KeyboardEvent& _evt)
{
	// Exit application
	if(_evt.isKeyPressed(KC_ESCAPE))
	{
		m_pApp->closeApplication();

		return;
	}

	if(m_pApp->isPlayerDead())
		return;


	float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_prev) * 0.001f;


	// Make movement
	Vector3 movement;
	bool moved = false;
	PlayerCharacter& player = m_pApp->getPlayer();
	Vector3 vecPos = player.getVisual()->getPositionLocal();
	if(_evt.isKeyDown(KC_W))
	{
		movement += Vector3::UNIT_Z;
		moved = true;
	}
	if(_evt.isKeyDown(KC_S))
	{
		movement += Vector3::NEGATIVE_UNIT_Z;
		moved = true;
	}
	if(_evt.isKeyDown(KC_A) && vecPos.x >= 8.5f)
	{
		movement += Vector3::NEGATIVE_UNIT_X;
		moved = true;
	}
	if(_evt.isKeyDown(KC_D) && vecPos.x <= 19.5f)
	{
		movement += Vector3::UNIT_X;
		moved = true;
	}
	/*if(_evt.isKeyDown(KC_Q) && vecPos.y >= 1.0f)
	{
		movement += Vector3::NEGATIVE_UNIT_Y;
		moved = true;
	}
	if(_evt.isKeyDown(KC_E) && vecPos.y <= 12.0f)
	{
		movement += Vector3::UNIT_Y;
		moved = true;
	}*/

	if(moved)
	{
		// Limit movement to the area boundaries
		if((vecPos.x <= 8.5f && movement.x < 0.0f) ||
		   (vecPos.x >= 19.5f && movement.x > 0.0f))
		{
			movement.x = 0.0f;
			if(!Maths::floatCompare(0.0f, movement.z) &&
			   !Maths::floatCompare(0.0f, movement.y))
			{
				moved = false;
			}
		}

		float scroll = m_pApp->getScrollStage();
		float z = m_pApp->getCamera()->getTarget().z - 3.0f;
		if((vecPos.z <= z && movement.z < 0.0f) ||
		   (vecPos.z >= z + 6.0f && movement.z > 0.0f))
		{
			movement.z = 0.0f;
			if(!Maths::floatCompare(0.0f, movement.x)&&
			   !Maths::floatCompare(0.0f, movement.y))
			{
				moved = false;
			}
		}
		
		if(moved)
		{
			movement.normalize();
			player.move(movement * player.getAgility());

			m_Angle1 = player.getAngle();

			if(movement.x > 0.5f)
			{
				Maths::clamp(m_Angle1, -m_kfMaxAngle, m_kfMaxAngle);
				m_Angle1 -= fDiff * m_kfRotationSpeed;

				Quaternion quatRot(Vector3::UNIT_Z, m_Angle1);
				player.getController()->setRotation(quatRot);

				player.setAngle(m_Angle1);
				player.setStrafing(true);

				//if(vecPos.x <= 17.5f)
					m_pApp->getCamera()->strafe(movement.x * m_pApp->getSpeedRate() * player.getAgility());
			}
			else if(movement.x < -0.5f)
			{
				Maths::clamp(m_Angle1, -m_kfMaxAngle, m_kfMaxAngle);
				m_Angle1 += fDiff * m_kfRotationSpeed;

				Quaternion quatRot(Vector3::UNIT_Z, m_Angle1);
				player.getController()->setRotation(quatRot);

				player.setAngle(m_Angle1);
				player.setStrafing(true);

				//if(vecPos.x >= 10.5f)
					m_pApp->getCamera()->strafe(movement.x * m_pApp->getSpeedRate() * player.getAgility());
			}
			else
			{
				player.setStrafing(false);
			}
		}
	}

	// Switch weapons
	if(_evt.isKeyPressed(KC_Z))
	{
		player.selectPrevWeapon();
	}
	if(_evt.isKeyPressed(KC_X))
	{
		player.selectNextWeapon();
	}

	m_prev = Engine::getSingleton().getTimer().getMilliseconds();
}
//----------------------------------------------------------------------