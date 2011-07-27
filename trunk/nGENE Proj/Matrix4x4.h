/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Matrix4x4.h
Version:	0.15
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MATRIX4X4_H_
#define __INC_MATRIX4X4_H_


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
		@par
			Matrix4x4 class takes advantage of SIMD extensions if they are
			available on the platform the engine is being run on. This can
			greatly speed up complex calculations.
		@par
			In case of rotation matrix functions note that following convention
			is used in nGENE: yaw stands for rotation around z axis, pitch - x
			and roll - y axis.
		@par
			Matrix4x4 is a set of 16-bytes aligned floating point values.
	*/
	class nGENEDLL Matrix4x4
	{
	private:
		/// Helper function used for inverse matrix calculation.
		Matrix4x4 adjoint() const;

	public:
		/// Matrix's members
		union {
			struct {
				Real _m11, _m12, _m13, _m14;
				Real _m21, _m22, _m23, _m24;
				Real _m31, _m32, _m33, _m34;
				Real _m41, _m42, _m43, _m44;
			};
			Real _m[4][4];
			Real m[16];
		};

		static const Matrix4x4 IDENTITY;

	public:
		Matrix4x4();
		explicit Matrix4x4(float _m11, float _m12, float _m13, float _m14,
						   float _m21, float _m22, float _m23, float _m24,
						   float _m31, float _m32, float _m33, float _m34,
						   float _m41, float _m42, float _m43, float _m44);
		~Matrix4x4();

		/// Sets all members at once.
		void set(float _m11, float _m12, float _m13, float _m14,
			     float _m21, float _m22, float _m23, float _m24,
			     float _m31, float _m32, float _m33, float _m34,
			     float _m41, float _m42, float _m43, float _m44);

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
				r0 through r2 are rows, and c0 to c2 - columns.
		*/
		Real minor(uint r0, uint r1, uint r2,
				   uint c0, uint c1, uint c2) const;

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
		void add(const Matrix4x4& _mat);
		friend Matrix4x4 add(const Matrix4x4& _mat1, const Matrix4x4& _mat2);

		void subtract(const Matrix4x4& _matrix);
		friend Matrix4x4 subtract(const Matrix4x4& _mat1, const Matrix4x4& _mat2);

		/** Multiplies current matrix by given matrix
			@remarks
				Note that in case of 4x4 matrices multiplication is
				always possible. However, order of performing this
				operation is not arbitrary.
			@todo
				Provide SSE optimized version.
		*/
		void multiply(const Matrix4x4& _matrix);
		/// Multiplies all matrix members by a given number.
		void multiply(const Real& _number);
		/** Multiplies matrix by 4D vector.
			@par
				This method multiplies matrix M and vector V in the following way:
				|m11 m12 m13 m14|    |x|   |result.x|
				|m21 m22 m23 m24| \/ |y|   |result.y|
				|m31 m32 m33 m34| /\ |z| = |result.z|
				|m41 m42 m43 m44|    |w|   |result.w|
		*/
		Vector4 multiply(const Vector4& _vec);
		friend Matrix4x4 multiply(const Matrix4x4& _mat1, const Matrix4x4& _mat2);

		/** Divides all cells by a real number.
			@param _number a divider - cannot be 0.
		*/
		void divide(const Real& _number);


		/// Builds translation matrix from the Vector3.
		void translate(const Vector3& _translate);
		/// Builds translation matrix from 3 float values.
		void translate(float _x, float _y, float _z);
		/** Sets translation.
			@remarks
				This function does not turn matrix into identity matrix
				before proceeding.
		*/
		void setTranslation(const Vector3& _translate);
		/** Sets translation.
			@remarks
				This function does not turn matrix into identity matrix
				before proceeding.
		*/
		void setTranslation(Real _x, Real _y, Real _z);
		/// Returns part of the matrix holding translation.
		Vector3 getTranslation() const;

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

		/** Builds view matrix.
			@param _eyePos position of the eye.
			@param _target direction which observer faces.
			@param _up vector pointing up.
		*/
		void lookAt(const Vector3& _eyePos, const Vector3& _target, const Vector3& _up);

		/** Builds orthogonal projection matrix.
			@param _width width of the screen.
			@param _height of the screen.
			@param _near near plane.
			@param _far far plane.
		*/
		void orthogonal(float _width, float _height, float _near, float _far);
		/** Builds perspective projection matrix.
			@param _fov field of view angle
			@param _aspect width to height ratio.
			@param _near near plane.
			@param _far far plane.
		*/
		void perspective(float _fov, float _aspect, float _near, float _far);

		/// Checks if this matrix is an identity matrix.
		bool isIdentity();
		/// Checks if this matrix contains zeros in all cells.
		bool isZero();


		// Below are overloaded operators.


		Matrix4x4& operator+=(const Matrix4x4& rhs);
		Matrix4x4& operator-=(const Matrix4x4& rhs);
		Matrix4x4& operator*=(const Matrix4x4& rhs);
		Vector4 operator*=(const Vector4& rhs);
		Matrix4x4& operator*=(const Real& rhs);

		friend const Matrix4x4 operator+(const Matrix4x4& lhs, const Matrix4x4& rhs);
		friend const Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs);
		friend const Matrix4x4 operator*(const Matrix4x4& lhs, const float& rhs);
		friend const Matrix4x4 operator*(const float& lhs, const Matrix4x4& rhs);
		friend const Vector4   operator*(const Matrix4x4& lhs, const Vector4& rhs);
		friend const Vector4   operator*(const Vector4& lhs, const Matrix4x4& rhs);
		friend const Matrix4x4 operator-(const Matrix4x4& lhs, const Matrix4x4& rhs);

		const Matrix4x4 operator-();

		bool operator==(const Matrix4x4& _mat);

		Real* operator[](uint i);
		const Real* const operator[](uint i) const;

		/// Returns array of data.
		Real* getData();

		/** Creates Matrix4x4 from string.
			@param
				_string keeping data of matrix.
		*/
		void fromString(const string& _string);
		/// Writes Matrix4x4 to string as "_m11 _m12 ... _m44".
		string toString();
	};



	inline void Matrix4x4::set(float _m11, float _m12, float _m13, float _m14,
						       float _m21, float _m22, float _m23, float _m24,
						       float _m31, float _m32, float _m33, float _m34,
						       float _m41, float _m42, float _m43, float _m44)
	{
		_m[0][0] = _m11; _m[0][1] = _m12; _m[0][2] = _m13; _m[0][3] = _m14;
		_m[1][0] = _m21; _m[1][1] = _m22; _m[1][2] = _m23; _m[1][3] = _m24;
		_m[2][0] = _m31; _m[2][1] = _m32; _m[2][2] = _m33; _m[2][3] = _m34;
		_m[3][0] = _m41; _m[3][1] = _m42; _m[3][2] = _m43; _m[3][3] = _m44;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::lookAt(const Vector3& _eyePos, const Vector3& _target, const Vector3& _up)
	{
		Vector3 z = Maths::normalize(_target - _eyePos);
		Vector3 x = Maths::normalize(_up.crossProduct(z));
		Vector3 y = z.crossProduct(x);

		_m11 = x.x;
		_m12 = y.x;
		_m13 = z.x;
		_m14 = 0.0f;

		_m21 = x.y;
		_m22 = y.y;
		_m23 = z.y;
		_m24 = 0.0f;

		_m31 = x.z;
		_m32 = y.z;
		_m33 = z.z;
		_m34 = 0.0f;

		_m41 = -x.dotProduct(_eyePos);
		_m42 = -y.dotProduct(_eyePos);
		_m43 = -z.dotProduct(_eyePos);
		_m44 = 1.0f;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::orthogonal(float _width, float _height, float _near, float _far)
	{
		identity();

		_m11 = 2.0f / _width;
		_m22 = 2.0f / _height;
		_m33 = 1.0f / (_far - _near);
		_m43 = _near / (_near - _far);
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::perspective(float _fov, float _aspect, float _near, float _far)
	{
		if(!Maths::floatCompare(_far, _near))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Near and far planes are too close");
			return;
		}

		zero();

		float sinFov = Maths::fsin(_fov * 0.5f);
		float cosFov = Maths::fcos(_fov * 0.5f);

		float h = (cosFov / sinFov);
		float w = h / _aspect;
		float q = _far / (_far - _near);

		_m[0][0] = w;
		_m[1][1] = h;
		_m[2][2] = q;
		_m[2][3] = 1.0f;
		_m[3][2] = -q * _near;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::identity()
	{
		float* f = &(this->_m[0][0]);
		memset(f, 0, sizeof(Matrix4x4));
		_m[0][0] = _m[1][1] = _m[2][2] = _m[3][3] = 1.0f;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::zero()
	{
		float* f = &(this->_m[0][0]);
		memset(f, 0, sizeof(Matrix4x4));
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::negate()
	{
		// Boring stuff... it is better however not to
		// use loop in this case as it will be slower
		_m[0][0] = -_m[0][0];
		_m[1][0] = -_m[1][0];
		_m[2][0] = -_m[2][0];
		_m[3][0] = -_m[3][0];
		_m[0][1] = -_m[0][1];
		_m[1][1] = -_m[1][1];
		_m[2][1] = -_m[2][1];
		_m[3][1] = -_m[3][1];
		_m[0][2] = -_m[0][2];
		_m[1][2] = -_m[1][2];
		_m[2][2] = -_m[2][2];
		_m[3][2] = -_m[3][2];
		_m[0][3] = -_m[0][3];
		_m[1][3] = -_m[1][3];
		_m[2][3] = -_m[2][3];
		_m[3][3] = -_m[3][3];
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::invert()
	{
		Real det = determinant();

		// If cannot be inverted, exit
		if(!det)
			return;

		Matrix4x4 adjoin = adjoint();
		adjoin.transpose();

		(*this) = adjoin * (1.0f / det);
	}
//----------------------------------------------------------------------
	inline Real Matrix4x4::determinant() const
	{
		// We use Laplace theorem to calculate determinant
		return (_m[0][0] * minor(1, 2, 3, 1, 2, 3) -
				_m[1][0] * minor(0, 2, 3, 1, 2, 3) +
				_m[2][0] * minor(0, 1, 3, 1, 2, 3) -
				_m[3][0] * minor(0, 1, 2, 1, 2, 3));
	}
//----------------------------------------------------------------------
	inline Real Matrix4x4::minor(uint r0, uint r1, uint r2,
								 uint c0, uint c1, uint c2) const
	{
		// We use Sarrus method here
		return (_m[r0][c0] * (_m[r1][c1] * _m[r2][c2] - _m[r2][c1] * _m[r1][c2]) +
				_m[r0][c1] * (_m[r1][c2] * _m[r2][c0] - _m[r1][c0] * _m[r2][c2]) +
				_m[r0][c2] * (_m[r1][c0] * _m[r2][c1] - _m[r1][c1] * _m[r2][c0]));
	}
//----------------------------------------------------------------------
	inline Matrix4x4 Matrix4x4::adjoint() const
	{
		return Matrix4x4(minor(1, 2, 3, 1, 2, 3),
						 -minor(1, 2, 3, 0, 2, 3),
						 minor(1, 2, 3, 0, 1, 3),
						 -minor(1, 2, 3, 0, 1, 2),
						 
						 -minor(0, 2, 3, 1, 2, 3),
						 minor(0, 2, 3, 0, 2, 3),
						 -minor(0, 2, 3, 0, 1, 3),
						 minor(0, 2, 3, 0, 1, 2),
						 
						 minor(0, 1, 3, 1, 2, 3),
						 -minor(0, 1, 3, 0, 2, 3),
						 minor(0, 1, 3, 0, 1, 3),
						 -minor(0, 1, 3, 0, 1, 2),
						 
						 -minor(0, 1, 2, 1, 2, 3),
						 minor(0, 1, 2, 0, 2, 3),
						 -minor(0, 1, 2, 0, 1, 3),
						 minor(0, 1, 2, 0, 1, 2));
	}
//----------------------------------------------------------------------
	inline Matrix4x4 add(const Matrix4x4& _mat1, const Matrix4x4& _mat2)
	{
		Matrix4x4 result = _mat1;
		result.add(_mat2);

		return result;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::subtract(const Matrix4x4& _mat)
	{
		_m11 -= _mat._m11; _m12 -= _mat._m12; _m13 -= _mat._m13; _m14 -= _mat._m14;
		_m21 -= _mat._m21; _m22 -= _mat._m22; _m23 -= _mat._m23; _m24 -= _mat._m24;
		_m31 -= _mat._m31; _m32 -= _mat._m32; _m33 -= _mat._m33; _m34 -= _mat._m34;
		_m41 -= _mat._m41; _m42 -= _mat._m42; _m43 -= _mat._m43; _m44 -= _mat._m44;
	}
//----------------------------------------------------------------------
	inline Matrix4x4 subtract(const Matrix4x4& _mat1, const Matrix4x4& _mat2)
	{
		Matrix4x4 result = _mat1;
		result.subtract(_mat2);

		return result;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::multiply(const Matrix4x4& _matrix)
	{
		Matrix4x4 mat;
		if(Maths::isUsingSSE())
		{
		}
		else
		{
		}

		mat[0][0] = _m[0][0] * _matrix[0][0] + _m[0][1] * _matrix[1][0] +
					_m[0][2] * _matrix[2][0] + _m[0][3] * _matrix[3][0];
		mat[0][1] = _m[0][0] * _matrix[0][1] + _m[0][1] * _matrix[1][1] +
					_m[0][2] * _matrix[2][1] + _m[0][3] * _matrix[3][1];
		mat[0][2] = _m[0][0] * _matrix[0][2] + _m[0][1] * _matrix[1][2] +
					_m[0][2] * _matrix[2][2] + _m[0][3] * _matrix[3][2];
		mat[0][3] = _m[0][0] * _matrix[0][3] + _m[0][1] * _matrix[1][3] +
					_m[0][2] * _matrix[2][3] + _m[0][3] * _matrix[3][3];

		mat[1][0] = _m[1][0] * _matrix[0][0] + _m[1][1] * _matrix[1][0] +
					_m[1][2] * _matrix[2][0] + _m[1][3] * _matrix[3][0];
		mat[1][1] = _m[1][0] * _matrix[0][1] + _m[1][1] * _matrix[1][1] +
					_m[1][2] * _matrix[2][1] + _m[1][3] * _matrix[3][1];
		mat[1][2] = _m[1][0] * _matrix[0][2] + _m[1][1] * _matrix[1][2] +
					_m[1][2] * _matrix[2][2] + _m[1][3] * _matrix[3][2];
		mat[1][3] = _m[1][0] * _matrix[0][3] + _m[1][1] * _matrix[1][3] +
					_m[1][2] * _matrix[2][3] + _m[1][3] * _matrix[3][3];

		mat[2][0] = _m[2][0] * _matrix[0][0] + _m[2][1] * _matrix[1][0] +
					_m[2][2] * _matrix[2][0] + _m[2][3] * _matrix[3][0];
		mat[2][1] = _m[2][0] * _matrix[0][1] + _m[2][1] * _matrix[1][1] +
					_m[2][2] * _matrix[2][1] + _m[2][3] * _matrix[3][1];
		mat[2][2] = _m[2][0] * _matrix[0][2] + _m[2][1] * _matrix[1][2] +
					_m[2][2] * _matrix[2][2] + _m[2][3] * _matrix[3][2];
		mat[2][3] = _m[2][0] * _matrix[0][3] + _m[2][1] * _matrix[1][3] +
					_m[2][2] * _matrix[2][3] + _m[2][3] * _matrix[3][3];

		mat[3][0] = _m[3][0] * _matrix[0][0] + _m[3][1] * _matrix[1][0] +
					_m[3][2] * _matrix[2][0] + _m[3][3] * _matrix[3][0];
		mat[3][1] = _m[3][0] * _matrix[0][1] + _m[3][1] * _matrix[1][1] +
					_m[3][2] * _matrix[2][1] + _m[3][3] * _matrix[3][1];
		mat[3][2] = _m[3][0] * _matrix[0][2] + _m[3][1] * _matrix[1][2] +
					_m[3][2] * _matrix[2][2] + _m[3][3] * _matrix[3][2];
		mat[3][3] = _m[3][0] * _matrix[0][3] + _m[3][1] * _matrix[1][3] +
					_m[3][2] * _matrix[2][3] + _m[3][3] * _matrix[3][3];

		(*this) = mat;
	}
//---------------------------------------------------------------------
	inline Matrix4x4 multiply(const Matrix4x4& _mat1, const Matrix4x4& _mat2)
	{
		Matrix4x4 temp = _mat1;
		temp.multiply(_mat2);

		return (temp);
	}
//---------------------------------------------------------------------
	inline void Matrix4x4::translate(const Vector3& _translate)
	{
		identity();
		_m[3][0] = _translate.x;
		_m[3][1] = _translate.y;
		_m[3][2] = _translate.z;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::translate(float _x, float _y, float _z)
	{
		identity();
		_m[3][0] = _x;
		_m[3][1] = _y;
		_m[3][2] = _z;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::setTranslation(const Vector3& _translate)
	{
		_m41 = _translate.x;
		_m42 = _translate.y;
		_m43 = _translate.z;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::setTranslation(Real _x, Real _y, Real _z)
	{
		_m41 = _x;
		_m42 = _y;
		_m43 = _z;
	}
//----------------------------------------------------------------------
	inline Vector3 Matrix4x4::getTranslation() const
	{
		return Vector3(_m41, _m42, _m43);
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::rotateAroundXAxis(nGENE::Real _angle)
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
	inline void Matrix4x4::rotateAroundYAxis(nGENE::Real _angle)
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
	inline void Matrix4x4::rotateAroundZAxis(nGENE::Real _angle)
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
	inline void Matrix4x4::rotate(const Vector3& _axis, Real _angle)
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
	inline void Matrix4x4::rotateYawPitchRoll(const Vector3& _euler)
	{
		float sy = Maths::fsin(_euler.z), cy = Maths::fcos(_euler.z);
		float sp = Maths::fsin(_euler.x), cp = Maths::fcos(_euler.x);
		float sr = Maths::fsin(_euler.y), cr = Maths::fcos(_euler.y);

		_m11 = cy*cr + sy*sp*sr;  _m12 = sr*cp; _m13 = -sy*cr + cy*sp*sr; _m14 = 0.0f;
		_m21 = -cy*sr + sy*sp*cr; _m22 = cr*cp; _m23 = sr*sy + cy*sp*cr;  _m24 = 0.0f;
		_m31 = sy*cp;             _m32 = -sp;   _m33 = cy*cp;             _m34 = 0.0f;
		_m41 = 0.0f;              _m42 = 0.0f;  _m43 = 0.0f;              _m44 = 1.0f;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::rotateYawPitchRoll(Real _yaw, Real _pitch, Real _roll)
	{
		float sy = Maths::fsin(_yaw), cy = Maths::fcos(_yaw);
		float sp = Maths::fsin(_pitch), cp = Maths::fcos(_pitch);
		float sr = Maths::fsin(_roll), cr = Maths::fcos(_roll);

		_m11 = cy*cr + sy*sp*sr;  _m12 = sr*cp; _m13 = -sy*cr + cy*sp*sr; _m14 = 0.0f;
		_m21 = -cy*sr + sy*sp*cr; _m22 = cr*cp; _m23 = sr*sy + cy*sp*cr;  _m24 = 0.0f;
		_m31 = sy*cp;             _m32 = -sp;   _m33 = cy*cp;             _m34 = 0.0f;
		_m41 = 0.0f;              _m42 = 0.0f;  _m43 = 0.0f;              _m44 = 1.0f;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::scale(const Vector3& _scale)
	{
		identity();
		_m11 = _scale.x;
		_m22 = _scale.y;
		_m33 = _scale.z;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::scale(float _x, float _y, float _z)
	{
		identity();
		_m11 = _x;
		_m22 = _y;
		_m33 = _z;
	}
//----------------------------------------------------------------------
	inline void Matrix4x4::scale(float _s)
	{
		identity();
		_m11 = _s;
		_m22 = _s;
		_m33 = _s;
	}
//----------------------------------------------------------------------
	inline bool Matrix4x4::isIdentity()
	{
		return ((*this) == Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
			 						 0.0f, 1.0f, 0.0f, 0.0f,
									 0.0f, 0.0f, 1.0f, 0.0f,
									 0.0f, 0.0f, 0.0f, 1.0f));
	}
//----------------------------------------------------------------------
	inline bool Matrix4x4::isZero()
	{
		return ((*this) == Matrix4x4(0.0f, 0.0f, 0.0f, 0.0f,
			 						 0.0f, 0.0f, 0.0f, 0.0f,
									 0.0f, 0.0f, 0.0f, 0.0f,
									 0.0f, 0.0f, 0.0f, 0.0f));
	}
//----------------------------------------------------------------------
	inline const Matrix4x4 Matrix4x4::operator-()
	{
		Matrix4x4 newMatrix(*this);
		newMatrix.negate();

		return newMatrix;
	}
//----------------------------------------------------------------------
	inline const Matrix4x4 operator+(const Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		Matrix4x4 result = lhs;
		result.add(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Matrix4x4 operator-(const Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		Matrix4x4 result = lhs;
		result.subtract(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Matrix4x4 operator*(const Matrix4x4& lhs, const float& rhs)
	{
		Matrix4x4 result = lhs;
		result.multiply(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Matrix4x4 operator*(const float& lhs, const Matrix4x4& rhs)
	{
		Matrix4x4 result = rhs;
		result.multiply(lhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		Matrix4x4 result = lhs;
		result.multiply(rhs);

		return result;
	}
//----------------------------------------------------------------------
	inline const Vector4 operator*(const Matrix4x4& lhs, const Vector4& rhs)
	{
		Matrix4x4 mat = lhs;

		return mat.multiply(rhs);
	}
//----------------------------------------------------------------------
	inline const Vector4 operator*(const Vector4& lhs, const Matrix4x4& rhs)
	{
		Vector4 vec = lhs;
		vec.multiply(rhs);

		return vec;
	}
//----------------------------------------------------------------------
	inline Real* Matrix4x4::operator[](uint i)
	{
		Assert(i < 4, "Matrix row index not allowed");
		return (_m[i]);
	}
//----------------------------------------------------------------------
	inline const Real* const Matrix4x4::operator[](uint i) const
	{
		Assert(i < 4, "Matrix row index not allowed");
		return (_m[i]);
	}
//----------------------------------------------------------------------
	inline Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
	{
		multiply(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Vector4 Matrix4x4::operator*=(const Vector4& rhs)
	{
		return multiply(rhs);
	}
//----------------------------------------------------------------------
	inline Matrix4x4& Matrix4x4::operator*=(const Real& rhs)
	{
		multiply(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& rhs)
	{
		add(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& rhs)
	{
		subtract(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	inline bool Matrix4x4::operator==(const Matrix4x4& _mat)
	{
		return (_m[0][0]==_mat[0][0] && _m[0][1]==_mat[0][1] && _m[0][2]==_mat[0][2] && _m[0][3]==_mat[0][3] &&
				_m[1][0]==_mat[1][0] && _m[1][1]==_mat[1][1] && _m[1][2]==_mat[1][2] && _m[1][3]==_mat[1][3] &&
				_m[2][0]==_mat[2][0] && _m[2][1]==_mat[2][1] && _m[2][2]==_mat[2][2] && _m[2][3]==_mat[2][3] &&
				_m[3][0]==_mat[3][0] && _m[3][1]==_mat[3][1] && _m[3][2]==_mat[3][2] && _m[3][3]==_mat[3][3]);
	}
//----------------------------------------------------------------------
	inline Real* Matrix4x4::getData()
	{
		return (&_m[0][0]);
	}
//----------------------------------------------------------------------
}


#endif