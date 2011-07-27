/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyFrameListener.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "MyFrameListener.h"
#include "nGENE.h"

#include "NodeLightning.h"

using nGENE::Nature::NodeLightning;

#include <sstream>

using namespace nGENE;


MyFrameListener::MyFrameListener():
	m_Prev(0),
	m_Time(0)
{
}
//----------------------------------------------------------------------
MyFrameListener::~MyFrameListener()
{
}
//----------------------------------------------------------------------
void MyFrameListener::handleEvent(const FrameEvent& _evt)
{
	dword now = Engine::getSingleton().getTimer().getMilliseconds();
	m_Time += (now - m_Prev);

	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	NodeLightning* pBolt = (NodeLightning*)sm->getNode(L"Bolt");

	if(m_Time >= pBolt->getUpdateFrequency())
	{
		uint index = rand() % 64;

		wstringstream name;
		name << L"Box_" << index;

		Vector3 pos = sm->getNode(name.str())->getPositionLocal();
		
		pBolt->setEndPoint(pos);

		m_Time = 0;
	}

	m_Prev = now;
}
//----------------------------------------------------------------------