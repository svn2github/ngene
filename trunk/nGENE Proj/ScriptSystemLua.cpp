/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScriptSystemLua.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ScriptSystemLua.h"
#include "ScriptLua.h"
#include "ScriptLuaLib.h"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>

using namespace nGENE::Script::Lua;


namespace nGENE
{
	// Initialize static members
	TypeInfo ScriptSystemLua::Type(L"ScriptSystemLua", &BaseClass::Type);


	ScriptSystemLua::ScriptSystemLua():
		m_pLuaVM(NULL)
	{
	}
//----------------------------------------------------------------------
	ScriptSystemLua::~ScriptSystemLua()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ScriptSystemLua::init()
	{
		m_pLuaVM = lua_open();
		if(!m_pLuaVM)
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Initializing Lua virtual machine failed");
			return;
		}

		// Initialize standard library
		luaL_openlibs(m_pLuaVM);

		// Initialize LuaBind
		luabind::open(m_pLuaVM);

		registerNGENELib(m_pLuaVM);


		Timer& timer = Engine::getSingleton().getTimer();
		m_dwLast = timer.getMilliseconds();
	}
//----------------------------------------------------------------------
	void ScriptSystemLua::cleanup()
	{
		for(uint i = 0; i < m_vScripts.size(); ++i)
			NGENE_DELETE(m_vScripts[i]);
		m_vScripts.clear();

		lua_close(m_pLuaVM);
	}
//----------------------------------------------------------------------
	ScriptLua* ScriptSystemLua::createScript()
	{
		if(!m_pLuaVM)
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Virtual machine is not yet initialized.");
			return NULL;
		}

		ScriptLua* result = new ScriptLua(m_pLuaVM);
		m_vScripts.push_back(result);

		return result;
	}
//----------------------------------------------------------------------
	void ScriptSystemLua::removeScript(uint _index)
	{
		if(_index >= m_vScripts.size())
			return;

		NGENE_DELETE(m_vScripts[_index]);
		m_vScripts.erase(m_vScripts.begin() + _index);
	}
//----------------------------------------------------------------------
	void ScriptSystemLua::removeScript(ScriptLua* _script)
	{
		uint i = 0;
		for(i = 0; i < m_vScripts.size(); ++i)
		{
			if(m_vScripts[i] == _script)
				break;
		}

		m_vScripts.erase(m_vScripts.begin() + i);
	}
//----------------------------------------------------------------------
	ScriptLua* ScriptSystemLua::getScript(uint _index) const
	{
		if(_index >= m_vScripts.size())
			return NULL;

		return m_vScripts[_index];
	}
//----------------------------------------------------------------------
	void ScriptSystemLua::update()
	{
		Timer& timer = Engine::getSingleton().getTimer();
		dword now = timer.getMilliseconds();
		dword delta = now - m_dwLast;

		for(uint i = 0; i < m_vScripts.size(); ++i)
			m_vScripts[i]->update(delta);

		m_dwLast = now;
	}
//----------------------------------------------------------------------
}