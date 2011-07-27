/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AABB.h
Version:	0.13
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_AABB_H_
#define __INC_AABB_H_


#include "BoundingVolume.h"
#include "Prerequisities.h"
#include "Vector3.h"


namespace nGENE
{
	/** AABB stands for axis-aligned bounding box, meaning
		each AABB's face is parallel to one of coordinate system
		axes.
		@par
			AABBs are especially useful when checking for
			collisions or testing objects visibility against
			viewing frustum.
		@par
			Thanks to axis-alignment only three points are
			necessary to describe AABB in the 3D space: its
			two corners and the centre.
		@par
			You can also add two AABBs to each other. All the coordinates
			will be maximum of both factors.
	*/
	class nGENEDLL AABB: public BoundingVolume
	{
		EXPOSE_TYPE
	private:
		Vector3 m_vecMinimum;							///< Minimum corner of the AABB
		Vector3 m_vecMaximum;							///< Maximum corner of the AABB
		Vector3 m_vecCentre;							///< Centre point of the AABB


		void computeCentre();

	public:
		AABB();
		AABB(const Vector3& _min, const Vector3& _max);
		explicit AABB(VertexBuffer& _vb);
		virtual ~AABB();

		/// Creates AABB around given set of vertices.
		void construct(VertexBuffer& _vb);
		/// Creates AABB around given set of vertices.
		void construct(VertexBuffer* _vb);
		/// Creates AABB around given set of vertices.
		void construct(void* _vertices, uint _count, uint _size, uint _offset);
		/// Creates AABB out of an OBB.
		void construct(OBB& _box);

		/// Returns minimum vector.
		Vector3& getMinimum() {return m_vecMinimum;}
		/// Returns maximum vector.
		Vector3& getMaximum() {return m_vecMaximum;}
		/// Returns AABB centre point.
		Vector3& getCentre() {return m_vecCentre;}

		/// Checks whether another AABB lies completely inside this one.
		bool fits(AABB& _aabb);

		/// Sets AABB minimum and maximum vectors.
		void setAABB(const Vector3& _min, const Vector3& _max);
		/// Sets AABB minimum vector.
		void setMinimum(const Vector3& _min);
		/// Sets AABB maximum vector.
		void setMaximum(const Vector3& _max);


		/// Returns distance from this AABB to a given vector.
		Real distanceToVector(const Vector3& _vec);
		/// Returns distance from this AABB to a given point.
		Real distanceToPoint(const Point& _pt);


		AABB& operator+=(const AABB& rhs);

		friend const AABB operator+(const AABB& lhs, const AABB& rhs);
	};



	inline AABB& AABB::operator+=(const AABB& rhs)
	{
		m_vecMinimum.x = (m_vecMinimum.x <= rhs.m_vecMinimum.x ? m_vecMinimum.x : rhs.m_vecMinimum.x);
		m_vecMinimum.y = (m_vecMinimum.y <= rhs.m_vecMinimum.y ? m_vecMinimum.y : rhs.m_vecMinimum.y);
		m_vecMinimum.z = (m_vecMinimum.z <= rhs.m_vecMinimum.z ? m_vecMinimum.z : rhs.m_vecMinimum.z);

		m_vecMaximum.x = (m_vecMaximum.x >= rhs.m_vecMaximum.x ? m_vecMaximum.x : rhs.m_vecMaximum.x);
		m_vecMaximum.y = (m_vecMaximum.y >= rhs.m_vecMaximum.y ? m_vecMaximum.y : rhs.m_vecMaximum.y);
		m_vecMaximum.z = (m_vecMaximum.z >= rhs.m_vecMaximum.z ? m_vecMaximum.z : rhs.m_vecMaximum.z);

		computeCentre();

		return (*this);
	}
//----------------------------------------------------------------------
	inline const AABB operator+(const AABB& lhs, const AABB& rhs)
	{
		AABB result;

		result.m_vecMinimum.x = (lhs.m_vecMinimum.x <= rhs.m_vecMinimum.x ? lhs.m_vecMinimum.x : rhs.m_vecMinimum.x);
		result.m_vecMinimum.y = (lhs.m_vecMinimum.y <= rhs.m_vecMinimum.y ? lhs.m_vecMinimum.y : rhs.m_vecMinimum.y);
		result.m_vecMinimum.z = (lhs.m_vecMinimum.z <= rhs.m_vecMinimum.z ? lhs.m_vecMinimum.z : rhs.m_vecMinimum.z);

		result.m_vecMaximum.x = (lhs.m_vecMaximum.x >= rhs.m_vecMaximum.x ? lhs.m_vecMaximum.x : rhs.m_vecMaximum.x);
		result.m_vecMaximum.y = (lhs.m_vecMaximum.y >= rhs.m_vecMaximum.y ? lhs.m_vecMaximum.y : rhs.m_vecMaximum.y);
		result.m_vecMaximum.z = (lhs.m_vecMaximum.z >= rhs.m_vecMaximum.z ? lhs.m_vecMaximum.z : rhs.m_vecMaximum.z);

		result.computeCentre();

		return result;
	}
//----------------------------------------------------------------------
}


#endif