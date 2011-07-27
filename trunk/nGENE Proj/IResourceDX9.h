/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IResourceDX9.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_IRESOURCEDX9_H_
#define __INC_IRESOURCEDX9_H_


#include "Listener.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Base class for all DirectX 9.0c resources.
		@remarks
			This is absolutely necessary all DirectX resources' related
			classes implemented this interface as it allows to control
			different situations (eg. device loss).
	*/
	class nGENEDLL IResourceDX9: public Listener <DeviceEvent>
	{
	private:
		/// Handles situation of rendering device loss.
		virtual void onDeviceLost()=0;

		/// Handles situation of rendering device restoration.
		virtual void onDeviceReset()=0;

	public:
		IResourceDX9();
		virtual ~IResourceDX9();

		/// Handles rendering device specific messages.
		virtual void handleEvent(const DeviceEvent& _evt);
	};
}


#endif