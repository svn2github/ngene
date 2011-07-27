/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Matrix3x3.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MATRIX3X3_H_
#define __INC_MATRIX3X3_H_


#include "Log.h"
#include "Maths.h"
#include "Vector3.h"
#include "Vector4.h"


namespace nGENE
{
	/** 4x4 matrix.
		@par
			Matrix is a mathematical object especially useful in linear
			transformations in vector spaces (eg. 2- or 3-dimensional
			euclidean space). Each vector of a space (point is also a
			vector) has to be simply multiplied by the transformation
			matrix to make transformation take place.
		@par
			Although many overloaded operators are available for clients
			convenience, in sake of efficiency it is better not to use
			them as many of them result in temporary object creation.
			It is better to use member functions instead. They provide
			the same functionality but are much faster due to lack of
			aforementioned drawback.
	*/
	class nGENEDLL Matrix3x3
	{
	private:
		/// Helper function used for inverse matrix calculation.
		Matrix3x3 adjoint() const;

	public:
		/// Matrix's members
		union {
			struct {
				Real _m11, _m12, _m13;
				Real _m21, _m22, _m23;
				Real _m31, _m32, _m33;
			};
			Real _m[3][3];
			Real m[9];
		};

		static const Matrix3x3 IDENTITY;

	public:
		Matrix3x3();
		explicit Matrix3x3(float _m11, float _m12, float _m13,
						   float _m21, float _m22, float _m23,
						   float _m31, float _m32, float _m33);
		~Matrix3x3();

		/// Sets all members at once.
		void set(float _m11, float _m12, float _m13,
			     float _m21, float _m22, float _m23,
			     float _m31, float _m32, float _m33);

		/** Matrix is turned into identity matrix.
			@remarks
				Identity matrix is a special kind of matrix
				which has 1s on its diagonal and 0s elsewhere.
		*/
		void identity();

		/** Matrix is turned into zero matrix.
			@remarks
				Zero matrix consists of 0s only.
		*/
		void zero();

		/** All members are negated.
		*/
		void negate();

		/** Calculates inverse matrix.
			@remarks
				Inverse matrix to the given matrix is matrix
				which multiplied by given matrix produces the
				identity one. In this special case, multiplication
				is alternate.
		*/
		void invert();
		/** Calculates the matrix determinant.
			@remarks
				Determinant is a number telling many things about
				the matrix eg. if it is zero, the matrix cannot be
				inverted.
		*/
		Real determinant() const;
		/** Calculates determinant of the given minor.
			@remarks
				A minor is a square matrix constructed by leaving
				out some of the columns and rows of the base matrix.
			@remarks
				r0 through r1 are rows, and c0 to c1 - columns.
		*/
		Real minor(uint r0, uint r1, uint c0, uint c1) const;

		/** Transposes the matrix.
			@remarks
				The rows of the matrix become columns of the
				transposed matrix. Analogous for columns.
		*/
		void transpose();

		/** Adds given matrix to the current one.
			@remarks
				Unlike multiplication, matrix addition is alternate.
		*/
		void add(const Matrix3x3& _mat);
		friend Matrix3x3 add(const Matrix3x3& _mat1, const Matrix3x3& _mat2);

		void subtract(const Matrix3x3& _matrix);
		friend Matrix3x3 subtract(const Matrix3x3& _mat1, const Matrix3x3& _mat2);

		/** Multiplies current matrix by given matrix
			@remarks
				Note that in case of 4x4 matrices multiplication is
				always possible. However, order of performing this
				operation is not arbitrary.
			@todo
				Provide SSE optimized version.
		*/
		void multiply(const Matrix3x3& _matrix);
		/// Multiplies all matrix members by a given number.
		void multiply(const Real& _number);
		/** Multiplies matrix by 3D vector.
			@par
				This method multiplies matrix M and vector V in the following way:
				|m11 m12 m13|    |x|   |result.x|
				|m21 m22 m23| \/ |y| = |result.y|
				|m31 m32 m33| /\ |z|   |result.z|
		*/
		Vector3 multiply(const Vector3& _vec);
		friend Matrix3x3 multiply(const Matrix3x3& _mat1, const Matrix3x3& _mat2);

		/** Divides all cells by a real number.
			@param _number a divider - cannot be 0.
		*/
		void divide(const Real& _number);


