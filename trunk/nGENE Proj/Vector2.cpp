/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Vector2.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Vector2.h"
#include "FuncSplit.h"


namespace nGENE
{
	// Static members initialization
	const Vector2 Vector2::UNIT_X(1.0f, 0.0f);
	const Vector2 Vector2::UNIT_Y(0.0f, 1.0f);
	const Vector2 Vector2::NEGATIVE_UNIT_X(-1.0f, 0.0f);
	const Vector2 Vector2::NEGATIVE_UNIT_Y(0.0f, -1.0f);

	const Vector2 Vector2::ZERO_VECTOR(0.0f, 0.0f);
	const Vector2 Vector2::ONE_VECTOR(1.0f, 1.0f);

	const Vector2 Vector2::INFINITY_VECTOR(1e9, 1e9);
	const Vector2 Vector2::NEGATIVE_INFINITY_VECTOR(-1e9, -1e9);



	Vector2::Vector2(float _x, float _y):
		x(_x),
		y(_y)
	{
	}
//----------------------------------------------------------------------
	Vector2::~Vector2()
	{
	}
//----------------------------------------------------------------------
	void Vector2::fromString(const string& _string)
	{
		vector <string> vecValues;
		FuncSplit <string> tokenizer;
		tokenizer(_string, vecValues, " ,;");

		if(vecValues.size() == 2)
		{
			vector <string>::iterator iter = vecValues.begin();
			x = atof((iter++)->c_str());
			y = atof((iter++)->c_str());
		}
	}
//----------------------------------------------------------------------
	string Vector2::toString()
	{
		stringstream buffer;
		buffer << x << " " << y;

		return buffer.str();
	}
//----------------------------------------------------------------------
}