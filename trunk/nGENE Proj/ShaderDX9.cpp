/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderDX9.cpp
Version:	0.19
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DX9Mapping.h"
#include "RendererDX9.h"
#include "ShaderConstant.h"
#include "ShaderDX9.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ShaderDX9::Type(L"ShaderDX9", &Shader::Type);


	ShaderDX9::ShaderDX9(const SHADER_DESC& _desc):
		Shader(_desc),
		m_pVertexShader(NULL),
		m_pConstants(NULL)
	{
	}
//----------------------------------------------------------------------
	ShaderDX9::~ShaderDX9()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ShaderDX9::cleanup()
	{
		NGENE_RELEASE(m_pConstants);
		NGENE_RELEASE(m_pEffect);
		NGENE_RELEASE(m_pPixelShader);
		NGENE_RELEASE(m_pVertexShader);
	}
//----------------------------------------------------------------------
	void ShaderDX9::bind() const
	{
		Shader::bind();


		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());


		switch(m_ShaderDesc.type)
		{
		case SHADER_VERTEX: pDevice->SetVertexShader(m_pVertexShader);
			break;
		case SHADER_PIXEL: pDevice->SetPixelShader(m_pPixelShader);
			break;
		case SHADER_EFFECT:
			break;
		default: return;
		}
	}
//----------------------------------------------------------------------
	void ShaderDX9::unbind() const
	{
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		switch(m_ShaderDesc.type)
		{
		case SHADER_VERTEX:	pDevice->SetVertexShader(NULL);
			break;
		case SHADER_PIXEL: pDevice->SetPixelShader(NULL);
			break;
		case SHADER_EFFECT:
			break;
		default: return;
		}
	}
//----------------------------------------------------------------------
	D3DXMACRO* ShaderDX9::getMacros()
	{
		D3DXMACRO* pMacros = NULL;
		uint count = m_vDefines.size();
		if(count)
		{
			pMacros = new D3DXMACRO[count + 1];
			for(uint i = 0; i < count; ++i)
			{
				pMacros[i].Name = m_vDefines[i].c_str();
				pMacros[i].Definition = "1";
			}

			pMacros[count].Name = NULL;
			pMacros[count].Definition = NULL;
		}

		return pMacros;
	}
//----------------------------------------------------------------------
	void ShaderDX9::createVertexShader()
	{
		DeviceRender& pDeviceRender = Renderer::getSingleton().getDeviceRender();
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(pDeviceRender.getDevicePtr());


		ID3DXBuffer* pCode = NULL;
		ID3DXBuffer* pErrors = NULL;

		// Get shader profile
		if(!m_ShaderDesc.profile.compare(""))
		{
			char temp[10];
			pDeviceRender.testCapabilities(CAPS_VERTEX_SHADER, &temp);
			m_ShaderDesc.profile = temp;
		}
		if(!m_ShaderDesc.function.compare(""))
			m_ShaderDesc.function = "main";

		dword flags = 0;
		if(m_ShaderDesc.debugInfo)
			flags |= D3DXSHADER_DEBUG;

		// Get macros
		D3DXMACRO* pMacros = getMacros();

		// Load shader file
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(m_ShaderDesc.fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint nSize = pFile->getSize();
		char* pBuffer = new char[nSize];
		pFile->read((char*)pBuffer, nSize);
		mgr.closeFile(m_ShaderDesc.fileName);

		D3DXCompileShader(pBuffer,
						  nSize,
						  pMacros,
						  NULL,
						  m_ShaderDesc.function.c_str(),
						  m_ShaderDesc.profile.c_str(),
						  flags,
						  &pCode,
						  &pErrors,
						  &m_pConstants);

		NGENE_DELETE_ARRAY(pMacros);
		NGENE_DELETE_ARRAY(pBuffer);
		
		if((pErrors) || (!pCode))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"One or more errors occurred while compiling vertex shader");
			if(pErrors)
			{
				handleErrors(pErrors);
				NGENE_RELEASE(pErrors);
			}
			return;
		}

		HRESULT hr;
		if(FAILED(hr = pDevice->CreateVertexShader((dword*)pCode->GetBufferPointer(), &m_pVertexShader)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader couldn't be created. Reason: %ls",	DX9Mapping::getDXErrorDescription(hr));

			NGENE_RELEASE(pCode);
			return;
		}
		m_pConstants->SetDefaults(pDevice);


		NGENE_RELEASE(pCode);


		loadConstants();
	}
//----------------------------------------------------------------------
	void ShaderDX9::createGeometryShader()
	{
		Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Geometry Shaders are not supported in DirectX 9.0c. Try using DirectX 10 or newer");
	}
