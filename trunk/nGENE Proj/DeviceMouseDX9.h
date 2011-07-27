/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceMouseDX9.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DEVICEMOUSEDX9_H_
#define __INC_DEVICEMOUSEDX9_H_


#include "DeviceMouse.h"
#include "InputSystemDX9.h"


namespace nGENE
{
	/** Mouse device for DirectX 9.0c.
	*/
	class DeviceMouseDX9: public DeviceMouse
	{
	private:
		/// DirectInput device
		IDirectInputDevice8* m_pMouse;

		/// Pointer to DirectInput
		IDirectInput8* m_pDInput;

		/// Buffer containing states of the mouse
		MouseEvent m_Events[16];

		dword m_dwSize;						///< Number of registered events


		void getMousePosition(Vector2& _pos);

	public:
		DeviceMouseDX9(IDirectInput8* _input);
		~DeviceMouseDX9();

		/// Creates the mouse device.
		void createDevice();
		/// Retrieves input from the mouse device.
		void retrieveData();

		/// Returns real number of events
		dword getEventsCount() const;
		MouseEvent& getEvent(uint _index);
	};


	/** Factory to be used for DeviceMouseDX9 class objects creation.
	*/
	class nGENEDLL MouseFactoryDX9: public InputDeviceFactory
	{
	private:
		/// Pointer to DirectInput device
		IDirectInput8* m_pDInput;

	public:
		MouseFactoryDX9(IDirectInput8* _input);
		~MouseFactoryDX9();

		InputDevice* createInputDevice();
	};
}


#endif