/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LogConsole.cpp
Version:	0.24
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "LogConsole.h"
#include "TimeDate.h"


namespace nGENE
{
	void LogConsole::init()
	{
		intro();
	}
//----------------------------------------------------------------------
	void LogConsole::log(const LogEvent& _evt)
	{
		wcout << getEventTypeName(_evt.type) << ": " << _evt.stMessage << " in " 
			  << _evt.stFunction << "() function in " << _evt.stSourceFile
			  << " at line " << _evt.nLine << endl;
	}
//----------------------------------------------------------------------
	void LogConsole::intro()
	{
		wcout << "Welcome to nGENE Tech" << endl << endl;
		wcout << "Logging system started..." << endl;
		TimeDate time;
		wcout << "It is now " << time.now() << endl << endl;
	}
//----------------------------------------------------------------------
}