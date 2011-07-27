/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyInputListener.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once

#include "InputListener.h"
#include "Vector3.h"
#include "Quaternion.h"

using nGENE::InputListener;
using nGENE::KeyboardEvent;
using nGENE::MouseEvent;
using nGENE::Vector3;
using nGENE::Real;
using nGENE::Quaternion;

class App;

class MyInputListener: public InputListener
{
private:
	App* m_pApp;

	Real m_Angle1;
	Real m_Angle2;

	Real m_fRadius;
	Real m_fRange;

	long m_Prev;

public:
	MyInputListener();
	~MyInputListener() {}

	void handleEvent(const MouseEvent& _evt);
	void handleEvent(const KeyboardEvent& _evt);

	void setApp(App* _app) {m_pApp = _app;}
};
