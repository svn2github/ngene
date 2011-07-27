/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderSemanticFunctionLua.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ShaderSemanticFunctionLua.h"
#include "ScriptLua.h"


namespace nGENE
{
	ShaderSemanticFunctionLua::ShaderSemanticFunctionLua(ScriptLua* _script,
		const string& _function):
		m_pScript(_script),
		m_stFunction(_function)
	{
	}
//----------------------------------------------------------------------
	ShaderSemanticFunctionLua::~ShaderSemanticFunctionLua()
	{
	}
//----------------------------------------------------------------------
	void ShaderSemanticFunctionLua::operator()(void* _values, uint _count)
	{
		uint size = sizeof(float);
		switch(_count)
		{
		case 1:
			m_pScript->callFunction <float>(m_stFunction, _values, _count * size);
			break;

		case 2:
			m_pScript->callFunction <Vector2>(m_stFunction, _values, _count * size);
			break;

		case 3:
			m_pScript->callFunction <Vector3>(m_stFunction, _values, _count * size);
			break;

		case 4:
			m_pScript->callFunction <Vector4>(m_stFunction, _values, _count * size);
			break;

		case 16:
			m_pScript->callFunction <Matrix4x4>(m_stFunction, _values, _count * size);
			break;
		}
	}
//----------------------------------------------------------------------
}