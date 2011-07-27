/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InputDeviceEnumerator.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "InputDeviceEnumerator.h"
#include "DeviceKeyboardDX9.h"
#include "DeviceMouseDX9.h"
#include "InputDevice.h"
#include "InputSystemDX9.h"


namespace nGENE
{
	InputDeviceEnumerator::InputDeviceEnumerator():
		m_pKeyboardDX9(NULL)
	{
		init();
	}
//----------------------------------------------------------------------
	InputDeviceEnumerator::~InputDeviceEnumerator()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void InputDeviceEnumerator::init()
	{
		switch(Engine::getSingleton().getRenderingAPI())
		{
		case API_DIRECTX9C:
			{
				InputSystemDX9* pInput = (InputSystemDX9*)(InputSystem::getSingletonPtr());
				m_pKeyboardDX9 = new KeyboardFactoryDX9(pInput->getDirectInput());
				m_pMouseDX9 = new MouseFactoryDX9(pInput->getDirectInput());
			}

			break;

		case API_DIRECTX10:
			{
				InputSystemDX9* pInput = (InputSystemDX9*)(InputSystem::getSingletonPtr());
				m_pKeyboardDX9 = new KeyboardFactoryDX9(pInput->getDirectInput());
				m_pMouseDX9 = new MouseFactoryDX9(pInput->getDirectInput());
			}

			break;
		}
	}
//----------------------------------------------------------------------
	void InputDeviceEnumerator::cleanup()
	{
		if(!m_pFactories.empty())
		{
			HashTable <wstring, InputDeviceFactory*>::iterator iter;
			for(iter = m_pFactories.begin(); iter != m_pFactories.end(); ++iter)
				NGENE_DELETE(iter->second);
			m_pFactories.clear();
		}
	}
//----------------------------------------------------------------------
	void InputDeviceEnumerator::addFactory(InputDeviceFactory& _factory,
		const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = &_factory;
	}
//----------------------------------------------------------------------
	void InputDeviceEnumerator::addFactory(InputDeviceFactory* _factory,
		const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = _factory;
	}
//----------------------------------------------------------------------
	void InputDeviceEnumerator::removeFactory(const wstring& _typename)
	{
		m_pFactories.erase(_typename);
	}
//----------------------------------------------------------------------
	InputDevice* InputDeviceEnumerator::create(const wstring& _typeName)
	{
		// Appropriate factory does not exist
		if(m_pFactories.find(_typeName) == m_pFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Factory for type: %ls does not exist!", _typeName.c_str());
			return NULL;
		}

		// Create new input device
		return (m_pFactories[_typeName]->createInputDevice());
	}
//----------------------------------------------------------------------
	void InputDeviceEnumerator::destroy(InputDevice* _device)
	{
		NGENE_DELETE(_device);
	}
//----------------------------------------------------------------------
	vector <wstring> InputDeviceEnumerator::getNamesList()
	{
		vector <wstring> vNames;

		if(!m_pFactories.empty())
		{
			HashTable <wstring, InputDeviceFactory*>::iterator iter;
			for(iter = m_pFactories.begin(); iter != m_pFactories.end(); ++iter)
				vNames.push_back(iter->first);
		}

		return vNames;
	}
//----------------------------------------------------------------------
}