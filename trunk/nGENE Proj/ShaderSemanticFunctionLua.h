/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderSemanticFunctionLua.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHADERSEMANTICFUNCTIONLUA_H_
#define __INC_SHADERSEMANTICFUNCTIONLUA_H_


#include "Prerequisities.h"
#include "ShaderConstantTypes.h"


namespace nGENE
{
	/// Shader semantic function to be used with Lua.
	class nGENEDLL ShaderSemanticFunctionLua: public ShaderSemanticFunction
	{
	private:
		ScriptLua* m_pScript;
		string m_stFunction;

	public:
		ShaderSemanticFunctionLua(ScriptLua* _script,
								  const string& _function);
		~ShaderSemanticFunctionLua();

		void operator()(void* _values, uint _count);
	};
}


#endif