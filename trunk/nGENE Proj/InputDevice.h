/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InputDevice.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_INPUTDEVICE_H_
#define __INC_INPUTDEVICE_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Input device base class.
		@par
			To create specific device class you have to derive this class.
			This way apart from creating regular devices like mouse, keyboard
			or joystick you can make use of any other input device.
	*/
	class nGENEDLL InputDevice
	{
	public:
		InputDevice();
		virtual ~InputDevice();

		/// Creates the device.
		virtual void createDevice()=0;
		/// Retrieves input from the device.
		virtual void retrieveData()=0;
	};

	/// Factory to be used for InputDevice class objects creation.
	class nGENEDLL InputDeviceFactory
	{
	public:
		virtual InputDevice* createInputDevice()=0;
	};
}


#endif