/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScriptSystemLua.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCRIPTSYSTEMLUA_H_
#define __INC_SCRIPTSYSTEMLUA_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "Task.h"


struct lua_State;


namespace nGENE
{
	/** ScriptSystemLua is a way to embed Lua in your applications.
		@par
			In nGENE Lua works using concept of threads, i.e. each 
			script has corresponding lua state. All states are children
			of "master" state.
		@remarks
			To create ScriptSystemLua make sure you make call to the 
			init() method after creating class instance.
	*/
	class nGENEDLL ScriptSystemLua: public BaseClass, public Task
	{
		EXPOSE_TYPE
	private:
		lua_State* m_pLuaVM;				///< Lua virtual machine

		/// Vector holding all created scripts
		vector <ScriptLua*> m_vScripts;

		dword m_dwLast;						///< Previous frame time

	public:
		ScriptSystemLua();
		virtual ~ScriptSystemLua();

		void init();
		void cleanup();

		/// Updates scripting system.
		void update();

		/** Creates new ScriptLua object.
			@returns
				ScriptLua* on success returns valid pointer to the newly
				created script object and NULL on failure.
		*/
		ScriptLua* createScript();

		/// Returns script with given index.
		ScriptLua* getScript(uint _index) const;

		/// Removes script with the given index.
		void removeScript(uint _index);
		/// Removes script with the given address.
		void removeScript(ScriptLua* _script);

		/// Returns number of registered scripts.
		uint getScriptsNum() const;

		/// Returns pointer to the "master" state.
		lua_State* getVM();
	};



	inline lua_State* ScriptSystemLua::getVM()
	{
		return m_pLuaVM;
	}
//----------------------------------------------------------------------
	inline uint ScriptSystemLua::getScriptsNum() const
	{
		return m_vScripts.size();
	}
//----------------------------------------------------------------------
}


#endif