/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsMaterial.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PhysicsMaterial.h"
#include "XMLDocument.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxMaterial.h"
	#include "NxMaterialDesc.h"
	#include "NxScene.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo PhysicsMaterial::Type(L"PhysicsMaterial", &BaseClass::Type);
	uint PhysicsMaterial::s_nMaterialCount = 1;


	PhysicsMaterial::PhysicsMaterial(NxScene* _scene):
		m_nMaterialIndex(s_nMaterialCount++),
		m_pScene(_scene),
		m_stName(L""),
		m_fRestitution(0.5f),
		m_fStaticFriction(0.5f),
		m_fDynamicFriction(0.5f)
	{
		init();
	}
//----------------------------------------------------------------------
	PhysicsMaterial::~PhysicsMaterial()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PhysicsMaterial::init()
	{
		// Create material with default settings
		NxMaterialDesc matDesc;
		matDesc.restitution		= m_fRestitution;
		matDesc.staticFriction	= m_fStaticFriction;
		matDesc.dynamicFriction	= m_fDynamicFriction;
		m_pMaterial				= m_pScene->createMaterial(matDesc);
	}
//----------------------------------------------------------------------
	void PhysicsMaterial::cleanup()
	{
		m_pScene->releaseMaterial(*m_pMaterial);
		m_pMaterial = NULL;
		m_pScene = NULL;
	}
//----------------------------------------------------------------------
	void PhysicsMaterial::setRestitution(float _restitution)
	{
		Maths::clamp <float> (_restitution, 0.0f, 1.0f);
		m_fRestitution = _restitution;
		m_pMaterial->setRestitution(m_fRestitution);
	}
//----------------------------------------------------------------------
	void PhysicsMaterial::setStaticFriction(float _friction)
	{
		Maths::clamp <float> (_friction, 0.0f, Maths::Infinity);
		m_fStaticFriction = _friction;
		m_pMaterial->setStaticFriction(m_fStaticFriction);
	}
//----------------------------------------------------------------------
	void PhysicsMaterial::setDynamicFriction(float _friction)
	{
		Maths::clamp <float> (_friction, 0.0f, Maths::Infinity);
		m_fDynamicFriction = _friction;
		m_pMaterial->setDynamicFriction(m_fDynamicFriction);
	}
//----------------------------------------------------------------------
	void PhysicsMaterial::save(File* _file)
	{
		setName(L"physics_material");

		// Add attributes
		Attribute atName;
		atName.first = "name";
		atName.second.assign(m_stName.begin(), m_stName.end());

		Attribute atRestitution;
		ostringstream buffer;
		buffer << m_fRestitution;
		atRestitution.first = "restitution";
		atRestitution.second= buffer.str();

		Attribute atDynFric;
		buffer << m_fDynamicFriction;
		atDynFric.first = "friction_dynamic";
		atDynFric.second= buffer.str();

		Attribute atStFric;
		buffer << m_fStaticFriction;
		atStFric.first = "friction_static";
		atStFric.second= buffer.str();


		addAttribute(atName);
		addAttribute(atRestitution);
		addAttribute(atDynFric);
		addAttribute(atStFric);

		// Save
		XMLNode::save(_file);
	}
//----------------------------------------------------------------------
	void PhysicsMaterial::parse(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;

		// Parse data
		string key = pNode->getAttribute("name");
		PhysicsMaterial::m_stName.assign(key.begin(), key.end());

		key=pNode->getAttribute("restitution");
		PhysicsMaterial::m_fRestitution = atof(key.c_str());

		key = pNode->getAttribute("friction_dynamic");
		PhysicsMaterial::m_fDynamicFriction = atof(key.c_str());

		key = pNode->getAttribute("friction_static");
		PhysicsMaterial::m_fStaticFriction = atof(key.c_str());
	}
//----------------------------------------------------------------------
}