/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Logger.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "StdAfx.h"
#include "Logger.h"


namespace nGENEToolset
{
	void ToolsetLog::log(const LogEvent& _evt)
	{
		String^ str = gcnew String(_evt.stMessage.c_str());
		int type = 2;
		if(_evt.type == LET_ERROR || _evt.type == LET_FATAL_ERROR)
			type = 0;
		else if(_evt.type == LET_WARNING)
			type = 1;
		m_pManaged->log(str + Environment::NewLine, type);
	}
//----------------------------------------------------------------------
}