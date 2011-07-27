/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MemoryManager.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MEMORYMANAGER_H_
#define __INC_MEMORYMANAGER_H_


#include "HashTable.h"
#include "Prerequisities.h"


namespace nGENE
{
	typedef struct SMemoryBlock
	{
		bool bIsFree;
		uint nSize;
	} MEMORY_BLOCK;

	enum MEMORY_BLOCK_STATE
	{
		MBS_FREE	= 0,
		MBS_OCCUPIED= 1
	};


	class MemoryAllocator
	{
	protected:
		byte* m_pMemory;
		dword m_dwSize;
		dword m_dwChunkSize;
		dword m_dwRows;

		byte* getState(dword _index);

		void* realloc(uint _newSize) {}

	public:
		MemoryAllocator(uint _size, uint _chunkSize);
		~MemoryAllocator();

		void init(uint _size, uint _chunkSize);

		void* malloc(uint _size);
		void free(void* _ptr);

		dword getBeginAddr() const;
		dword getEndAddr() const;
		dword getChunkSize() const;
	};

	struct SAllocList
	{
		MemoryAllocator* pAllocator;
		SAllocList* pNext;
	};

	/** MemoryManager is responsible for allocating and deallocating
		of the nGENE's objects.
	*/
	class nGENEDLL MemoryManager
	{
	private:
		static SAllocList* m_pList;

		//static HashTable <dword, MemoryAllocator*> m_pAllocators;
		static bool m_bIsInitialized;				///< Is manager already initialized?

	public:
		MemoryManager();
		~MemoryManager();

		/// Initializes the manager.
		static void init();


		/** Allocates block of memory.
			@param
				_size number of bytes to be allocated.
		*/
		static void* malloc(uint _size);
		/// Frees the block of memory.
		static void free(void* _ptr);
	};
}

// Define in to receive some memory debug info
#ifdef _DEBUG_MEMORY
	inline void* operator new(size_t size) {return nGENE::MemoryManager::malloc(size);}
	inline void* operator new[](size_t size) {return nGENE::MemoryManager::malloc(size);}
	inline void  operator delete(void *ptr) {nGENE::MemoryManager::free(ptr);}
	inline void  operator delete[](void *ptr) {nGENE::MemoryManager::free(ptr);}
#endif


#endif