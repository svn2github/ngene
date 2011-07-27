/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InputSystem.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "InputDeviceEnumerator.h"
#include "InputSystem.h"


namespace nGENE
{
	// Initialize static member
	TypeInfo InputSystem::Type(L"InputSystem", &BaseClass::Type);


	InputSystem::InputSystem():
		m_bEnabled(true),
		m_bWindowed(false)
	{
	}
//----------------------------------------------------------------------
	InputSystem::~InputSystem()
	{
		// Remove all listeners
		for(uint i = 0; i < m_vInputListeners.size(); ++i)
		{
			AutoPointer <InputListener> Listener = m_vInputListeners[i];
			ListenerRegistry <MouseEvent>::removeListener(Listener.getPointer());
			ListenerRegistry <KeyboardEvent>::removeListener(Listener.getPointer());
		}

		// Remove all devices
		if(!m_pDevices.empty())
		{
			HashTable <wstring, InputDevice*>::iterator iter;
			for(iter = m_pDevices.begin(); iter != m_pDevices.end(); ++iter)
				delete iter->second;

			m_pDevices.clear();
		}
	}
//----------------------------------------------------------------------
	void InputSystem::processData()
	{
		// InputSystem disabled, so return
		if(!m_bEnabled)
			return;

		HashTable <wstring, InputDevice*>::iterator iter;
		for(iter = m_pDevices.begin(); iter != m_pDevices.end(); ++iter)
		{
			iter->second->retrieveData();
		}
	}
//----------------------------------------------------------------------
	void InputSystem::update()
	{
		processData();
	}
//----------------------------------------------------------------------
	InputDevice* InputSystem::createDevice(const wstring& _name, const wstring& _typeName)
	{
		// If device exists, return it
		if(m_pDevices.find(_name) != m_pDevices.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Devices with the name: %ls already exists", _name.c_str());
			return m_pDevices[_name];
		}

		m_pDevices[_name] = m_pInputEnumerator->create(_typeName);

		return m_pDevices[_name];
	}
//----------------------------------------------------------------------
	InputDevice* InputSystem::getDevice(const wstring& _name)
	{
		// Device does not exist
		if(m_pDevices.find(_name) == m_pDevices.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Device: %ls does not exist!", _name.c_str());
			return NULL;
		}

		return (m_pDevices[_name]);
	}
//----------------------------------------------------------------------
	void InputSystem::addInputListener(InputListener* _listener)
	{
		// Add listener to the registry so it
		// could handle upcoming messages.
		AutoPointer <InputListener> Listener(_listener);
		ListenerRegistry <MouseEvent>::addListener(Listener.getPointer());
		ListenerRegistry <KeyboardEvent>::addListener(Listener.getPointer());

		m_vInputListeners.push_back(Listener);
	}
//----------------------------------------------------------------------
	void InputSystem::removeInputListener(uint _index)
	{
		if(_index >= m_vInputListeners.size())
			return;

		InputListener* pListener = m_vInputListeners[_index];
		ListenerRegistry <MouseEvent>::removeListener(pListener);
		ListenerRegistry <KeyboardEvent>::removeListener(pListener);

		m_vInputListeners.erase(m_vInputListeners.begin() + _index);
	}
//----------------------------------------------------------------------
	void InputSystem::removeAllInputListeners()
	{
		for(uint i = 0; i < m_vInputListeners.size(); ++i)
		{
			InputListener* pListener = m_vInputListeners[i];
			ListenerRegistry <MouseEvent>::removeListener(pListener);
			ListenerRegistry <KeyboardEvent>::removeListener(pListener);
		}

		m_vInputListeners.clear();
	}
//----------------------------------------------------------------------
}