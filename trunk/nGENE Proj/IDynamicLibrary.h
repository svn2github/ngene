/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IDynamicLibrary.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_IDYNAMICLIBRARY_H_
#define __INC_IDYNAMICLIBRARY_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Pure interface for dynamic loaded libraries.
		@remarks
			Each implementation is very OS specific.
	*/
	class nGENEDLL IDynamicLibrary
	{
	public:
		IDynamicLibrary();
		virtual ~IDynamicLibrary();

		/// Loads library file
		virtual void load(const wstring& _name)=0;

		/// Returns symbol from a dynamic library.
		virtual void* getSymbol(const wstring& _name)=0;
	};
}


#endif