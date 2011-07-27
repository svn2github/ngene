/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyInputListener.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once

#include "InputListener.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "NodeLightning.h"

using nGENE::InputListener;
using nGENE::KeyboardEvent;
using nGENE::MouseEvent;
using nGENE::Vector3;
using nGENE::Real;
using nGENE::Quaternion;

using nGENE::Nature::NodeLightning;

class App;

class MyInputListener: public InputListener
{
private:
	App* m_pApp;

	Real m_Angle1;
	Real m_Angle2;

	Real m_fNear;
	Real m_fFar;
	Real m_fFocus;

	bool m_bFreezeMouseMovement;

	long m_prev;
	long m_BoxesNum;

	long m_BoltsNum;

	int m_Water;

	NodeLightning* m_pLightnings[10];

public:
	MyInputListener();
	~MyInputListener();

	void handleEvent(const MouseEvent& _evt);
	void handleEvent(const KeyboardEvent& _evt);

	void setApp(App* _app) {m_pApp = _app;}
};
