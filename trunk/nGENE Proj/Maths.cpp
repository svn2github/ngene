/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Maths.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Maths.h"


namespace nGENE
{
	// Some important mathematical constants and numbers
	// needs to be initialized here as they were declared static
	Maths::ANGLE_UNIT Maths::s_AngleUnit = Maths::ANGLE_RADIAN;

	const Real Maths::PI = 3.14159265f;
	const Real Maths::PI_DOUBLE = 6.28318853f;
	const Real Maths::PI_HALF = 1.570796325f;

	const Real Maths::Infinity = 1e9;
	const Real Maths::MinusInfinity = -Maths::Infinity;
	const Real Maths::FloatEpsilon = 0.000001f;

	FastFloat* Maths::s_FastFloat = new FastFloat();

	bool Maths::m_bUseTables = true;
	bool Maths::m_bUseSSE = false;

	const Real Maths::m_kDegToRad = PI_DOUBLE / 360.0f;
	const Real Maths::m_kRadToDeg = 180.0f / PI;


	Maths::Maths()
	{
	}
//----------------------------------------------------------------------	
	Maths::~Maths()
	{
	}
//----------------------------------------------------------------------	
}