/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IndexedBuffer.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "IndexedBuffer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo IndexedBuffer::Type(L"IndexedBuffer", &BaseClass::Type);


	IndexedBuffer::IndexedBuffer(const INDEXEDBUFFER_DESC& _desc):
		m_IVBDesc(_desc),
		m_nReferenceCount(0),
		m_nIVBIndex(-1)
	{
	}
//----------------------------------------------------------------------
	IndexedBuffer::~IndexedBuffer()
	{
	}
//----------------------------------------------------------------------
	void IndexedBuffer::init()
	{
	}
//----------------------------------------------------------------------
	void IndexedBuffer::cleanup()
	{
	}
//----------------------------------------------------------------------
}
