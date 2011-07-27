/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexBufferDX9.h
Version:	0.09
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VERTEXBUFFERDX9_H_
#define __INC_VERTEXBUFFERDX9_H_


#include "IResourceDX9.h"
#include "TypeInfo.h"
#include "VertexBuffer.h"


namespace nGENE
{
	/** VertexBuffer class for DirectX 9.0c.
		@remarks
			Usage of this class should in most cases remain
			transparent for the programmer.
	*/
	class nGENEDLL VertexBufferDX9: public IResourceDX9, public VertexBuffer
	{
		EXPOSE_TYPE
	private:
		/// DirectX 9 Vertex Buffer
		IDirect3DVertexBuffer9* m_pVB;
		/// Pointer to Direct3D Device.
		IDirect3DDevice9* m_pD3DDev;


		void onDeviceChanged() {}
		void onDeviceLost();
		void onDeviceReset();

	public:
		VertexBufferDX9();
		VertexBufferDX9(const VERTEXBUFFER_DESC& _desc);
		~VertexBufferDX9();

		void init();
		void cleanup();

		void lock(uint _start=0, uint _count=0, void** _vertices=NULL);
		void unlock();
		void setVertexData(void* _data, uint _start=0, uint _count=100);

		/** Gets the underlying DirectX 9.0c Vertex Buffer.
			@remarks
				In the majority of cases there is no need to access
				vertex buffer in this way.
		*/
		IDirect3DVertexBuffer9* getVertexBuffer() const {return m_pVB;}

		void bind();
		void prepare();
		void render();
		void render(uint _verticesNum, uint _primitivesNum);
		void render(const VERTEXBUFFER_DESC& _desc);

		void getVertexData(void* _data, uint _start, uint _count) const;
	};


//----------------------------------------------------------------------
//----------------------------------------------------------------------


	/// Factory to be used for DirectX 9.0c VertexBuffer class objects creation.
	class nGENEDLL VertexBufferDX9Factory: public VertexBufferFactory
	{
	public:
		VertexBuffer* create(const VERTEXBUFFER_DESC& _desc) {return new VertexBufferDX9(_desc);}
	};
}


#endif
