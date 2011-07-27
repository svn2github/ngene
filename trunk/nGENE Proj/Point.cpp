/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Point.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Point.h"
#include "FuncSplit.h"


namespace nGENE
{
	// Statics initialization
	const Point Point::ZERO_POINT(0.0f, 0.0f, 0.0f);


	Point::Point()
	{
		(*this)=ZERO_POINT;
	}
//----------------------------------------------------------------------
	Point::Point(float _x, float _y, float _z):
		x(_x),
		y(_y),
		z(_z)
	{
	}
//----------------------------------------------------------------------
	Point::Point(const nGENE::Vector3& _point):
		x(_point.x),
		y(_point.y),
		z(_point.z)
	{
	}
//----------------------------------------------------------------------
	Point::~Point()
	{
	}
//----------------------------------------------------------------------
	string Point::toString()
	{
		stringstream buffer;
		buffer << x << " " << y << " " << z;

		return buffer.str();
	}
//----------------------------------------------------------------------
	void Point::fromString(const string& _string)
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
}