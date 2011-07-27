/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InputSystem.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_INPUTSYSTEM_H_
#define __INC_INPUTSYSTEM_H_


#include "AutoPointer.h"
#include "BaseClass.h"
#include "HashTable.h"
#include "InputDevice.h"
#include "InputListener.h"
#include "Prerequisities.h"
#include "Singleton.h"
#include "Task.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** InputSystem lets you create input devices and track data
		send by them.
		@remarks
			InputSystem is rather API dependent so to make use of it
			you have to provide own implementation for the API used.
		@param
			After creating InputSystem you have to manually call its
			init() member function.
	*/
	class nGENEDLL InputSystem: public BaseClass,
		public Singleton <InputSystem>, public Task
	{
		EXPOSE_TYPE
	protected:
		/// Input Devices enumerator
		InputDeviceEnumerator* m_pInputEnumerator;

	private:
		/// All devices available
		HashTable <wstring, InputDevice*> m_pDevices;

		/** Input listeners.
			@remarks
				Note that multiple input listeners can be used at once.
		*/
		vector <AutoPointer <InputListener> > m_vInputListeners;

		/// Is InputSystem enabled?
		bool m_bEnabled;
		/// Is full screen mode used?
		bool m_bWindowed;
		
	public:
		InputSystem();
		virtual ~InputSystem();

		virtual void init()=0;
		virtual void cleanup()=0;

		/** Creates new device and adds it to the "list" of existing ones.
			@param
				_name name of the device by which it could be accessed.
			@param
				_typeName type of the device to create.
		*/
		virtual InputDevice* createDevice(const wstring& _name, const wstring& _typeName);

		/// Returns existing input listener.
		virtual AutoPointer <InputListener> getInputListener(uint _index) const;

		/** Registers new input listener in the input system. This way you
			can easily handle incoming keyboard and mouse events.
			@param
				_listener pointer to the InputListener object. Note that this
				object should be allocated on the heap as internally smart pointers
				are used, and trying to release them might result in heap corruption.
		*/
		virtual void addInputListener(InputListener* _listener);
		/// Removes existing input listener.
		virtual void removeInputListener(uint _index);
		/// Removes all existing input listeners.
		virtual void removeAllInputListeners();
		
		/// Processes all devices.
		void processData();

		void update();

		/** Returns specific device.
			@remarks
				This way, using type cast, you can obtain device specific data
				or get access to its input buffer.
		*/
		InputDevice* getDevice(const wstring& _name);

		void setEnabled(bool _value);
		bool isEnabled() const;

		void setWindowed(bool _value);
		bool isWindowed() const;
	};



	inline AutoPointer <InputListener> InputSystem::getInputListener(uint _index) const
	{
		return m_vInputListeners[_index];
	}
//----------------------------------------------------------------------
	inline void InputSystem::setWindowed(bool _value)
	{
		m_bWindowed = _value;
	}
//----------------------------------------------------------------------
	inline bool InputSystem::isWindowed() const
	{
		return m_bWindowed;
	}
//----------------------------------------------------------------------
	inline void InputSystem::setEnabled(bool _value)
	{
		m_bEnabled = _value;
	}
//----------------------------------------------------------------------
	inline bool InputSystem::isEnabled() const
	{
		return m_bEnabled;
	}
//----------------------------------------------------------------------
}


#endif