/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Vector3.h
Version:	0.11
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VECTOR3_H_
#define __INC_VECTOR3_H_


#include "Engine.h"
#include "Maths.h"


namespace nGENE
{
	/** 3-dimensional vector class.
		@par
			Although many overloaded operators are available for clients
			convenience, in sake of efficiency it is better not to use
			them as some of them (eg. +, -, *) result in temporary object
			creation. It is better to use member functions instead.
			They provide the same functionality but are much faster due
			to lack of aforementioned drawback.
		@par
			Vector3 class takes advantage of SIMD extensions if they are
			available on the platform the engine is being run on. This can
			greatly speed up complex calculations.
	*/
	class nGENEDLL Vector3
	{
	public:
		union {
			struct {
				Real x;
				Real y;
				Real z;
			};
			Real vals[3];
		};

		// Some static members. They make up the standard base
		// in euclidean 3-dimensional space. Also negated version
		// of them is provided.
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;

		static const Vector3 ZERO_VECTOR;
		static const Vector3 ONE_VECTOR;

		static const Vector3 INFINITY_VECTOR;
		static const Vector3 NEGATIVE_INFINITY_VECTOR;

	public:
		Vector3();
		explicit Vector3(Real _x, Real _y, Real _z);
		explicit Vector3(const Vector2& _vec);
		explicit Vector3(const Vector2& _vec, Real _z);
		~Vector3();

		Vector3& operator=(const Vector3& _vec);

		/// Lets you set new members' values.
		void set(Real _x, Real _y, Real _z);
		/// Lets you set new members' values.
		void set(const Vector3& _vec);

		/// Adds given vector to the current one.
		void add(const Vector3& _vec);
		/// Adds given vector to the current one.
		void add(Real _x, Real _y, Real _z);
		/// Adds real number to all components.
		void add(Real _num);

		/// Subtract given vector from the current one.
		void subtract(const Vector3& _vec);

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
		friend Vector3 Maths::normalize(const Vector3& _vec);

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
				Real dot product value
		*/
		Real dotProduct(const Vector3& vec) const;

		/** Calculates cross product of the current vector and the
			vector given.
			@return
				Vector3 a vector which is perpendicular to the vectors
				given.
		*/
		Vector3 crossProduct(const Vector3& vec) const;

		/// Deflects a vector against given plane.
		Vector3 deflect(const Plane& _pl);

		/// Reflects a vector against given plane.
		Vector3 reflect(const Plane& _pl);

		/** Multiplies vector by 4x4 matrix.
			@par
				This method multiplies matrix M and vector V in the following way:
				             |m11 m12 m13 m14|
				          \/ |m21 m22 m23 m24|
				|x y z w| /\ |m31 m32 m33 m34| = |result.x result.y result.z result.w|
				             |m41 m42 m43 m44|
		*/
		void multiply(const Matrix3x3& _mat);


		// Below are overloaded operators.


		Vector3& operator+=(const Vector3& rhs);
		Vector3& operator-=(const Vector3& rhs);
		Vector3& operator*=(const Matrix3x3& rhs);
		Vector3& operator*=(const Real& rhs);
		Vector3& operator/=(const Real& rhs);
		
		friend const Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
		friend const Vector3 operator-(const Vector3& lhs, const Vector3& rhs);

		friend const Vector3 operator/(const Vector3& lhs, const Real& rhs);

		friend const Vector3 operator*(const Vector3& lhs, const Real& rhs);
		friend const Vector3 operator*(const Real& lhs, const Vector3& rhs);
		friend const Vector3 operator*(const Vector3& lhs, const Vector3& rhs);

		friend const Real operator%(const Vector3& lhs, const Vector3& rhs);

		const Vector3 operator-();

		bool operator==(const Vector3& _vec);
		friend bool operator==(const Vector3& _vec1, const Vector3& _vec2);

		bool operator!=(const Vector3& _vec);
		friend bool operator!=(const Vector3& _vec1, const Vector3& _vec2);


		Real operator[](uint i);
		Real operator[](uint i) const;


		/// Returns array of data.
		Real* getData();

		/** Creates Vector3 from string.
			@param
				_string keeping data of vector. Three formats are accepted:
				"x y z", "x;y;z" and "x,y,z".
		*/
		void fromString(const string& _string);
		/// Writes vector to string as "x y z".
		string toString();
	};



