/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Vector4.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VECTOR4_H_
#define __INC_VECTOR4_H_


#include "Engine.h"
#include "Maths.h"


namespace nGENE
{
	/** 4-dimensional vector class.
		@par
			Although many overloaded operators are available for clients
			convenience, in sake of efficiency it is better not to use
			them as some of them (eg. +, -, *) result in temporary object
			creation. It is better to use member functions instead.
			They provide the same functionality but are much faster due
			to lack of aforementioned drawback.
		@par
			Vector4 class takes advantage of SIMD extensions if they are
			available on the platform the engine is being run on. This can
			greatly speed up complex calculations.
	*/
	class nGENEDLL Vector4
	{
	public:
		union {
			struct {
				Real x;
				Real y;
				Real z;
				Real w;
			};
			Real vals[4];
		};

		// Some static members. They make up the standard base
		// in euclidean 4-dimensional space. Also negated version
		// of them is provided.
		static const Vector4 UNIT_X;
		static const Vector4 UNIT_Y;
		static const Vector4 UNIT_Z;
		static const Vector4 UNIT_W;
		static const Vector4 NEGATIVE_UNIT_X;
		static const Vector4 NEGATIVE_UNIT_Y;
		static const Vector4 NEGATIVE_UNIT_Z;
		static const Vector4 NEGATIVE_UNIT_W;

		static const Vector4 ZERO_VECTOR;
		static const Vector4 ONE_VECTOR;

		static const Vector4 INFINITY_VECTOR;
		static const Vector4 NEGATIVE_INFINITY_VECTOR;

	public:
		Vector4();
		explicit Vector4(float _x, float _y, float _z, float _w);
		explicit Vector4(const Vector2& _vec);
		explicit Vector4(const Vector2& _vec, Real _z, Real _w);
		explicit Vector4(const Vector3& _vec);
		explicit Vector4(const Vector3& _vec, Real _w);
		explicit Vector4(const Quaternion& _quat);
		~Vector4();

		Vector4& operator=(const Vector4& _vec);

		/// Lets you set new members' values.
		void set(float _x, float _y, float _z, float _w);
		/// Lets you set new members' values.
		void set(const Vector4& _vec);

		/// Adds given vector to the current one.
		void add(const Vector4& _vec);
		/// Adds given vector to the current one.
		void add(Real _x, Real _y, Real _z, Real _w);
		/// Adds real number to all components.
		void add(Real _num);

		/// Subtract given vector from the current one.
		void subtract(const Vector4& _vec);

		/** Vector by scalar multiplication.
			@remarks
				Each vector's member is multiplied by a given
				number.
		*/
		void multiplyByNumber(const Real& _num);
		/** Vector by scalar divide.
			@remarks
				Each vector's member is divided by a given
				number.
		*/
		void divideByNumber(const Real& _num);

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
		friend Vector4 Maths::normalize(const Vector4& _vec);

		/** Negates current vector.
			@remarks
				The negated vector has the same length as it has
				before negating operation took place but all its
				members are opposite.
		*/
		void negate();

		/// Calculates the length (magnitude) of the vector.
		float getLength() const;
		/// Calculates the length (magnitude) of the vector.
		float length() const;

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
		float dotProduct(const Vector4& _vec) const;

		/** Multiplies vector by 4x4 matrix.
			@par
				This method multiplies matrix M and vector V in the following way:
				             |m11 m12 m13 m14|
				          \/ |m21 m22 m23 m24|
				|x y z w| /\ |m31 m32 m33 m34| = |result.x result.y result.z result.w|
				             |m41 m42 m43 m44|
		*/
		void multiply(const Matrix4x4& _mat);


		// Below are overloaded operators.


		Vector4& operator+=(const Vector4& rhs);
		Vector4& operator-=(const Vector4& rhs);
		Vector4& operator*=(const Real& rhs);
		Vector4& operator*=(const Matrix4x4& rhs);
		Vector4& operator/=(const Real& rhs);
		
		friend const Vector4 operator+(const Vector4& lhs, const Vector4& rhs);
		friend const Vector4 operator-(const Vector4& lhs, const Vector4& rhs);

		friend const Vector4 operator/(const Vector4& lhs, const Real& rhs);

		friend const Vector4 operator*(const Vector4& lhs, const Real& rhs);
		friend const Vector4 operator*(const Real& lhs, const Vector4& rhs);

		friend const float operator%(const Vector4& lhs, const Vector4& rhs);

		const Vector4 operator-();

		bool operator==(const Vector4& _vec);
		friend bool operator==(const Vector4& _vec1, const Vector4& _vec2);

		bool operator!=(const Vector4& _vec);
		friend bool operator!=(const Vector4& _vec1, const Vector4& _vec2);

