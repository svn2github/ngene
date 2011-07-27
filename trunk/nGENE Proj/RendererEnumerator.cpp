/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RendererEnumerator.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "RendererEnumerator.h"
#include "RendererDX9.h"
#include "RendererDX10.h"


namespace nGENE
{
	RendererEnumerator::RendererEnumerator():
		m_pDX9(NULL),
		m_pDX10(NULL)
	{
		init();
	}
//----------------------------------------------------------------------
	RendererEnumerator::~RendererEnumerator()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void RendererEnumerator::init()
	{
		// Create default factories
		m_pDX9 = new RendererDX9Factory();
		//m_pDX10 = new RendererDX10Factory();

		addFactory(m_pDX9, L"DirectX 9.0");
		//addFactory(m_pDX10, L"DirectX 10.0");
	}
//----------------------------------------------------------------------
	void RendererEnumerator::cleanup()
	{
		if(!m_pFactories.empty())
		{
			HashTable <wstring, RendererFactory*>::iterator iter;
			for(iter = m_pFactories.begin(); iter != m_pFactories.end(); ++iter)
				NGENE_DELETE(iter->second);
			m_pFactories.clear();
		}
	}
//----------------------------------------------------------------------
	void RendererEnumerator::addFactory(RendererFactory& _factory, const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = &_factory;
	}
//----------------------------------------------------------------------
	void RendererEnumerator::addFactory(RendererFactory* _factory, const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = _factory;
	}
//----------------------------------------------------------------------
	void RendererEnumerator::removeFactory(const wstring& _typename)
	{
		m_pFactories.erase(_typename);
	}
//----------------------------------------------------------------------
	Renderer* RendererEnumerator::create(const wstring& _typeName, bool _windowed,
		uint _renderWndsNum, bool _multiThreading)
	{
		// Appropriate factory does not exist
		if(m_pFactories.find(_typeName) == m_pFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Factory for type: %ls does not exist!", _typeName.c_str());
			return NULL;
		}

		// Create new renderer
		Renderer* pRenderer = m_pFactories[_typeName]->createRenderer(_windowed,
																	  _renderWndsNum,
																	  _multiThreading);
		pRenderer->init();

		return pRenderer;
	}
//----------------------------------------------------------------------
	void RendererEnumerator::destroy(Renderer* _renderer)
	{
		NGENE_DELETE(_renderer);
	}
//----------------------------------------------------------------------
	vector <wstring> RendererEnumerator::getNamesList()
	{
		vector <wstring> vNames;

		if(!m_pFactories.empty())
		{
			HashTable <wstring, RendererFactory*>::iterator iter;
			for(iter = m_pFactories.begin(); iter != m_pFactories.end(); ++iter)
				vNames.push_back(iter->first);
		}

		return vNames;
	}
//----------------------------------------------------------------------
}