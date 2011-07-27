/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Quaternion.h
Version:	0.12
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_QUATERNION_H_
#define __INC_QUATERNION_H_


#include "Engine.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Quaternion is a class useful when smooth rotations are required
		as in this case gimbal lock does not occur.
		@remarks
			From Mathematics point of view, a quaternion is more generalized
			form of complex number having interpretation in 4D space as axis
			and rotation around it.
	*/
	class nGENEDLL Quaternion
	{
	public:
		union {
			struct {
				Real x;
				Real y;
				Real z;
				Real w;
			};
			Real q[4];
		};

	public:
		Quaternion(Real _x=0.0f, Real _y=0.0f, Real _z=0.0f, Real _w=1.0f);
		explicit Quaternion(const Vector3& _axis, const Real _angle);
		~Quaternion();

		Quaternion& operator=(const Quaternion& _quat);

		/// Sets quaternion value.
		void set(Real _x, Real _y, Real _z, Real _w);
		/// Sets quaternion value.
		void set(const Vector3& _axis, const Real _angle);
		/// Sets quaternion value from Euler angles.
		void set(Real _yaw, Real _pitch, Real _roll);

		/** Builds identity quaternion.
			@remarks
				Identity quaternion is a quaternion with x, y, z (complex part)
				being equal to 0.0 and w (real part) to 1.0.
		*/
		void identity();

		/** Conjugates the quaternion.
			@remarks
				Conjugated quaternion has x, y, z negated, whereas w stays the
				same.
		*/
		void conjugate();
		/** Conjugates the quaternion.
			@remarks
			Conjugated quaternion has x, y, z negated, whereas w stays the
			same.
		*/
		friend Quaternion conjugate(const Quaternion& _quat);

		/// Returns the length of the quaternion.
		Real getLength() const;
		/// Returns the length of the quaternion.
		Real length() const;

		/** Normalizes the quaternion (computes an unit length quaternion).
		*/
		void normalize();
		
		/** Calculates inverse quaternion.
			@remarks
				Quaternion has to be of non-zero length.
		*/
		void invert();

		/// Multiplies quaternion by another quaternion.
		void multiply(const Quaternion& _quat);
		/// Multiplies quaternion by 3D Vector resulting in a 3D Vector.
		Vector3 multiply(const Vector3& _vec) const;

		/** Interpolates between two quaternions using spherical linear interpolation	.
			@param
				_quat1 source quaternion.
			@param
				_quat2 destination quaternion.
			@param
				_t indicates how far to interpolate between given quaternions.
				It ranges 0 to 1, where 0 indicates that the interpolation
				result equals to source quaternion and 1 - that the results
				is equal to the destination quaternion.
		*/
		friend Quaternion slerp(const Quaternion& _quat1, const Quaternion& _quat2, Real _t);

		/** Converts the quaternion to Matrix4x4.
			@par
				This way you actually extract rotation matrix from the
				quaternion.
		*/
		Matrix4x4 toMatrix() const;
		/** Builds quaternion from rotation matrix.
			@par
				Based on the algorithm described in Ken Shoemake's article
				from SIGGRAPH 1987: "Quaternion Calculus and Fast Animation".
		*/
		void fromMatrix(const Matrix4x4& _mat);

		/// Builds quaternion from Euler angles.
		void fromEulerAngles(Real _yaw, Real _pitch, Real _roll);
		/// Builds quaternion from Euler angles.
		void fromEulerAngles(const Vector3& _angles);

		/// Convert the quaternion to Euler representation.
		Vector3 toEuler() const;


		/** Finds a quaternion rotating one vector onto another.
			@param _vec1 first vector. Can be of arbitrary length.
			@param _vec2 first vector. Can be of arbitrary length.
			@returns
				Quaternion resulting quaternion. The result will be different
				if _vec1 and _vec2 were switched. The result is not necessarily
				an unit quaternion.
			@remarks
				As there might be more than one quaternion transforming one vector
				onto another this function finds only one of the solutions -
				rotation by the shortest arc.
		*/
		friend Quaternion quaternionBetweenVectors(const Vector3& _vec1, const Vector3& _vec2);
		/** Finds a quaternion rotating one vector onto another.
			@param _vec1 first vector. Can be of arbitrary length.
			@param _vec2 first vector. Can be of arbitrary length.
			@returns
				Quaternion resulting quaternion. The result will be different
				if _vec1 and _vec2 were switched. The result is an unit quaternion.
			@remarks
				As there might be more than one quaternion transforming one vector
				onto another this function finds only one of the solutions -
				rotation by the shortest arc.
			*/
		friend Quaternion quaternionBetweenVectorsNormalized(const Vector3& _vec1, const Vector3& _vec2);


		// Below are overloaded operators.


		Quaternion& operator*=(const Quaternion& _quat);
		
		Vector3 operator*(const Vector3& _vec) const;
		friend const Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);


		bool Quaternion::operator==(const Quaternion& _quat);
		bool Quaternion::operator!=(const Quaternion& _quat);

		Real operator[](uint i);
		Real operator[](uint i) const;


		/** Creates Quaternion from string.
			@param
				_string keeping data of quaternion. Three formats are accepted:
				"x y z w", "x;y;z;w" and "x,y,z,w".
		*/
		void fromString(const string& _string);
		/// Writes quaternion to string as "x y z w".
		string toString();
	};



	inline Quaternion& Quaternion::operator=(const Quaternion& _quat)
	{
		x = _quat.x;
		y = _quat.y;
		z = _quat.z;
		w = _quat.w;

		return (*this);
	}
