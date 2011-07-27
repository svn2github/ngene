/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeBox.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHAPEBOX_H_
#define __INC_SHAPEBOX_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "ShapeFactory.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Box shape.
		@remarks
			Box shape is the simplest shape available in the nGENE.
			Using it for collisions is in many cases sufficient, though.
	*/
	class nGENEDLL ShapeBox: public Shape
	{
		EXPOSE_TYPE
	private:
		/// PhysX box descriptor
		NxBoxShapeDesc* m_pBoxDesc;

	public:
		ShapeBox();
		~ShapeBox();

		void init();
		void cleanup();

		void setTrigger(bool _value);

		void setDimensions(const Vector3& _dims);
		void setLocalPos(const Vector3& _pos);
		NxShapeDesc* getShapeDesc();
	};

	/// Factory to be used for creating box shapes.
	class nGENEDLL BoxShapeFactory: public ShapeFactory
	{
	public:
		BoxShapeFactory();
		~BoxShapeFactory();

		Shape* createShape();
	};
}


#endif