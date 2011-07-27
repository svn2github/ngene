/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyUpdateListener.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once

#include "EngineUpdateListener.h"
#include "Vector3.h"
#include "Quaternion.h"

using nGENE::EngineUpdateListener;
using nGENE::EngineUpdateEvent;
using nGENE::Vector3;
using nGENE::Real;
using nGENE::uint;
using nGENE::Quaternion;

class App;

class MyUpdateListener: public EngineUpdateListener
{
private:
	App* m_pApp;

	float m_fScrollSpeed;

	uint m_nPrev;
	uint m_nPassedRotate;

	bool m_bNeedsRotating;

public:
	MyUpdateListener();
	~MyUpdateListener();

	void handleEvent(const EngineUpdateEvent& _evt);
	
	void setApp(App* _app);
	void setScrollSpeed(Real _speed);
};



inline void MyUpdateListener::setApp(App* _app)
{
	m_pApp = _app;
}
//----------------------------------------------------------------------
inline void MyUpdateListener::setScrollSpeed(Real _speed)
{
	m_fScrollSpeed = _speed;
}
//----------------------------------------------------------------------