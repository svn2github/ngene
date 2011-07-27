/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MaterialManager.cpp
Version:	0.09
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "XMLDocument.h"
#include "XMLParser.h"


namespace nGENE
{
	MaterialManager::MaterialManager()
	{
	}
//----------------------------------------------------------------------
	MaterialManager::~MaterialManager()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void MaterialManager::cleanup()
	{
		m_vMaterialLibs.clear();

		HashTable <wstring, MaterialLibrary*>::iterator iter;
		for(iter = m_pMaterialLibs.begin(); iter != m_pMaterialLibs.end(); ++iter)
			delete iter->second;

		m_pMaterialLibs.clear();
	}
//----------------------------------------------------------------------
	MaterialLibrary* MaterialManager::createLibrary(const wstring& _name, bool _loadOnDemand)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating material library: %ls", _name.c_str());

		// If library exists, exit
		if(m_pMaterialLibs.find(_name) != m_pMaterialLibs.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material Library with the name: %ls already exists", _name.c_str());
			return m_pMaterialLibs[_name];
		}

		MaterialLibrary* pLib = new MaterialLibrary(_loadOnDemand);
		pLib->setName(_name);
		m_pMaterialLibs[_name] = pLib;

		// Add library to a vector
		m_vMaterialLibs.push_back(pLib);

		return pLib;
	}
//----------------------------------------------------------------------
	MaterialLibrary* MaterialManager::createLibraryFromFile(const wstring& _fileName, const wstring& _name,
		bool _loadOnDemand)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Loading material library file: %ls", _fileName.c_str());

		// If library exists, exit
		if(m_pMaterialLibs.find(_name) != m_pMaterialLibs.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material Library with the name: %ls already exists", _name.c_str());
			return m_pMaterialLibs[_name];
		}

		// Open file
		MaterialLibrary* pLib = new MaterialLibrary(_loadOnDemand);
		pLib->load(_fileName);
		pLib->setName(_name);
		m_pMaterialLibs[_name] = pLib;

		// Add library to a vector
		m_vMaterialLibs.push_back(pLib);


		return pLib;
	}
//----------------------------------------------------------------------
	void MaterialManager::removeLibrary(const wstring& _name)
	{
		if(m_pMaterialLibs.find(_name) != m_pMaterialLibs.end())
		{
			// Remove from vector
			uint index = static_cast<uint>(getIndex(m_pMaterialLibs[_name]));
			m_vMaterialLibs.erase(m_vMaterialLibs.begin() + index);

			// Remove from hash table
			NGENE_DELETE(m_pMaterialLibs[_name]);
			m_pMaterialLibs.erase(_name);
		}
	}
//----------------------------------------------------------------------
	void MaterialManager::removeLibrary(uint _index)
	{
		if(_index >= m_vMaterialLibs.size())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material Library with index: %d does not exits", _index);
			return;
		}

		// Remove from vector
		wstring& name = m_vMaterialLibs[_index]->getName();
		m_vMaterialLibs.erase(m_vMaterialLibs.begin() + _index);

		// Remove from hash table
		NGENE_DELETE(m_pMaterialLibs[name]);
		m_pMaterialLibs.erase(name);
	}
//----------------------------------------------------------------------
	int MaterialManager::getIndex(MaterialLibrary* _lib) const
	{
		for(uint i = 0; i < m_vMaterialLibs.size(); ++i)
		{
			if(m_vMaterialLibs[i] == _lib)
				return i;
		}

		return -1;
	}
//----------------------------------------------------------------------
	Material* MaterialManager::createMaterial(const wstring& _name, const wstring& _library)
	{
		// If library does not exist, exit
		if(m_pMaterialLibs.find(_library) == m_pMaterialLibs.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material Library with the name: %ls does not exist", _library.c_str());
			return NULL;
		}

		return m_pMaterialLibs[_library]->createMaterial(_name);
	}
//----------------------------------------------------------------------
	uint MaterialManager::getMaterialsLibsCount() const
	{
		return m_pMaterialLibs.size();
	}
//----------------------------------------------------------------------
	MaterialLibrary* MaterialManager::getLibrary(const wstring& _name)
	{
		// If library does not exist, exit
		if(m_pMaterialLibs.find(_name) == m_pMaterialLibs.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material Library with the name: %ls does not exist", _name.c_str());
			return NULL;
		}

		return m_pMaterialLibs[_name];
	}
//----------------------------------------------------------------------
	MaterialLibrary* MaterialManager::getLibrary(uint _index)
	{
		// If library does not exist, exit
		if(_index >= m_vMaterialLibs.size())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material Library with the index: %d does not exist", _index);

			return NULL;
		}
		
		return m_vMaterialLibs[_index];
	}
//----------------------------------------------------------------------
}