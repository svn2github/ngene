/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileArchiveEnumerator.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FILEARCHIVEENUMERATOR_H_
#define __INC_FILEARCHIVEENUMERATOR_H_


#include "DefaultFileArchive.h"
#include "HashTable.h"
#include "Prerequisities.h"
#include "Singleton.h"


namespace nGENE
{
	/** FileArchive Enumerator class.
		@remarks
			Enumerators lets you easily extend nGENE Tech by just adding
			factories of new type to the enumerator. Then by specifying
			the type name you can create objects of any available type
			in the enumerator.
	*/
	class nGENEDLL FileArchiveEnumerator: public Singleton <FileArchiveEnumerator>
	{
	protected:
		// Hash table containing all factories
		HashTable <wstring, FileArchiveFactory*> m_pFactories;
		
		// Default factories
		DefaultFileArchiveFactory m_DefaultFactory;

	public:
		FileArchiveEnumerator();
		virtual ~FileArchiveEnumerator();

		/** Adds new factory to the enumerator.
			@remarks
				Factory has to be added to the enumerator prior to
				using it.
		*/
		void addFactory(FileArchiveFactory& _factory, const wstring& _typeName);
		void removeFactory(const wstring& _typeName);

		/// Creates new FileArchive object.
		FileArchive* create(const wstring& _typeName, const wstring& _fileName, dword _openMode);
	};
}


#endif