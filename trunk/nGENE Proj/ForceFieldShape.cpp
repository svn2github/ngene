/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShape.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ForceFieldShape.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ForceFieldShape::Type(L"ForceFieldShape", &BaseClass::Type);


	ForceFieldShape::ForceFieldShape()
	{
		init();
	}
//----------------------------------------------------------------------
	ForceFieldShape::~ForceFieldShape()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ForceFieldShape::init()
	{
	}
//----------------------------------------------------------------------
	void ForceFieldShape::cleanup()
	{
	}
//----------------------------------------------------------------------
}