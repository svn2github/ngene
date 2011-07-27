/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapesGroup.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ForceFieldShapesGroup.h"
#include "ForceFieldShape.h"


#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
	#include "NxPhysics.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo ForceFieldShapesGroup::Type(L"ForceFieldShapesGroup", &BaseClass::Type);


	ForceFieldShapesGroup::ForceFieldShapesGroup(NxScene* _scene, bool _isIncluded):
		m_bIsIncluded(_isIncluded),
		m_pScene(_scene),
		m_pShapeGroup(NULL)
	{
		init();
	}
//----------------------------------------------------------------------
	ForceFieldShapesGroup::~ForceFieldShapesGroup()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ForceFieldShapesGroup::init()
	{
		NxForceFieldShapeGroupDesc shapeGroupDesc;
		shapeGroupDesc.flags = m_bIsIncluded ? 0 : NX_FFSG_EXCLUDE_GROUP;
		m_pShapeGroup = m_pScene->createForceFieldShapeGroup(shapeGroupDesc);
	}
//----------------------------------------------------------------------
	void ForceFieldShapesGroup::cleanup()
	{
		if(m_vShapes.size())
		{
			for(uint i = 0; i < m_vShapes.size(); ++i)
				NGENE_DELETE(m_vShapes[i]);
			m_vShapes.clear();
		}
	}
//----------------------------------------------------------------------
	void ForceFieldShapesGroup::addShape(ForceFieldShape* _shape)
	{
		m_pShapeGroup->createShape(*_shape->getForceFieldShapeDesc());
		m_vShapes.push_back(_shape);
	}
//----------------------------------------------------------------------
	NxForceFieldShapeGroup* ForceFieldShapesGroup::getShapeGroup() const
	{
		return m_pShapeGroup;
	}
//----------------------------------------------------------------------
}