		/// Builds rotation matrix around the x-axis.
		void rotateAroundXAxis(Real _angle);
		/// Builds rotation matrix around the y-axis.
		void rotateAroundYAxis(Real _angle);
		/// Builds rotation matrix around the z-axis.
		void rotateAroundZAxis(Real _angle);

		/** Builds a rotation matrix around specified arbitrary axis.
			@remarks
				Vector has to be normalized prior to calling this function.
			@param
				_axis axis to rotate around.
			@param
				_angle angle to rotate.
		*/
		void rotate(const Vector3& _axis, Real _angle);

		/** Builds a rotation matrix from Euler angles.
			@par
				This function build rotation matrix from Euler angles, namely
				yaw, pitch and roll.
			@param
				_euler Euler angles. x - stands for Pitch, y - for Roll,
				z - for	Yaw.
		*/
		void rotateYawPitchRoll(const Vector3& _euler);
		/** Builds a rotation matrix from Euler angles.
			@par
				This function build rotation matrix from Euler angles, namely
				yaw, pitch and roll.
			@param _yaw yaw
			@param _pitch pitch
			@param _roll roll
		*/
		void rotateYawPitchRoll(Real _yaw, Real _pitch, Real _roll);

		/// Builds non-uniform scaling matrix.
		void scale(const Vector3& _scale);
		/// Builds non-uniform scaling matrix.
		void scale(float _x, float _y, float _z);
		/// Builds uniform scaling matrix.
		void scale(float _s);

		/// Checks if matrix is an identity matrix.
		bool isIdentity();
		/// Checks if matrix contains only zeros.
		bool isZero();


		// Below are overloaded operators.


		Matrix3x3& operator+=(const Matrix3x3& rhs);
		Matrix3x3& operator-=(const Matrix3x3& rhs);
		Matrix3x3& operator*=(const Matrix3x3& rhs);
		Vector3 operator*=(const Vector3& rhs);
		Matrix3x3& operator*=(const Real& rhs);

		friend const Matrix3x3 operator+(const Matrix3x3& lhs, const Matrix3x3& rhs);
		friend const Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs);
		friend const Matrix3x3 operator*(const Matrix3x3& lhs, const float& rhs);
		friend const Matrix3x3 operator*(const float& lhs, const Matrix3x3& rhs);
		friend const Vector3   operator*(const Matrix3x3& lhs, const Vector3& rhs);
		friend const Vector3   operator*(const Vector3& lhs, const Matrix3x3& rhs);
		friend const Matrix3x3 operator-(const Matrix3x3& lhs, const Matrix3x3& rhs);

		const Matrix3x3 operator-();

		bool operator==(const Matrix3x3& _mat);

		Real* operator[](uint i);
		const Real* const operator[](uint i) const;

		/// Returns array of data.
		Real* getData();

