/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PluginManager.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PLUGINMANAGER_H_
#define __INC_PLUGINMANAGER_H_


#include "Plugin.h"
#include "Prerequisities.h"
#include "Singleton.h"


namespace nGENE
{
	/** Plugin manager for nGENE Tech.
		@remarks
			It allows you to load plugins which will extend basic
			functionality of nGENE.
	*/
	class nGENEDLL PluginManager: public Singleton <PluginManager>
	{
	public:
		PluginManager();
		~PluginManager();

		/// Loads plugin with the given name.
		Plugin& loadPlugin(const wstring& _name);
	};
}


#endif