	inline Vector3& Vector3::operator=(const Vector3& _vec)
	{
		x = _vec.x;
		y = _vec.y;
		z = _vec.z;

		return (*this);
	}
//----------------------------------------------------------------------
	inline void Vector3::set(Real _x, Real _y, Real _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
//----------------------------------------------------------------------
	inline void Vector3::set(const Vector3& _vec)
	{
		x = _vec.x;
		y = _vec.y;
		z = _vec.z;
	}
//----------------------------------------------------------------------
	inline void Vector3::add(const Vector3& _vec)
	{
		x += _vec.x;
		y += _vec.y;
		z += _vec.z;
	}
//----------------------------------------------------------------------
	inline void Vector3::add(Real _x, Real _y, Real _z)
	{
		x += _x;
		y += _y;
		z += _z;
	}
//----------------------------------------------------------------------
	inline void Vector3::add(Real _num)
	{
		x += _num;
		y += _num;
		z += _num;
	}
//----------------------------------------------------------------------
	inline void Vector3::subtract(const Vector3& _vec)
	{
		x -= _vec.x;
		y -= _vec.y;
		z -= _vec.z;
	}
//----------------------------------------------------------------------
	inline void Vector3::multiplyByNumber(const Real& _num)
	{
		x *= _num;
		y *= _num;
		z *= _num;
	}
//----------------------------------------------------------------------
	inline void Vector3::divideByNumber(const Real& _num)
	{
		x /= _num;
		y /= _num;
		z /= _num;
	}
//----------------------------------------------------------------------
	inline void Vector3::normalize()
	{
		Real length = Maths::fsqrt((x * x) + (y * y) + (z * z));
		if(!Maths::floatCompare(0.0f, length))
			return;

		if(Maths::isUsingSSE())
		{
			__asm
			{
				mov		esi, this
				push	eax
				mov		eax, 0
				xchg	[esi+12], eax
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

				xchg	[esi+12], eax
				pop		eax
			}

			// These conditions make it running slower (still faster than non-SSE
			// way), but the results are more precise.
			if(1.0f - x < 0.00025f)
				x = 1.0f;
			if(1.0f - y < 0.00025f)
				y = 1.0f;
			if(1.0f - z < 0.00025f)
				z = 1.0f;
		}
		else
		{
			Real inv = 1.0f / length;
			x *= inv;
			y *= inv;
			z *= inv;
		}
	}
//----------------------------------------------------------------------
	inline void Vector3::negate()
	{
		x = -x;
		y = -y;
		z = -z;
	}
//----------------------------------------------------------------------
	inline Real Vector3::getLength() const
	{
		Real length;

		if(Maths::isUsingSSE())
		{
			Real* pf = &length;

			__asm
			{
				mov		ecx, pf
				mov		esi, this
				push	eax
				mov		eax, 0
				xchg	[esi+12], eax
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

				xchg	[esi+12], eax
				pop		eax
			}
		}
		else
			length = Maths::fsqrt(x*x+y*y+z*z);

		return length;
	}
//----------------------------------------------------------------------
	inline Real Vector3::length() const
	{
		return getLength();
	}
//----------------------------------------------------------------------
	inline Real Vector3::dotProduct(const Vector3& vec) const
	{
		return (x * vec.x + y * vec.y + z * vec.z);
	}
//----------------------------------------------------------------------
	inline Vector3 Vector3::crossProduct(const Vector3& vec) const
	{
		Vector3 res;

		if(Maths::isUsingSSE())
		{
			// @todo
			// It contains quite a number of errors. Look into it.
			// More less it's okay - but SIMD should be used for Vector4 only.
			/*Real vec1[4] = {x, y, z, 1.0f};
			Real vec2[4] = {vec.x, vec.y, vec.z, 1.0f};
			Real* r = new Real[4];
			Real* v1= &vec1[0];
			Real* v2= &vec2[0];
			__asm
			{
				mov		esi, vec1
				mov		edi, vec2

				push	eax
				push	ecx

				movups	xmm0, [esi]
				movups	xmm1, [edi]
				movaps	xmm2, xmm0
				movaps	xmm3, xmm1

				shufps	xmm0, xmm0, 0xc9
				shufps	xmm1, xmm1, 0xd2
				mulps	xmm0, xmm1

				shufps	xmm2, xmm2, 0xd2
				shufps	xmm3, xmm3, 0xc9
				mulps	xmm2, xmm3

				subps	xmm0, xmm2

				mov		esi, r
				movups	[esi], xmm0
			}

			res.set(r[0], r[1], r[2]);
			delete[] r;*/
		}
		//else
		{
			res.x = (y * vec.z) - (z * vec.y);
			res.y = (z * vec.x) - (x * vec.z);
			res.z = (x * vec.y) - (y * vec.x);
		}

		return res;
	}
//----------------------------------------------------------------------
	inline Vector3 Maths::normalize(const Vector3& _vec)
	{
		Real length = _vec.getLength();
		if(!Maths::floatCompare(0.0f, length))
			return Vector3(_vec);

		Vector3 result = _vec;

		if(Maths::isUsingSSE())
		{
			Vector3* r = &result;
			__asm
			{
				mov		esi, r
				push	eax
				mov		eax, 0
				xchg	[esi+12], eax
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

				xchg	[esi+12], eax
				pop		eax
			}

			// These conditions make it running slower (still faster than non-SSE
			// way), but the results are more precise.
			if(1.0f - result.x < 0.00025f)
				result.x = 1.0f;
			if(1.0f - result.y < 0.00025f)
				result.y = 1.0f;
			if(1.0f - result.z < 0.00025f)
				result.z = 1.0f;
		}
		else
		{
			Real inv = 1.0f / length;
			result.x *= inv;
			result.y *= inv;
			result.z *= inv;
		}

		return result;
	}
//----------------------------------------------------------------------
	inline Vector3& Vector3::operator+=(const Vector3& rhs)
	{
		add(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector3& Vector3::operator-=(const Vector3& rhs)
	{
		subtract(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector3& Vector3::operator*=(const Real& rhs)
	{
		multiplyByNumber(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector3& Vector3::operator/=(const Real& rhs)
	{
		divideByNumber(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline const Vector3 Vector3::operator-()
	{
		Vector3 newVector(*this);
		newVector.negate();

		return (newVector);
	}
//----------------------------------------------------------------------
	inline bool Vector3::operator==(const Vector3& _vec)
	{
		return (!Maths::floatCompare(x, _vec.x) &&
				!Maths::floatCompare(y, _vec.y) &&
				!Maths::floatCompare(z, _vec.z));
	}
//----------------------------------------------------------------------
	inline bool Vector3::operator!=(const Vector3& _vec)
	{
		return (Maths::floatCompare(x, _vec.x) ||
				Maths::floatCompare(y, _vec.y) ||
				Maths::floatCompare(z, _vec.z));
	}
//----------------------------------------------------------------------
	inline const Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.x + rhs.x,
					   lhs.y + rhs.y,
					   lhs.z + rhs.z);
	}
//----------------------------------------------------------------------
	inline const Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.x - rhs.x,
					   lhs.y - rhs.y,
					   lhs.z - rhs.z);
	}
//----------------------------------------------------------------------
	inline const Vector3 operator/(const Vector3& lhs, const Real& rhs)
	{
		Vector3 result = lhs;
		result.divideByNumber(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Vector3 operator*(const Vector3& lhs, const Real& rhs)
	{
		Vector3 result = lhs;
		result.multiplyByNumber(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Vector3 operator*(const Real& lhs, const Vector3& rhs)
	{
		Vector3 result = rhs;
		result.multiplyByNumber(lhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Vector3 operator*(const Vector3& lhs, const Vector3& rhs)
	{
		return (lhs.crossProduct(rhs));
	}
//----------------------------------------------------------------------
	inline const Real operator%(const Vector3& lhs, const Vector3& rhs)
	{
		return (lhs.dotProduct(rhs));
	}
//----------------------------------------------------------------------
	inline bool operator==(const Vector3& _vec1, const Vector3& _vec2)
	{
		return (!Maths::floatCompare(_vec1.x, _vec2.x) &&
				!Maths::floatCompare(_vec1.y, _vec2.y) &&
				!Maths::floatCompare(_vec1.z, _vec2.z));
	}
//----------------------------------------------------------------------
	inline bool operator!=(const Vector3& _vec1, const Vector3& _vec2)
	{
		return (Maths::floatCompare(_vec1.x, _vec2.x) ||
				Maths::floatCompare(_vec1.y, _vec2.y) ||
				Maths::floatCompare(_vec1.z, _vec2.z));
	}
//----------------------------------------------------------------------
	inline Real* Vector3::getData()
	{
		return &vals[0];
	}
//----------------------------------------------------------------------
	inline Real Vector3::operator[](uint i)
	{
		Assert(i < 3, "Vector dimension index not allowed");
		return (vals[i]);
	}
//----------------------------------------------------------------------
	inline Real Vector3::operator[](uint i) const
	{
		Assert(i < 3, "Vector dimension index not allowed");
		return (vals[i]);
	}
//----------------------------------------------------------------------
}


#endif