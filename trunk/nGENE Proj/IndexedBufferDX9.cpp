/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IndexedBufferDX9.cpp
Version:	0.07
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DeviceRender.h"
#include "IndexedBufferDX9.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo IndexedBufferDX9::Type(L"IndexedBufferDX9", &IndexedBuffer::Type);


	IndexedBufferDX9::IndexedBufferDX9(const INDEXEDBUFFER_DESC& _desc):
		IndexedBuffer(_desc),
		m_pIVB(NULL),
		m_pD3DDev(NULL)
	{
	}
//----------------------------------------------------------------------
	IndexedBufferDX9::~IndexedBufferDX9()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void IndexedBufferDX9::init()
	{
		DeviceRender& renderDevice = Renderer::getSingleton().getDeviceRender();

		// Find out which format has to be used (16- or 32-bit)
		D3DFORMAT indexFmt;
		if(m_IVBDesc.indicesNum > 65535 && m_IVBDesc.is32bit)
		{
			uint nMaxIndex = 0;
			renderDevice.testCapabilities(CAPS_MAX_INDICES_COUNT, &nMaxIndex);
			if(m_IVBDesc.indicesNum > nMaxIndex)
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"%d indices requested. Device supports: %d", m_IVBDesc.indicesNum, nMaxIndex);
				return;
			}
			indexFmt = D3DFMT_INDEX32;
		}
		else
		{
			indexFmt = D3DFMT_INDEX16;
			m_IVBDesc.indexSize = sizeof(ushort);
		}

		dword dwUsage = 0;
		D3DPOOL pool = D3DPOOL_MANAGED;
		if(m_IVBDesc.type == IVB_DYNAMIC)
		{
			dwUsage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
			pool = D3DPOOL_DEFAULT;
		}

		// Create DirectX index buffer
		m_pD3DDev = reinterpret_cast<IDirect3DDevice9*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());
		m_pD3DDev->CreateIndexBuffer(m_IVBDesc.indicesNum * m_IVBDesc.indexSize, dwUsage,
									 indexFmt, pool, &m_pIVB, NULL);
	}
//----------------------------------------------------------------------
	void IndexedBufferDX9::cleanup()
	{
		NGENE_RELEASE(m_pIVB);
	}
//----------------------------------------------------------------------
	void IndexedBufferDX9::lock(uint _start, uint _count, void** _indices)
	{
		SharedObject sharedObj(&m_CriticalSection);

		dword dwLock = 0;
		if(m_IVBDesc.type == IVB_DYNAMIC)
			dwLock = (m_IVBDesc.discardData ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);

		m_pIVB->Lock(_start, _count * m_IVBDesc.indexSize, reinterpret_cast<void**>(_indices), dwLock);
	}
//----------------------------------------------------------------------
	void IndexedBufferDX9::unlock()
	{
		SharedObject sharedObj(&m_CriticalSection);

		m_pIVB->Unlock();
	}
//----------------------------------------------------------------------
	void IndexedBufferDX9::setIndexData(void* _data, uint _start, uint _count)
	{
		SharedObject sharedObj(&m_CriticalSection);

		void* pIndices = NULL;
		lock(_start, _count, &pIndices);

		if(_data)
			memcpy(pIndices, _data, _count * m_IVBDesc.indexSize);

		unlock();
	}
//----------------------------------------------------------------------
	void IndexedBufferDX9::getIndexData(void* _data, uint _start, uint _count) const
	{
		void* pIndices;
		dword dwLock = 0;
		if(m_IVBDesc.type == IVB_DYNAMIC)
			dwLock = (m_IVBDesc.discardData ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);

		m_pIVB->Lock(_start, _count * m_IVBDesc.indexSize, reinterpret_cast<void**>(&pIndices), dwLock);
		if(pIndices)
			memcpy(_data, pIndices, _count * m_IVBDesc.indexSize);
		m_pIVB->Unlock();
	}
//----------------------------------------------------------------------
	void IndexedBufferDX9::bind()
	{
		m_pD3DDev->SetIndices(m_pIVB);
	}
//----------------------------------------------------------------------
	void IndexedBufferDX9::onDeviceLost()
	{
		if(m_IVBDesc.type != IVB_DYNAMIC)
			return;

		cleanup();
	}
//----------------------------------------------------------------------
	void IndexedBufferDX9::onDeviceReset()
	{
		if(m_IVBDesc.type != IVB_DYNAMIC)
			return;

		init();
	}
//----------------------------------------------------------------------
}