/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		BoundingVolume.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_BOUNDINGVOLUME_H_
#define __INC_BOUNDINGVOLUME_H_


#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** BoundingVolume.
		@remarks
			Bounding volume is an object approximating shape
			and size of the node in 3D making it possible
			to speed up complex calculations like frustum
			culling or collision-detection.
	*/
	class nGENEDLL BoundingVolume
	{
		EXPOSE_TYPE
	public:
		BoundingVolume();
		virtual ~BoundingVolume();

		/// Creates volume around given set of vertices.
		virtual void construct(VertexBuffer& _vb)=0;
		/// Creates volume around given set of vertices.
		virtual void construct(VertexBuffer* _vb)=0;
		/** Creates volume around given set of vertices.
			@param
				_vertices array of vertices.
			@param
				_count number of vertices in the array.
			@param
				_size size (in bytes) of a single vertex.
			@param
				_offset offset of a position data in bytes within single
				vertex.
		*/
		virtual void construct(void* _vertices, uint _count, uint _size, uint _offset)=0;
	};
}


#endif