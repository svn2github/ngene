/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Shape.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Shape.h"
#include "PhysXMapping.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxShape.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo Shape::Type(L"Shape", &BaseClass::Type);


	Shape::Shape():
		m_pActor(NULL),
		m_bTrigger(false),
		m_bBelongsToCharacterController(false),
		m_bCreated(false)
	{
		init();
	}
//----------------------------------------------------------------------
	Shape::~Shape()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Shape::init()
	{
	}
//----------------------------------------------------------------------
	void Shape::cleanup()
	{
	}
//----------------------------------------------------------------------
	void Shape::setLocalPos(const Vector3& _pos)
	{
		m_vecLocalPos = _pos;

		if(m_bCreated)
			m_pShape->setLocalPosition(PhysXMapping::vector3ToNxVec3(_pos));
	}
//----------------------------------------------------------------------
}