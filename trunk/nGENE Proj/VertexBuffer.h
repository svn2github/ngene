/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexBuffer.h
Version:	0.12
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VERTEXBUFFER_H_
#define __INC_VERTEXBUFFER_H_


#include <D3D9.h>
#include "BaseClass.h"
#include "IRenderable.h"
#include "Prerequisities.h"
#include "Thread.h"
#include "TypeInfo.h"
#include "VertexBufferTypes.h"


namespace nGENE
{
	/** Base VertexBuffer class. It has to be subclassed
		for the graphical API used by the engine.
		@par
			VertexBuffer is a container for the vertices
			sharing the same declaration and very often
			making up one entity (eg. mesh or particle system).
	*/
	class nGENEDLL VertexBuffer: public BaseClass, public IRenderable
	{
		EXPOSE_TYPE
	protected:
		CriticalSection m_CriticalSection;

		VERTEXBUFFER_DESC m_VBDesc;				///< Description of the vertex buffer

		uint m_nStreamIndex;					///< Index of the stream this VB should be bound to
		uint m_nReferenceCount;					/**< How many times this VertexBuffer
													 has been used? */
		int m_nVBIndex;


		void destroyIVB();

	public:
		/// VertexBuffer constructor.
		VertexBuffer();
		VertexBuffer(const VERTEXBUFFER_DESC& _desc);
		virtual ~VertexBuffer();

		virtual void init();
		virtual void cleanup();

		/** Locks the buffer so you can change vertices	attributes.
			@remarks
				Locking the buffer is a rather slow operation and
				therefore it is essential to ensure it is not
				being overused.
			@par
				Via locking you can get content of the buffer. However if
				you unlock it, it is not guaranteed it will be still valid.
		*/
		virtual void lock(uint _start=0, uint _count=0, void** _vertices=NULL)=0;

		/** Unlocking the buffer makes it impossible to
			alter vertices attributes.
		*/
		virtual void unlock()=0;

		/** Lets you set vertex buffer contents.
			@param
				data pointer to the beginning of an array containing vertices
				you want to store in the buffer.
			@param
				start offset in vertices at which vertices data will be stored
				in the buffer.
			@param
				count how many vertices data is to be put in the buffer.
		*/
		virtual void setVertexData(void* _data, uint _start, uint _count)=0;

		/** Gets vertex buffer content.
			@remarks
				In contrast to lock() method data is guaranteed to be valid after
				unlock() call. However, it comes at cost of speed as internally
				memory is being copied. Therefore it is advised to use lock()/unlock()
				methods as often as possible.
		*/
		virtual void getVertexData(void* _data, uint _start, uint _count) const=0;

		/// Binds this vertex buffer with the given index buffer.
		virtual void setIndexedBuffer(IndexedBuffer* _buffer);
		/// Returns index buffer associated with this vertex buffer.
		virtual IndexedBuffer* getIndexedBuffer() const;

		/// Binds the buffer for rendering.
		virtual void bind()=0;
		/// Prepares the buffer for rendering.
		virtual void prepare()=0;
		/// Renders buffer.
		virtual void render()=0;
		/// Renders buffer.
		virtual void render(uint _verticesNum, uint _primitivesNum)=0;
		/// Immediately renders given set of vertices.
		virtual void render(const VERTEXBUFFER_DESC& _desc)=0;

		/** Sets the number of primitives to render.
			@param _number number of primitives to render.
		*/
		void setPrimitivesNum(uint _number);
		/// Returns number of primitives to render.
		uint getPrimitivesNum() const;

		/** Sets the type of primitives.
			@param _type type of the primitives to render.
		*/
		void setPrimitivesType(PRIMITIVE_TYPE _type);
		/// Returns type of the primitives being drawn.
		PRIMITIVE_TYPE getPrimitivesType() const;

		/// Returns number of vertices in the buffer.
		uint getVerticesNum() const;

		/// Sets vertex declaration.
		void setVertexDeclaration(VertexDeclaration* _decl);
		/// Returns vertex declaration.
		VertexDeclaration* getVertexDeclaration() const;

		/// Sets new description of the buffer.
		void setVertexBufferDesc(const VERTEXBUFFER_DESC& _desc);
		/// Returns description of the vertex buffer.
		VERTEXBUFFER_DESC& getDesc();

		void setReferenceCount(uint _value);
		uint getReferenceCount() const;

		void setStreamIndex(uint _value);
		uint getStreamIndex() const;

		void setVBIndex(int _value);
		int getVBIndex() const;
	};



//----------------------------------------------------------------------
//----------------------------------------------------------------------


	/// Factory to be used for VertexBuffer class objects creation.
	class nGENEDLL VertexBufferFactory
	{
	public:
		virtual VertexBuffer* create(const VERTEXBUFFER_DESC& _desc)=0;
	};



//----------------------------------------------------------------------
	inline void VertexBuffer::setReferenceCount(uint _value)
	{
		m_nReferenceCount = _value;
	}
//----------------------------------------------------------------------
	inline uint VertexBuffer::getReferenceCount() const
	{
		return m_nReferenceCount;
	}
//----------------------------------------------------------------------
	inline void VertexBuffer::setVBIndex(int _value)
	{
		m_nVBIndex = _value;
	}
//----------------------------------------------------------------------
	inline int VertexBuffer::getVBIndex() const
	{
		return m_nVBIndex;
	}
//----------------------------------------------------------------------
	inline IndexedBuffer* VertexBuffer::getIndexedBuffer() const
	{
		return m_VBDesc.indices;
	}
//----------------------------------------------------------------------
	inline void VertexBuffer::setPrimitivesNum(uint _number)
	{
		m_VBDesc.primitivesNum = _number;
	}
//----------------------------------------------------------------------
	inline uint VertexBuffer::getPrimitivesNum() const
	{
		return m_VBDesc.primitivesNum;
	}
//----------------------------------------------------------------------
	inline void VertexBuffer::setPrimitivesType(PRIMITIVE_TYPE _type)
	{
		m_VBDesc.primitivesType = _type;
	}
//----------------------------------------------------------------------
	inline PRIMITIVE_TYPE VertexBuffer::getPrimitivesType() const
	{
		return m_VBDesc.primitivesType;
	}
//----------------------------------------------------------------------
	inline uint VertexBuffer::getVerticesNum() const
	{
		return m_VBDesc.verticesNum;
	}
//----------------------------------------------------------------------
	inline void VertexBuffer::setVertexBufferDesc(const VERTEXBUFFER_DESC& _desc)
	{
		m_VBDesc = _desc;
	}
//----------------------------------------------------------------------
	inline VERTEXBUFFER_DESC& VertexBuffer::getDesc()
	{
		return m_VBDesc;
	}
//----------------------------------------------------------------------
	inline void VertexBuffer::setStreamIndex(uint _value)
	{
		m_nStreamIndex = _value;
	}
//----------------------------------------------------------------------
	inline uint VertexBuffer::getStreamIndex() const
	{
		return m_nStreamIndex;
	}
//----------------------------------------------------------------------
	inline void VertexBuffer::setVertexDeclaration(VertexDeclaration* _decl)
	{
		m_VBDesc.vertexDeclaration = _decl;
	}
//----------------------------------------------------------------------
	inline VertexDeclaration* VertexBuffer::getVertexDeclaration() const
	{
		return m_VBDesc.vertexDeclaration;
	}
//----------------------------------------------------------------------
}


#endif
