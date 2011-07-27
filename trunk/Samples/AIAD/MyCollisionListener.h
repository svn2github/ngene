/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyCollisionListener.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once

#include "CollisionListener.h"
#include "Vector3.h"
#include "Quaternion.h"

using nGENE::CollisionListener;
using nGENE::CollisionEvent;
using nGENE::ControllerCollisionEvent;
using nGENE::Vector3;
using nGENE::Real;
using nGENE::uint;
using nGENE::Quaternion;

class App;

class MyCollisionListener: public CollisionListener
{
private:
	uint counter;

	App* m_pApp;

public:
	MyCollisionListener();
	~MyCollisionListener();

	void handleEvent(const CollisionEvent& _evt);
	void handleEvent(const ControllerCollisionEvent& _evt);

	void setApp(App* _app) {m_pApp = _app;}
};