//----------------------------------------------------------------------
	inline void Quaternion::set(Real _x, Real _y, Real _z, Real _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}
//----------------------------------------------------------------------
	inline void Quaternion::set(const Vector3& _axis, const Real _angle)
	{
		Real halfAngle = _angle * 0.5f;
		x = Maths::fsin(halfAngle) * _axis.x;
		y = Maths::fsin(halfAngle) * _axis.y;
		z = Maths::fsin(halfAngle) * _axis.z;
		w = Maths::fcos(halfAngle);
	}
//----------------------------------------------------------------------
	inline void Quaternion::set(Real _yaw, Real _pitch, Real _roll)
	{
		Real halfYaw = _yaw * 0.5f;
		Real halfPitch = _pitch * 0.5f;
		Real halfRoll = _roll * 0.5f;

		Real sinHY = Maths::fsin(halfYaw);
		Real cosHY = Maths::fcos(halfYaw);

		Real sinHP = Maths::fsin(halfPitch);
		Real cosHP = Maths::fcos(halfPitch);

		Real sinHR = Maths::fsin(halfRoll);
		Real cosHR = Maths::fcos(halfRoll);

		x = cosHY * cosHP * sinHR - sinHY * sinHP * cosHR;
		y = cosHY * sinHP * cosHR + sinHY * cosHP * sinHR;
		z = sinHY * cosHP * cosHR - cosHY * sinHP * sinHR;
		w = cosHY * cosHP * cosHR + sinHY * sinHP * sinHR;
	}
//----------------------------------------------------------------------
	inline void Quaternion::identity()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}
//----------------------------------------------------------------------
	inline void Quaternion::conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}
//----------------------------------------------------------------------
	inline Quaternion conjugate(const Quaternion& _quat)
	{
		Quaternion result = _quat;
		result.conjugate();

		return result;
	}
//----------------------------------------------------------------------
	inline Real Quaternion::getLength() const
	{
		return (Maths::fsqrt(x * x + y * y + z * z + w * w));
	}
//----------------------------------------------------------------------
	inline Real Quaternion::length() const
	{
		return (Maths::fsqrt(x * x + y * y + z * z + w * w));
	}
//----------------------------------------------------------------------
	inline void Quaternion::normalize()
	{
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
			Real inv = 1.0f / length();
			x *= inv;
			y *= inv;
			z *= inv;
			w *= inv;
		}
	}
//----------------------------------------------------------------------
	inline void Quaternion::invert()
	{
		Real len = length();

		// Zero-length quaternion cannot be inverted
		if(len == 0.0f)
			return;

		Quaternion conj = (*this);
		conj.conjugate();
		Real invSqrLen = 1.0f / len * len;

		(*this) = conj * invSqrLen;
	}
//----------------------------------------------------------------------
	inline void Quaternion::multiply(const Quaternion& _quat)
	{
		Real X, Y, Z, W;

		X = w * _quat.x + x * _quat.w + y * _quat.z - z * _quat.y;
		Y = w * _quat.y + y * _quat.w + z * _quat.x - x * _quat.z;
		Z = w * _quat.z + z * _quat.w + x * _quat.y - y * _quat.x;
		W = w * _quat.w - x * _quat.x - y * _quat.y - z * _quat.z;

		(*this) = Quaternion(X, Y, Z, W);
	}
