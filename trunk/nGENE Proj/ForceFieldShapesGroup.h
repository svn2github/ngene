/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapesGroup.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FORCEFIELDSHAPESGROUP_H_
#define __INC_FORCEFIELDSHAPESGROUP_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "TypeInfo.h"
#include "Vector3.h"


namespace nGENE
{
	/** A group of shapes for force field.
	*/
	class nGENEDLL ForceFieldShapesGroup: public BaseClass
	{
		EXPOSE_TYPE
	protected:
		/// Specifies if is included or excluded from force field
		bool m_bIsIncluded;

		/// Pointer to PhysX scene object.
		NxScene* m_pScene;

		/// Shape group object.
		NxForceFieldShapeGroup* m_pShapeGroup;

		/// Vector of shapes.
		vector <ForceFieldShape*> m_vShapes;

	public:
		ForceFieldShapesGroup(NxScene* _scene, bool _isIncluded);
		virtual ~ForceFieldShapesGroup();

		virtual void init();
		virtual void cleanup();

		/// Adds new force field shape to the group.
		virtual void addShape(ForceFieldShape* _shape);

		/// Returns shape group.
		virtual NxForceFieldShapeGroup* getShapeGroup() const;
	};
}


#endif