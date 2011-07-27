/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileArchiveEnumerator.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FileArchiveEnumerator.h"


namespace nGENE
{
	FileArchiveEnumerator::FileArchiveEnumerator()
	{
		addFactory(m_DefaultFactory, L"Default");
	}
//----------------------------------------------------------------------
	FileArchiveEnumerator::~FileArchiveEnumerator()
	{
	}
//----------------------------------------------------------------------
	void FileArchiveEnumerator::addFactory(FileArchiveFactory& _factory, const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = &_factory;
	}
//----------------------------------------------------------------------
	void FileArchiveEnumerator::removeFactory(const wstring& _typename)
	{
		m_pFactories.erase(_typename);
	}
//----------------------------------------------------------------------
	FileArchive* FileArchiveEnumerator::create(const wstring& _typeName, const wstring& _fileName, dword _openMode)
	{
		// Appropriate factory does not exist
		if(m_pFactories.find(_typeName) == m_pFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Factory for archive type: %ls does not exist!", _typeName.c_str());
			return NULL;
		}

		// Create new FileArchive
		FileArchive* pFileArchive = m_pFactories[_typeName]->createFileArchive(_fileName, _openMode);
		pFileArchive->init();

		return pFileArchive;
	}
//----------------------------------------------------------------------
}