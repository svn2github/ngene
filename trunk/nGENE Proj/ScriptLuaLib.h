/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScriptLuaLib.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCRIPTLUALIB_H_
#define __INC_SCRIPTLUALIB_H_


namespace nGENE
{
namespace Script
{
namespace Lua
{
	/// Registers nGENE Lua standard library
	void registerNGENELib(lua_State* _state);
}
}
}


#endif