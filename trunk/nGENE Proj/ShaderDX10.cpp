/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderDX10.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ShaderDX10.h"
#include "ShaderConstant.h"
#include "DX10Mapping.h"
#include "RendererDX10.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ShaderDX10::Type(L"ShaderDX10", &Shader::Type);


	ShaderDX10::ShaderDX10(const SHADER_DESC& _desc):
		Shader(_desc),
		m_pVertexShader(NULL),
		m_pConstants(NULL),
		m_pCode(NULL)
	{
	}
//----------------------------------------------------------------------
	ShaderDX10::~ShaderDX10()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ShaderDX10::cleanup()
	{
		NGENE_RELEASE(m_pConstants);
		NGENE_RELEASE(m_pPixelShader);
		NGENE_RELEASE(m_pGeometryShader);
		NGENE_RELEASE(m_pVertexShader);
		NGENE_RELEASE(m_pCode);
	}
//----------------------------------------------------------------------
	void ShaderDX10::bind() const
	{
		ID3D10Device* pDevice = reinterpret_cast<ID3D10Device*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		//m_pConstants->SetDefaults(pDevice);
		Shader::bind();

		switch(m_ShaderDesc.type)
		{
		case SHADER_VERTEX: pDevice->VSSetShader(m_pVertexShader);
			break;
		case SHADER_GEOMETRY: pDevice->GSSetShader(m_pGeometryShader);
			break;
		case SHADER_PIXEL: pDevice->PSSetShader(m_pPixelShader);
			break;
		case SHADER_EFFECT:
			break;
		default: return;
		}
	}
//----------------------------------------------------------------------
	void ShaderDX10::unbind() const
	{
		ID3D10Device* pDevice = reinterpret_cast<ID3D10Device*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		switch(m_ShaderDesc.type)
		{
		case SHADER_VERTEX: pDevice->VSSetShader(NULL);
			break;
		case SHADER_GEOMETRY: pDevice->GSSetShader(NULL);
			break;
		case SHADER_PIXEL: pDevice->PSSetShader(NULL);
			break;
		case SHADER_EFFECT:
			break;
		default: return;
		}
	}
