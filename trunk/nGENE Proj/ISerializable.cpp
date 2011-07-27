/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ISerializable.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ISerializable.h"
#include "ISerializer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ISerializable::Type(L"ISerializable", NULL);


	ISerializable::ISerializable():
		m_bIsSerializable(true)
	{
	}
//----------------------------------------------------------------------
	ISerializable::~ISerializable()
	{
	}
//----------------------------------------------------------------------
}
