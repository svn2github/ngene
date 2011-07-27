/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysXStream.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PhysXStream.h"


#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
	#include "NxPhysics.h"
#pragma warning(pop)


namespace nGENE
{
	MemoryWriteBuffer::MemoryWriteBuffer():
		m_nCurrentSize(0),
		m_nMaxSize(0),
		m_pData(NULL)
	{
	}
//----------------------------------------------------------------------
	MemoryWriteBuffer::~MemoryWriteBuffer()
	{
		NxGetPhysicsSDKAllocator()->free(m_pData);
	}
//----------------------------------------------------------------------
	NxStream& MemoryWriteBuffer::storeByte(byte b)
	{
		storeBuffer(&b, sizeof(byte));
		return *this;
	}
//----------------------------------------------------------------------
	NxStream& MemoryWriteBuffer::storeWord(ushort w)
	{
		storeBuffer(&w, sizeof(ushort));
		return *this;
	}
//----------------------------------------------------------------------
	NxStream& MemoryWriteBuffer::storeDword(uint d)
	{
		storeBuffer(&d, sizeof(uint));
		return *this;
	}
//----------------------------------------------------------------------
	NxStream& MemoryWriteBuffer::storeFloat(Real f)
	{
		storeBuffer(&f, sizeof(Real));
		return *this;
	}
//----------------------------------------------------------------------
	NxStream& MemoryWriteBuffer::storeDouble(double f)
	{
		storeBuffer(&f, sizeof(double));
		return *this;
	}
//----------------------------------------------------------------------
	NxStream& MemoryWriteBuffer::storeBuffer(const void* buffer, uint size)
	{
		uint expectedSize = m_nCurrentSize + size;
		if(expectedSize > m_nMaxSize)
		{
			m_nMaxSize = expectedSize + 4096;

			byte* newData = (byte*)NxGetPhysicsSDKAllocator()->malloc(m_nMaxSize, NX_MEMORY_PERSISTENT);
			if(m_pData)
			{
				memcpy(newData, m_pData, m_nCurrentSize);
				NxGetPhysicsSDKAllocator()->free(m_pData);
			}

			m_pData = newData;
		}
		memcpy(m_pData + m_nCurrentSize, buffer, size);
		m_nCurrentSize += size;

		return *this;
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	MemoryReadBuffer::MemoryReadBuffer(const byte* data):
		m_pBuffer(data)
	{
	}
//----------------------------------------------------------------------
	MemoryReadBuffer::~MemoryReadBuffer()
	{
		// We don't own the data => no delete
	}
//----------------------------------------------------------------------
	byte MemoryReadBuffer::readByte() const
	{
		byte b;
		memcpy(&b, m_pBuffer, sizeof(byte));
		m_pBuffer += sizeof(byte);
		return b;
	}
//----------------------------------------------------------------------
	ushort MemoryReadBuffer::readWord() const
	{
		ushort w;
		memcpy(&w, m_pBuffer, sizeof(ushort));
		m_pBuffer += sizeof(ushort);
		return w;
	}
//----------------------------------------------------------------------
	uint MemoryReadBuffer::readDword() const
	{
		uint d;
		memcpy(&d, m_pBuffer, sizeof(uint));
		m_pBuffer += sizeof(uint);
		return d;
	}
//----------------------------------------------------------------------
	Real MemoryReadBuffer::readFloat() const
	{
		float f;
		memcpy(&f, m_pBuffer, sizeof(float));
		m_pBuffer += sizeof(float);
		return f;
	}
//----------------------------------------------------------------------
	double MemoryReadBuffer::readDouble() const
	{
		double f;
		memcpy(&f, m_pBuffer, sizeof(double));
		m_pBuffer += sizeof(double);
		return f;
	}
//----------------------------------------------------------------------
	void MemoryReadBuffer::readBuffer(void* dest, uint size) const
	{
		memcpy(dest, m_pBuffer, size);
		m_pBuffer += size;
	}
//----------------------------------------------------------------------
}