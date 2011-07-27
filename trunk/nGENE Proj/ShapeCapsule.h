/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeCapsule.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHAPECAPSULE_H_
#define __INC_SHAPECAPSULE_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "ShapeFactory.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Capsule shape.
	*/
	class nGENEDLL ShapeCapsule: public Shape
	{
		EXPOSE_TYPE
	private:
		/// PhysX capsule descriptor
		NxCapsuleShapeDesc* m_pCapsuleDesc;

	public:
		ShapeCapsule();
		~ShapeCapsule();

		void init();
		void cleanup();

		void setTrigger(bool _value);

		/** Sets radius and height of the capsule.
			@param
				_dims radius is stored in 'x' and height in 'y'.
		*/
		void setDimensions(const Vector3& _dims);
		void setLocalPos(const Vector3& _pos);
		NxShapeDesc* getShapeDesc();
	};

	/// Factory to be used for creating box shapes.
	class nGENEDLL CapsuleShapeFactory: public ShapeFactory
	{
	public:
		CapsuleShapeFactory();
		~CapsuleShapeFactory();

		Shape* createShape();
	};
}


#endif