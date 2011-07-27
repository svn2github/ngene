/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeSphere.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHAPESPHERE_H_
#define __INC_SHAPESPHERE_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "ShapeFactory.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Sphere shape.
	*/
	class nGENEDLL ShapeSphere: public Shape
	{
		EXPOSE_TYPE
	private:
		/// PhysX sphere descriptor
		NxSphereShapeDesc* m_pSphereDesc;

	public:
		ShapeSphere();
		~ShapeSphere();

		void init();
		void cleanup();

		void setTrigger(bool _value);

		/** Sets dimensions of the sphere.
			@param
				_dims x coordinate stores the sphere radius.
		*/
		void setDimensions(const Vector3& _dims);
		void setLocalPos(const Vector3& _pos);

		NxShapeDesc* getShapeDesc();
	};

	/// Factory to be used for creating sphere shapes.
	class nGENEDLL SphereShapeFactory: public ShapeFactory
	{
	public:
		SphereShapeFactory();
		virtual ~SphereShapeFactory();

		Shape* createShape();
	};
}


#endif