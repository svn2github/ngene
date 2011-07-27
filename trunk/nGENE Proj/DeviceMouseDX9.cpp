/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceMouseDX9.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DeviceMouseDX9.h"
#include "DX9Mapping.h"
#include "InputDeviceEnumerator.h"
#include "Vector3.h"


namespace nGENE
{
	DeviceMouseDX9::DeviceMouseDX9(IDirectInput8* _input):
		m_pMouse(NULL),
		m_pDInput(_input),
		m_dwSize(0)
	{
		createDevice();
	}
//----------------------------------------------------------------------
	DeviceMouseDX9::~DeviceMouseDX9()
	{
		m_pMouse->Unacquire();
		NGENE_RELEASE(m_pMouse);
	}
//----------------------------------------------------------------------
	void DeviceMouseDX9::createDevice()
	{
		HRESULT hr = 0;

		// Create mouse device
		if(FAILED(hr = m_pDInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Creating mouse failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}

		// Set data format
		if(FAILED(hr = m_pMouse->SetDataFormat(&c_dfDIMouse)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Setting mouse data format failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}

		// Set co-operation level
		if(FAILED(hr = m_pMouse->SetCooperativeLevel(Engine::getSingleton().getTopLevelWndHandle(),
			DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		{
			if(FAILED(hr = m_pMouse->SetCooperativeLevel(NULL,	DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Setting mouse cooperative level failed: %ls",
					DX9Mapping::getDXErrorDescription(hr).c_str());
				return;
			}
		}

		DIPROPDWORD dipdw;
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = 16;

		if(FAILED(hr = m_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Creating input buffer failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}

		// Acquire mouse
		if(FAILED(hr = m_pMouse->Acquire()))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Acquiring mouse failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}

		memset(m_Events, 0, sizeof(m_Events));
	}
//----------------------------------------------------------------------
	void DeviceMouseDX9::retrieveData()
	{
		ZeroMemory(&m_Events, sizeof(MouseEvent) * 16);
		m_dwSize = 16;
		DIDEVICEOBJECTDATA events[16];
		HRESULT hr;
		if((hr = m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), events, &m_dwSize, 0)) != DI_OK)
		{
			// If we lost mouse or haven't acquired it
			// we grab it.
			if(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
			{
				m_pMouse->Acquire();
			}

			m_dwSize = 0;
			return;
		}

		for(uint i = 0; i < m_dwSize; ++i)
		{
			m_Events[i].nvalue = events[i].dwData;

			switch(events[i].dwOfs)
			{
			case DIMOFS_BUTTON0: m_Events[i].type = MET_BUTTON_LEFT;
								 break;
		    case DIMOFS_BUTTON1: m_Events[i].type = MET_BUTTON_RIGHT;
								 break;
			case DIMOFS_BUTTON2: m_Events[i].type = MET_BUTTON_MIDDLE;
								 break;
			case DIMOFS_BUTTON3: m_Events[i].type = MET_BUTTON_ADDITIONAL_1;
								 break;
			case DIMOFS_BUTTON4: m_Events[i].type = MET_BUTTON_ADDITIONAL_2;
								 break;
			case DIMOFS_BUTTON5: m_Events[i].type = MET_BUTTON_ADDITIONAL_3;
								 break;
			case DIMOFS_BUTTON6: m_Events[i].type = MET_BUTTON_ADDITIONAL_4;
								 break;
			case DIMOFS_BUTTON7: m_Events[i].type = MET_BUTTON_ADDITIONAL_5;
								 break;
			case DIMOFS_X:
				{
					m_Events[i].fvalue = static_cast<Real>(static_cast<int>(events[i].dwData));
					m_Events[i].type = MET_MOVE_X;
					m_Events[i].fvalue *= m_fSensitivity;
				}
				break;

			case DIMOFS_Y:
				{
					m_Events[i].fvalue = static_cast<Real>(static_cast<int>(events[i].dwData));
					m_Events[i].type = MET_MOVE_Y;
					m_Events[i].fvalue *= m_fSensitivity;
				}
				break;

			case DIMOFS_Z:
				{
					m_Events[i].fvalue = static_cast<Real>(static_cast<int>(events[i].dwData));
					m_Events[i].type = MET_MOVE_Z;
					m_Events[i].fvalue *= m_fSensitivity;
				}
				break;
			}


			// Get mouse position
			getMousePosition(m_Events[i].mousePosition);

			ListenerRegistry <MouseEvent>::handleEvent(m_Events[i]);
		}
	}
//----------------------------------------------------------------------
	void DeviceMouseDX9::getMousePosition(Vector2& _pos)
	{
		POINT pt;
		GetCursorPos(&pt);
		if(InputSystem::getSingleton().isWindowed())
		{
			HWND hWnd = Engine::getSingleton().getTopLevelWndHandle();
			ScreenToClient(hWnd, &pt);
		}

		_pos.x = pt.x;
		_pos.y = pt.y;
	}
//----------------------------------------------------------------------
	dword DeviceMouseDX9::getEventsCount() const
	{
		return m_dwSize;
	}
//----------------------------------------------------------------------
	MouseEvent& DeviceMouseDX9::getEvent(uint _index)
	{
		if(m_dwSize > 1)
			Maths::clamp <uint>(_index, 0, m_dwSize - 1);
		else
			_index = 0;

		return m_Events[_index];
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	MouseFactoryDX9::MouseFactoryDX9(IDirectInput8* _input):
		m_pDInput(_input)
	{
		InputDeviceEnumerator::getSingleton().addFactory(*this, L"MouseDX9");
	}
//----------------------------------------------------------------------
	MouseFactoryDX9::~MouseFactoryDX9()
	{
		m_pDInput = NULL;
	}
//----------------------------------------------------------------------
	InputDevice* MouseFactoryDX9::createInputDevice()
	{
		return (new DeviceMouseDX9(m_pDInput));
	}
//----------------------------------------------------------------------
}