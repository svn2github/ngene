/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CrashDump.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CRASH_DUMP_H_
#define __INC_CRASH_DUMP_H_


#ifdef _WIN32
	#define NOMINMAX
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0400
	#endif
	#include <windows.h>
#endif


namespace nGENE
{
namespace Application
{
	/// This is a top-level filter "catching" unhandled exceptions.
	LONG WINAPI createCrashDump(struct _EXCEPTION_POINTERS* pExceptionPointers);
}
}


#endif