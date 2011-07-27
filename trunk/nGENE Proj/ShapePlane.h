/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapePlane.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHAPEPLANE_H_
#define __INC_SHAPEPLANE_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "Shape.h"
#include "ShapeFactory.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Plane shape.
	*/
	class nGENEDLL ShapePlane: public Shape
	{
		EXPOSE_TYPE
	private:
		/// PhysX plane descriptor
		NxPlaneShapeDesc* m_pPlaneDesc;

	public:
		ShapePlane();
		~ShapePlane();

		void init();
		void cleanup();

		void setTrigger(bool _value);

		/// Sets "D" plane parameter.
		void setDistanceFromOrigin(Real _d);

		/// Sets normal vector of the plane.
		void setNormal(const Vector3& _normal);

		NxShapeDesc* getShapeDesc();
	};


	/// Factory to be used for creating box shapes.
	class nGENEDLL PlaneShapeFactory: public ShapeFactory
	{
	public:
		PlaneShapeFactory();
		~PlaneShapeFactory();

		Shape* createShape();
	};
}


#endif