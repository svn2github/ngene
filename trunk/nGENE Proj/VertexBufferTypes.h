/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexBufferTypes.h
Version:	0.02
Info:		This file defines some vertex buffer related types
			which are used by renderer.
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VERTEXBUFFERTYPES_H_
#define __INC_VERTEXBUFFERTYPES_H_


#include "Prerequisities.h"
#include "Vertex.h"


namespace nGENE
{
	/** Type of the primitives to render.
		@remarks
			Enumeration is actually wrapped D3DPRIMITIVETYPE type
			from DirectX 9.0c. This way no additional work is needed
			to use it with this API. In case of other rendering
			systems it might be essential to provide appropriate
			methods.
	*/
	enum PRIMITIVE_TYPE
	{
		PT_UNKNOWN		 = 0,
		PT_POINT		 = 1,
		PT_LINELIST		 = 2,
		PT_LINESTRIP	 = 3,
		PT_TRIANGLELIST  = 4,
		PT_TRIANGLESTRIP = 5,
		PT_TRIANGLEFAN	 = 6
	};

	/// Type of the VertexBuffer.
	enum VB_TYPE
	{
		VB_UNKNOWN,
		VB_STATIC,
		VB_DYNAMIC
	};

	/// Type of the IndexedBuffer.
	enum IVB_TYPE
	{
		IVB_UNKNOWN,
		IVB_STATIC,
		IVB_DYNAMIC
	};

	/// Description of the indexed buffer
	typedef struct SIndexedBufferDescription
	{
		uint indicesNum;						///< How many indices are used
		uint indexSize;							/**< Size of a single index item. Typically
													it is 16-bit or 32-bit unsigned integer
													meaning it is 2 or 4 bytes in size.	*/
		IVB_TYPE type;							///< Type of the indexed buffer (static/dynamic)
		void* indices;							///< Indices data
		bool is32bit;							///< Are indices represented as 32-bit integers?

		bool discardData;						/**< Flag used with VB_DYNAMIC type of buffer
												telling if data can be discarded. */


		SIndexedBufferDescription():
			indicesNum(0),
			indexSize(2),
			type(IVB_STATIC),
			indices(NULL),
			is32bit(false),
			discardData(true)
		{
		}
	} INDEXEDBUFFER_DESC;

	/// Description of the vertex buffer.
	typedef struct SVertexBufferDescription
	{
		dword verticesNum;						///< How many vertices are currently used
		dword vertexSize;						///< Size of a single vertex in bytes
		dword primitivesNum;					///< How many primitives is to be drawn
		PRIMITIVE_TYPE primitivesType;			///< Type of primitive being rendered
		VB_TYPE type;							///< Type of the vertex buffer (static/dynamic)
		void* vertices;							///< Vertices data
		IndexedBuffer* indices;					///< Associated indexed buffer
		VertexDeclaration* vertexDeclaration;	///< Pointer to the vertex declaration

		bool discardData;						/**< Flag used with VB_DYNAMIC type of buffer
													 telling if data can be discarded. */


		SVertexBufferDescription():
			verticesNum(0),
			vertexSize(sizeof(SVertex)),
			primitivesNum(0),
			primitivesType(PT_TRIANGLELIST),
			type(VB_STATIC),
			vertices(NULL),
			indices(NULL),
			vertexDeclaration(NULL),
			discardData(true)
		{
		}

		bool isValid() const
		{
			if(type == VB_UNKNOWN)
				return false;

			if((type == VB_STATIC && !vertices) || !vertexSize || !vertexDeclaration)
				return false;

			if((type == VB_STATIC && !verticesNum))
				return false;

			return true;
		}
	} VERTEXBUFFER_DESC;
}


#endif