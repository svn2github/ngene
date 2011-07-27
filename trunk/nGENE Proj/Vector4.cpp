/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Vector4.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Vector4.h"
#include "FuncSplit.h"
#include "Quaternion.h"


namespace nGENE
{
	// Initialize static members
	const Vector4 Vector4::UNIT_X(1.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::UNIT_Y(0.0f, 1.0f, 0.0f, 0.0f);
	const Vector4 Vector4::UNIT_Z(0.0f, 0.0f, 1.0f, 0.0f);
	const Vector4 Vector4::UNIT_W(0.0f, 0.0f, 0.0f, 1.0f);

	const Vector4 Vector4::NEGATIVE_UNIT_X(-1.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::NEGATIVE_UNIT_Y(0.0f, -1.0f, 0.0f, 0.0f);
	const Vector4 Vector4::NEGATIVE_UNIT_Z(0.0f, 0.0f, -1.0f, 0.0f);
	const Vector4 Vector4::NEGATIVE_UNIT_W(0.0f, 0.0f, 0.0f, -1.0f);

	const Vector4 Vector4::ZERO_VECTOR(0.0f, 0.0f, 0.0f, 0.0f);
	const Vector4 Vector4::ONE_VECTOR(1.0f, 1.0f, 1.0f, 1.0f);

	const Vector4 Vector4::INFINITY_VECTOR(1e9, 1e9, 1e9, 1e9);
	const Vector4 Vector4::NEGATIVE_INFINITY_VECTOR(-1e9, -1e9, -1e9, -1e9);


	Vector4::Vector4():
		x(0.0f),
		y(0.0f),
		z(0.0f),
		w(0.0f)
	{
	}
//----------------------------------------------------------------------
	Vector4::Vector4(float _x, float _y, float _z, float _w):
		x(_x),
		y(_y),
		z(_z),
		w(_w)
	{
	}
//----------------------------------------------------------------------
	Vector4::Vector4(const Vector2& _vec):
		x(_vec.x),
		y(_vec.y),
		z(0.0f),
		w(0.0f)
	{
	}
//----------------------------------------------------------------------
	Vector4::Vector4(const Vector2& _vec, Real _z, Real _w):
		x(_vec.x),
		y(_vec.y),
		z(_z),
		w(_w)
	{
	}
//----------------------------------------------------------------------
	Vector4::Vector4(const Vector3& _vec):
		x(_vec.x),
		y(_vec.y),
		z(_vec.z),
		w(0.0f)
	{
	}
//----------------------------------------------------------------------
	Vector4::Vector4(const Vector3& _vec, Real _w):
		x(_vec.x),
		y(_vec.y),
		z(_vec.z),
		w(_w)
	{
	}
//----------------------------------------------------------------------
	Vector4::Vector4(const Quaternion& _quat):
		x(_quat.x),
		y(_quat.y),
		z(_quat.z),
		w(_quat.w)
	{
	}
//----------------------------------------------------------------------
	Vector4::~Vector4()
	{
	}
//----------------------------------------------------------------------
	Vector4& Vector4::operator*=(const Matrix4x4& rhs)
	{
		multiply(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	void Vector4::multiply(const Matrix4x4& _mat)
	{
		/// @todo Implement SSE version
		if(Maths::isUsingSSE())
		{
		}
		else
		{
		}

		float X, Y, Z, W;

		X = x * _mat._m11 + y * _mat._m21 + z * _mat._m31 + w * _mat._m41;
		Y = x * _mat._m12 + y * _mat._m22 + z * _mat._m32 + w * _mat._m42;
		Z = x * _mat._m13 + y * _mat._m23 + z * _mat._m33 + w * _mat._m43;
		W = x * _mat._m14 + y * _mat._m24 + z * _mat._m34 + w * _mat._m44;

		this->x = X;
		this->y = Y;
		this->z = Z;
		this->w = W;
	}
//---------------------------------------------------------------------
	void Vector4::fromString(const string& _string)
	{
		vector <string> vecValues;
		FuncSplit <string> tokenizer;
		tokenizer(_string, vecValues, " ,;");

		if(vecValues.size() == 4)
		{
			vector <string>::iterator iter = vecValues.begin();
			x = atof((iter++)->c_str());
			y = atof((iter++)->c_str());
			z = atof((iter++)->c_str());
			w = atof((iter++)->c_str());
		}
	}
//----------------------------------------------------------------------
	string Vector4::toString()
	{
		stringstream buffer;
		buffer << x << " " << y << " " << z << " " << w;

		return buffer.str();
	}
//----------------------------------------------------------------------
}