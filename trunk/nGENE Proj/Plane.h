/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Plane.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PLANE_H_
#define __INC_PLANE_H_


#include "Point.h"
#include "Vector3.h"


namespace nGENE
{
	/** A plane is mathematical set of points in 3-dimensional
		euclidean space. Points satisfy the equation:
		A*x + B*y + C*z + D = 0.
		@par
			The plane is described by the normal vector (perpendicular
			to the plane) and the point belonging to this plane.
	*/
	class nGENEDLL Plane
	{
	protected:
		Vector3 m_vecNormal;				///< Plane's normal vector
		Real D;								///< D = - A*x0 - B*y0 - C*z0
		Point m_ptPoint;					///< Point lying on the plane


		void calculatePoint(const Vector3& _normal, Real _d);

	public:
		/// Some static constants
		static const Plane UNIT_X;
		static const Plane UNIT_Y;
		static const Plane UNIT_Z;
		static const Plane NEGATIVE_UNIT_X;
		static const Plane NEGATIVE_UNIT_Y;
		static const Plane NEGATIVE_UNIT_Z;
	
	public:
		Plane();
		explicit Plane(Real _a, Real _b, Real _c, Real _d);
		explicit Plane(const Vector3& _normal, Real _d);
		explicit Plane(const Vector3& _normal, const Point& _pt);
		/** The plane can be also made of 2 vectors. Normal vector
			is than equal to their cross product.
		*/
		explicit Plane(const Vector3& _vec1, const Vector3& _vec2, const Point& _pt);
		/// Plane can be also constructed from 3 points which lay on it.
		explicit Plane(const Vector3& _vec1, const Vector3& _vec2, const Vector3& _vec3);
		~Plane();

		/// Returns plane normal.
		const Vector3& getPlaneNormal() const;
		/// Returns plane D parameter.
		Real getPlaneD() const;
		/// Returns point P0 belonging to the plane.
		const Point& getPlanePoint() const;

		/** Sets plane.
			@param _normal normal vector of the plane
			@param _d d parameter of the plane.
		*/
		void setPlane(const Vector3& _normal, Real _d);
		/** Sets plane.
			@param _normal normal vector of the plane
			@param _pt point P0 laying on the plane.
		*/
		void setPlane(const Vector3& _normal, const Point& _pt);
		/// Sets plane.
		void setPlane(Real _a, Real _b, Real _c, Real _d);
		/// Constructs a plane from 3 points belonging to it.
		void setPlane(const Vector3& _vec1, const Vector3& _vec2, const Vector3& _vec3);

		/** Transforms the plane by the matrix.
			@param
				_transform transformation matrix has to be inverse transpose
				of the actual matrix to properly transform both normal and point
				of the plane.
		*/
		void transform(const Matrix4x4& _transform);

		/// Calculates distance from the plane for a given point.
		Real getDistanceFromPlane(const Vector3& _vec) const;

		/** Creates Plane from string.
			@param
				_string keeping data of plane. Three formats are accepted:
				"a b c d", "a;b;c;d" and "a,b,c,d".
		*/
		void fromString(const string& _string);
		/// Writes Plane to string as "a b c d".
		string toString();
	};



	inline const Vector3& Plane::getPlaneNormal() const
	{
		return m_vecNormal;
	}
//----------------------------------------------------------------------
	inline Real Plane::getPlaneD() const
	{
		return D;
	}
//----------------------------------------------------------------------
	inline const Point& Plane::getPlanePoint() const
	{
		return m_ptPoint;
	}
//----------------------------------------------------------------------
}


#endif