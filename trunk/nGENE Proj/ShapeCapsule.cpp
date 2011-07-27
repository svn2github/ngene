/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeCapsule.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PhysXMapping.h"
#include "Shape.h"
#include "ShapeCapsule.h"
#include "ShapeEnumerator.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxCapsuleShapeDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo ShapeCapsule::Type(L"ShapeCapsule", &Shape::Type);


	ShapeCapsule::ShapeCapsule():
		m_pCapsuleDesc(new NxCapsuleShapeDesc())
	{
		init();
	}
//----------------------------------------------------------------------
	ShapeCapsule::~ShapeCapsule()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ShapeCapsule::init()
	{
	}
//----------------------------------------------------------------------
	void ShapeCapsule::cleanup()
	{
		NGENE_DELETE(m_pCapsuleDesc);
	}
//----------------------------------------------------------------------
	void ShapeCapsule::setDimensions(const Vector3& _dims)
	{
		Shape::setDimensions(_dims);
		m_pCapsuleDesc->radius = _dims.x;
		m_pCapsuleDesc->height = _dims.y;
	}
//----------------------------------------------------------------------
	void ShapeCapsule::setTrigger(bool _value)
	{
		Shape::setTrigger(_value);
		if(_value)
			m_pCapsuleDesc->shapeFlags |= NX_TRIGGER_ENABLE;
		else
			m_pCapsuleDesc->shapeFlags &= ~NX_TRIGGER_ENABLE;
	}
//----------------------------------------------------------------------
	void ShapeCapsule::setLocalPos(const Vector3& _pos)
	{
		Shape::setLocalPos(_pos);
		m_pCapsuleDesc->localPose.t.x = _pos.x;
		m_pCapsuleDesc->localPose.t.y = _pos.y;
		m_pCapsuleDesc->localPose.t.z = _pos.z;
	}
//----------------------------------------------------------------------
	NxShapeDesc* ShapeCapsule::getShapeDesc()
	{
		return (static_cast<NxShapeDesc*>(m_pCapsuleDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	CapsuleShapeFactory::CapsuleShapeFactory()
	{
	}
//----------------------------------------------------------------------
	CapsuleShapeFactory::~CapsuleShapeFactory()
	{
	}
//----------------------------------------------------------------------
	Shape* CapsuleShapeFactory::createShape()
	{
		return (new ShapeCapsule());
	}
//----------------------------------------------------------------------
}