/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Matrix3x3.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Matrix3x3.h"
#include "FuncSplit.h"


namespace nGENE
{
	// Initialize static members
	const Matrix3x3 Matrix3x3::IDENTITY(1.0f, 0.0f, 0.0f,
										0.0f, 1.0f, 0.0f,
										0.0f, 0.0f, 1.0f);


	Matrix3x3::Matrix3x3()
	{
		identity();
	}
//----------------------------------------------------------------------
	Matrix3x3::Matrix3x3(float _m11, float _m12, float _m13,
						 float _m21, float _m22, float _m23,
						 float _m31, float _m32, float _m33)
	{
		set(_m11, _m12, _m13,
			_m21, _m22, _m23,
			_m31, _m32, _m33);
	}
//----------------------------------------------------------------------
	Matrix3x3::~Matrix3x3()
	{
	}
//----------------------------------------------------------------------
	void Matrix3x3::fromString(const string& _string)
	{
		vector <string> vecValues;
		FuncSplit <string> tokenizer;
		tokenizer(_string, vecValues, " ,;");

		if(vecValues.size() == 9)
		{
			vector <string>::iterator iter = vecValues.begin();
			for(uint i = 0; i < 9; ++i)
				m[i] = atof((iter++)->c_str());
		}
	}
//----------------------------------------------------------------------
	string Matrix3x3::toString()
	{
		stringstream buffer;
		for(uint i = 0; i < 8; ++i)
			buffer << m[i] << " ";
		buffer << m[9];

		return buffer.str();
	}
//----------------------------------------------------------------------
}