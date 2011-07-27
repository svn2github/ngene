/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CollisionListener.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "MyCollisionListener.h"
#include "nGENE.h"

#include "App.h"

#include <sstream>

using namespace nGENE;


MyCollisionListener::MyCollisionListener():
	counter(0)
{
}
//----------------------------------------------------------------------
MyCollisionListener::~MyCollisionListener()
{
}
//----------------------------------------------------------------------
void MyCollisionListener::handleEvent(const CollisionEvent& _evt)
{
	if(_evt.type & CT_START)
	{
		wstringstream name;

		wstring material1 = _evt.actor1->getShape(0)->getMaterial()->getName();
		wstring material2 = _evt.actor2->getShape(0)->getMaterial()->getName();

		SOUND_DESC soundDesc;
		soundDesc.isUsingHardware = false;
		soundDesc.is3D = true;
		
		if(material1 == L"Stone" && material2 == L"Stone")
		{
			soundDesc.fileName = L"hit_stone_stone.wav";
		}
		else if(material1 == L"Steel" || material2 == L"Steel")
		{
			soundDesc.fileName = L"hit_metal.wav";
		}
		else if(material1 == L"Wood" || material2 == L"Wood")
		{
			soundDesc.fileName = L"hit_wood.wav";
		}
		else
			return;

		name << L"HitSound" << counter++;

		Vector3 contactPoint = _evt.contactPoints[0];

		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		NodeSound* pSound = sm->createSound(name.str(), soundDesc);
		ISoundInstance* pInternalSound = pSound->getSound();
		pInternalSound->getSound()->setLooped(false);
		pSound->setPosition(contactPoint);
		sm->getRootNode()->addChild(name.str(), pSound);
		pInternalSound->play();
	}
}
//----------------------------------------------------------------------