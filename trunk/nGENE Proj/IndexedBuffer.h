/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IndexedBuffer.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_INDEXEDBUFFER_H_
#define __INC_INDEXEDBUFFER_H_


#include "BaseClass.h"
#include "TypeInfo.h"
#include "Thread.h"
#include "VertexBuffer.h"
#include "VertexBufferTypes.h"


namespace nGENE
{
	/** Base IndexedBuffer class. It has to be subclassed
		for the graphical API used by the engine.
		@remarks
			IndexedBuffer is a container of the vertex indices. If
			it is bind to the VertexBuffer it can greatly improve
			VertexBuffer rendering process as fewer vertices
			has to be used to render the same shape. Look at the example
			below:
		   3		2			It can be rendered using 4 vertices only.
			+------+            Numbers in the corners are indices of
			|     /|            vertices to be used. And as both
			|    / |			triangles share 0th and 2nd vertices,
			|   /  |			they are used twice. The more complex the
			|  /   |			shape (mesh) is used, the more vertices
			| /    |			you can save.
			|/	   |
			+------+
		   0		1
	*/
	class nGENEDLL IndexedBuffer: public BaseClass
	{
		EXPOSE_TYPE
	protected:
		CriticalSection m_CriticalSection;

		INDEXEDBUFFER_DESC m_IVBDesc;			///< Description of the buffer

		uint m_nReferenceCount;					/**< How many times this IndexedBuffer
													 has been used? */
		int m_nIVBIndex;

	public:
		/** IndexedBuffer constructor.
			@param
				indicesNum how many indices has to be created.
		*/
		IndexedBuffer(const INDEXEDBUFFER_DESC& _desc);
		virtual ~IndexedBuffer();

		virtual void init();
		virtual void cleanup();

		/** Locks the IndexedBuffer so that the data could be written
			to it.
			@param
				_start offset at which locking begins.
			@param
				_count how many indices are to be locked.
			@param
				_indices pointer to the index data.
		*/
		virtual void lock(uint _start=0, uint _count=0, void** _indices=NULL)=0;
		/** Unlocks the IndexedBuffer. Writing to the buffer is not
			possible after call to this function.
		*/
		virtual void unlock()=0;
		/** First locks the buffer, then passes the index data and
			finally unlocks the buffer.
		*/
		virtual void setIndexData(void* _data, uint _start, uint _count)=0;
		/// Returns the data held in the buffer.
		virtual void getIndexData(void* _data, uint _start, uint _count) const=0;

		/// Prepares IndexedBuffer to be used in the rendering process.
		virtual void bind()=0;

		uint getIndicesNum() const {return m_IVBDesc.indicesNum;}
		uint getIndexSize() const {return m_IVBDesc.indexSize;}

		bool is32Bit() const {return m_IVBDesc.is32bit;}

		/// Returns description of the vertex buffer.
		INDEXEDBUFFER_DESC& getDesc() {return m_IVBDesc;}

		void setReferenceCount(uint _value);
		uint getReferenceCount() const;

		void setIVBIndex(int _value);
		int getIVBIndex() const;
	};


//----------------------------------------------------------------------
//----------------------------------------------------------------------


	/// Factory to be used for IndexedBuffer class objects creation.
	class nGENEDLL IndexedBufferFactory
	{
	public:
		virtual IndexedBuffer* create(const INDEXEDBUFFER_DESC& _desc)=0;
	};



//----------------------------------------------------------------------
	inline void IndexedBuffer::setReferenceCount(uint _value)
	{
		m_nReferenceCount = _value;
	}
//----------------------------------------------------------------------
	inline uint IndexedBuffer::getReferenceCount() const
	{
		return m_nReferenceCount;
	}
//----------------------------------------------------------------------
	inline void IndexedBuffer::setIVBIndex(int _value)
	{
		m_nIVBIndex = _value;
	}
//----------------------------------------------------------------------
	inline int IndexedBuffer::getIVBIndex() const
	{
		return m_nIVBIndex;
	}
//----------------------------------------------------------------------
}


#endif