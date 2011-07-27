/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexBufferDX9.cpp
Version:	0.11
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "VertexBufferDX9.h"
#include "DeviceRender.h"
#include "DX9Mapping.h"
#include "IndexedBuffer.h"
#include "VertexBufferManager.h"
#include "VertexDeclarationDX9.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo VertexBufferDX9::Type(L"VertexBufferDX9", &VertexBuffer::Type);


	VertexBufferDX9::VertexBufferDX9():
		m_pVB(NULL),
		m_pD3DDev(NULL)
	{
	}
//----------------------------------------------------------------------
	VertexBufferDX9::VertexBufferDX9(const VERTEXBUFFER_DESC& _desc):
		VertexBuffer(_desc),
		m_pVB(NULL),
		m_pD3DDev(NULL)
	{
	}
//----------------------------------------------------------------------
	VertexBufferDX9::~VertexBufferDX9()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::init()
	{
		dword dwUsage = 0;
		D3DPOOL pool = D3DPOOL_MANAGED;
		if(m_VBDesc.type == VB_DYNAMIC)
		{
			dwUsage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
			pool = D3DPOOL_DEFAULT;
		}

		// Create DirectX 9.0 Vertex Buffer
		IDirect3DDevice9* pRenderer = ((IDirect3DDevice9*)(Renderer::getSingleton().getDeviceRender().getDevicePtr()));
		HRESULT hr = 0;
		if(FAILED(hr = pRenderer->CreateVertexBuffer(m_VBDesc.verticesNum * m_VBDesc.vertexSize, dwUsage, 0, 
							   pool, &m_pVB, NULL)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}

		m_pD3DDev = reinterpret_cast<IDirect3DDevice9*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::cleanup()
	{
		NGENE_RELEASE(m_pVB);
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::lock(uint _start, uint _count, void** _vertices)
	{
		// Lock DirectX 9.0 Vertex Buffer
		dword dwLock = 0;
		if(m_VBDesc.type == VB_DYNAMIC)
			dwLock = (m_VBDesc.discardData ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);

		SharedObject sharedObj(&m_CriticalSection);

		m_pVB->Lock(_start, _count * m_VBDesc.vertexSize, _vertices, dwLock);
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::unlock()
	{
		SharedObject sharedObj(&m_CriticalSection);

		m_pVB->Unlock();
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::setVertexData(void* _data, uint _start, uint _count)
	{
		SharedObject sharedObj(&m_CriticalSection);

		void* pVertices = NULL;
		lock(_start, _count, &pVertices);

		if(_data)
			memcpy(pVertices, _data, _count * m_VBDesc.vertexSize);

		unlock();
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::bind()
	{
		m_pD3DDev->SetStreamSource(m_nStreamIndex, m_pVB, 0, m_VBDesc.vertexSize);
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::prepare()
	{
		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		manager.bindVertexBuffer(this);
		manager.bindVertexDeclaration(m_VBDesc.vertexDeclaration);

		if(!m_nStreamIndex && m_VBDesc.indices)
		{
			manager.bindIndexedBuffer(m_VBDesc.indices);
		}
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::render()
	{
		// Nothing to render
		if(!m_VBDesc.primitivesNum)
			return;

		if(m_VBDesc.indices)
		{
			m_pD3DDev->DrawIndexedPrimitive(static_cast<D3DPRIMITIVETYPE>(m_VBDesc.primitivesType),
											0, 0, m_VBDesc.verticesNum, 0, m_VBDesc.primitivesNum);
		}
		else
		{
			m_pD3DDev->DrawPrimitive(static_cast<D3DPRIMITIVETYPE>(m_VBDesc.primitivesType),
									 0, m_VBDesc.primitivesNum);
		}
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::render(uint _verticesNum, uint _primitivesNum)
	{
		if(m_VBDesc.indices)
		{
			m_pD3DDev->DrawIndexedPrimitive(static_cast<D3DPRIMITIVETYPE>(m_VBDesc.primitivesType),
											0, 0, _verticesNum, 0, _primitivesNum);
		}
		else
		{
			m_pD3DDev->DrawPrimitive(static_cast<D3DPRIMITIVETYPE>(m_VBDesc.primitivesType),
									 0, _primitivesNum);
		}
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::render(const VERTEXBUFFER_DESC& _desc)
	{
		if(_desc.indices)
		{
			m_pD3DDev->DrawIndexedPrimitiveUP(static_cast<D3DPRIMITIVETYPE>(_desc.primitivesType),
											  0, _desc.verticesNum, _desc.primitivesNum,
											  _desc.indices->getDesc().indices, D3DFMT_INDEX16,
											  _desc.vertices, _desc.vertexSize);
		}
		else
		{
			m_pD3DDev->DrawPrimitiveUP(static_cast<D3DPRIMITIVETYPE>(_desc.primitivesType),
									   _desc.primitivesNum, _desc.vertices, _desc.vertexSize);
		}
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::getVertexData(void* _data, uint _start, uint _count) const
	{
		void* pVertices;
		dword dwLock = 0;
		if(m_VBDesc.type == VB_DYNAMIC)
			dwLock = (m_VBDesc.discardData ? D3DLOCK_DISCARD : D3DLOCK_NOOVERWRITE);

		m_pVB->Lock(_start, _count * m_VBDesc.vertexSize, &pVertices, 0);
		if(pVertices)
			memcpy(_data, pVertices, _count * m_VBDesc.vertexSize);
		m_pVB->Unlock();
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::onDeviceLost()
	{
		if(m_VBDesc.type != VB_DYNAMIC)
			return;

		cleanup();
	}
//----------------------------------------------------------------------
	void VertexBufferDX9::onDeviceReset()
	{
		if(m_VBDesc.type != VB_DYNAMIC)
			return;

		init();
	}
//----------------------------------------------------------------------
}