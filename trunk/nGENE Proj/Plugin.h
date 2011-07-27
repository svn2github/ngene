/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Plugin.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PLUGIN_H_
#define __INC_PLUGIN_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Plugin extending nGENE Tech functionality.
	*/
	class nGENEDLL Plugin
	{
	public:
		Plugin();
		~Plugin();
	};


	typedef Plugin* (*plugin_init_function)(void);
}


#endif