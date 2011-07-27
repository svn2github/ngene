/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Quaternion.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Quaternion.h"
#include "FuncSplit.h"
#include "Matrix4x4.h"


namespace nGENE
{
	Quaternion::Quaternion(float _x, float _y, float _z, float _w)
	{
		set(_x, _y, _z, _w);
	}
//----------------------------------------------------------------------
	Quaternion::Quaternion(const Vector3& _axis, const float _angle)
	{
		set(_axis, _angle);
	}
//----------------------------------------------------------------------
	Quaternion::~Quaternion()
	{
	}
//----------------------------------------------------------------------
	Matrix4x4 Quaternion::toMatrix() const
	{
		Matrix4x4 result;

		// Some auxiliary variables
		float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

		x2=x+x;		y2=y+y;		z2=z+z;
		xx=x*x2;	yy=y*y2;	zz=z*z2;

		xy=x*y2;
		xz=x*z2;
		yz=y*z2;

		wx=w*x2;
		wy=w*y2;
		wz=w*z2;

		result._m11=1.0f-yy-zz;
		result._m12=xy+wz;
		result._m13=xz-wy;

		result._m21=xy-wz;
		result._m22=1.0f-xx-zz;
		result._m23=yz+wx;

		result._m31=xz+wy;
		result._m32=yz-wx;
		result._m33=1.0f-xx-yy;


		return result;
	}
//----------------------------------------------------------------------
	void Quaternion::fromEulerAngles(Real _yaw, Real _pitch, Real _roll)
	{
		set(_yaw, _pitch, _roll);
	}
//----------------------------------------------------------------------
	void Quaternion::fromEulerAngles(const Vector3& _angles)
	{
		set(_angles.x, _angles.y, _angles.z);
	}
//----------------------------------------------------------------------
	void Quaternion::fromMatrix(const Matrix4x4& _mat)
	{
		float trace = _mat._m11 + _mat._m22 + _mat._m33;
		float root;

		if(trace > 0.0f)
		{
			root = Maths::fsqrt(trace + 1.0f);

			this->w = 0.5f * root;
			root = 0.5f / root;
			this->x = -(_mat._m32 - _mat._m23) * root;
			this->y = -(_mat._m13 - _mat._m31) * root;
			this->z = -(_mat._m21 - _mat._m12) * root;
		}
		else
		{
			uint next[3] = {1, 2, 0};
			uint i = 0;
			if(_mat[1][1] > _mat[0][0])
				i = 1;
			if(_mat[2][2] > _mat[i][i])
				i = 2;
			uint j = next[i];
			uint k = next[j];

			root = Maths::fsqrt(_mat[i][i] - _mat[j][j] - _mat[k][k] + 1.0f);
			float* pQuat[3] = {&this->x, &this->y, &this->z};
			*pQuat[i] = -0.5f * root;
			root = 0.5f / root;
			this->w = (_mat[k][j] - _mat[j][k]) * root;
			*pQuat[j] = -(_mat[j][i] + _mat[i][j]) * root;
			*pQuat[k] = -(_mat[k][i] + _mat[i][k]) * root;
		}
	}
//----------------------------------------------------------------------
	void Quaternion::fromString(const string& _string)
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
	string Quaternion::toString()
	{
		stringstream buffer;
		buffer << x << " " << y << " " << z << " " << w;

		return buffer.str();
	}
//----------------------------------------------------------------------
}