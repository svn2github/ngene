/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		World.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "World.h"
#include "XMLDocument.h"


namespace nGENE
{
	World::World():
		m_stName(L""),
		m_stAuthor(L""),
		m_fVersion(NGENE_SDK_VERSION)
	{
	}
//----------------------------------------------------------------------
	World::~World()
	{
	}
//----------------------------------------------------------------------
	void World::parse(XMLDocument* _document)
	{
		XMLDocument* pDocument = _document;

		// Parse data
		string key = pDocument->getAttribute("name");
		World::m_stName.assign(key.begin(), key.end());
		
		key = pDocument->getAttribute("author");
		World::m_stAuthor.assign(key.begin(), key.end());

		key = pDocument->getAttribute("version");
		World::m_fVersion = atof(key.c_str());

		// Parse child nodes
		XMLDocument* pChild = pDocument->getFirstChild();
		XMLDocument* pTemp = NULL;
		while(pChild)
		{
			// Load node
			/// @todo Loading routines for every node type should be provided.
			if(pChild->getTagName() == "node_light")
			{
			}

			pTemp = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(pTemp);
		}

		// Release document
		pDocument = NULL;
	}
//----------------------------------------------------------------------
	void World::save(File* _file)
	{
	}
//----------------------------------------------------------------------
}
