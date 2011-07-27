/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysXStream.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PHYSXSTREAM_H_
#define __INC_PHYSXSTREAM_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"

#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
	#include "NxStream.h"
#pragma warning(pop)


namespace nGENE
{
	/** NxStream implementation by Ageia Corp.
	*/
	class MemoryWriteBuffer: public NxStream
	{
	private:
		uint m_nCurrentSize;
		uint m_nMaxSize;
		byte* m_pData;

	public:
		MemoryWriteBuffer();
		virtual ~MemoryWriteBuffer();

		virtual	byte readByte()	const {return 0;}
		virtual	ushort readWord() const	{return 0;}
		virtual	uint readDword() const {return 0;}
		virtual	Real readFloat() const {return 0.0f;}
		virtual	double readDouble()	const {return 0.0;}
		virtual	void readBuffer(void* buffer, uint size) const {}

		virtual	NxStream& storeByte(byte b);
		virtual	NxStream& storeWord(ushort w);
		virtual	NxStream& storeDword(uint d);
		virtual	NxStream& storeFloat(Real f);
		virtual	NxStream& storeDouble(double f);
		virtual	NxStream& storeBuffer(const void* buffer, uint size);

		byte* getData() const {return m_pData;}
	};

	/** NxStream implementation by Ageia Corp.
	*/
	class MemoryReadBuffer: public NxStream
	{
	private:
		mutable	const byte* m_pBuffer;

	public:
		MemoryReadBuffer(const byte* data);
		virtual	~MemoryReadBuffer();

		virtual	byte readByte() const;
		virtual	ushort readWord() const;
		virtual	uint readDword() const;
		virtual	Real readFloat() const;
		virtual	double readDouble() const;
		virtual	void readBuffer(void* buffer, uint size) const;

		virtual	NxStream& storeByte(byte b) {return *this;}
		virtual	NxStream& storeWord(ushort w) {return *this;}
		virtual	NxStream& storeDword(uint d) {return *this;}
		virtual	NxStream& storeFloat(Real f) {return *this;}
		virtual	NxStream& storeDouble(double f) {return *this;}
		virtual	NxStream& storeBuffer(const void* buffer, uint size) {return *this;}
	};
}


#endif