//----------------------------------------------------------------------
	void ShaderDX9::createPixelShader()
	{
		DeviceRender& pDeviceRender = Renderer::getSingleton().getDeviceRender();
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(pDeviceRender.getDevicePtr());


		ID3DXBuffer* pCode = NULL;
		ID3DXBuffer* pErrors = NULL;

		// Get shader profile
		if(!m_ShaderDesc.profile.compare(""))
		{
			char temp[10];
			pDeviceRender.testCapabilities(CAPS_PIXEL_SHADER, &temp);
			m_ShaderDesc.profile = temp;
		}
		if(!m_ShaderDesc.function.compare(""))
			m_ShaderDesc.function = "main";

		dword flags = 0;
		if(m_ShaderDesc.debugInfo)
			flags |= D3DXSHADER_DEBUG;

		// Get macros
		D3DXMACRO* pMacros = getMacros();

		// Load shader file
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(m_ShaderDesc.fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint nSize = pFile->getSize();
		char* pBuffer = new char[nSize];
		pFile->read((char*)pBuffer, nSize);
		mgr.closeFile(m_ShaderDesc.fileName);

		D3DXCompileShader(pBuffer,
						  nSize,
						  pMacros,
						  NULL,
						  m_ShaderDesc.function.c_str(),
						  m_ShaderDesc.profile.c_str(),
						  flags,
						  &pCode,
						  &pErrors,
						  &m_pConstants);

		NGENE_DELETE_ARRAY(pMacros);
		NGENE_DELETE_ARRAY(pBuffer);
		
		if((pErrors) || (!pCode))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"One or more errors occurred while compiling pixel shader");
			if(pErrors)
				handleErrors(pErrors);
			return;
		}

		HRESULT hr;
		if(FAILED(hr = pDevice->CreatePixelShader((dword*)pCode->GetBufferPointer(), &m_pPixelShader)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader couldn't be created. Reason: %ls", DX9Mapping::getDXErrorDescription(hr));
		}
		m_pConstants->SetDefaults(pDevice);


		NGENE_RELEASE(pCode);


		loadConstants();
	}
//----------------------------------------------------------------------
	void ShaderDX9::loadConstants()
	{
		D3DXCONSTANTTABLE_DESC tableDesc;
		m_pConstants->GetDesc(&tableDesc);

		for(uint i = 0; i < tableDesc.Constants; ++i)
		{
			D3DXHANDLE constant = m_pConstants->GetConstant(NULL, i);
			D3DXCONSTANT_DESC constantDesc;
			uint count;

			m_pConstants->GetConstantDesc(constant, &constantDesc, &count);

			// We are skipping textures and other objects
			if(constantDesc.Class == D3DXPC_OBJECT)
				continue;


			// Get default values
			SShaderConstantType::BASE_TYPE type;
			void* pDefault = NULL;
			switch(constantDesc.Type)
			{
			case D3DXPT_FLOAT:
				if(constantDesc.DefaultValue)
				{
					pDefault = new float[constantDesc.Rows * constantDesc.Columns * constantDesc.RegisterCount];
					float* pInput = (float*)pDefault;
					float* pBuf = (float*)constantDesc.DefaultValue;
					for(uint j = 0; j < constantDesc.RegisterCount; ++j)
					{
						for(uint i = 0; i < constantDesc.Rows; ++i)
						{
							memcpy(pInput, pBuf, constantDesc.Columns * sizeof(float));
							pInput += constantDesc.Columns;
							pBuf += sizeof(float);
						}
					}
				}

				type = SShaderConstantType::BT_FLOAT;

				break;

			case D3DXPT_INT:
				if(constantDesc.DefaultValue)
				{
					pDefault = new int[constantDesc.Rows * constantDesc.Columns * constantDesc.RegisterCount];
					int* pInput = (int*)pDefault;
					int* pBuf = (int*)constantDesc.DefaultValue;
					for(uint j = 0; j < constantDesc.RegisterCount; ++j)
					{
						for(uint i = 0; i < constantDesc.Rows; ++i)
						{
							memcpy(pInput, pBuf, constantDesc.Columns * sizeof(int));
							pInput += constantDesc.Columns;
							pBuf += sizeof(int);
						}
					}
				}

				type = SShaderConstantType::BT_INT;

				break;

			case D3DXPT_BOOL:
				if(constantDesc.DefaultValue)
				{
					pDefault = new bool[constantDesc.Rows * constantDesc.Columns * constantDesc.RegisterCount];
					bool* pInput = (bool*)pDefault;
					int* pBuf = (int*)constantDesc.DefaultValue;
					for(uint j = 0; j < constantDesc.RegisterCount; ++j)
					{
						for(uint i = 0; i < constantDesc.Rows; ++i)
						{
							memset(pInput, (*pBuf ? true : false), constantDesc.Columns * sizeof(bool));
							pInput += constantDesc.Columns;
							pBuf += sizeof(int);
						}
					}
				}

				type = SShaderConstantType::BT_BOOL;

				break;

			default:
				if(constantDesc.DefaultValue)
				{
					pDefault = new float[constantDesc.Rows * constantDesc.Columns * constantDesc.RegisterCount];
					float* pInput = (float*)pDefault;
					float* pBuf = (float*)constantDesc.DefaultValue;
					for(uint j = 0; j < constantDesc.RegisterCount; ++j)
					{
						for(uint i = 0; i < constantDesc.Rows; ++i)
						{
							memcpy(pInput, pBuf, constantDesc.Columns * sizeof(float));
							pInput += constantDesc.Columns;
							pBuf += sizeof(float);
						}
					}
				}

				type = SShaderConstantType::BT_FLOAT;

				break;
			}
			
			ShaderConstant shaderConstant;
			shaderConstant.setName(constantDesc.Name);
			shaderConstant.setValuesCount(constantDesc.Elements * constantDesc.Columns * constantDesc.Rows);
			shaderConstant.setDefaultValue(pDefault);
			shaderConstant.setType(type);

			ShaderConstant* pConstant = new ShaderConstant(shaderConstant);
			m_pVariables.push_back(pConstant);
			m_pVariablesRef[pConstant->getName()] = pConstant;

			NGENE_DELETE_ARRAY(pDefault);
		}
	}
