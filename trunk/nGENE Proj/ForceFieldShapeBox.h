/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapeBox.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FORCEFIELDSHAPEBOX_H_
#define __INC_FORCEFIELDSHAPEBOX_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "ForceFieldShape.h"
#include "ForceFieldShapeFactory.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Box force field shape.
		@remarks
			Box shape is the simplest shape available in the nGENE.
	*/
	class nGENEDLL ForceFieldShapeBox: public ForceFieldShape
	{
		EXPOSE_TYPE
	private:
		/// PhysX box descriptor
		NxBoxForceFieldShapeDesc* m_pBoxDesc;

	public:
		ForceFieldShapeBox();
		~ForceFieldShapeBox();

		void init();
		void cleanup();

		void setDimensions(const Vector3& _dims);
		void setLocalPos(const Vector3& _pos);
		NxForceFieldShapeDesc* getForceFieldShapeDesc();
	};

	/// Factory to be used for creating box shapes.
	class nGENEDLL ForceFieldBoxShapeFactory: public ForceFieldShapeFactory
	{
	public:
		ForceFieldBoxShapeFactory();
		~ForceFieldBoxShapeFactory();

		ForceFieldShape* createShape();
	};
}


#endif