/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Ray.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RAY_H_
#define __INC_RAY_H_


#include "Prerequisities.h"
#include "Point.h"
#include "Vector3.h"


namespace nGENE
{
	/// Description of the intersection
	typedef struct nGENEDLL SIntersectionDescription
	{
		Point intersectionPoint;		///< Point of intersection
		Vector3 normal;					///< Surface normal at intersection point
	} INTERSECTION_DESC;

	/** Ray mathematical object.
		@remarks
			Ray is a line having beginning specified and ending in
			infinity.
		@par
			In case all intersection tests assumption is made, that
			intersection occurs only if the ray points in an opposite
			direction as the normal of the surface.
	*/
	class nGENEDLL Ray
	{
	private:
		Point m_ptOrigin;						///< Beginning of the ray
		Vector3 m_vecDirection;					///< Direction of the ray
		Real m_fTMin;							///< Minimum t-value found during intersection test

		Point m_ptIntersectionPoint;			///< Point of last intersection

	public:
		Ray();
		explicit Ray(Point& _origin, Vector3& _direction);
		~Ray();

		/// Sets up ray.
		void set(Point& _origin, Vector3& _direction);

		/// Does ray intersect AABB?
		bool intersectAABB(AABB& _aabb);
		/// Does ray intersect plane?
		bool intersectPlane(const Plane& _plane);
		/// Does ray intersect triangle?
		bool intersectTriangle(const Vector3& _vec0,
							   const Vector3& _vec1,
							   const Vector3& _vec2);
		
		/** Does ray intersect triangle mesh?
			@remarks
				Before calling this function, in most cases you would
				have to move ray into object's local space. If you know
				the matrix you can call moveToObjectSpace() method and
				provide it as an argument.
		*/
		bool intersectSurface(const Surface& _surface);
		/** Does ray intersect triangle mesh?
			@remarks
			Before calling this function, in most cases you would
			have to move ray into object's local space. If you know
			the matrix you can call moveToObjectSpace() method and
			provide it as an argument.
		*/
		bool intersectSurface(const Surface* _surface);
		/** Does ray intersect triangle mesh?
			@param
				_surface surface to test against.
			@param
				_desc details of intersection. It is OUT parameter.
			@remarks
				Before calling this function, in most cases you would
				have to move ray into object's local space. If you know
				the matrix you can call moveToObjectSpace() method and
				provide it as an argument.
		*/
		bool intersectSurface(const Surface& _surface, INTERSECTION_DESC& _desc);

		/// Moves ray to the given space.
		void moveToObjectSpace(const Matrix4x4& _mat);

		/// Sets ray origin.
		void setOrigin(const Point& _origin);
		/// Returns ray origin.
		Point& getOrigin();

		/// Sets ray direction.
		void setDirection(const Vector3& _direction);
		/// Returns ray direction.
		Vector3& getDirection();

		/** Returns intersection point.
			@returns
				Point& if intersection test has been carried then
				result is point of this intersection test or (0, 0, 0)
				otherwise.
		*/
		Point& getIntersectionPoint();

		/// Returns t-value.
		Real getTValue() const;
	};



	inline void Ray::set(Point& _origin, Vector3& _direction)
	{
		m_ptOrigin = _origin;
		m_vecDirection = _direction;
	}
//----------------------------------------------------------------------
	inline void Ray::setOrigin(const Point& _origin)
	{
		m_ptOrigin = _origin;
	}
//----------------------------------------------------------------------
	inline Point& Ray::getOrigin()
	{
		return m_ptOrigin;
	}
//----------------------------------------------------------------------
	inline void Ray::setDirection(const Vector3& _direction)
	{
		m_vecDirection = _direction;
	}
//----------------------------------------------------------------------
	inline Vector3& Ray::getDirection()
	{
		return m_vecDirection;
	}
//----------------------------------------------------------------------
	inline Point& Ray::getIntersectionPoint()
	{
		return m_ptIntersectionPoint;
	}
//----------------------------------------------------------------------
	inline Real Ray::getTValue() const
	{
		return m_fTMin;
	}
//----------------------------------------------------------------------
}


#endif