/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MaterialManager.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MATERIALMANAGER_H_
#define __INC_MATERIALMANAGER_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "Singleton.h"


namespace nGENE
{
	/** Lets you create and remove materials to the scene.
		@remarks
			Via this interface you control MaterialLibraries and
			thus material creation and removal.
	*/
	class nGENEDLL MaterialManager: public Singleton <MaterialManager>
	{
	protected:
		/// HashTable containing all materials
		HashTable <wstring, MaterialLibrary*> m_pMaterialLibs;
		vector <MaterialLibrary*> m_vMaterialLibs;

	public:
		MaterialManager();
		virtual ~MaterialManager();

		/// Removes all libraries.
		void cleanup();

		/// Creates new MaterialLibrary.
		MaterialLibrary* createLibrary(const wstring& _name, bool _loadOnDemand=true);
		/// Opens MaterialLibrary file.
		MaterialLibrary* createLibraryFromFile(const wstring& _fileName, const wstring& _name,
			bool _loadOnDemand=true);

		/// Returns library with the given name.
		MaterialLibrary* getLibrary(const wstring& _name);
		/// Returns library with the given index.
		MaterialLibrary* getLibrary(uint _index);

		/// Removes the library with the given name.
		void removeLibrary(const wstring& _name);
		/// Removes the library with the given index.
		void removeLibrary(uint _index);
		/// Removes the given library.
		void removeLibrary(MaterialLibrary* _lib);

		/** Gets index of the given library.
			@returns
				int index of the material library. If no library was found returns -1.
		*/
		int getIndex(MaterialLibrary* _lib) const;

		/** Creates material and adds it to the specified library.
			@param
				_name name of the material being created.
			@param
				_library destination library for the material.
		*/
		Material* createMaterial(const wstring& _name, const wstring& _library);
		Material* copy(const wstring& _toCopy, const wstring& _newName, const wstring& _library);
		Material* clone(const wstring& _toClone, const wstring& _newName, const wstring& _library);

		/// Returns number of loaded materials libraries.
		uint getMaterialsLibsCount() const;

		void bind();
	};
}


#endif