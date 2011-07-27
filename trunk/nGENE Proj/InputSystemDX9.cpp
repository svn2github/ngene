/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InputSystemDX9.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DeviceKeyboardDX9.h"
#include "InputDeviceEnumerator.h"
#include "InputSystemDX9.h"


namespace nGENE
{
	// Initialize static member
	TypeInfo InputSystemDX9::Type(L"InputSystemDX9", &InputSystem::Type);


	InputSystemDX9::InputSystemDX9():
		m_pDInput(NULL)
	{
	}
//----------------------------------------------------------------------
	InputSystemDX9::~InputSystemDX9()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void InputSystemDX9::init()
	{
		// Create DirectInput interface
		#ifdef _DEBUG
			HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle("nGENE_d"));
		#else
			HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle("nGENE"));
		#endif

		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, NULL)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Creating DirectInput8 failed");
			return;
		}

		// Create enumerator
		m_pInputEnumerator = new InputDeviceEnumerator();
	}
//----------------------------------------------------------------------
	void InputSystemDX9::cleanup()
	{
		NGENE_DELETE(m_pInputEnumerator);
		NGENE_RELEASE(m_pDInput);
	}
//----------------------------------------------------------------------
}