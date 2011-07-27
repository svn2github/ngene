/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IResourceDX9.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "IResourceDX9.h"
#include "DeviceRender.h"


namespace nGENE
{
	IResourceDX9::IResourceDX9()
	{
		// Make resource a listener
		ListenerRegistry <DeviceEvent>::addListener(this);
	}
//----------------------------------------------------------------------
	IResourceDX9::~IResourceDX9()
	{
		ListenerRegistry <DeviceEvent>::removeListener(this);
	}
//----------------------------------------------------------------------
	void IResourceDX9::handleEvent(const DeviceEvent& _evt)
	{
		switch(_evt.type)
		{
		case DET_LOST: onDeviceLost();
					   break;

		case DET_RESET: onDeviceReset();
						break;

		default: break;
		}
	}
//----------------------------------------------------------------------
}