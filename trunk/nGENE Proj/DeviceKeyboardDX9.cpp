/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceKeyboardDX9.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DeviceKeyboardDX9.h"
#include "DX9Mapping.h"
#include "InputDeviceEnumerator.h"


namespace nGENE
{
	DeviceKeyboardDX9::DeviceKeyboardDX9(IDirectInput8* _input):
		m_pKeyboard(NULL),
		m_pDInput(_input)
	{
		createDevice();
	}
//----------------------------------------------------------------------
	DeviceKeyboardDX9::~DeviceKeyboardDX9()
	{
		m_pKeyboard->Unacquire();
		NGENE_RELEASE(m_pKeyboard);
	}
//----------------------------------------------------------------------
	void DeviceKeyboardDX9::createDevice()
	{
		HRESULT hr = 0;

		// Create keyboard device
		if(FAILED(hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Creating keyboard failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}

		// Set data format
		if(FAILED(hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Setting keyboard data format failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}

		// Set co-operation level
		HWND hWnd = Engine::getSingleton().getTopLevelWndHandle();
		if(FAILED(hr = m_pKeyboard->SetCooperativeLevel(hWnd,
			DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			if(FAILED(hr = m_pKeyboard->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Setting keyboard cooperative level failed: %ls",
					DX9Mapping::getDXErrorDescription(hr).c_str());
				return;
			}
		}

		// Acquire keyboard
		if(FAILED(hr = m_pKeyboard->Acquire()))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Acquiring keyboard failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}

		memset(m_cKeyBuffer, 0, sizeof(m_cKeyBuffer));
	}
//----------------------------------------------------------------------
	void DeviceKeyboardDX9::retrieveData()
	{
		KeyboardEvent evt;
		memcpy(&evt.m_cOldKeyBuffer, m_cKeyBuffer, sizeof(m_cKeyBuffer));

		m_pKeyboard->GetDeviceState(sizeof(m_cKeyBuffer), (LPVOID)&m_cKeyBuffer);

		memcpy(&evt.m_cKeyBuffer, m_cKeyBuffer, sizeof(m_cKeyBuffer));

		ListenerRegistry <KeyboardEvent>::handleEvent(evt);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	KeyboardFactoryDX9::KeyboardFactoryDX9(IDirectInput8* _input):
		m_pDInput(_input)
	{
		InputDeviceEnumerator::getSingleton().addFactory(*this, L"KeyboardDX9");
	}
//----------------------------------------------------------------------
	KeyboardFactoryDX9::~KeyboardFactoryDX9()
	{
		m_pDInput = NULL;
	}
//----------------------------------------------------------------------
	InputDevice* KeyboardFactoryDX9::createInputDevice()
	{
		return (new DeviceKeyboardDX9(m_pDInput));
	}
//----------------------------------------------------------------------
}