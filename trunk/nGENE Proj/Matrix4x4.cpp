/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Matrix4x4.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Matrix4x4.h"
#include "FuncSplit.h"

#include <emmintrin.h>
#include <xmmintrin.h>


namespace nGENE
{
	// Initialize static members
	const Matrix4x4 Matrix4x4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
										0.0f, 1.0f, 0.0f, 0.0f,
										0.0f, 0.0f, 1.0f, 0.0f,
										0.0f, 0.0f, 0.0f, 1.0f);


	Matrix4x4::Matrix4x4()
	{
		identity();
	}
//----------------------------------------------------------------------
	Matrix4x4::Matrix4x4(float _m11, float _m12, float _m13, float _m14,
						 float _m21, float _m22, float _m23, float _m24,
						 float _m31, float _m32, float _m33, float _m34,
						 float _m41, float _m42, float _m43, float _m44)
	{
		set(_m11, _m12, _m13, _m14,
			_m21, _m22, _m23, _m24,
			_m31, _m32, _m33, _m34,
			_m41, _m42, _m43, _m44);
	}
//----------------------------------------------------------------------
	Matrix4x4::~Matrix4x4()
	{
	}
//----------------------------------------------------------------------
	void Matrix4x4::fromString(const string& _string)
	{
		vector <string> vecValues;
		FuncSplit <string> tokenizer;
		tokenizer(_string, vecValues, " ,;");

		if(vecValues.size() == 16)
		{
			vector <string>::iterator iter = vecValues.begin();
			for(uint i = 0; i < 16; ++i)
				m[i] = atof((iter++)->c_str());
		}
	}
//----------------------------------------------------------------------
	string Matrix4x4::toString()
	{
		stringstream buffer;
		for(uint i = 0; i < 15; ++i)
			buffer << m[i] << " ";
		buffer << m[15];

		return buffer.str();
	}
//----------------------------------------------------------------------
	void Matrix4x4::transpose()
	{
		if(Maths::isUsingSSE())
		{
			__m128 row0 = _mm_loadu_ps((float*)&m[0]);
			__m128 row1 = _mm_loadu_ps((float*)&m[4]);
			__m128 row2 = _mm_loadu_ps((float*)&m[8]);
			__m128 row3 = _mm_loadu_ps((float*)&m[12]);

			_MM_TRANSPOSE4_PS(row0, row1, row2, row3);

			_mm_storeu_ps((float*)&m[0], row0);
			_mm_storeu_ps((float*)&m[4], row1);
			_mm_storeu_ps((float*)&m[8], row2);
			_mm_storeu_ps((float*)&m[12], row3);
		}
		else
		{
			Matrix4x4 mat= (*this);
			mat._m[0][0] = _m[0][0];
			mat._m[0][1] = _m[1][0];
			mat._m[0][2] = _m[2][0];
			mat._m[0][3] = _m[3][0];

			mat._m[1][0] = _m[0][1];
			mat._m[1][1] = _m[1][1];
			mat._m[1][2] = _m[2][1];
			mat._m[1][3] = _m[3][1];

			mat._m[2][0] = _m[0][2];
			mat._m[2][1] = _m[1][2];
			mat._m[2][2] = _m[2][2];
			mat._m[2][3] = _m[3][2];

			mat._m[3][0] = _m[0][3];
			mat._m[3][1] = _m[1][3];
			mat._m[3][2] = _m[2][3];
			mat._m[3][3] = _m[3][3];

			(*this) = mat;
		}
	}
//----------------------------------------------------------------------
	Vector4 Matrix4x4::multiply(const Vector4& _vec)
	{
		Vector4 vecResult;
		if(Maths::isUsingSSE())
		{
			float* pMat = &m[0];
			float* pDst = &vecResult.vals[0];

			__m128 vector = _mm_loadu_ps((float*)&_vec.vals[0]);

			__m128 v2v3 = _mm_shuffle_ps(vector, vector, 238);
			__m128 v0v1 = _mm_shuffle_ps(vector, vector, 68);

			__m128 _xmm2 = _mm_setzero_ps();
			_xmm2 = _mm_loadl_pi(_xmm2, (__m64*)pMat);
			_xmm2 = _mm_loadh_pi(_xmm2, (__m64*)(pMat + 4));
			_xmm2 = _mm_mul_ps(_xmm2, v0v1);

			__m128 _xmm3 = _mm_setzero_ps();
			_xmm3 = _mm_loadl_pi(_xmm3, (__m64*)(pMat + 2));
			_xmm3 = _mm_loadh_pi(_xmm3, (__m64*)(pMat + 6));
			_xmm3 = _mm_mul_ps(_xmm3, v2v3);

			__m128 _xmm4 = _mm_setzero_ps();
			_xmm4 = _mm_loadl_pi(_xmm4, (__m64*)(pMat + 8));
			_xmm4 = _mm_loadh_pi(_xmm4, (__m64*)(pMat + 12));
			v0v1 = _mm_mul_ps(_xmm4, v0v1);

			__m128 _xmm5 = _mm_setzero_ps();
			_xmm5 = _mm_loadl_pi(_xmm5, (__m64*)(pMat + 10));
			_xmm5 = _mm_loadh_pi(_xmm5, (__m64*)(pMat + 14));
			v2v3 = _mm_mul_ps(_xmm5, v2v3);

			_xmm2 = _mm_add_ps(_xmm2, _xmm3);
			_xmm3 = _xmm2;
			v0v1 = _mm_add_ps(v0v1, v2v3);

			_xmm2 = _mm_shuffle_ps(_xmm2, v0v1, 221);
			_xmm3 = _mm_shuffle_ps(_xmm3, v0v1, 136);

			_xmm3 = _mm_add_ps(_xmm3, _xmm2);
			_mm_storeu_ps(pDst, _xmm3);
		}
		else
		{
			vecResult.set(_m11 * _vec.x + _m12 * _vec.y + _m13 * _vec.z + _m14 * _vec.w,
						  _m21 * _vec.x + _m22 * _vec.y + _m23 * _vec.z + _m24 * _vec.w,
						  _m31 * _vec.x + _m32 * _vec.y + _m33 * _vec.z + _m34 * _vec.w,
						  _m41 * _vec.x + _m42 * _vec.y + _m43 * _vec.z + _m44 * _vec.w);
		}

		return vecResult;
	}
