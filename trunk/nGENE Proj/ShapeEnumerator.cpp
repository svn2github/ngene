/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeEnumerator.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ShapeEnumerator.h"
#include "ShapeFactory.h"


namespace nGENE
{
	ShapeEnumerator::ShapeEnumerator()
	{
		addFactory(m_BoxFactory, L"Box");
		addFactory(m_CapsuleFactory, L"Capsule");
		addFactory(m_PlaneFactory, L"Plane");
		addFactory(m_SphereFactory, L"Sphere");
		addFactory(m_TerrainFactory, L"Terrain");
		addFactory(m_MeshFactory, L"Mesh");
	}
//----------------------------------------------------------------------
	ShapeEnumerator::~ShapeEnumerator()
	{
		if(!m_pFactories.empty())
			m_pFactories.clear();
	}
//----------------------------------------------------------------------
	void ShapeEnumerator::addFactory(ShapeFactory& _factory, const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = &_factory;
	}
//----------------------------------------------------------------------
	void ShapeEnumerator::removeFactory(const wstring& _typename)
	{
		m_pFactories.erase(_typename);
	}
//----------------------------------------------------------------------
	Shape* ShapeEnumerator::create(const wstring& _typeName)
	{
		// Appropriate factory does not exist
		if(m_pFactories.find(_typeName) == m_pFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Factory for type: %ls does not exist!", _typeName.c_str());
			return NULL;
		}

		// Create new shape
		return (m_pFactories[_typeName]->createShape());
	}
//----------------------------------------------------------------------
	void ShapeEnumerator::destroy(Shape* _shape)
	{
		NGENE_DELETE(_shape);
	}
//----------------------------------------------------------------------
}