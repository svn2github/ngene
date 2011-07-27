/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Vector2.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VECTOR2_H_
#define __INC_VECTOR2_H_


#include "Maths.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** 2-dimensional vector class.
		@par
			Although many overloaded operators are available for clients
			convenience, in sake of efficiency it is better not to use
			them as some of them (eg. +, -, *) result in temporary object
			creation. It is better to use member functions instead.
			They provide the same functionality but are much faster due
			to lack of aforementioned drawback.
	*/
	class nGENEDLL Vector2
	{
	public:
		union {
			struct {
				Real x;
				Real y;
			};
			Real vals[2];
		};

		// Some static members. They make up the standard base
		// in euclidean 2-dimensional space. Also negated version
		// of them is provided.
		static const Vector2 UNIT_X;
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;

		static const Vector2 ZERO_VECTOR;
		static const Vector2 ONE_VECTOR;

		static const Vector2 INFINITY_VECTOR;
		static const Vector2 NEGATIVE_INFINITY_VECTOR;

	public:
		explicit Vector2(Real _x=0.0f, Real _y=0.0f);
		~Vector2();

		Vector2& operator=(const Vector2& _vec);

		/// Lets you set new members' values.
		void set(Real _x, Real _y);
		/// Lets you set new members' values.
		void set(const Vector2& _vec);


		/// Adds given vector to the current one.
		void add(const Vector2& _vec);
		/// Adds given vector to the current one.
		void add(Real _x, Real _y);
		/// Adds real number to all components.
		void add(Real _num);

		/// Subtract given vector from the current one.
		void subtract(const Vector2& _vec);

		/** Vector by scalar multiplication.
			@remarks
				Each vector's member is multiplied by a given
				number.
		*/
		void multiplyByNumber(const Real& _num);

		/** Normalizes current vector.
			@par
				If the vector length is 0 it does nothing.
			@remarks
				The new vector has the same direction as it had, but
				its	length is equal to 1. Therefore it is usually
				called unit	vector.
		*/
		void normalize();
		/** Normalizes current vector.
			@par
				If the vector length is 0 it does nothing.
			@remarks
				The new vector has the same direction as it had, but
				its	length is equal to 1. Therefore it is usually
				called unit	vector.
		*/
		friend Vector2 Maths::normalize(const Vector2& _vec);

		/** Negates current vector.
			@remarks
				The negated vector has the same length as it has
				before negating operation took place but all its
				members are opposite.
		*/
		void negate();

		/// Calculates the length (magnitude) of the vector.
		Real getLength() const;
		/// Calculates the length (magnitude) of the vector.
		Real length() const;

		/** Calculates dot product of the current vector and the
			vector given.
			@remarks
				Dot product, also called scalar product is especially
				useful when you need to obtain the angle between two
				vectors. Dot product of two unit vectors equals cosine
				of the angle between them, so calculating the angle is
				straightforward. For vectors of different lengths to
				get cosine you only need to divide result of this
				function by product of the given vectors' length.
			@return
				float dot product value
		*/
		float dotProduct(const Vector2& vec) const;


		// Below are overloaded operators.


		Vector2& operator+=(const Vector2& rhs);
		Vector2& operator-=(const Vector2& rhs);
		Vector2& operator*=(const Real& rhs);
		
		friend const Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
		friend const Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
		friend const Vector2 operator*(const Vector2& lhs, const Real& rhs);
		friend const Vector2 operator*(const Real& lhs, const Vector2& rhs);

		friend const float operator%(const Vector2& lhs, const Vector2& rhs);

		const Vector2 operator-();

		bool operator==(const Vector2& _vec);
		friend bool operator==(const Vector2& _vec1, const Vector2& _vec2);

		bool operator!=(const Vector2& _vec);
		friend bool operator!=(const Vector2& _vec1, const Vector2& _vec2);

		Real operator[](uint i);
		Real operator[](uint i) const;

		/// Returns array of data.
		Real* getData();

		/** Creates Vector2 from string.
			@param
				_string keeping data of vector. Three formats are accepted:
				"x y", "x;y" and "x,y".
		*/
		void fromString(const string& _string);
		/// Writes vector to string as "x y".
		string toString();
	};


	
	inline Vector2& Vector2::operator=(const Vector2& _vec)
	{
		x = _vec.x;
		y = _vec.y;

		return (*this);
	}
//----------------------------------------------------------------------
	inline void Vector2::set(Real _x, Real _y)
	{
		x = _x;
		y = _y;
	}
//----------------------------------------------------------------------
	inline void Vector2::set(const Vector2& _vec)
	{
		x = _vec.x;
		y = _vec.y;
	}
