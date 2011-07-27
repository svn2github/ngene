/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Log.cpp
Version:	0.26
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Log.h"
#include "TimeDate.h"


namespace nGENE
{
	// Initialize static members
	bool Log::s_bIsEnabled = true;


	void Log::log(LOG_EVENT_TYPE type, const wstring &space,
				  const wstring &source, const wstring &function,
				  int line, const wchar_t* message, ...)
	{
		if(!s_bIsEnabled)
			return;

		// Create arguments list to provide functionality
		// similar to that of C printf() function
		wchar_t* tempMsg;
		int len = 0;
		va_list arg_ptr;

		va_start(arg_ptr, message);
			len = _vscwprintf(message, arg_ptr) + 1;
			tempMsg = new wchar_t[len];
			vswprintf(tempMsg, message, arg_ptr);
		va_end(arg_ptr);

		LogEvent evt = {type,
					    space,
					    source,
					    function,
					    tempMsg,
					    line,
					    Engine::getSingleton().getTimer().getMilliseconds()};

		ListenerRegistry <LogEvent>::handleEvent(evt);

		NGENE_DELETE_ARRAY(tempMsg);
	}
//----------------------------------------------------------------------
}