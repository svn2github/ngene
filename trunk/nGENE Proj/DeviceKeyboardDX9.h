/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceKeyboardDX9.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DEVICEKEYBOARDDX9_H_
#define __INC_DEVICEKEYBOARDDX9_H_


#include "DeviceKeyboard.h"
#include "InputSystemDX9.h"


namespace nGENE
{
	/** Keyboard device for DirectX 9.0c.
	*/
	class DeviceKeyboardDX9: public DeviceKeyboard
	{
	private:
		/// DirectInput device
		IDirectInputDevice8* m_pKeyboard;

		/// Pointer to DirectInput
		IDirectInput8* m_pDInput;

		/// Buffer containing states of all the keys
		char m_cKeyBuffer[256];

	public:
		DeviceKeyboardDX9(IDirectInput8* _input);
		virtual ~DeviceKeyboardDX9();

		/// Creates the keyboard device.
		void createDevice();
		/// Retrieves input from the keyboard device.
		void retrieveData();

		bool isKeyDown(dword _key) const;
	};


	/** Factory to be used for DeviceKeyboardDX9 class objects creation.
	*/
	class nGENEDLL KeyboardFactoryDX9: public InputDeviceFactory
	{
	private:
		/// Pointer to DirectInput device
		IDirectInput8* m_pDInput;

	public:
		KeyboardFactoryDX9(IDirectInput8* _input);
		virtual ~KeyboardFactoryDX9();

		InputDevice* createInputDevice();
	};



	inline bool DeviceKeyboardDX9::isKeyDown(dword _key) const
	{
		return (m_cKeyBuffer[_key] & 0x80) != 0;
	}
//----------------------------------------------------------------------
}


#endif