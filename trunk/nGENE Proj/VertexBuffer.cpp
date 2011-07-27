/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexBuffer.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "VertexBuffer.h"
#include "IndexedBuffer.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo VertexBuffer::Type(L"VertexBuffer", &BaseClass::Type);


	VertexBuffer::VertexBuffer():
		m_nStreamIndex(0),
		m_nReferenceCount(0),
		m_nVBIndex(-1)
	{
	}
//----------------------------------------------------------------------
	VertexBuffer::VertexBuffer(const VERTEXBUFFER_DESC& _desc):
		m_VBDesc(_desc),
		m_nStreamIndex(0),
		m_nReferenceCount(0),
		m_nVBIndex(-1)
	{
		IndexedBuffer* pIVB = m_VBDesc.indices;
		if(pIVB)
		{
			pIVB->setReferenceCount(pIVB->getReferenceCount() + 1);
		}
	}
//----------------------------------------------------------------------
	VertexBuffer::~VertexBuffer()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void VertexBuffer::init()
	{
	}
//----------------------------------------------------------------------
	void VertexBuffer::cleanup()
	{
		destroyIVB();
	}
//----------------------------------------------------------------------
	void VertexBuffer::setIndexedBuffer(IndexedBuffer* _buffer)
	{
		destroyIVB();

		m_VBDesc.indices = _buffer;
		if(_buffer)
		{
			_buffer->setReferenceCount(_buffer->getReferenceCount() + 1);
		}
	}
//----------------------------------------------------------------------
	void VertexBuffer::destroyIVB()
	{
		IndexedBuffer* pIVB = m_VBDesc.indices;
		if(pIVB)
		{
			pIVB->setReferenceCount(pIVB->getReferenceCount() - 1);
			if(!pIVB->getReferenceCount())
				VertexBufferManager::getSingleton().removeIndexedBuffer(pIVB);
		}
	}
//----------------------------------------------------------------------
}