//----------------------------------------------------------------------
	inline Vector3 Quaternion::multiply(const Vector3& _vec) const
	{
		// nVidia SDK implementation
		Vector3 uv, uuv;
		Vector3 qvec(x, y, z);
		uv = qvec.crossProduct(_vec);
		uuv = qvec.crossProduct(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;

		return (_vec + uv + uuv);
	}
//----------------------------------------------------------------------
	inline Quaternion quaternionBetweenVectorsNormalized(const Vector3& _vec1, const Vector3& _vec2)
	{
		Vector3 axis = _vec1 * _vec2;
		axis.normalize();
		float angle = acosf((_vec1 % _vec2) / (_vec1.length() * _vec2.length()));
		
		Quaternion quat(axis, angle);
		quat.normalize();

		return quat;
	}
//----------------------------------------------------------------------
	inline Quaternion quaternionBetweenVectors(const Vector3& _vec1, const Vector3& _vec2)
	{
		Vector3 axis = _vec1 * _vec2;
		axis.normalize();
		float angle = acosf((_vec1 % _vec2) / (_vec1.length() * _vec2.length()));

		return Quaternion(axis, angle);
	}
//----------------------------------------------------------------------
	inline Quaternion& Quaternion::operator*=(const Quaternion& _quat)
	{
		multiply(_quat);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector3 Quaternion::operator*(const Vector3& _vec) const
	{
		return (multiply(_vec));
	}
//----------------------------------------------------------------------
	inline const Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
	{
		Quaternion result=lhs;
		result *= rhs;

		return result;
	}
//----------------------------------------------------------------------
	inline bool Quaternion::operator==(const Quaternion& _quat)
	{
		return ((x == _quat.x) &&
				(y == _quat.y) &&
				(z == _quat.z) &&
				(w == _quat.w));
	}
//----------------------------------------------------------------------
	inline bool Quaternion::operator!=(const Quaternion& _quat)
	{
		return ((x != _quat.x) ||
				(y != _quat.y) ||
				(z != _quat.z) ||
				(w != _quat.w));
	}
//----------------------------------------------------------------------
	inline Quaternion slerp(const Quaternion& _quat1, const Quaternion& _quat2, Real _t)
	{
		Quaternion quat1 = _quat1;
		Quaternion quat2 = _quat2;

		float dot = quat1.x * quat2.x +
					quat1.y * quat2.y +
					quat1.z * quat2.z +
					quat1.w * quat2.w;

		if(dot < 0.0f)
		{
			quat2.conjugate();
			quat2.w = -quat2.w;
			dot = FastFloat::fabs(dot);
		}

		float factor1, factor2;
		if(dot > 0.9999f)
		{
			// Too close - we apply linear interpolation
			factor1 = 1.0f - _t;
			factor2 = _t;
		}
		else
		{
			float sin = sqrt(1.0f - dot * dot);
			float omega = atan2(sin, dot);

			float oneOverSin = 1.0f / sin;

			factor1 = sinf((1.0f - _t) * omega) * oneOverSin;
			factor2 = sinf(_t * omega) * oneOverSin;
		}

		Quaternion result(quat1.x * factor1 + quat2.x * factor2,
						  quat1.y * factor1 + quat2.y * factor2,
						  quat1.z * factor1 + quat2.z * factor2,
						  quat1.w * factor1 + quat2.w * factor2);

		return result;
	}
//----------------------------------------------------------------------
	inline Vector3 Quaternion::toEuler() const
	{		
		float sqw = w * w;
		float sqx = x * x;
		float sqy = y * y;
		float sqz = z * z;

		float rotx = atan2f(2.0f * (y * z + x * w), (-sqx - sqy + sqz + sqw));
		float roty = asinf(-2.0f * (x * z - y * w));
		float rotz = atan2f(2.0f * (x * y + z * w), (sqx - sqy - sqz + sqw));
		
		return Vector3(rotx, roty, rotz);
	}
//----------------------------------------------------------------------
	inline Real Quaternion::operator[](uint i)
	{
		Assert(i < 4, "Quaternion dimension index not allowed");
		return (q[i]);
	}
//----------------------------------------------------------------------
	inline Real Quaternion::operator[](uint i) const
	{
		Assert(i < 4, "Quaternion dimension index not allowed");
		return (q[i]);
	}
//----------------------------------------------------------------------
}


#endif