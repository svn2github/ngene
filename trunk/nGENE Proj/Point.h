/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Point.cpp
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_POINT_H_
#define __INC_POINT_H_


#include "Vector3.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** In terms of linear algebra a point is just
		a free vector.
		@remarks
			The class is much poorer than regular Vector3
			class as in case of points such functionality
			is not necessary.
	*/
	class nGENEDLL Point
	{
	public:
		/// Position of the point in 3D space
		union {
			struct {Real x, y, z;};
			Real point[3];
		};

		// Some important constants
		static const Point ZERO_POINT;

	public:
		Point();
		Point(const Vector3& _point);
		explicit Point(Real _x, Real _y, Real _z);
		~Point();

		Point& operator=(const Point& _point);

		/// Sets the new member values.
		void set(Real _x, Real _y, Real _z);
		/// Sets the new member values.
		void set(const Vector3& _point);

		/** Calculates the distance between the Point and the Point
			given.
		*/
		Real distanceToPoint(const Point& _a);
		/** Calculates the distance between two given points.
		*/
		friend Real distanceBetweenPoints(const Point& _a, const Point& _b);

		/// Translates the point.
		void translate(Real _x, Real _y, Real _z);
		/// Translates the point.
		void translate(const Vector3& _vec);


		// Below are overloaded operators.


		Point& operator+=(const Vector3& rhs);

		friend const Vector3 operator-(const Point& lhs, const Point& rhs);

		bool operator==(const Point& _a);
		friend bool operator==(const Point& _a, const Point& _b);

		bool operator!=(const Point& _a);
		friend bool operator!=(const Point& _a, const Point& _b);

		operator Vector3() const;

		Real operator[](uint i);
		Real operator[](uint i) const;

		/** Creates Vector3 from string.
		@param
		_string keeping data of vector. Three formats are accepted:
		"x y z", "x;y;z" and "x,y,z".
		*/
		void fromString(const string& _string);
		/// Writes vector to string as "x y z".
		string toString();
	};



	inline Point& Point::operator=(const Point& _point)
	{
		x = _point.x;
		y = _point.y;
		z = _point.z;

		return (*this);
	}
//----------------------------------------------------------------------
	inline void Point::set(Real _x, Real _y, Real _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
//----------------------------------------------------------------------
	inline void Point::set(const Vector3& _point)
	{
		x = _point.x;
		y = _point.y;
		z = _point.z;
	}
//----------------------------------------------------------------------
	inline Real Point::distanceToPoint(const Point& _a)
	{
		Real dx = x - _a.x;
		Real dy = y - _a.y;
		Real dz = z - _a.z;

		return (Maths::fsqrt(dx * dx + dy * dy + dz * dz));
	}
//----------------------------------------------------------------------
	inline Real distanceBetweenPoints(const Point& _a, const Point& _b)
	{
		Point temp = _a;
		return temp.distanceToPoint(_b);
	}
//----------------------------------------------------------------------
	inline void Point::translate(Real _x, Real _y, Real _z)
	{
		x += _x;
		y += _y;
		z += _z;
	}
//----------------------------------------------------------------------
	inline void Point::translate(const Vector3& _vec)
	{
		x += _vec.x;
		y += _vec.y;
		z += _vec.z;
	}
//----------------------------------------------------------------------
	inline Point& Point::operator+=(const Vector3& rhs)
	{
		translate(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline const Vector3 operator-(const Point& lhs, const Point& rhs)
	{
		return Vector3(lhs.x - rhs.x,
					   lhs.y - rhs.y,
					   lhs.z - rhs.z);
	}
//----------------------------------------------------------------------
	inline bool Point::operator==(const Point& _a)
	{
		return (!Maths::floatCompare(x, _a.x) &&
				!Maths::floatCompare(y, _a.y) &&
				!Maths::floatCompare(z, _a.z));
	}
//----------------------------------------------------------------------
	inline bool Point::operator!=(const Point& _a)
	{
		return (Maths::floatCompare(x, _a.x) ||
				Maths::floatCompare(y, _a.y) ||
				Maths::floatCompare(z, _a.z));
	}
//----------------------------------------------------------------------
	inline bool operator==(const Point& _a, const Point& _b)
	{
		return (!Maths::floatCompare(_a.x, _b.x) &&
				!Maths::floatCompare(_a.y, _b.y) &&
				!Maths::floatCompare(_a.z, _b.z));
	}
//----------------------------------------------------------------------
	inline bool operator!=(const Point& _a, const Point& _b)
	{
		return (Maths::floatCompare(_a.x, _b.x) ||
				Maths::floatCompare(_a.y, _b.y) ||
				Maths::floatCompare(_a.z, _b.z));
	}
//----------------------------------------------------------------------
	inline Point::operator Vector3() const
	{
		return (Vector3(x, y, z));
	}
//----------------------------------------------------------------------
	inline Real Point::operator[](uint i)
	{
		Assert(i < 3, "Point dimension index not allowed");
		return (point[i]);
	}
//----------------------------------------------------------------------
	inline Real Point::operator[](uint i) const
	{
		Assert(i < 3, "Point dimension index not allowed");
		return (point[i]);
	}
//----------------------------------------------------------------------
}


#endif