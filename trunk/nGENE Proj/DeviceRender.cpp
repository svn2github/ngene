/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceRender.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DeviceRender.h"


namespace nGENE
{
	DeviceRender::DeviceRender(PROCESSING_TYPE _type):
		m_bIsLost(false),
		m_Processing(_type)
	{
	}
//----------------------------------------------------------------------
	DeviceRender::~DeviceRender()
	{
	}
//----------------------------------------------------------------------
	void DeviceRender::handleEvent(const DeviceEvent& _evt)
	{
		// Choose an action based on the message type
		switch(_evt.type)
		{
		case DET_CREATED  : onDeviceCreate();
						    break;
		case DET_LOST	  : onDeviceLost();
						    break;
		case DET_DESTROYED: onDeviceDestroy();
						    break;
		default: break;
		}
	}
//----------------------------------------------------------------------
}