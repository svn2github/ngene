/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SoundManagerEnumerator.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "SoundManagerEnumerator.h"


namespace nGENE
{
	SoundManagerEnumerator::SoundManagerEnumerator()
	{
		addFactory(m_FMODFactory, L"FMOD");
	}
//----------------------------------------------------------------------
	SoundManagerEnumerator::~SoundManagerEnumerator()
	{
	}
//----------------------------------------------------------------------
	void SoundManagerEnumerator::addFactory(SoundManagerFactory& _factory,
		const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = &_factory;
	}
//----------------------------------------------------------------------
	void SoundManagerEnumerator::addFactory(SoundManagerFactory* _factory,
		const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = _factory;
	}
//----------------------------------------------------------------------
	void SoundManagerEnumerator::removeFactory(const wstring& _typeName)
	{
		// Factory doesn't exist
		if(m_pFactories.find(_typeName) == m_pFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Sound manager factory for type: %ls does not exist", _typeName.c_str());
			return;
		}

		m_pFactories.erase(_typeName);
	}
//----------------------------------------------------------------------
	ISoundManager* SoundManagerEnumerator::create(const wstring& _typeName)
	{
		// Appropriate factory does not exist
		if(m_pFactories.find(_typeName) == m_pFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Sound manager factory for type: %ls does not exist",
				_typeName.c_str());
			return NULL;
		}

		// Create new camera
		ISoundManager* pManager = m_pFactories[_typeName]->createSoundManager();
		pManager->init();

		return pManager;
	}
//----------------------------------------------------------------------
	vector <wstring> SoundManagerEnumerator::getNamesList()
	{
		vector <wstring> vNames;

		if(!m_pFactories.empty())
		{
			HashTable <wstring, SoundManagerFactory*>::iterator iter;
			for(iter = m_pFactories.begin(); iter != m_pFactories.end(); ++iter)
				vNames.push_back(iter->first);
		}

		return vNames;
	}
//----------------------------------------------------------------------
}