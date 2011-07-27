/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ICullable.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ICullable.h"
#include "AABB.h"
#include "OBB.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ICullable::Type(L"ICullable", &IPickable::Type);


	ICullable::ICullable():
		m_pAABB(new AABB()),
		m_pOBB(new OBB()),
		m_bCulled(false),
		m_bHasChanged(false)
	{
	}
//----------------------------------------------------------------------
	ICullable::ICullable(const ICullable& src)
	{
		m_pAABB = new AABB(*src.m_pAABB);
		m_pOBB = new OBB(*src.m_pOBB);
		m_bCulled = src.m_bCulled;
		m_bHasChanged = src.m_bHasChanged;
	}
//----------------------------------------------------------------------
	ICullable::~ICullable()
	{
		NGENE_DELETE(m_pAABB);
		NGENE_DELETE(m_pOBB);
	}
//----------------------------------------------------------------------
	ICullable& ICullable::operator=(const ICullable& rhs)
	{
		if(this == &rhs)
			return (*this);

		(*m_pAABB) = (*rhs.m_pAABB);
		(*m_pOBB) = (*rhs.m_pOBB);
		m_bCulled = rhs.m_bCulled;
		m_bHasChanged = rhs.m_bHasChanged;

		return (*this);
	}
//----------------------------------------------------------------------
	void ICullable::setAABB(const AABB& _aabb)
	{
		(*m_pAABB) = _aabb;
	}
//----------------------------------------------------------------------
	void ICullable::setOBB(const OBB& _obb)
	{
		(*m_pOBB) = _obb;
	}
//----------------------------------------------------------------------
}
