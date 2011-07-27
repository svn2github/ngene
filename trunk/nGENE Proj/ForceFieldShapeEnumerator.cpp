/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapeEnumerator.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ForceFieldShapeEnumerator.h"
#include "ForceFieldShapeFactory.h"


namespace nGENE
{
	ForceFieldShapeEnumerator::ForceFieldShapeEnumerator()
	{
		addFactory(m_BoxFactory, L"Box");
		addFactory(m_SphereFactory, L"Sphere");
		addFactory(m_CapsuleFactory, L"Capsule");
	}
//----------------------------------------------------------------------
	ForceFieldShapeEnumerator::~ForceFieldShapeEnumerator()
	{
		if(!m_pFactories.empty())
			m_pFactories.clear();
	}
//----------------------------------------------------------------------
	void ForceFieldShapeEnumerator::addFactory(ForceFieldShapeFactory& _factory, const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = &_factory;
	}
//----------------------------------------------------------------------
	void ForceFieldShapeEnumerator::removeFactory(const wstring& _typename)
	{
		m_pFactories.erase(_typename);
	}
//----------------------------------------------------------------------
	ForceFieldShape* ForceFieldShapeEnumerator::create(const wstring& _typeName)
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
	void ForceFieldShapeEnumerator::destroy(ForceFieldShape* _shape)
	{
		NGENE_DELETE(_shape);
	}
//----------------------------------------------------------------------
}