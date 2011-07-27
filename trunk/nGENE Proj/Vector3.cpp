/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Vector3.cpp
Version:	0.07
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Vector3.h"
#include "FuncSplit.h"
#include "Matrix3x3.h"
#include "Plane.h"
#include "FuncSplit.h"


namespace nGENE
{
	// Static members initialization
	const Vector3 Vector3::UNIT_X(1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::UNIT_Y(0.0f, 1.0f, 0.0f);
	const Vector3 Vector3::UNIT_Z(0.0f, 0.0f, 1.0f);
	const Vector3 Vector3::NEGATIVE_UNIT_X(-1.0f, 0.0f, 0.0f);
	const Vector3 Vector3::NEGATIVE_UNIT_Y(0.0f, -1.0f, 0.0f);
	const Vector3 Vector3::NEGATIVE_UNIT_Z(0.0f, 0.0f, -1.0f);

	const Vector3 Vector3::ZERO_VECTOR(0.0f, 0.0f, 0.0f);
	const Vector3 Vector3::ONE_VECTOR(1.0f, 1.0f, 1.0f);

	const Vector3 Vector3::INFINITY_VECTOR(1e9, 1e9, 1e9);
	const Vector3 Vector3::NEGATIVE_INFINITY_VECTOR(-1e9, -1e9, -1e9);


	Vector3::Vector3():
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{
	}
//----------------------------------------------------------------------
	Vector3::Vector3(float _x, float _y, float _z):
		x(_x),
		y(_y),
		z(_z)
	{
	}
//----------------------------------------------------------------------
	Vector3::Vector3(const Vector2& _vec):
		x(_vec.x),
		y(_vec.y),
		z(0.0f)
	{
	}
//----------------------------------------------------------------------
	Vector3::Vector3(const Vector2& _vec, Real _z):
		x(_vec.x),
		y(_vec.y),
		z(_z)
	{
	}
//----------------------------------------------------------------------
	Vector3::~Vector3()
	{
	}
//----------------------------------------------------------------------
	Vector3 Vector3::reflect(const Plane& _pl)
	{
		Real distance = _pl.getDistanceFromPlane(*this);
		Vector3 normal = _pl.getPlaneNormal();

		return ((*this) - normal * (2.0f * distance));
	}
//----------------------------------------------------------------------
	Vector3 Vector3::deflect(const Plane& _pl)
	{
		Vector3 normal = _pl.getPlaneNormal();
		Real dot = (*this) % normal;

		return ((*this) - normal * (2.0f * dot));
	}
//----------------------------------------------------------------------
	Vector3& Vector3::operator*=(const Matrix3x3& rhs)
	{
		multiply(rhs);
		return (*this);
	}
//----------------------------------------------------------------------
	void Vector3::multiply(const Matrix3x3& _mat)
	{
		/// @todo Implement SSE version
		if(Maths::isUsingSSE())
		{
		}
		else
		{
		}

		float X, Y, Z;

		X = x * _mat._m11 + y * _mat._m21 + z * _mat._m31;
		Y = x * _mat._m12 + y * _mat._m22 + z * _mat._m32;
		Z = x * _mat._m13 + y * _mat._m23 + z * _mat._m33;

		this->x = X;
		this->y = Y;
		this->z = Z;
	}
//---------------------------------------------------------------------
	void Vector3::fromString(const string& _string)
	{
		vector <string> vecValues;
		FuncSplit <string> tokenizer;
		tokenizer(_string, vecValues, " ,;");

		if(vecValues.size() == 3)
		{
			vector <string>::iterator iter = vecValues.begin();
			x = atof((iter++)->c_str());
			y = atof((iter++)->c_str());
			z = atof((iter++)->c_str());
		}
	}
//----------------------------------------------------------------------
	string Vector3::toString()
	{
		stringstream buffer;
		buffer << x << " " << y << " " << z;

		return buffer.str();
	}
//----------------------------------------------------------------------
}