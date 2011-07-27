/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CameraEnumerator.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "CameraEnumerator.h"


namespace nGENE
{
	CameraEnumerator::CameraEnumerator()
	{
		addFactory(m_DefaultFactory, L"DefaultCamera");
		addFactory(m_FPPFactory, L"CameraFirstPerson");
		addFactory(m_TPPFactory, L"CameraThirdPerson");
		addFactory(m_Factory2D, L"Camera2D");
	}
//----------------------------------------------------------------------
	CameraEnumerator::~CameraEnumerator()
	{
	}
//----------------------------------------------------------------------
	void CameraEnumerator::addFactory(CameraFactory& _factory, const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = &_factory;
	}
//----------------------------------------------------------------------
	void CameraEnumerator::removeFactory(const wstring& _typename)
	{
		// Factory doesn't exist
		if(m_pFactories.find(_typename) == m_pFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Camera factory for type: %ls does not exist", _typename.c_str());
			return;
		}

		m_pFactories.erase(_typename);
	}
//----------------------------------------------------------------------
	Camera* CameraEnumerator::create(const wstring& _typeName)
	{
		// Appropriate factory does not exist
		if(m_pFactories.find(_typeName) == m_pFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Camera factory for type: %ls does not exist", _typeName.c_str());
			return NULL;
		}

		// Create new camera
		Camera* pCamera = m_pFactories[_typeName]->createCamera();
		pCamera->init();

		return pCamera;
	}
//----------------------------------------------------------------------
	void CameraEnumerator::destroy(Camera* _camera)
	{
		NGENE_DELETE(_camera);
	}
//----------------------------------------------------------------------
}