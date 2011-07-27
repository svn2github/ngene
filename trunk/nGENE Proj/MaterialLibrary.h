/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MaterialLibrary.h
Version:	0.09
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MATERIALLIBRARY_H_
#define __INC_MATERIALLIBRARY_H_


#include "BaseClass.h"
#include "HashTable.h"
#include "Prerequisities.h"
#include "TypeInfo.h"
#include "XMLNode.h"


namespace nGENE
{
	/** MaterialLibrary is a container for materials.
		@remarks
			At least one library must exist at a time.
	*/
	class nGENEDLL MaterialLibrary: public BaseClass,
		public XMLNode
	{
		EXPOSE_TYPE
	private:
		/** HashTable containing all materials existing
			in the library.
		*/
		HashTable <wstring, Material*> m_pMaterials;
		/// Vector containing pointer to materials in the library.
		vector <Material*> m_vMaterials;

		/// Names of the global textures.
		vector <string> m_vTexturesNames;

		wstring m_stName;								/// Library's name
		wstring m_stAuthor;								/// Library Author's name
		float m_fVersion;								/// Library's version number

		bool m_bLoadOnDemand;							/// Are materials loaded on demand


		void parseTextures(XMLDocument* _node);
		void parseMaterials(XMLDocument* _node);

		void saveTextures(XMLNode& _textures);
		void saveMaterials(XMLNode& _materials);

	public:
		MaterialLibrary(bool _loadOnDemand=true);
		~MaterialLibrary();

		void init();
		void cleanup();

		void parse(XMLDocument* _document);
		void load() {}
		void load(const wstring& _fileName);
		void save(File* _file);

		/** Adds existing material to the library.
			@remarks
				This method copies the Material object.
		*/
		void addMaterial(const wstring& _name, const Material& _material);
		/** Adds existing material to the library.
			@remarks
				This method copies the Material object.
		*/
		void addMaterial(const wstring& _name, const Material* _material);
		
		/// Creates new material.
		Material* createMaterial(const wstring& _name);

		/// Returns material with the given name or NULL if not found.
		Material* getMaterial(const wstring& _name);
		/// Returns material with the given index or NULL if not found.
		Material* getMaterial(uint _index);

		/// Sets material library name.
		void setName(const wstring& _name);
		/// Returns material library name.
		wstring& getName();

		/// Sets library author name.
		void setAuthorName(const wstring& _author);
		/// Returns library author name.
		wstring& getAuthorName();

		/// Sets material library version.
		void setVersionNumber(float _version);
		/// Returns material library version.
		float getVersionNumber() const;

		/// Returns number of materials in library.
		uint getMaterialsNum() const;

		/// Are materials loaded on demand?
		bool isLoadedOnDemand() const;
	};



	inline bool MaterialLibrary::isLoadedOnDemand() const
	{
		return m_bLoadOnDemand;
	}
//----------------------------------------------------------------------
	inline uint MaterialLibrary::getMaterialsNum() const
	{
		return m_pMaterials.size();
	}
//----------------------------------------------------------------------
	inline void MaterialLibrary::setVersionNumber(float _version)
	{
		m_fVersion = _version;
	}
//----------------------------------------------------------------------
	inline float MaterialLibrary::getVersionNumber() const
	{
		return m_fVersion;
	}
//----------------------------------------------------------------------
	inline void MaterialLibrary::setAuthorName(const wstring& _author)
	{
		m_stAuthor=_author;
	}
//----------------------------------------------------------------------
	inline wstring& MaterialLibrary::getAuthorName()
	{
		return m_stAuthor;
	}
//----------------------------------------------------------------------
	inline void MaterialLibrary::setName(const wstring& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
	inline wstring& MaterialLibrary::getName()
	{
		return m_stName;
	}
//----------------------------------------------------------------------
}


#endif