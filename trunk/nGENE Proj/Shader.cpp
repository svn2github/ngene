/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Shader.cpp
Version:	0.14
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Shader.h"
#include "FuncSplit.h"
#include "ShaderConstant.h"
#include "XMLDocument.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Shader::Type(L"Shader", NULL);


	Shader::Shader(const SHADER_DESC& _desc):
		m_ShaderDesc(_desc)
	{
	}
//----------------------------------------------------------------------
	Shader::~Shader()
	{
		if(!m_pVariables.empty())
		{
			m_pVariablesRef.clear();

			vector <ShaderConstant*>::iterator iter;
			for(iter = m_pVariables.begin(); iter != m_pVariables.end(); ++iter)
				delete (*iter);

			m_pVariables.clear();
		}
	}
//----------------------------------------------------------------------
	void Shader::create()
	{
		// Specify type of the shader to create.
		switch(m_ShaderDesc.type)
		{
		case SHADER_VERTEX: createVertexShader();
			break;
		case SHADER_GEOMETRY: createGeometryShader();
			break;
		case SHADER_PIXEL: createPixelShader();
			break;
		case SHADER_EFFECT: createEffect();
			break;
		default:
			break;
		}
	}
//----------------------------------------------------------------------
	void Shader::update() const
	{
		// Update variables
		if(!m_pVariables.empty())
		{
			vector <ShaderConstant*>::iterator iter;
			for(iter = m_pVariables.begin(); iter != m_pVariables.end(); ++iter)
			{
				ShaderConstant* pConstant = (*iter);
				if(pConstant->isDynamic())
				{
					pConstant->update();
					
					switch(pConstant->getType())
					{
					case SShaderConstantType::BT_FLOAT:
						setFloatArray(pConstant->getName().c_str(),
							(float*)pConstant->getDefaultValues(),
							pConstant->getValuesCount());
						break;
					case SShaderConstantType::BT_INT:
						setIntegerArray(pConstant->getName().c_str(),
							(int*)pConstant->getDefaultValues(),
							pConstant->getValuesCount());
						break;
					case SShaderConstantType::BT_BOOL:
						setBooleanArray(pConstant->getName().c_str(),
							(bool*)pConstant->getDefaultValues(),
							pConstant->getValuesCount());
						break;
					}
				}
			}
		}
	}
//----------------------------------------------------------------------
	void Shader::bind() const
	{
		// Reset all constants
		setDefaults();
	}
//----------------------------------------------------------------------
	void Shader::setDefaults() const
	{
		if(!m_pVariables.empty())
		{
			vector <ShaderConstant*>::iterator iter;
			for(iter = m_pVariables.begin(); iter != m_pVariables.end(); ++iter)
			{
				ShaderConstant* pConstant = *iter;

				switch(pConstant->getType())
				{
				case SShaderConstantType::BT_FLOAT:
					setFloatArray(pConstant->getName().c_str(),
						(float*)pConstant->getDefaultValues(),
						pConstant->getValuesCount());
					break;
				case SShaderConstantType::BT_INT:
					setIntegerArray(pConstant->getName().c_str(),
						(int*)pConstant->getDefaultValues(),
						pConstant->getValuesCount());
					break;
				case SShaderConstantType::BT_BOOL:
					setBooleanArray(pConstant->getName().c_str(),
						(bool*)pConstant->getDefaultValues(),
						pConstant->getValuesCount());
					break;
				}
			}
		}
	}
//----------------------------------------------------------------------
	void Shader::parse(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;

		// Parse data
		string key  = pNode->getAttribute("name");
		m_ShaderDesc.profile = pNode->getAttribute("profile");
		m_ShaderDesc.function= pNode->getAttribute("function");

		key = pNode->getAttribute("defines");
		if(key.compare("") != 0)
		{
			FuncSplit <string> tokenizer;
			tokenizer(key, m_vDefines, ";, ");
		}

		create();


		// Get rid of defines
		m_vDefines.clear();
	}
//----------------------------------------------------------------------
	ShaderConstant* Shader::getConstant(const string& _name) const
	{
		// Shader constant not found, so exit
		if(m_pVariablesRef.find(_name) == m_pVariablesRef.end())
		{
			wstring name(_name.begin(), _name.end());
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader constant %ls not found in shader", name.c_str());
			return NULL;
		}

		return m_pVariablesRef[_name];
	}
//----------------------------------------------------------------------
}