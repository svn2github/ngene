/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FastFloat.cpp
Version:	0.51
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FastFloat.h"
#include "Maths.h"


namespace nGENE
{
	// There is quite a lot of static members in this class
	// so we need to initialize them.
	uint FastFloat::s_knSinTableSize = 512;
	int FastFloat::s_nMask = 511;
	float* FastFloat::s_pSinTable = NULL;
	float FastFloat::s_fModifier = 511.0f / (2.0f * Maths::PI);
	float FastFloat::s_fQuarter = 128.0f;
	const float FastFloat::FTOIBIAS = 12582912.0f;
	

	FastFloat::FastFloat(uint _sinTableSize)
	{
		s_knSinTableSize = _sinTableSize;
		s_nMask = s_knSinTableSize - 1;
		s_fModifier = static_cast<float>((s_knSinTableSize) / (2.0f * Maths::PI));
		s_fQuarter = static_cast<float>((s_knSinTableSize) * 0.25f);

		initSinTable();
	}
//----------------------------------------------------------------------
	FastFloat::~FastFloat()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void FastFloat::initSinTable()
	{
		s_pSinTable = new float[s_knSinTableSize];
		for(uint i = 0; i < s_knSinTableSize; ++i)
			s_pSinTable[i] = (float)sin((double)i * 2.0 * Maths::PI / double(s_knSinTableSize));
	}
//----------------------------------------------------------------------
	uint FastFloat::getSinTableSize()
	{
		return s_knSinTableSize;
	}
//----------------------------------------------------------------------
	void FastFloat::cleanup()
	{
		delete[] s_pSinTable;
	}
//----------------------------------------------------------------------
}
