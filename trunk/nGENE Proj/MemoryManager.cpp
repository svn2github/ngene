/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This progm_pMemory is free software.

File:		MemoryManager.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "MemoryManager.h"


namespace nGENE
{
	// Initialize static members
	bool MemoryManager::m_bIsInitialized = false;
	//HashTable <dword, MemoryAllocator*> MemoryManager::m_pAllocators;
	SAllocList* MemoryManager::m_pList = NULL;


	MemoryManager::MemoryManager()
	{
		init();
	}
//----------------------------------------------------------------------
	MemoryManager::~MemoryManager()
	{
		/*if(!m_pAllocators.empty())
		{
			HashTable <dword, MemoryAllocator*>::iterator iter;
			for(iter = m_pAllocators.begin(); iter != m_pAllocators.end(); ++iter)
				delete iter->second;

			m_pAllocators.clear();
		}*/

		SAllocList* pAllocator = m_pList;
		SAllocList* pTemp = NULL;
		while(pAllocator)
		{
			pTemp = pAllocator;
			pAllocator = pAllocator->pNext;
			::free(pTemp->pAllocator);
			::free(pTemp);
		}
	}
//----------------------------------------------------------------------
	void MemoryManager::init()
	{
		m_pList = (SAllocList*)::malloc(sizeof(SAllocList));
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(4 * 1000 * 50, 4);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));
		SAllocList* pHead = m_pList;

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(8 * 1000 * 50, 8);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(16 * 1000 * 50, 16);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(32 * 100 * 50, 32);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(64 * 100, 64);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(128 * 100, 128);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(256 * 100, 256);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(512 * 100, 512);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(1024 * 100, 1024);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(2048 * 100, 2048);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(4096 * 100, 4096);
		m_pList->pNext = (SAllocList*)::malloc(sizeof(SAllocList));

		m_pList = m_pList->pNext;
		m_pList->pAllocator = (MemoryAllocator*)::malloc(sizeof(MemoryAllocator));
		m_pList->pAllocator->init(65 * 100000, 65 * 100000);
		m_pList->pNext = NULL;

		m_pList = pHead;

		m_bIsInitialized = true;
	}
//----------------------------------------------------------------------
	void* MemoryManager::malloc(uint _size)
	{
		if(!m_bIsInitialized)
			init();

		if(!_size)
			return NULL;

		void* pResult = NULL;

	//	if(m_pAllocators.find(_size) == m_pAllocators.end())
	//		m_pAllocators[_size] = new MemoryAllocator(_size * 100, _size);
		SAllocList* pAllocator = m_pList;
		while(pAllocator)
		{
			// @todo if there is no free space in allocator proceed to the next one.
			if(_size <= pAllocator->pAllocator->getChunkSize())
				break;
			pAllocator = pAllocator->pNext;
		}

		Assert(pAllocator, "Appropriate allocator not found!");
		pResult = pAllocator->pAllocator->malloc(_size);//m_pAllocators[_size]->malloc(_size);
		Assert(pResult, "Failed to allocate memory!");

		return pResult;
	}
//----------------------------------------------------------------------
	void MemoryManager::free(void* _ptr)
	{
		if(!_ptr)
			return;

		// We have to find out to which allocator the pointer belongs
		dword addr = reinterpret_cast<int>(_ptr);
		SAllocList* pDeallocator = m_pList;
		while(pDeallocator)
		{
			if(addr >= pDeallocator->pAllocator->getBeginAddr() && addr <= pDeallocator->pAllocator->getEndAddr())
				break;
			pDeallocator = pDeallocator->pNext;
		}

		Assert(pDeallocator, "Memory could not be freed!");
		pDeallocator->pAllocator->free(_ptr);
	}
//----------------------------------------------------------------------
	MemoryAllocator::MemoryAllocator(uint _size, uint _chunkSize):
		m_dwSize(_size),
		m_dwChunkSize(_chunkSize)
	{
		init(_size, _chunkSize);
	}
//----------------------------------------------------------------------
	void MemoryAllocator::init(uint _size, uint _chunkSize)
	{
		if(!_chunkSize || !_size || _size < _chunkSize)
			exit(1);

		m_dwSize = _size;
		m_dwChunkSize = _chunkSize;

		m_pMemory = (byte*)::malloc(m_dwSize);
		memset(m_pMemory, 0xcc, m_dwSize);
		m_dwRows = m_dwSize / m_dwChunkSize;

		for(dword i = 1; i < m_dwSize; i += m_dwChunkSize + 1)
		{
			*getState(i) = MBS_FREE;
		}
	}
//----------------------------------------------------------------------
	MemoryAllocator::~MemoryAllocator()
	{
		// Free any undeallocated memory
		::free(m_pMemory);
	}
//----------------------------------------------------------------------
	byte* MemoryAllocator::getState(dword _index)
	{
		return (byte*)(&m_pMemory[--_index]);
	}
//----------------------------------------------------------------------
	void* MemoryAllocator::malloc(uint _size)
	{
		for(dword i = 1; i < m_dwSize - _size - 1; i += m_dwChunkSize + 1)
		{
			if(*getState(i) == MBS_FREE)
			{
				*getState(i) = MBS_OCCUPIED;
				return (void*)(&m_pMemory[i]);
			}
		}

		return NULL;
	}
//----------------------------------------------------------------------
	void MemoryAllocator::free(void* _ptr)
	{
		if(_ptr > (void*)&m_pMemory[m_dwSize - 1] || _ptr < (void*)&m_pMemory[0])
			return;

		dword free = (dword)(((byte*)_ptr) - &m_pMemory[0]);

		Assert(free, "Address in first byte!");
		Assert(*getState(free) == MBS_OCCUPIED, "Attempt to release invalid region!");

		*getState(free) = MBS_FREE;
//		_ptr = (void*)0x00000000;
	}
//----------------------------------------------------------------------
	dword MemoryAllocator::getBeginAddr() const
	{
		return (dword)(&m_pMemory[0]);
	}
//----------------------------------------------------------------------
	dword MemoryAllocator::getEndAddr() const
	{
		return (dword)(&m_pMemory[m_dwSize-1]);
	}
//----------------------------------------------------------------------
	dword MemoryAllocator::getChunkSize() const
	{
		return m_dwChunkSize;
	}
//----------------------------------------------------------------------
}