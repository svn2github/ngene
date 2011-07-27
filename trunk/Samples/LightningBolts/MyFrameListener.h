/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyFrameListener.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once

#include "FrameListener.h"
#include "Vector3.h"

using nGENE::FrameListener;
using nGENE::FrameEvent;
using nGENE::Vector3;
using nGENE::Real;
using nGENE::dword;


class MyFrameListener: public FrameListener
{
private:
	dword m_Prev;
	dword m_Time;

public:
	MyFrameListener();
	~MyFrameListener();

	void handleEvent(const FrameEvent& _event);
};
