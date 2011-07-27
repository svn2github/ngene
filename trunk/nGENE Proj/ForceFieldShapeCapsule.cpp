/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapeCapsule.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ForceFieldShapeCapsule.h"
#include "PhysXMapping.h"
#include "ForceFieldShape.h"
#include "ForceFieldShapeEnumerator.h"


#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
	#include "NxForceFieldShapeDesc.h"
	#include "NxCapsuleForceFieldShapeDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo ForceFieldShapeCapsule::Type(L"ForceFieldShapeCapsule", &ForceFieldShape::Type);


	ForceFieldShapeCapsule::ForceFieldShapeCapsule():
		m_pCapsuleDesc(new NxCapsuleForceFieldShapeDesc())
	{
		init();
	}
//----------------------------------------------------------------------
	ForceFieldShapeCapsule::~ForceFieldShapeCapsule()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ForceFieldShapeCapsule::init()
	{
	}
//----------------------------------------------------------------------
	void ForceFieldShapeCapsule::cleanup()
	{
		NGENE_DELETE(m_pCapsuleDesc);
	}
//----------------------------------------------------------------------
	void ForceFieldShapeCapsule::setDimensions(const Vector3& _dims)
	{
		ForceFieldShape::setDimensions(_dims);
		m_pCapsuleDesc->radius = _dims.x;
		m_pCapsuleDesc->height = _dims.y;
	}
//----------------------------------------------------------------------
	void ForceFieldShapeCapsule::setLocalPos(const Vector3& _pos)
	{
		ForceFieldShape::setLocalPos(_pos);
		m_pCapsuleDesc->pose.t.x = _pos.x;
		m_pCapsuleDesc->pose.t.y = _pos.y;
		m_pCapsuleDesc->pose.t.z = _pos.z;
	}
//----------------------------------------------------------------------
	NxForceFieldShapeDesc* ForceFieldShapeCapsule::getForceFieldShapeDesc()
	{
		return (static_cast<NxForceFieldShapeDesc*>(m_pCapsuleDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	ForceFieldCapsuleShapeFactory::ForceFieldCapsuleShapeFactory()
	{
	}
//----------------------------------------------------------------------
	ForceFieldCapsuleShapeFactory::~ForceFieldCapsuleShapeFactory()
	{
	}
//----------------------------------------------------------------------
	ForceFieldShape* ForceFieldCapsuleShapeFactory::createShape()
	{
		return (new ForceFieldShapeCapsule());
	}
//----------------------------------------------------------------------
}