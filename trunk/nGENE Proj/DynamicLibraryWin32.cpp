/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DynamicLibraryWin32.h
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DynamicLibraryWin32.h"


#ifdef _WIN32
	#define NOMINMAX
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0400
	#endif
	#include <windows.h>
#endif


namespace nGENE
{
	DynamicLibraryWin32::DynamicLibraryWin32()
	{
	}
//----------------------------------------------------------------------
	DynamicLibraryWin32::~DynamicLibraryWin32()
	{
	}
//----------------------------------------------------------------------
	void DynamicLibraryWin32::load(const wstring& _name)
	{
		m_Handle = LoadLibraryW(_name.c_str());

		if(!m_Handle)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Library: %ls could not be loaded", _name.c_str());
		}
	}
//----------------------------------------------------------------------
	void* DynamicLibraryWin32::getSymbol(const wstring& _name)
	{
		string name(_name.begin(), _name.end());
		void* pSymbol = GetProcAddress(m_Handle, name.c_str());

		if(!pSymbol)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Symbol: %ls not found", _name.c_str());
		}

		return pSymbol;
	}
//----------------------------------------------------------------------
}
