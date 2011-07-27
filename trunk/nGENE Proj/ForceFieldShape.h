/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShape.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FORCEFIELDSHAPE_H_
#define __INC_FORCEFIELDSHAPE_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "TypeInfo.h"
#include "Vector3.h"


namespace nGENE
{
	/** ForceFieldShape description.
		@par
			Objects of this class are used to describe force field
			shape and inclusion/exclusion groups.
		@see
			ForceField for more details on exclusion and inclusion
			groups.
	*/
	class nGENEDLL ForceFieldShape: public BaseClass
	{
		EXPOSE_TYPE
	protected:
		/// Dimensions of the shape
		Vector3 m_vecDimensions;

		/// Position of shape in ForceFields's space.
		Vector3 m_vecLocalPos;

	public:
		ForceFieldShape();
		virtual ~ForceFieldShape();

		virtual void init();
		virtual void cleanup();

		/** Sets the dimensions of the shape.
			@remarks
				Although all the time the Vector3 structure is used, it has different
				meanings depending on the type of the shape used, eg. for box it
				describes width, length and height, but for sphere 1st dimension
				stores the radius where is the rest plays no role.
			@par
				As PhysX uses dimensions' radii, the given value has to be divided
				by two.
		*/
		virtual void setDimensions(const Vector3& _dims);
		Vector3& getDimensions();

		/// Sets local position of the shape.
		virtual void setLocalPos(const Vector3& _pos);


		virtual NxForceFieldShapeDesc* getForceFieldShapeDesc()=0;
	};



	inline void ForceFieldShape::setDimensions(const Vector3& _dims)
	{
		m_vecDimensions = _dims * 0.5f;
	}
//----------------------------------------------------------------------
	inline Vector3& ForceFieldShape::getDimensions()
	{
		return m_vecDimensions;
	}
//----------------------------------------------------------------------
	inline void ForceFieldShape::setLocalPos(const Vector3& _pos)
	{
		m_vecLocalPos = _pos;
	}
//----------------------------------------------------------------------
}


#endif