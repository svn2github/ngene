/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapePlane.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ShapePlane.h"
#include "PhysXMapping.h"
#include "ShapeEnumerator.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxPlaneShapeDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo ShapePlane::Type(L"ShapePlane", &Shape::Type);


	ShapePlane::ShapePlane():
		m_pPlaneDesc(new NxPlaneShapeDesc())
	{
		init();
	}
//----------------------------------------------------------------------
	ShapePlane::~ShapePlane()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ShapePlane::init()
	{
	}
//----------------------------------------------------------------------
	void ShapePlane::cleanup()
	{
		NGENE_DELETE(m_pPlaneDesc);
	}
//----------------------------------------------------------------------
	NxShapeDesc* ShapePlane::getShapeDesc()
	{
		return (static_cast<NxShapeDesc*>(m_pPlaneDesc));
	}
//----------------------------------------------------------------------
	void ShapePlane::setTrigger(bool _value)
	{
		Shape::setTrigger(_value);
		if(_value)
			m_pPlaneDesc->shapeFlags |= NX_TRIGGER_ENABLE;
		else
			m_pPlaneDesc->shapeFlags &= ~NX_TRIGGER_ENABLE;
	}
//----------------------------------------------------------------------
	void ShapePlane::setDistanceFromOrigin(Real _d)
	{
		m_pPlaneDesc->d = _d;
	}
//----------------------------------------------------------------------
	void ShapePlane::setNormal(const Vector3& _normal)
	{
		m_pPlaneDesc->normal = PhysXMapping::vector3ToNxVec3(_normal);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	PlaneShapeFactory::PlaneShapeFactory()
	{
	}
//----------------------------------------------------------------------
	PlaneShapeFactory::~PlaneShapeFactory()
	{
	}
//----------------------------------------------------------------------
	Shape* PlaneShapeFactory::createShape()
	{
		return new ShapePlane();
	}
//----------------------------------------------------------------------
}