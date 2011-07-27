/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderManager.cpp
Version:	0.14
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ShaderManager.h"
#include "ShaderConstantTypes.h"
#include "ShaderDX9.h"
#include "ShaderDX10.h"
#include "XMLDocument.h"
#include "XMLParser.h"


namespace nGENE
{
	ShaderManager::ShaderManager():
		m_bDebugEnabled(false),
		m_pActiveVS(NULL),
		m_pActiveGS(NULL),
		m_pActivePS(NULL)
	{
		init();
	}
//----------------------------------------------------------------------
	ShaderManager::~ShaderManager()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ShaderManager::init()
	{
	}
//----------------------------------------------------------------------
	void ShaderManager::cleanup()
	{
		if(!m_pShaders.empty())
		{
			HashTable <wstring, Shader*>::iterator iter;
			for(iter = m_pShaders.begin(); iter != m_pShaders.end(); ++iter)
				delete iter->second;

			m_pShaders.clear();
		}

		if(!m_pDataTypes.empty())
		{
			HashTable <wstring, DATATYPE_DESC*>::iterator iter;
			for(iter = m_pDataTypes.begin(); iter != m_pDataTypes.end(); ++iter)
				delete iter->second;

			m_pDataTypes.clear();
		}

		if(!m_pSemantics.empty())
		{
			HashTable <wstring, SEMANTIC_DESC*>::iterator iter;
			for(iter = m_pSemantics.begin(); iter != m_pSemantics.end(); ++iter)
				delete iter->second;

			m_pSemantics.clear();
		}
	}
//----------------------------------------------------------------------
	DATATYPE_DESC* ShaderManager::createDataType()
	{
		return new DATATYPE_DESC;
	}
//----------------------------------------------------------------------
	void ShaderManager::addDataType(DATATYPE_DESC* _type)
	{
		// Data-type already exists, so exit
		if(m_pDataTypes.find(_type->stName) != m_pDataTypes.end())
			return;

		m_pDataTypes[_type->stName] = _type;
	}
//----------------------------------------------------------------------
	SEMANTIC_DESC* ShaderManager::createSemantic()
	{
		return new SEMANTIC_DESC;
	}
//----------------------------------------------------------------------
	void ShaderManager::addSemantic(SEMANTIC_DESC* _semantic)
	{
		// Semantic already exists, so exit
		if(m_pSemantics.find(_semantic->stName) != m_pSemantics.end())
			return;

		m_pSemantics[_semantic->stName] = _semantic;
	}
//----------------------------------------------------------------------
	Shader* ShaderManager::createShader(const SHADER_DESC& _desc)
	{
		// If shader exists, return it
		wstring name = _desc.name;
		if(m_pShaders.find(name) != m_pShaders.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader with the name: %ls already exists", name.c_str());
			return m_pShaders[name];
		}

		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new shader: %ls", name.c_str());

		SHADER_DESC desc = _desc;
		desc.debugInfo = m_bDebugEnabled;

		switch(Engine::getSingleton().getRenderingAPI())
		{
		case API_DIRECTX9C: m_pShaders[name] = new ShaderDX9(desc);
			break;
		case API_DIRECTX10: m_pShaders[name] = new ShaderDX10(desc);
			break;
		}

		return m_pShaders[name];
	}
//----------------------------------------------------------------------
	Shader* ShaderManager::getShader(const wstring& _fileName)
	{
		// If shader doesn't exist, return
		if(m_pShaders.find(_fileName) == m_pShaders.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader with the name: %ls does not exist", _fileName.c_str());
			return NULL;
		}

		return m_pShaders[_fileName];
	}
//----------------------------------------------------------------------
	void ShaderManager::removeShader(const wstring& _name)
	{
		// If shader doesn't exist, return
		if(m_pShaders.find(_name) == m_pShaders.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader with the name: %ls does not exist", _name.c_str());
			return;
		}

		delete m_pShaders[_name];
		m_pShaders.erase(_name);
	}
//----------------------------------------------------------------------
	bool ShaderManager::findShader(const wstring& _name)
	{
		if(m_pShaders.find(_name) == m_pShaders.end())
			return false;
		return true;
	}
//----------------------------------------------------------------------
	void ShaderManager::bindShader(const wstring& _name)
	{
		// If shader doesn't exist, return
		if(m_pShaders.find(_name) == m_pShaders.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader with the name: %ls does not exist", _name.c_str());
			return;
		}

		m_pShaders[_name]->bind();
	}
//----------------------------------------------------------------------
	void ShaderManager::bindShader(Shader* _shader)
	{
		SHADER_TYPE type = _shader->getDesc().type;
		switch(type)
		{
		case SHADER_VERTEX:
			bindVertexShader(_shader);
			break;
		case SHADER_PIXEL:
			bindPixelShader(_shader);
			break;
		case SHADER_GEOMETRY:
			bindGeometryShader(_shader);
			break;
		default:
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Unknown shader type when binding");
			break;
		}
	}
//----------------------------------------------------------------------
	void ShaderManager::bindVertexShader(Shader* _vertexShader)
	{
		if(_vertexShader != m_pActiveVS)
		{
			m_pActiveVS = _vertexShader;
			m_pActiveVS->bind();
		}
		else
			m_pActiveVS->setDefaults();
	}
//----------------------------------------------------------------------
	void ShaderManager::bindGeometryShader(Shader* _geometryShader)
	{
		if(_geometryShader != m_pActiveGS)
		{
			m_pActiveGS = _geometryShader;
			m_pActiveGS->bind();
		}
		else
			m_pActiveGS->setDefaults();
	}
//----------------------------------------------------------------------
	void ShaderManager::bindPixelShader(Shader* _pixelShader)
	{
		if(_pixelShader != m_pActivePS)
		{
			m_pActivePS = _pixelShader;
			m_pActivePS->bind();
		}
		else
			m_pActivePS->setDefaults();
	}
//----------------------------------------------------------------------
	void ShaderManager::unbindShader(SHADER_TYPE _type)
	{
		switch(_type)
		{
		case SHADER_VERTEX:
			{
				if(m_pActiveVS)
					m_pActiveVS->unbind();
				m_pActiveVS = NULL;
			}
			break;
		case SHADER_PIXEL:
			{
				if(m_pActivePS)
					m_pActivePS->unbind();
				m_pActivePS = NULL;
			}
			break;
		case SHADER_GEOMETRY:
			{
				if(m_pActiveGS)
					m_pActiveGS->unbind();
				m_pActiveGS = NULL;
			}
			break;
		default:
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Unknown shader type when unbinding");
			break;
		}
	}
//----------------------------------------------------------------------
	DATATYPE_DESC* ShaderManager::getDataType(const wstring& _name)
	{
		// If data-type doesn't exist, return
		if(m_pDataTypes.find(_name) == m_pDataTypes.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Data-type: %ls does not exist", _name.c_str());
			return NULL;
		}

		return m_pDataTypes[_name];
	}
//----------------------------------------------------------------------
	SEMANTIC_DESC* ShaderManager::getSemantic(const wstring& _name)
	{
		// If semantic doesn't exist, return
		if(m_pSemantics.find(_name) == m_pSemantics.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Semantic: %ls does not exist", _name.c_str());
			return NULL;
		}

		return m_pSemantics[_name];
	}
//----------------------------------------------------------------------
}