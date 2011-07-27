/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScriptLua.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ScriptLua.h"
#include "ScriptSystemLua.h"

#include "FileManager.h"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/object.hpp>
#include <luabind/tag_function.hpp>


namespace nGENE
{
	// Initialize static members
	TypeInfo ScriptLua::Type(L"ScriptLua", NULL);


	ScriptLua::ScriptLua(lua_State* _parent):
		m_State(STATE_NOT_RUNNING),
		m_nWaitTime(0),
		m_nWaitFrames(0),
		m_nTimePassed(0)
	{
		m_pState = lua_newthread(_parent);

		lua_pushlightuserdata(_parent, m_pState);
		lua_pushlightuserdata(_parent, this);
		lua_settable(_parent, LUA_GLOBALSINDEX);

		m_pParent = _parent;
	}
//----------------------------------------------------------------------
	ScriptLua::~ScriptLua()
	{
		if(m_State != STATE_DONE)
			kill();
	}
//----------------------------------------------------------------------
	void ScriptLua::runScriptFromFile(const string& _fileName)
	{
		if(luaL_loadfile(m_pState, _fileName.c_str()))
		{
			string temp = getLastError();
			wstring error(temp.begin(), temp.end());
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Error encountered while running script: %ls", error.c_str());

			return;
		}

		resume(0.0f);
	}
//----------------------------------------------------------------------
	void ScriptLua::loadScriptFromFile(const string& _fileName)
	{
		if(luaL_dofile(m_pState, _fileName.c_str()))
		{
			wstring name(_fileName.begin(), _fileName.end());

			FileManager& mgr = FileManager::getSingleton();
			IFile* pFile = mgr.openFile(name, OPEN_READ | OPEN_BINARY, FT_NORMAL);
			uint size = pFile->getSize();
			char* buffer = new char[size];
			pFile->read((char*)buffer, size);
			mgr.closeFile(name);

			int status = luaL_loadbuffer(m_pState, buffer, size, _fileName.c_str());
			NGENE_DELETE_ARRAY(buffer);

			if(!status)
				status = lua_pcall(m_pState, 0, LUA_MULTRET, 0);
			if(status)
			{
				string temp = getLastError();
				wstring error(temp.begin(), temp.end());
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Error encountered while running script: %ls", error.c_str());

				return;
			}
		}
	}
//----------------------------------------------------------------------
	void ScriptLua::runScriptFromString(const string& _script)
	{
		int status = luaL_loadbuffer(m_pState, _script.c_str(), _script.length(), "Console");
		if(!status)
			status = lua_pcall(m_pState, lua_gettop(m_pState)-1, 0, 0);

		if(status)
		{
			string temp = getLastError();
			wstring error(temp.begin(), temp.end());
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Error encountered while running script: %ls", error.c_str());

			return;
		}
	}
//----------------------------------------------------------------------
	string ScriptLua::getLastError()
	{
		const char* message = lua_tostring(m_pState, -1);
		string result = "Unknown error";
		if(message)
			result = message;
		lua_pop(m_pState, 1);

		return result;
	}
//----------------------------------------------------------------------
	void ScriptLua::update(dword _delta)
	{
		m_nTimePassed += _delta;

		switch(m_State)
		{
		case STATE_NOT_RUNNING:
			break;
		case STATE_WAITING_TIME:
			if(m_nTimePassed >= m_nWaitTime)
				resume(0.0f);
			break;
		case STATE_WAITING_FRAMES:
			if(!--m_nWaitFrames)
				resume(0.0f);
			break;
		}
	}
//----------------------------------------------------------------------
	int ScriptLua::yield()
	{
		return lua_yield(m_pState, 1);
	}
//----------------------------------------------------------------------
	void ScriptLua::resume(float _param)
	{
		m_State = STATE_RUNNING;

		lua_pushnumber(m_pState, _param);
		int status = lua_resume(m_pState, 1);
		if(status && status != LUA_YIELD)
		{
			string temp = getLastError();
			wstring error(temp.begin(), temp.end());
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Error encountered while resuming script: %ls", error.c_str());

			return;
		}
	}
//----------------------------------------------------------------------
	void ScriptLua::kill()
	{
		for (int i = 0; i < lua_gettop(m_pParent); ++i)
		{
			if((lua_touserdata(m_pParent, i) == m_pState) || lua_touserdata(m_pParent, i) == this)
			{
				lua_remove(m_pParent, i--);
			}
		}

		m_State = STATE_DONE;
	}
//----------------------------------------------------------------------
}