//----------------------------------------------------------------------
	void ShaderDX10::createVertexShader()
	{
		DeviceRender& pDeviceRender = Renderer::getSingleton().getDeviceRender();
		ID3D10Device* pDevice = reinterpret_cast<ID3D10Device*>(pDeviceRender.getDevicePtr());


		ID3D10Blob* pErrors = NULL;

		// Get shader profile
		m_ShaderDesc.profile = "vs_4_0";
		if(!m_ShaderDesc.function.compare(""))
			m_ShaderDesc.function = "main";

		dword flags = 0;
		if(m_ShaderDesc.debugInfo)
			flags |= D3D10_SHADER_DEBUG;

		// Load shader file
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(m_ShaderDesc.fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint size = pFile->getSize();
		char* buffer = new char[size];
		pFile->read((char*)buffer, size);
		mgr.closeFile(m_ShaderDesc.fileName);

		string stName(m_ShaderDesc.fileName.begin(), m_ShaderDesc.fileName.end());

		D3DX10CompileFromMemory(buffer,
								size,
								stName.c_str(),
							    NULL,
							    NULL,
							    m_ShaderDesc.function.c_str(),
							    m_ShaderDesc.profile.c_str(),
							    flags,
							    0,
							    NULL,
							    &m_pCode,
							    &pErrors,
							    NULL);

		NGENE_DELETE_ARRAY(buffer);
		
		if((pErrors) || (!m_pCode))
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
		if(FAILED(hr = pDevice->CreateVertexShader((dword*)m_pCode->GetBufferPointer(),
												   m_pCode->GetBufferSize(),
												   &m_pVertexShader)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader couldn't be created. Reason: %ls", DX10Mapping::getDXErrorDescription(hr));
			return;
		}


//		m_pConstants->SetDefaults(pDevice);

		loadConstants();
	}
//----------------------------------------------------------------------
	void ShaderDX10::createGeometryShader()
	{
		DeviceRender& pDeviceRender = Renderer::getSingleton().getDeviceRender();
		ID3D10Device* pDevice = reinterpret_cast<ID3D10Device*>(pDeviceRender.getDevicePtr());


		ID3D10Blob* pErrors = NULL;

		// Get shader profile
		m_ShaderDesc.profile = "gs_4_0";
		if(!m_ShaderDesc.function.compare(""))
			m_ShaderDesc.function = "main";

		dword flags = 0;
		if(m_ShaderDesc.debugInfo)
			flags |= D3D10_SHADER_DEBUG;

		// Load shader file
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(m_ShaderDesc.fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint size = pFile->getSize();
		char* buffer = new char[size];
		pFile->read((char*)buffer, size);
		mgr.closeFile(m_ShaderDesc.fileName);

		string stName(m_ShaderDesc.fileName.begin(), m_ShaderDesc.fileName.end());

		D3DX10CompileFromMemory(buffer,
								size,
								stName.c_str(),
							    NULL,
							    NULL,
							    m_ShaderDesc.function.c_str(),
							    m_ShaderDesc.profile.c_str(),
							    flags,
							    0,
							    NULL,
							    &m_pCode,
							    &pErrors,
							    NULL);

		NGENE_DELETE_ARRAY(buffer);
		
		if((pErrors) || (!m_pCode))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"One or more errors occured while compiling geometry shader");
			if(pErrors)
			{
				handleErrors(pErrors);
				NGENE_RELEASE(pErrors);
			}
			return;
		}


		HRESULT hr;
		if(FAILED(hr = pDevice->CreateGeometryShader((dword*)m_pCode->GetBufferPointer(), m_pCode->GetBufferSize(), &m_pGeometryShader)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader couldn't be created. Reason: %ls", DX10Mapping::getDXErrorDescription(hr));
		}


		loadConstants();
	}
//----------------------------------------------------------------------
	void ShaderDX10::createPixelShader()
	{
		DeviceRender& pDeviceRender = Renderer::getSingleton().getDeviceRender();
		ID3D10Device* pDevice = reinterpret_cast<ID3D10Device*>(pDeviceRender.getDevicePtr());


		ID3D10Blob* pErrors = NULL;

		// Get shader profile
		m_ShaderDesc.profile = "ps_4_0";
		if(!m_ShaderDesc.function.compare(""))
			m_ShaderDesc.function = "main";
		
		dword flags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
		if(m_ShaderDesc.debugInfo)
			flags |= D3D10_SHADER_DEBUG;

		// Load shader file
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(m_ShaderDesc.fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint size = pFile->getSize();
		char* buffer = new char[size];
		pFile->read((char*)buffer, size);
		mgr.closeFile(m_ShaderDesc.fileName);
		
		string stName(m_ShaderDesc.fileName.begin(), m_ShaderDesc.fileName.end());

		D3DX10CompileFromMemory(buffer,
								size,
								stName.c_str(),
							    NULL,
							    NULL,
							    m_ShaderDesc.function.c_str(),
							    m_ShaderDesc.profile.c_str(),
							    flags,
							    0,
							    NULL,
							    &m_pCode,
							    &pErrors,
							    NULL);

		NGENE_DELETE_ARRAY(buffer);
		
		if((pErrors) || (!m_pCode))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"One or more errors occured while compiling pixel shader");
			if(pErrors)
			{
				handleErrors(pErrors);
				NGENE_RELEASE(pErrors);
			}
			return;
		}


		HRESULT hr;
		if(FAILED(hr = pDevice->CreatePixelShader((dword*)m_pCode->GetBufferPointer(), m_pCode->GetBufferSize(), &m_pPixelShader)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader couldn't be created. Reason: %ls", DX10Mapping::getDXErrorDescription(hr));
		}


		loadConstants();
	}
//----------------------------------------------------------------------
	void ShaderDX10::loadConstants()
	{
		HRESULT hr;
		ID3D10ShaderReflection* pReflection = NULL;
		hr = D3D10ReflectShader((void*)m_pCode->GetBufferPointer(),
								m_pCode->GetBufferSize(),
								&pReflection);

		if(FAILED(hr))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Shader reflection could not be obtained. Reason: %ls",
				DX10Mapping::getDXErrorDescription(hr));
			return;
		}

		D3D10_SHADER_DESC desc;
		pReflection->GetDesc(&desc);

		ID3D10ShaderReflectionConstantBuffer* pConstantBuffer = NULL;
		uint totalSize = 0;
		for(uint i = 0; i < desc.BoundResources; ++i)
		{
			pConstantBuffer = pReflection->GetConstantBufferByIndex(i);

			uint count = 0;
			D3D10_SHADER_BUFFER_DESC bufferDesc;
			pConstantBuffer->GetDesc(&bufferDesc);
			if(bufferDesc.Type == D3D10_CT_TBUFFER)
				continue;

			count = bufferDesc.Variables;

			ID3D10ShaderReflectionVariable* pConstant = NULL;
			D3D10_SHADER_VARIABLE_DESC constantDesc;
			for(uint j = 0; j < count; ++j)
			{
				pConstant = pConstantBuffer->GetVariableByIndex(j);
				pConstant->GetDesc(&constantDesc);

				float* pDefault = NULL;
				if(constantDesc.DefaultValue)
				{
					pDefault = new float[constantDesc.Size / 4];
					memcpy(pDefault, constantDesc.DefaultValue, constantDesc.Size);
				}

				ShaderConstant shaderConstant;
				shaderConstant.setName(constantDesc.Name);
				shaderConstant.setValuesCount(constantDesc.Size / 4);
				shaderConstant.setDefaultValue(pDefault);
				shaderConstant.setOffset(constantDesc.StartOffset);

				ShaderConstant* pConstant = new ShaderConstant(shaderConstant);
				m_pVariables.push_back(pConstant);
				m_pVariablesRef[pConstant->getName()] = pConstant;

				totalSize += constantDesc.Size;

				NGENE_DELETE_ARRAY(pDefault);
			}
		}

		if(totalSize)
		{
			DeviceRender& pDeviceRender = Renderer::getSingleton().getDeviceRender();
			ID3D10Device* pDevice = reinterpret_cast<ID3D10Device*>(pDeviceRender.getDevicePtr());

			D3D10_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = totalSize;
			bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			pDevice->CreateBuffer(&bufferDesc, NULL, &m_pConstants);
		}
	}
