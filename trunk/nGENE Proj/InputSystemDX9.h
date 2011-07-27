/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InputSystemDX9.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_INPUTSYSTEMDX9_H_
#define __INC_INPUTSYSTEMDX9_H_


#define DIRECTINPUT_VERSION 0x0800


#include <dinput.h>

#include "InputSystem.h"


namespace nGENE
{
	/** InputSystem implementation for DirectX 9.0c.
	*/
	class InputSystemDX9: public InputSystem
	{
		EXPOSE_TYPE
	private:
		/// Input interface from DirectX
		IDirectInput8* m_pDInput;

	public:
		InputSystemDX9();
		virtual ~InputSystemDX9();

		void init();
		void cleanup();

		/// Returns pointer to the DirectInput Interface.
		IDirectInput8* getDirectInput() const {return m_pDInput;}
	};
}


#endif