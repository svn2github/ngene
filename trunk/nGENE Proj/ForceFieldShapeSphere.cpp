/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapeSphere.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ForceFieldShapeSphere.h"
#include "PhysXMapping.h"
#include "ForceFieldShapeEnumerator.h"


#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
	#include "NxSphereForceFieldShapeDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo ForceFieldShapeSphere::Type(L"ForceFieldShapeSphere", &ForceFieldShape::Type);


	ForceFieldShapeSphere::ForceFieldShapeSphere():
		m_pSphereDesc(new NxSphereForceFieldShapeDesc())
	{
		init();
	}
//----------------------------------------------------------------------
	ForceFieldShapeSphere::~ForceFieldShapeSphere()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ForceFieldShapeSphere::init()
	{
	}
//----------------------------------------------------------------------
	void ForceFieldShapeSphere::cleanup()
	{
		NGENE_DELETE(m_pSphereDesc);
	}
//----------------------------------------------------------------------
	void ForceFieldShapeSphere::setDimensions(const Vector3& _dims)
	{
		ForceFieldShape::setDimensions(_dims);
		m_pSphereDesc->radius = _dims.x;
	}
//----------------------------------------------------------------------
	void ForceFieldShapeSphere::setLocalPos(const Vector3& _pos)
	{
		ForceFieldShape::setLocalPos(_pos);
		m_pSphereDesc->pose.t.x = _pos.x;
		m_pSphereDesc->pose.t.y = _pos.y;
		m_pSphereDesc->pose.t.z = _pos.z;
	}
//----------------------------------------------------------------------
	NxForceFieldShapeDesc* ForceFieldShapeSphere::getForceFieldShapeDesc()
	{
		return (static_cast<NxForceFieldShapeDesc*>(m_pSphereDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	ForceFieldSphereShapeFactory::ForceFieldSphereShapeFactory()
	{
	}
//----------------------------------------------------------------------
	ForceFieldSphereShapeFactory::~ForceFieldSphereShapeFactory()
	{
	}
//----------------------------------------------------------------------
	ForceFieldShape* ForceFieldSphereShapeFactory::createShape()
	{
		return (new ForceFieldShapeSphere());
	}
//----------------------------------------------------------------------
}