//----------------------------------------------------------------------
	inline void Vector2::add(const Vector2& _vec)
	{
		x += _vec.x;
		y += _vec.y;
	}
//----------------------------------------------------------------------
	inline void Vector2::add(Real _x, Real _y)
	{
		x += _x;
		y += _y;
	}
//----------------------------------------------------------------------
	inline void Vector2::add(Real _num)
	{
		x += _num;
		y += _num;
	}
//----------------------------------------------------------------------
	inline void Vector2::subtract(const Vector2& _vec)
	{
		x -= _vec.x;
		y -= _vec.y;
	}
//----------------------------------------------------------------------
	inline void Vector2::multiplyByNumber(const Real& _num)
	{
		x *= _num;
		y *= _num;
	}
//----------------------------------------------------------------------
	inline void Vector2::normalize()
	{
		float length = Maths::fsqrt(x * x + y * y);
		if(Maths::floatCompare(0.0f, length))
		{
			float inv = 1.0f / length;
			x *= inv;
			y *= inv;
		}
	}
//----------------------------------------------------------------------
	inline void Vector2::negate()
	{
		x = -x;
		y = -y;
	}
//----------------------------------------------------------------------
	inline float Vector2::getLength() const
	{
		return (Maths::fsqrt(x * x + y * y));
	}
//----------------------------------------------------------------------
	inline float Vector2::length() const
	{
		return (Maths::fsqrt(x * x + y * y));
	}
//----------------------------------------------------------------------
	inline float Vector2::dotProduct(const Vector2& vec) const
	{
		return (x * vec.x + y * vec.y);
	}
//----------------------------------------------------------------------
	inline Vector2 Maths::normalize(const Vector2& _vec)
	{
		Vector2 result = _vec;

		float length = result.getLength();
		if(Maths::floatCompare(0.0f, length))
		{
			float inv = 1.0f / length;
			result.x *= inv;
			result.y *= inv;
		}

		return result;
	}
//----------------------------------------------------------------------
	inline Vector2& Vector2::operator+=(const Vector2& rhs)
	{
		add(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector2& Vector2::operator-=(const Vector2& rhs)
	{
		subtract(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector2& Vector2::operator*=(const Real& rhs)
	{
		multiplyByNumber(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline const Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x + rhs.x,
					   lhs.y + rhs.y);
	}
//----------------------------------------------------------------------
	inline const Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x - rhs.x,
					   lhs.y - rhs.y);
	}
//----------------------------------------------------------------------
	inline const Vector2 operator*(const Vector2& lhs, const Real& rhs)
	{
		Vector2 result = lhs;
		result.multiplyByNumber(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Vector2 operator*(const Real& lhs, const Vector2& rhs)
	{
		Vector2 result = rhs;
		result.multiplyByNumber(lhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const float operator%(const Vector2& lhs, const Vector2& rhs)
	{
		return (lhs.dotProduct(rhs));
	}
//----------------------------------------------------------------------
	inline const Vector2 Vector2::operator-()
	{
		Vector2 newVector(*this);
		newVector.negate();

		return (newVector);
	}
//----------------------------------------------------------------------
	inline Real Vector2::operator[](uint i)
	{
		Assert(i < 2, "Vector dimension index not allowed");
		return (vals[i]);
	}
//----------------------------------------------------------------------
	inline Real Vector2::operator[](uint i) const
	{
		Assert(i < 2, "Vector dimension index not allowed");
		return (vals[i]);
	}
//----------------------------------------------------------------------
	inline Real* Vector2::getData()
	{
		return &vals[0];
	}
//----------------------------------------------------------------------
	inline bool Vector2::operator==(const Vector2& _vec)
	{
		return (!Maths::floatCompare(x, _vec.x) &&
				!Maths::floatCompare(y, _vec.y));
	}
//----------------------------------------------------------------------
	inline bool Vector2::operator!=(const Vector2& _vec)
	{
		return (Maths::floatCompare(x, _vec.x) ||
				Maths::floatCompare(y, _vec.y));
	}
//----------------------------------------------------------------------
	inline bool operator==(const Vector2& _vec1, const Vector2& _vec2)
	{
		return (!Maths::floatCompare(_vec1.x, _vec2.x) &&
				!Maths::floatCompare(_vec1.y, _vec2.y));
	}
//----------------------------------------------------------------------
	inline bool operator!=(const Vector2& _vec1, const Vector2& _vec2)
	{
		return (Maths::floatCompare(_vec1.x, _vec2.x) ||
				Maths::floatCompare(_vec1.y, _vec2.y));
	}
//----------------------------------------------------------------------
}


#endif