//----------------------------------------------------------------------
	void ShaderDX10::handleErrors(ID3D10Blob* _errors)
	{
		void* buffer = _errors->GetBufferPointer();
		string msg = (char*)buffer;

		wstring msg2 = L"Shader error: ";
		msg2.append(msg.begin(), msg.end());

		Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			msg2.c_str());
	}
//----------------------------------------------------------------------
	void ShaderDX10::createEffect()
	{
	}
//----------------------------------------------------------------------
	void ShaderDX10::setFloat(const string& _name, const float& _value)
	{
		ID3D10Buffer* pBuffer = NULL;
		m_pConstants->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&pBuffer);

			void* pTemp = pBuffer + m_pVariablesRef[_name]->getOffset();
			memcpy(pTemp, &_value, sizeof(float));

		m_pConstants->Unmap();
	}
//----------------------------------------------------------------------
	void ShaderDX10::setFloatArray(const string& _name, const float* _values,
		const uint _count) const
	{
		ID3D10Buffer* pBuffer = NULL;
		m_pConstants->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&pBuffer);

			void* pTemp = pBuffer + m_pVariablesRef[_name]->getOffset();
			memcpy(pTemp, &_values, sizeof(float) * _count);

		m_pConstants->Unmap();
	}
//----------------------------------------------------------------------
	void ShaderDX10::setMatrix(const string& _name, const Matrix4x4& _mat)
	{
		ID3D10Device* pDevice = reinterpret_cast<ID3D10Device*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());
	}
//----------------------------------------------------------------------
	void ShaderDX10::setInteger(const string& _name, const int& _value)
	{
	}
//----------------------------------------------------------------------
	void ShaderDX10::setIntegerArray(const string& _name, const int* _values,
		const uint _count) const
	{
	}
//----------------------------------------------------------------------
	void ShaderDX10::setBoolean(const string& _name, const bool& _value)
	{
	}
//----------------------------------------------------------------------
	void ShaderDX10::setBooleanArray(const string& _name, const bool* _values,
		const uint _count) const
	{
	}
//----------------------------------------------------------------------
}