/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScriptLua.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCRIPTLUA_H_
#define __INC_SCRIPTLUA_H_


#include "BaseClass.h"
#include "Prerequisities.h"

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

#include "Matrix4x4.h"

struct lua_State;


namespace nGENE
{
	/// Enum describing state of the script.
	enum SCRIPT_STATE
	{
		STATE_NOT_RUNNING,				///< Script is currently not running
		STATE_RUNNING,					///< Script is currently running
		STATE_WAITING_TIME,				///< Script is sleeping for N milliseconds
		STATE_WAITING_FRAMES,			///< Script is sleeping for N frames
		STATE_DONE						///< Script is finished
	};


	/** Single lua script.
		@par
			Each of the scripts has its own lua state.
		@par
			You can either load script from file (either in 
			text or precompiled format) or text buffer through
			runScriptFromFile() and runScriptFromString() methods
			respectively.
	*/
	class nGENEDLL ScriptLua
	{
		EXPOSE_TYPE
	private:
		SCRIPT_STATE m_State;				///< State in which the script is

		lua_State* m_pState;				///< This scripts state (thread)
		lua_State* m_pParent;				///< Parent state (thread)


		uint m_nWaitTime;					///< Number of milliseconds to sleep
		uint m_nWaitFrames;					///< Number of frames to sleep

		uint m_nTimePassed;					///< Time passed


		string getLastError();

	public:
		ScriptLua(lua_State* _parent);
		~ScriptLua();

		/** Runs Lua script from file.
			@param
				_fileName name of the file containing script. It 
				can contain either text data or Lua byte-code.
		*/
		void runScriptFromFile(const string& _fileName);

		/** Runs Lua script from text buffer.
			@param
				_script text buffer containing script to be executed.
		*/
		void runScriptFromString(const string& _script);

		/** Loads Lua script from file.
			@param
				_fileName name of the file containing script. It 
				can contain either text data or Lua byte-code.
			@remarks
				This method should be used if script contains functions
				which has to be called manually (i.e. they are not called
				in the script's global scope. To call this functions
				use callFunction().
		*/
		void loadScriptFromFile(const string& _fileName);

		/// Updates script.
		void update(dword _delta);

		/// Yields thread execution.
		int yield();
		/// Resumes thread execution.
		void resume(float _param=0.0f);
		/// Kills a thread.
		void kill();

		/// Suspends thread for specified number of frames.
		void setWaitFrames(uint _value);
		/// Suspends thread for specified number of milliseconds.
		void setWaitTime(uint _time);

		/// Returns time passed since script creation.
		uint getTime() const;

		/// Returns lua_State associated with this script.
		lua_State* getThread() const;

		/// Returns state of the script.
		SCRIPT_STATE getState() const;

		/** Calls a specified function from the script.
			@param
				_function name of the function to invoke.
			@param
				_buffer to which value of function call should be copied.
			@param
				_size number of bytes to be copied.
			@remarks
				If no function is found an error will be risen.
		*/
		template <typename RetVal>
		void callFunction(const string& _function, void* _buffer, uint _size);
	};



	inline void ScriptLua::setWaitFrames(uint _value)
	{
		m_nWaitFrames = _value;
		m_State = STATE_WAITING_FRAMES;
	}
//----------------------------------------------------------------------
	inline void ScriptLua::setWaitTime(uint _time)
	{
		m_nWaitTime = _time;
		m_State = STATE_WAITING_TIME;
	}
//----------------------------------------------------------------------
	inline uint ScriptLua::getTime() const
	{
		return m_nTimePassed;
	}
//----------------------------------------------------------------------
	inline lua_State* ScriptLua::getThread() const
	{
		return m_pState;
	}
//----------------------------------------------------------------------
	template <typename RetVal>
	inline void ScriptLua::callFunction(const string& _function, void* _buffer, uint _size)
	{
		luabind::adl::object obj = luabind::call_function<luabind::adl::object>(m_pState, _function.c_str());
		RetVal val = luabind::object_cast<RetVal>(obj);
		memcpy(_buffer, &val, _size);
	}
//----------------------------------------------------------------------
	inline SCRIPT_STATE ScriptLua::getState() const
	{
		return m_State;
	}
//----------------------------------------------------------------------
}


#endif