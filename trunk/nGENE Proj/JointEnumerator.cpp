/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointEnumerator.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "JointEnumerator.h"
#include "JointFactory.h"


namespace nGENE
{
	JointEnumerator::JointEnumerator()
	{
		addFactory(m_CylinderFactory, L"Cylinder");
		addFactory(m_DistanceFactory, L"Distance");
		addFactory(m_FixedFactory, L"Fixed");
		addFactory(m_HingeFactory, L"Hinge");
		addFactory(m_PointInPlane, L"PointInPlane");
		addFactory(m_PointOnLine, L"PointOnLine");
		addFactory(m_PrismFactory, L"Prism");
		addFactory(m_PulleyFactory, L"Pulley");
		addFactory(m_SphereFactory, L"Sphere");
	}
//----------------------------------------------------------------------
	JointEnumerator::~JointEnumerator()
	{
		if(!m_pFactories.empty())
			m_pFactories.clear();
	}
//----------------------------------------------------------------------
	void JointEnumerator::addFactory(JointFactory& _factory, const wstring& _typeName)
	{
		// If factory already exists, return
		if(m_pFactories.find(_typeName) != m_pFactories.end())
			return;

		m_pFactories[_typeName] = &_factory;
	}
//----------------------------------------------------------------------
	void JointEnumerator::removeFactory(const wstring& _typename)
	{
		m_pFactories.erase(_typename);
	}
//----------------------------------------------------------------------
	Joint* JointEnumerator::create(NxScene* _scene, const wstring& _typeName, JOINT_DESC& _desc)
	{
		// Appropriate factory does not exist
		if(m_pFactories.find(_typeName) == m_pFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Factory for type: %ls does not exist!", _typeName.c_str());
			return NULL;
		}

		// Create new shape
		return (m_pFactories[_typeName]->createJoint(_scene, _desc));
	}
//----------------------------------------------------------------------
	void JointEnumerator::destroy(Joint* _shape)
	{
		NGENE_DELETE(_shape);
	}
//----------------------------------------------------------------------
}