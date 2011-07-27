/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapeBox.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ForceFieldShapeBox.h"
#include "ForceFieldShape.h"
#include "ForceFieldShapeEnumerator.h"
#include "PhysXMapping.h"


#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
	#include "NxForceFieldShapeDesc.h"
	#include "NxBoxForceFieldShapeDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo ForceFieldShapeBox::Type(L"ForceFieldShapeBox", &ForceFieldShape::Type);


	ForceFieldShapeBox::ForceFieldShapeBox():
		m_pBoxDesc(new NxBoxForceFieldShapeDesc)
	{
		init();
	}
//----------------------------------------------------------------------
	ForceFieldShapeBox::~ForceFieldShapeBox()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ForceFieldShapeBox::init()
	{
	}
//----------------------------------------------------------------------
	void ForceFieldShapeBox::cleanup()
	{
		NGENE_DELETE(m_pBoxDesc);
	}
//----------------------------------------------------------------------
	void ForceFieldShapeBox::setDimensions(const Vector3& _dims)
	{
		ForceFieldShape::setDimensions(_dims);
		m_pBoxDesc->dimensions = PhysXMapping::vector3ToNxVec3(_dims * 0.5f);
	}
//----------------------------------------------------------------------
	void ForceFieldShapeBox::setLocalPos(const Vector3& _pos)
	{
		ForceFieldShape::setLocalPos(_pos);
		m_pBoxDesc->pose.t.x = _pos.x;
		m_pBoxDesc->pose.t.y = _pos.y;
		m_pBoxDesc->pose.t.z = _pos.z;
	}
//----------------------------------------------------------------------
	NxForceFieldShapeDesc* ForceFieldShapeBox::getForceFieldShapeDesc()
	{
		return (static_cast<NxForceFieldShapeDesc*>(m_pBoxDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	ForceFieldBoxShapeFactory::ForceFieldBoxShapeFactory()
	{
	}
//----------------------------------------------------------------------
	ForceFieldBoxShapeFactory::~ForceFieldBoxShapeFactory()
	{
	}
//----------------------------------------------------------------------
	ForceFieldShape* ForceFieldBoxShapeFactory::createShape()
	{
		return (new ForceFieldShapeBox());
	}
//----------------------------------------------------------------------
}