		/** Creates Matrix3x3 from string.
			@param
				_string keeping data of matrix.
		*/
		void fromString(const string& _string);
		/// Writes Matrix3x3 to string as "_m11 _m12 ... _m44".
		string toString();
	};



	inline void Matrix3x3::set(float _m11, float _m12, float _m13,
						       float _m21, float _m22, float _m23,
						       float _m31, float _m32, float _m33)
	{
		_m[0][0] = _m11; _m[0][1] = _m12; _m[0][2] = _m13;
		_m[1][0] = _m21; _m[1][1] = _m22; _m[1][2] = _m23;
		_m[2][0] = _m31; _m[2][1] = _m32; _m[2][2] = _m33;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::identity()
	{
		float* f = &(this->_m[0][0]);
		memset(f, 0, sizeof(Matrix3x3));
		_m[0][0] = _m[1][1] = _m[2][2] = 1.0f;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::zero()
	{
		float* f = &(this->_m[0][0]);
		memset(f, 0, sizeof(Matrix3x3));
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::negate()
	{
		// Boring stuff... it is better however not to
		// use loop in this case as it will be slower
		_m[0][0] = -_m[0][0];
		_m[1][0] = -_m[1][0];
		_m[2][0] = -_m[2][0];

		_m[0][1] = -_m[0][1];
		_m[1][1] = -_m[1][1];
		_m[2][1] = -_m[2][1];

		_m[0][2] = -_m[0][2];
		_m[1][2] = -_m[1][2];
		_m[2][2] = -_m[2][2];
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::invert()
	{
		Real det = determinant();

		// If cannot be inverted, exit
		if(!det)
			return;

		Matrix3x3 adjoin = adjoint();
		adjoin.transpose();

		(*this) = adjoin * (1.0f / det);
	}
//----------------------------------------------------------------------
	inline Real Matrix3x3::determinant() const
	{
		return (_m11 * (_m22 * _m33 - _m23 * _m32) +
				_m12 * (_m23 * _m31 - _m21 * _m33) +
				_m13 * (_m21 * _m32 - _m22 * _m31));
	}
//----------------------------------------------------------------------
	inline Real Matrix3x3::minor(uint r0, uint r1, uint c0, uint c1) const
	{
		// We use Sarrus method here
		return (_m[r0][c0] * _m[r1][c1] - _m[r0][c1] * _m[r1][c0]);
	}
//----------------------------------------------------------------------
	inline Matrix3x3 Matrix3x3::adjoint() const
	{
		return Matrix3x3(minor(1, 2, 1, 2),
						 -minor(1, 2, 0, 2),
						 minor(1, 2, 0, 1),
						 
						 -minor(0, 2, 1, 2),
						 minor(0, 2, 0, 2),
						 -minor(0, 2, 0, 1),
						 
						 minor(0, 1, 1, 2),
						 -minor(0, 1, 0, 2),
						 minor(0, 1, 0, 1));
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::transpose()
	{
		Matrix3x3 mat= (*this);
		mat._m[0][0] = _m[0][0];
		mat._m[0][1] = _m[1][0];
		mat._m[0][2] = _m[2][0];

		mat._m[1][0] = _m[0][1];
		mat._m[1][1] = _m[1][1];
		mat._m[1][2] = _m[2][1];

		mat._m[2][0] = _m[0][2];
		mat._m[2][1] = _m[1][2];
		mat._m[2][2] = _m[2][2];

		(*this) = mat;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::add(const Matrix3x3& _mat)
	{
		_m11 += _mat._m11; _m12 += _mat._m12; _m13 += _mat._m13;
		_m21 += _mat._m21; _m22 += _mat._m22; _m23 += _mat._m23;
		_m31 += _mat._m31; _m32 += _mat._m32; _m33 += _mat._m33;
	}
//----------------------------------------------------------------------
	inline Matrix3x3 add(const Matrix3x3& _mat1, const Matrix3x3& _mat2)
	{
		Matrix3x3 result = _mat1;
		result.add(_mat2);

		return result;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::subtract(const Matrix3x3& _mat)
	{
		_m11 -= _mat._m11; _m12 -= _mat._m12; _m13 -= _mat._m13;
		_m21 -= _mat._m21; _m22 -= _mat._m22; _m23 -= _mat._m23;
		_m31 -= _mat._m31; _m32 -= _mat._m32; _m33 -= _mat._m33;
	}
//----------------------------------------------------------------------
	inline Matrix3x3 subtract(const Matrix3x3& _mat1, const Matrix3x3& _mat2)
	{
		Matrix3x3 result = _mat1;
		result.subtract(_mat2);

		return result;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::multiply(const Matrix3x3& _matrix)
	{
		Matrix3x3 mat;
		if(Maths::isUsingSSE())
		{
		}
		else
		{
		}

		mat[0][0] = _m[0][0] * _matrix[0][0] + _m[0][1] * _matrix[1][0] +
					_m[0][2] * _matrix[2][0];
		mat[0][1] = _m[0][0] * _matrix[0][1] + _m[0][1] * _matrix[1][1] +
					_m[0][2] * _matrix[2][1];
		mat[0][2] = _m[0][0] * _matrix[0][2] + _m[0][1] * _matrix[1][2] +
					_m[0][2] * _matrix[2][2];

		mat[1][0] = _m[1][0] * _matrix[0][0] + _m[1][1] * _matrix[1][0] +
					_m[1][2] * _matrix[2][0];
		mat[1][1] = _m[1][0] * _matrix[0][1] + _m[1][1] * _matrix[1][1] +
					_m[1][2] * _matrix[2][1];
		mat[1][2] = _m[1][0] * _matrix[0][2] + _m[1][1] * _matrix[1][2] +
					_m[1][2] * _matrix[2][2];

		mat[2][0] = _m[2][0] * _matrix[0][0] + _m[2][1] * _matrix[1][0] +
					_m[2][2] * _matrix[2][0];
		mat[2][1] = _m[2][0] * _matrix[0][1] + _m[2][1] * _matrix[1][1] +
					_m[2][2] * _matrix[2][1];
		mat[2][2] = _m[2][0] * _matrix[0][2] + _m[2][1] * _matrix[1][2] +
					_m[2][2] * _matrix[2][2];

		(*this) = mat;
	}
//---------------------------------------------------------------------
	inline void Matrix3x3::multiply(const Real& _number)
	{
		_m11 *= _number; _m12 *= _number; _m13 *= _number;
		_m21 *= _number; _m22 *= _number; _m23 *= _number;
		_m31 *= _number; _m32 *= _number; _m33 *= _number;
	}
//---------------------------------------------------------------------
	inline Vector3 Matrix3x3::multiply(const Vector3& _vec)
	{
		Vector3 vecResult;
		/// @todo Implement SSE version
		if(Maths::isUsingSSE())
		{
		}
		else
		{
		}

		vecResult.set(_m11 * _vec.x + _m12 * _vec.y + _m13 * _vec.z,
					  _m21 * _vec.x + _m22 * _vec.y + _m23 * _vec.z,
					  _m31 * _vec.x + _m32 * _vec.y + _m33 * _vec.z);

		return vecResult;
	}
//---------------------------------------------------------------------
	inline Matrix3x3 multiply(const Matrix3x3& _mat1, const Matrix3x3& _mat2)
	{
		Matrix3x3 temp = _mat1;
		temp.multiply(_mat2);
		return (temp);
	}
//---------------------------------------------------------------------
	inline void Matrix3x3::divide(const Real& _number)
	{
		_m11 /= _number; _m12 /= _number; _m13 /= _number;
		_m21 /= _number; _m22 /= _number; _m23 /= _number;
		_m31 /= _number; _m32 /= _number; _m33 /= _number;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::rotateAroundXAxis(nGENE::Real _angle)
	{
		identity();
		float cs = Maths::fcos(_angle);
		float sn = Maths::fsin(_angle);

		_m[1][1] = cs;
		_m[1][2] = sn;
		_m[2][1] = -sn;
		_m[2][2] = cs;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::rotateAroundYAxis(nGENE::Real _angle)
	{
		identity();
		float cs = Maths::fcos(_angle);
		float sn = Maths::fsin(_angle);

		_m[0][0] = cs;
		_m[0][2] = -sn;
		_m[2][0] = sn;
		_m[2][2] = cs;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::rotateAroundZAxis(nGENE::Real _angle)
	{
		identity();
		float cs = Maths::fcos(_angle);
		float sn = Maths::fsin(_angle);

		_m[0][0] = cs;
		_m[0][1] = sn;
		_m[1][0] = -sn;
		_m[1][1] = cs;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::rotate(const Vector3& _axis, Real _angle)
	{
		identity();
		float fCos = Maths::fcos(_angle);
		float fSin = Maths::fsin(_angle);
		float fSum = 1.0f - fCos;

		_m[0][0] = (_axis.x * _axis.x) * fSum + fCos;
		_m[0][1] = (_axis.x * _axis.y) * fSum - _axis.z * fSin;
		_m[0][2] = (_axis.x * _axis.z) * fSum + _axis.y * fSin;

		_m[1][0] = (_axis.y * _axis.x) * fSum + _axis.z * fSin;
		_m[1][1] = (_axis.y * _axis.y) * fSum + fCos;
		_m[1][2] = (_axis.y * _axis.z) * fSum - _axis.x * fSin;

		_m[2][0] = (_axis.z * _axis.x) * fSum - _axis.y * fSin;
		_m[2][1] = (_axis.z * _axis.y) * fSum + _axis.x * fSin;
		_m[2][2] = (_axis.z * _axis.z) * fSum + fCos;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::rotateYawPitchRoll(const Vector3& _euler)
	{
		float sy = Maths::fsin(_euler.z), cy = Maths::fcos(_euler.z);
		float sp = Maths::fsin(_euler.x), cp = Maths::fcos(_euler.x);
		float sr = Maths::fsin(_euler.y), cr = Maths::fcos(_euler.y);

		_m11 = cy*cr + sy*sp*sr;  _m12 = sr*cp; _m13 = -sy*cr + cy*sp*sr;
		_m21 = -cy*sr + sy*sp*cr; _m22 = cr*cp; _m23 = sr*sy + cy*sp*cr;
		_m31 = sy*cp;             _m32 = -sp;   _m33 = cy*cp;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::rotateYawPitchRoll(Real _yaw, Real _pitch, Real _roll)
	{
		float sy = Maths::fsin(_yaw), cy = Maths::fcos(_yaw);
		float sp = Maths::fsin(_pitch), cp = Maths::fcos(_pitch);
		float sr = Maths::fsin(_roll), cr = Maths::fcos(_roll);

		_m11 = cy*cr + sy*sp*sr;  _m12 = sr*cp; _m13 = -sy*cr + cy*sp*sr;
		_m21 = -cy*sr + sy*sp*cr; _m22 = cr*cp; _m23 = sr*sy + cy*sp*cr;
		_m31 = sy*cp;             _m32 = -sp;   _m33 = cy*cp;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::scale(const Vector3& _scale)
	{
		identity();
		_m11 = _scale.x;
		_m22 = _scale.y;
		_m33 = _scale.z;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::scale(float _x, float _y, float _z)
	{
		identity();
		_m11 = _x;
		_m22 = _y;
		_m33 = _z;
	}
//----------------------------------------------------------------------
	inline void Matrix3x3::scale(float _s)
	{
		identity();
		_m11 = _s;
		_m22 = _s;
		_m33 = _s;
	}
//----------------------------------------------------------------------
	inline bool Matrix3x3::isIdentity()
	{
		return ((*this) == Matrix3x3(1.0f, 0.0f, 0.0f,
			 						 0.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 1.0f));
	}
//----------------------------------------------------------------------
	inline bool Matrix3x3::isZero()
	{
		return ((*this) == Matrix3x3(0.0f, 0.0f, 0.0f,
			 						 0.0f, 0.0f, 0.0f,
									 0.0f, 0.0f, 0.0f));
	}
//----------------------------------------------------------------------
	inline const Matrix3x3 Matrix3x3::operator-()
	{
		Matrix3x3 newMatrix(*this);
		newMatrix.negate();

		return newMatrix;
	}
//----------------------------------------------------------------------
	inline const Matrix3x3 operator+(const Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		Matrix3x3 result = lhs;
		result.add(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Matrix3x3 operator-(const Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		Matrix3x3 result = lhs;
		result.subtract(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Matrix3x3 operator*(const Matrix3x3& lhs, const float& rhs)
	{
		Matrix3x3 result = lhs;
		result.multiply(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Matrix3x3 operator*(const float& lhs, const Matrix3x3& rhs)
	{
		Matrix3x3 result = rhs;
		result.multiply(lhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		Matrix3x3 result = lhs;
		result.multiply(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Vector3 operator*(const Matrix3x3& lhs, const Vector3& rhs)
	{
		Matrix3x3 mat = lhs;

		return mat.multiply(rhs);
	}
//----------------------------------------------------------------------
	inline const Vector3 operator*(const Vector3& lhs, const Matrix3x3& rhs)
	{
		Vector3 vec = lhs;
		vec.multiply(rhs);

		return vec;
	}
//----------------------------------------------------------------------
	inline Real* Matrix3x3::operator[](uint i)
	{
		Assert(i < 3, "Matrix row index not allowed");
		return (_m[i]);
	}
//----------------------------------------------------------------------
	inline const Real* const Matrix3x3::operator[](uint i) const
	{
		Assert(i < 3, "Matrix row index not allowed");
		return (_m[i]);
	}
//----------------------------------------------------------------------
	inline Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& rhs)
	{
		multiply(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector3 Matrix3x3::operator*=(const Vector3& rhs)
	{
		return multiply(rhs);
	}
//----------------------------------------------------------------------
	inline Matrix3x3& Matrix3x3::operator*=(const Real& rhs)
	{
		multiply(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& rhs)
	{
		add(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& rhs)
	{
		subtract(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline bool Matrix3x3::operator==(const Matrix3x3& _mat)
	{
		return (_m[0][0]==_mat[0][0] && _m[0][1]==_mat[0][1] && _m[0][2]==_mat[0][2] &&
				_m[1][0]==_mat[1][0] && _m[1][1]==_mat[1][1] && _m[1][2]==_mat[1][2] &&
				_m[2][0]==_mat[2][0] && _m[2][1]==_mat[2][1] && _m[2][2]==_mat[2][2]);
	}
//----------------------------------------------------------------------
	inline Real* Matrix3x3::getData()
	{
		return (&_m[0][0]);
	}
//----------------------------------------------------------------------
}


#endif