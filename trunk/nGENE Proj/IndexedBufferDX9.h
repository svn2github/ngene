/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IndexedBufferDX9.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_INDEXEDBUFFERDX9_H_
#define __INC_INDEXEDBUFFERDX9_H_


#include "IndexedBuffer.h"
#include "TypeInfo.h"
#include "VertexBufferDX9.h"


namespace nGENE
{
	/** IndexedBuffer implementation for DirectX 9.0c.
	*/
	class IndexedBufferDX9: public IResourceDX9, public IndexedBuffer
	{
		EXPOSE_TYPE
	private:
		/// DirectX 9 index buffer
		IDirect3DIndexBuffer9* m_pIVB;
		IDirect3DDevice9* m_pD3DDev;


		void onDeviceLost();
		void onDeviceReset();

	public:
		IndexedBufferDX9(const INDEXEDBUFFER_DESC& _desc);
		~IndexedBufferDX9();

		void init();
		void cleanup();

		void lock(uint _start=0, uint _count=0, void** _indices=NULL);
		void unlock();

		void setIndexData(void* _data, uint _start, uint _count);
		void getIndexData(void* _data, uint _start, uint _count) const;

		void bind();
	};


//----------------------------------------------------------------------
//----------------------------------------------------------------------


	/// Factory to be used for DirectX 9.0c IndexedBuffer class objects creation.
	class nGENEDLL IndexedBufferDX9Factory: public IndexedBufferFactory
	{
	public:
		IndexedBuffer* create(const INDEXEDBUFFER_DESC& _desc) {return new IndexedBufferDX9(_desc);}
	};
}


#endif