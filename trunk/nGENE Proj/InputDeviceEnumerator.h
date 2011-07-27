/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InputDeviceEnumerator.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_INPUTDEVICEENUMERATOR_H_
#define __INC_INPUTDEVICEENUMERATOR_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "Singleton.h"


namespace nGENE
{
	/** InputDevice Enumerator class.
		@remarks
			Enumerators lets you easily extend nGENE Tech by just adding
			factories of new type to the enumerator. Then by specifying
			the type name you can create objects of any available type
			in the enumerator.
	*/
	class nGENEDLL InputDeviceEnumerator: public Singleton <InputDeviceEnumerator>
	{
	private:
		/// Hash table containing all factories
		HashTable <wstring, InputDeviceFactory*> m_pFactories;

		// Default factories:

		/// KeyboardDX9 factory
		KeyboardFactoryDX9* m_pKeyboardDX9;
		/// MouseDX9 factory
		MouseFactoryDX9* m_pMouseDX9;

	public:
		InputDeviceEnumerator();
		virtual ~InputDeviceEnumerator();

		void init();
		void cleanup();

		/// Registers new factory.
		void addFactory(InputDeviceFactory& _factory, const wstring& _typeName);
		/// Registers new factory.
		void addFactory(InputDeviceFactory* _factory, const wstring& _typeName);

		/// Removes factory.
		void removeFactory(const wstring& _typeName);

		/// Creates new input device.
		InputDevice* create(const wstring& _typeName);
		/// Destroys existing input device.
		void destroy(InputDevice* _device);

		/// Returns names of the input devices.
		vector <wstring> getNamesList();
	};
}


#endif