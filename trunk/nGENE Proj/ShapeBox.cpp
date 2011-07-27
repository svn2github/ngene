/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeBox.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PhysXMapping.h"
#include "Shape.h"
#include "ShapeBox.h"
#include "ShapeEnumerator.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxBoxShapeDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo ShapeBox::Type(L"ShapeBox", &Shape::Type);


	ShapeBox::ShapeBox():
		m_pBoxDesc(new NxBoxShapeDesc())
	{
		init();
	}
//----------------------------------------------------------------------
	ShapeBox::~ShapeBox()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ShapeBox::init()
	{
	}
//----------------------------------------------------------------------
	void ShapeBox::cleanup()
	{
		NGENE_DELETE(m_pBoxDesc);
	}
//----------------------------------------------------------------------
	void ShapeBox::setDimensions(const Vector3& _dims)
	{
		Shape::setDimensions(_dims);
		m_pBoxDesc->dimensions = PhysXMapping::vector3ToNxVec3(_dims * 0.5f);
	}
//----------------------------------------------------------------------
	void ShapeBox::setLocalPos(const Vector3& _pos)
	{
		Shape::setLocalPos(_pos);
		m_pBoxDesc->localPose.t.x = _pos.x;
		m_pBoxDesc->localPose.t.y = _pos.y;
		m_pBoxDesc->localPose.t.z = _pos.z;
	}
//----------------------------------------------------------------------
	void ShapeBox::setTrigger(bool _value)
	{
		Shape::setTrigger(_value);
		if(_value)
			m_pBoxDesc->shapeFlags |= NX_TRIGGER_ENABLE;
		else
			m_pBoxDesc->shapeFlags &= ~NX_TRIGGER_ENABLE;
	}
//----------------------------------------------------------------------
	NxShapeDesc* ShapeBox::getShapeDesc()
	{
		return (static_cast<NxShapeDesc*>(m_pBoxDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	BoxShapeFactory::BoxShapeFactory()
	{
	}
//----------------------------------------------------------------------
	BoxShapeFactory::~BoxShapeFactory()
	{
	}
//----------------------------------------------------------------------
	Shape* BoxShapeFactory::createShape()
	{
		return (new ShapeBox());
	}
//----------------------------------------------------------------------
}