		Real operator[](uint i);
		Real operator[](uint i) const;

		/// Returns array of data.
		Real* getData();

		/** Creates Vector4 from string.
			@param
				_string keeping data of vector. Three formats are accepted:
				"x y z w", "x;y;z;w" and "x,y,z,w".
		*/
		void fromString(const string& _string);
		/// Writes vector to string as "x y z w".
		string toString();
	};



	inline Vector4& Vector4::operator=(const Vector4& _vec)
	{
		x = _vec.x;
		y = _vec.y;
		z = _vec.z;
		w = _vec.w;

		return (*this);
	}
//----------------------------------------------------------------------
	inline void Vector4::set(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
//----------------------------------------------------------------------
	inline void Vector4::set(const Vector4& _vec)
	{
		x = _vec.x;
		y = _vec.y;
		z = _vec.z;
		w = _vec.w;
	}
//----------------------------------------------------------------------
	inline void Vector4::add(const Vector4& _vec)
	{
		x += _vec.x;
		y += _vec.y;
		z += _vec.z;
		w += _vec.w;
	}
//----------------------------------------------------------------------
	inline void Vector4::add(Real _x, Real _y, Real _z, Real _w)
	{
		x += _x;
		y += _y;
		z += _z;
		w += _w;
	}
//----------------------------------------------------------------------
	inline void Vector4::add(Real _num)
	{
		x += _num;
		y += _num;
		z += _num;
		w += _num;
	}
//----------------------------------------------------------------------
	inline void Vector4::subtract(const Vector4& _vec)
	{
		x -= _vec.x;
		y -= _vec.y;
		z -= _vec.z;
		w -= _vec.w;
	}
//----------------------------------------------------------------------
	inline void Vector4::multiplyByNumber(const Real& _num)
	{
		x *= _num;
		y *= _num;
		z *= _num;
		w *= _num;
	}
//----------------------------------------------------------------------
	inline void Vector4::divideByNumber(const Real& _num)
	{
		x /= _num;
		y /= _num;
		z /= _num;
		w /= _num;
	}
//----------------------------------------------------------------------
	inline void Vector4::normalize()
	{
		float length = Maths::fsqrt((x * x) + (y * y) + (z * z) + (w * w));
		if(!Maths::floatCompare(0.0f, length))
			return;

		if(Maths::isUsingSSE())
		{
			__asm
			{
				mov		esi, this
				movups	xmm0, [esi]
				movaps	xmm2, xmm0
				mulps	xmm0, xmm0
				movaps	xmm1, xmm0
				shufps	xmm1, xmm1, 4Eh
				addps	xmm0, xmm1
				movaps	xmm1, xmm0
				shufps	xmm1, xmm1, 11h
				addps	xmm0, xmm1

				rsqrtps	xmm0, xmm0
				mulps	xmm2, xmm0
				movups	[esi], xmm2
			}

			// These conditions make it running slower (still faster than non-SSE
			// way), but the results are more precise.
			if(1.0f - x < 0.00025f)
				x = 1.0f;
			if(1.0f - y < 0.00025f)
				y = 1.0f;
			if(1.0f - z < 0.00025f)
				z = 1.0f;
			if(1.0f - w < 0.00025f)
				w = 1.0f;
		}
		else
		{
			float inv = 1.0f / length;
			x *= inv;
			y *= inv;
			z *= inv;
			w *= inv;
		}
	}
//----------------------------------------------------------------------
	inline void Vector4::negate()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}
//----------------------------------------------------------------------
	inline float Vector4::getLength() const
	{
		float length;

		if(Maths::isUsingSSE())
		{
			float* pf = &length;

			__asm
			{
				mov		ecx, pf
				mov		esi, this
				movups	xmm0, [esi]
				mulps	xmm0, xmm0
				movaps	xmm1, xmm0
				shufps	xmm1, xmm1, 4Eh
				addps	xmm0, xmm1
				movaps	xmm1, xmm0
				shufps	xmm1, xmm1, 11h
				addps	xmm0, xmm1
				sqrtss	xmm0, xmm0
				movss	[ecx], xmm0
			}
		}
		else
			length = Maths::fsqrt(x * x + y * y + z * z + w * w);

		return length;
	}
//----------------------------------------------------------------------
	inline float Vector4::length() const
	{
		return getLength();
	}
//----------------------------------------------------------------------
	inline float Vector4::dotProduct(const Vector4& vec) const
	{
		return (x * vec.x + y * vec.y + z * vec.z + w * vec.w);
	}
//----------------------------------------------------------------------
	inline Vector4 Maths::normalize(const Vector4& _vec)
	{
		Real length = _vec.getLength();
		if(!Maths::floatCompare(0.0f, length))
			return Vector4(_vec);

		Vector4 result = _vec;

		if(Maths::isUsingSSE())
		{
			Vector4* r = &result;
			__asm
			{
				mov		esi, r
				movups	xmm0, [esi]
				movaps	xmm2, xmm0
				mulps	xmm0, xmm0
				movaps	xmm1, xmm0
				shufps	xmm1, xmm1, 4Eh
				addps	xmm0, xmm1
				movaps	xmm1, xmm0
				shufps	xmm1, xmm1, 11h
				addps	xmm0, xmm1

				rsqrtps	xmm0, xmm0
				mulps	xmm2, xmm0
				movups	[esi], xmm2
			}

			// These conditions make it running slower (still faster than non-SSE
			// way), but the results are more precise.
			if(1.0f - result.x<0.00025f)
				result.x = 1.0f;
			if(1.0f - result.y < 0.00025f)
				result.y = 1.0f;
			if(1.0f - result.z < 0.00025f)
				result.z = 1.0f;
			if(1.0f - result.w < 0.00025f)
				result.w = 1.0f;
		}
		else
		{
			float inv = 1.0f / length;
			result.x *= inv;
			result.y *= inv;
			result.z *= inv;
		}

		return result;
	}
//----------------------------------------------------------------------
	inline Vector4& Vector4::operator+=(const Vector4& rhs)
	{
		add(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector4& Vector4::operator-=(const Vector4& rhs)
	{
		subtract(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector4& Vector4::operator*=(const Real& rhs)
	{
		multiplyByNumber(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector4& Vector4::operator/=(const Real& rhs)
	{
		divideByNumber(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline const Vector4 Vector4::operator-()
	{
		Vector4 newVector(*this);
		newVector.negate();

		return (newVector);
	}
//----------------------------------------------------------------------
	inline bool Vector4::operator==(const Vector4& _vec)
	{
		return (!Maths::floatCompare(x, _vec.x) &&
				!Maths::floatCompare(y, _vec.y) &&
				!Maths::floatCompare(z, _vec.z) &&
				!Maths::floatCompare(w, _vec.w));
	}
//----------------------------------------------------------------------
	inline bool Vector4::operator!=(const Vector4& _vec)
	{
		return (Maths::floatCompare(x, _vec.x) ||
				Maths::floatCompare(y, _vec.y) ||
				Maths::floatCompare(z, _vec.z) ||
				Maths::floatCompare(w, _vec.w));
	}
//----------------------------------------------------------------------
	inline const Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x + rhs.x,
					   lhs.y + rhs.y,
					   lhs.z + rhs.z,
					   lhs.w + rhs.w);
	}
//----------------------------------------------------------------------
	inline const Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x - rhs.x,
					   lhs.y - rhs.y,
					   lhs.z - rhs.z,
					   lhs.w - rhs.w);
	}
//----------------------------------------------------------------------
	inline const Vector4 operator/(const Vector4& lhs, const Real& rhs)
	{
		Vector4 result = lhs;
		result.divideByNumber(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Vector4 operator*(const Vector4& lhs, const Real& rhs)
	{
		Vector4 result = lhs;
		result.multiplyByNumber(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Vector4 operator*(const Real& lhs, const Vector4& rhs)
	{
		Vector4 result = rhs;
		result.multiplyByNumber(lhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const float operator%(const Vector4& lhs, const Vector4& rhs)
	{
		return (lhs.dotProduct(rhs));
	}
//----------------------------------------------------------------------
	inline bool operator==(const Vector4& _vec1, const Vector4& _vec2)
	{
		return (!Maths::floatCompare(_vec1.x, _vec2.x) &&
				!Maths::floatCompare(_vec1.y, _vec2.y) &&
				!Maths::floatCompare(_vec1.z, _vec2.z) &&
				!Maths::floatCompare(_vec1.w, _vec2.w));
	}
//----------------------------------------------------------------------
	inline bool operator!=(const Vector4& _vec1, const Vector4& _vec2)
	{
		return (Maths::floatCompare(_vec1.x, _vec2.x) ||
				Maths::floatCompare(_vec1.y, _vec2.y) ||
				Maths::floatCompare(_vec1.z, _vec2.z) ||
				Maths::floatCompare(_vec1.w, _vec2.w));
	}
//----------------------------------------------------------------------
	inline Real Vector4::operator[](uint i)
	{
		Assert(i < 4, "Vector dimension index not allowed");
		return (vals[i]);
	}
//----------------------------------------------------------------------
	inline Real Vector4::operator[](uint i) const
	{
		Assert(i < 4, "Vector dimension index not allowed");
		return (vals[i]);
	}
//----------------------------------------------------------------------
	inline Real* Vector4::getData()
	{
		return &vals[0];
	}
//----------------------------------------------------------------------
}


#endif