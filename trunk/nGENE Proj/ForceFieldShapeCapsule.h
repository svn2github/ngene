/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapeCapsule.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FORCEFIELDSHAPECAPSULE_H_
#define __INC_FORCEFIELDSHAPECAPSULE_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "ForceFieldShape.h"
#include "ForceFieldShapeFactory.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Capsule shape.
	*/
	class nGENEDLL ForceFieldShapeCapsule: public ForceFieldShape
	{
		EXPOSE_TYPE
	private:
		/// PhysX capsule descriptor
		NxCapsuleForceFieldShapeDesc* m_pCapsuleDesc;

	public:
		ForceFieldShapeCapsule();
		~ForceFieldShapeCapsule();

		void init();
		void cleanup();

		/** Sets radius and height of the capsule.
			@param
				_dims radius is stored in 'x' and height in 'y'.
		*/
		void setDimensions(const Vector3& _dims);
		void setLocalPos(const Vector3& _pos);

		NxForceFieldShapeDesc* getForceFieldShapeDesc();
	};

	/// Factory to be used for creating box shapes.
	class nGENEDLL ForceFieldCapsuleShapeFactory: public ForceFieldShapeFactory
	{
	public:
		ForceFieldCapsuleShapeFactory();
		~ForceFieldCapsuleShapeFactory();

		ForceFieldShape* createShape();
	};
}


#endif