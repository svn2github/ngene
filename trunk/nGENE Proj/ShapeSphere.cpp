/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeSphere.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PhysXMapping.h"
#include "Shape.h"
#include "ShapeEnumerator.h"
#include "ShapeSphere.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxSphereShapeDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo ShapeSphere::Type(L"ShapeSphere", &Shape::Type);


	ShapeSphere::ShapeSphere():
		m_pSphereDesc(new NxSphereShapeDesc())
	{
		init();
	}
//----------------------------------------------------------------------
	ShapeSphere::~ShapeSphere()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ShapeSphere::init()
	{
	}
//----------------------------------------------------------------------
	void ShapeSphere::cleanup()
	{
		NGENE_DELETE(m_pSphereDesc);
	}
//----------------------------------------------------------------------
	void ShapeSphere::setDimensions(const Vector3& _dims)
	{
		Shape::setDimensions(_dims);
		m_pSphereDesc->radius = _dims.x;
	}
//----------------------------------------------------------------------
	void ShapeSphere::setTrigger(bool _value)
	{
		Shape::setTrigger(_value);
		if(_value)
			m_pSphereDesc->shapeFlags |= NX_TRIGGER_ENABLE;
		else
			m_pSphereDesc->shapeFlags &= ~NX_TRIGGER_ENABLE;
	}
//----------------------------------------------------------------------
	void ShapeSphere::setLocalPos(const Vector3& _pos)
	{
		Shape::setLocalPos(_pos);
		m_pSphereDesc->localPose.t.x = _pos.x;
		m_pSphereDesc->localPose.t.y = _pos.y;
		m_pSphereDesc->localPose.t.z = _pos.z;
	}
//----------------------------------------------------------------------
	NxShapeDesc* ShapeSphere::getShapeDesc()
	{
		return (static_cast<NxShapeDesc*>(m_pSphereDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	SphereShapeFactory::SphereShapeFactory()
	{
	}
//----------------------------------------------------------------------
	SphereShapeFactory::~SphereShapeFactory()
	{
	}
//----------------------------------------------------------------------
	Shape* SphereShapeFactory::createShape()
	{
		return new ShapeSphere();
	}
//----------------------------------------------------------------------
}