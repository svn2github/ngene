/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ITexture.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ITexture.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ITexture::Type(L"ITexture", &BaseClass::Type);


	ITexture::ITexture():
		m_fRatioWidth(1.0f),
		m_fRatioHeight(1.0f)
	{
	}
//----------------------------------------------------------------------
	ITexture::~ITexture()
	{
	}
//----------------------------------------------------------------------
}