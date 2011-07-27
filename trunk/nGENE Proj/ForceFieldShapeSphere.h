/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapeSphere.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FORCEFIELDSHAPESPHERE_H_
#define __INC_FORCEFIELDSHAPESPHERE_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "ForceFieldShape.h"
#include "ForceFieldShapeFactory.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Sphere shape.
	*/
	class nGENEDLL ForceFieldShapeSphere: public ForceFieldShape
	{
		EXPOSE_TYPE
	private:
		/// PhysX sphere descriptor
		NxSphereForceFieldShapeDesc* m_pSphereDesc;

	public:
		ForceFieldShapeSphere();
		~ForceFieldShapeSphere();

		void init();
		void cleanup();

		/** Sets dimensions of the sphere.
			@param
				_dims x coordinate stores the sphere radius.
		*/
		void setDimensions(const Vector3& _dims);
		void setLocalPos(const Vector3& _pos);

		NxForceFieldShapeDesc* getForceFieldShapeDesc();
	};

	/// Factory to be used for creating box shapes.
	class nGENEDLL ForceFieldSphereShapeFactory: public ForceFieldShapeFactory
	{
	public:
		ForceFieldSphereShapeFactory();
		~ForceFieldSphereShapeFactory();

		ForceFieldShape* createShape();
	};
}


#endif