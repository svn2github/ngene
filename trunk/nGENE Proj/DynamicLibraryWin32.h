/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DynamicLibraryWin32.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DYNAMICLIBRARYWIN32_H_
#define __INC_DYNAMICLIBRARYWIN32_H_


#include "IDynamicLibrary.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Dynamic library implementation for Win32 plaftorm.
	*/
	class nGENEDLL DynamicLibraryWin32:	public IDynamicLibrary
	{
	private:
		HINSTANCE m_Handle;					///< Handle to library

	public:
		DynamicLibraryWin32();
		~DynamicLibraryWin32();

		void load(const wstring& _name);
		void* getSymbol(const wstring& _name);
	};
}


#endif