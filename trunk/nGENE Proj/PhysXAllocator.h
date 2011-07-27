/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysXAllocator.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PHYSXALLOCATOR_H_
#define __INC_PHYSXALLOCATOR_H_


#include "PrerequisitiesPhysX.h"

#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxUserAllocator.h"
#pragma warning(pop)


namespace nGENE
{
	/// Memory allocator for PhysX by Ageia Corp.
	class PhysXAllocator: public NxUserAllocator
	{
	public:
		PhysXAllocator();
		virtual ~PhysXAllocator();

		void reset();

		void* malloc(NxU32 size);
		void* malloc(NxU32 size, NxMemoryType type);
		void* mallocDEBUG(NxU32 size, const char* file, int line);
		void* mallocDEBUG(NxU32 size, const char* file, int line, const char* className, NxMemoryType type);
		void* realloc(void* memory, NxU32 size);
		void free(void* memory);

		NxU32* mMemBlockList;
		NxU32 mMemBlockListSize;
		NxU32 mMemBlockFirstFree;
		NxU32 mMemBlockUsed;

		NxI32 mNbAllocatedBytes;
		NxI32 mHighWaterMark;
		NxI32 mTotalNbAllocs;
		NxI32 mNbAllocs;
		NxI32 mNbReallocs;
	};
}


#endif
