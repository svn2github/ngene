/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LogConsole.h
Version:	0.24
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_LOGCONSOLE_H_
#define __INC_LOGCONSOLE_H_


#include "LogListener.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Logs messages and produces them on the standard output.
	*/
	class nGENEDLL LogConsole: public LogListener
	{
	public:
		void init();
		void intro();

		void log(const LogEvent& _evt);
	};
}


#endif