//---------------------------------------------------------------------
	void Matrix4x4::multiply(const Real& _number)
	{
		if(Maths::isUsingSSE())
		{
			__m128 row0 = _mm_loadu_ps((float*)&m[0]);
			__m128 row1 = _mm_loadu_ps((float*)&m[4]);
			__m128 row2 = _mm_loadu_ps((float*)&m[8]);
			__m128 row3 = _mm_loadu_ps((float*)&m[12]);

			__m128 number = _mm_set_ps1(_number);

			row0 = _mm_mul_ps(row0, number);
			row1 = _mm_mul_ps(row1, number);
			row2 = _mm_mul_ps(row2, number);
			row3 = _mm_mul_ps(row3, number);

			_mm_storeu_ps((float*)&m[0], row0);
			_mm_storeu_ps((float*)&m[4], row1);
			_mm_storeu_ps((float*)&m[8], row2);
			_mm_storeu_ps((float*)&m[12], row3);
		}
		else
		{
			_m[0][0] *= _number; _m[0][1] *= _number; _m[0][2] *= _number; _m[0][3] *= _number;
			_m[1][0] *= _number; _m[1][1] *= _number; _m[1][2] *= _number; _m[1][3] *= _number;
			_m[2][0] *= _number; _m[2][1] *= _number; _m[2][2] *= _number; _m[2][3] *= _number;
			_m[3][0] *= _number; _m[3][1] *= _number; _m[3][2] *= _number; _m[3][3] *= _number;
		}
	}
//---------------------------------------------------------------------
	void Matrix4x4::divide(const Real& _number)
	{
		float invNumber = 1.0f / _number;

		if(Maths::isUsingSSE())
		{
			__m128 row0 = _mm_loadu_ps((float*)&m[0]);
			__m128 row1 = _mm_loadu_ps((float*)&m[4]);
			__m128 row2 = _mm_loadu_ps((float*)&m[8]);
			__m128 row3 = _mm_loadu_ps((float*)&m[12]);

			__m128 number = _mm_set_ps1(invNumber);

			row0 = _mm_mul_ps(row0, number);
			row1 = _mm_mul_ps(row1, number);
			row2 = _mm_mul_ps(row2, number);
			row3 = _mm_mul_ps(row3, number);

			_mm_storeu_ps((float*)&m[0], row0);
			_mm_storeu_ps((float*)&m[4], row1);
			_mm_storeu_ps((float*)&m[8], row2);
			_mm_storeu_ps((float*)&m[12], row3);
		}
		else
		{
			_m11 *= invNumber; _m12 *= invNumber; _m13 *= invNumber; _m14 *= invNumber;
			_m21 *= invNumber; _m22 *= invNumber; _m23 *= invNumber; _m24 *= invNumber;
			_m31 *= invNumber; _m32 *= invNumber; _m33 *= invNumber; _m34 *= invNumber;
			_m41 *= invNumber; _m42 *= invNumber; _m43 *= invNumber; _m44 *= invNumber;
		}
	}
//---------------------------------------------------------------------
	void Matrix4x4::add(const Matrix4x4& _mat)
	{
		if(Maths::isUsingSSE())
		{
			__m128 row0m1 = _mm_loadu_ps(&m[0]);
			__m128 row0m2 = _mm_loadu_ps((float*)&_mat.m[0]);
			row0m1 = _mm_add_ps(row0m1, row0m2);
			_mm_storeu_ps((float*)&m[0], row0m1);

			row0m1 = _mm_loadu_ps(&m[4]);
			row0m2 = _mm_loadu_ps((float*)&_mat.m[4]);
			row0m1 = _mm_add_ps(row0m1, row0m2);
			_mm_storeu_ps((float*)&m[4], row0m1);

			row0m1 = _mm_loadu_ps(&m[8]);
			row0m2 = _mm_loadu_ps((float*)&_mat.m[8]);
			row0m1 = _mm_add_ps(row0m1, row0m2);
			_mm_storeu_ps((float*)&m[8], row0m1);

			row0m1 = _mm_loadu_ps(&m[12]);
			row0m2 = _mm_loadu_ps((float*)&_mat.m[12]);
			row0m1 = _mm_add_ps(row0m1, row0m2);
			_mm_storeu_ps((float*)&m[12], row0m1);
		}
		else
		{
			_m11 += _mat._m11; _m12 += _mat._m12; _m13 += _mat._m13; _m14 += _mat._m14;
			_m21 += _mat._m21; _m22 += _mat._m22; _m23 += _mat._m23; _m24 += _mat._m24;
			_m31 += _mat._m31; _m32 += _mat._m32; _m33 += _mat._m33; _m34 += _mat._m34;
			_m41 += _mat._m41; _m42 += _mat._m42; _m43 += _mat._m43; _m44 += _mat._m44;
		}
	}
//----------------------------------------------------------------------
}