/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FrameListener.cpp
Version:	0.53
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FrameListener.h"


namespace nGENE
{
	FrameListener::FrameListener()
	{
	}
//----------------------------------------------------------------------
	FrameListener::~FrameListener()
	{
	}
//----------------------------------------------------------------------
	void FrameListener::handleEvent(const FrameEvent& _event)
	{
		if(_event.isFrameBegin)
			onFrameBegin();
		else
			onFrameEnd();
	}
//----------------------------------------------------------------------
}