//----------------------------------------------------------------------
	void ShaderDX9::handleErrors(ID3DXBuffer* _errors)
	{
		void* buffer = _errors->GetBufferPointer();
		string msg = (char*)buffer;

		wstring msg2 = L"Shader error: ";
		msg2.append(msg.begin(), msg.end());

		Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			msg2.c_str());
	}
//----------------------------------------------------------------------
	void ShaderDX9::createEffect()
	{
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		ID3DXBuffer* pErrors = NULL;

		// Load texture file
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(m_ShaderDesc.fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint nSize = pFile->getSize();
		void* pBuffer = new char[nSize];
		pFile->read((char*)pBuffer, nSize);
		mgr.closeFile(m_ShaderDesc.fileName);

		// Create effect
		D3DXCreateEffect(pDevice,
						 pBuffer,
						 nSize,
						 NULL,
						 NULL,
						 0,
						 NULL,
						 &m_pEffect,
						 &pErrors);

		NGENE_DELETE_ARRAY(pBuffer);

		if(pErrors)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"One or more errors occured while creating effect");
			return;
		}
	}
//----------------------------------------------------------------------
	void ShaderDX9::setFloat(const string& _name, const float& _value)
	{
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		D3DXHANDLE hConstant = getVariableByName(_name);
		if(hConstant)
			m_pConstants->SetFloat(pDevice, hConstant, _value);
	}
//----------------------------------------------------------------------
	void ShaderDX9::setFloatArray(const string& _name, const float* _values,
		const uint _count) const
	{
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		D3DXHANDLE hConstant = getVariableByName(_name);
		if(hConstant)
			m_pConstants->SetFloatArray(pDevice, hConstant, _values, _count);
	}
//----------------------------------------------------------------------
	void ShaderDX9::setMatrix(const string& _name, const Matrix4x4& _mat)
	{
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		D3DXHANDLE hConstant = getVariableByName(_name);
		if(hConstant)
		{
			D3DXMATRIX mat = DX9Mapping::matrix4ToD3DXMATRIX(_mat);
			m_pConstants->SetMatrix(pDevice, hConstant, &mat);
		}
	}
//----------------------------------------------------------------------
	void ShaderDX9::setInteger(const string& _name, const int& _value)
	{
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		D3DXHANDLE hConstant = getVariableByName(_name);
		if(hConstant)
			m_pConstants->SetInt(pDevice, hConstant, _value);
	}
//----------------------------------------------------------------------
	void ShaderDX9::setIntegerArray(const string& _name, const int* _values,
		const uint _count) const
	{
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		D3DXHANDLE hConstant = getVariableByName(_name);
		if(hConstant)
			m_pConstants->SetIntArray(pDevice, hConstant, _values, _count);
	}
//----------------------------------------------------------------------
	void ShaderDX9::setBoolean(const string& _name, const bool& _value)
	{
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		D3DXHANDLE hConstant = getVariableByName(_name);
		if(hConstant)
			m_pConstants->SetBool(pDevice, hConstant, _value);
	}
//----------------------------------------------------------------------
	void ShaderDX9::setBooleanArray(const string& _name, const bool* _values,
		const uint _count) const
	{
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		D3DXHANDLE hConstant = getVariableByName(_name);
		if(hConstant)
		{
			BOOL* pData = new BOOL[_count];
			for(uint i = 0; i < _count; ++i)
				pData[i] = _values[i];
			m_pConstants->SetBoolArray(pDevice, hConstant, pData, _count);
			NGENE_DELETE_ARRAY(pData);
		}
	}
//----------------------------------------------------------------------
	D3DXHANDLE ShaderDX9::getVariableByName(const string& _name) const
	{
		D3DXHANDLE result = m_pConstants->GetConstantByName(NULL, _name.c_str());

		if(!result)
		{
			wstring temp;
			temp.assign(_name.begin(), _name.end());
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Variable: %ls not found in shader: %ls", temp.c_str(),
				m_ShaderDesc.fileName.c_str());
		}

		return result;
	}
//----------------------------------------------------------------------
}