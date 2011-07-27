/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScreenOverlayBatched.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ScreenOverlayBatched.h"
#include "Material.h"
#include "Renderer.h"
#include "RenderPass.h"
#include "RenderTechnique.h"
#include "StateManager.h"
#include "VertexBufferManager.h"
#include "VertexDeclaration.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ScreenOverlayBatched::Type(L"ScreenOverlayBatched", &ScreenOverlay::Type);


	ScreenOverlayBatched::ScreenOverlayBatched(const RECT_FLOAT& _rect, uint _batchSize):
		ScreenOverlay(_rect),
		m_nCurrentIndex(0),
		m_nGSMask(0),
		m_pIVB(NULL),
		m_pVertices(NULL),
		m_pIndices(NULL),
		m_kBatchSize(_batchSize)
	{
		init();
	}
//----------------------------------------------------------------------
	ScreenOverlayBatched::ScreenOverlayBatched(float _x, float _y, float _width,
		float _height, uint _batchSize):
		ScreenOverlay(_x, _y, _width, _height),
		m_nCurrentIndex(0),
		m_nGSMask(0),
		m_pIVB(NULL),
		m_pVertices(NULL),
		m_pIndices(NULL),
		m_kBatchSize(_batchSize)
	{
		init();
	}
//----------------------------------------------------------------------
	ScreenOverlayBatched::~ScreenOverlayBatched()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ScreenOverlayBatched::cleanup()
	{
		m_pMaterial = NULL;
		m_pVB = NULL;
	}
//----------------------------------------------------------------------
	void ScreenOverlayBatched::init()
	{	
		VertexBufferManager& manager = VertexBufferManager::getSingleton();


		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indicesNum = m_kBatchSize * 6;
		ivbDesc.type = IVB_DYNAMIC;
		m_pIVB = manager.createIndexedBuffer(ivbDesc);

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.verticesNum = m_kBatchSize * 4;
		vbDesc.primitivesNum = m_kBatchSize * 2;
		vbDesc.primitivesType = PT_TRIANGLELIST;
		vbDesc.type = VB_DYNAMIC;
		vbDesc.indices = m_pIVB;
		vbDesc.vertexSize = sizeof(SVertex2D);
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Declaration2D");
		m_pVB = manager.createVertexBuffer(vbDesc);
	}
//----------------------------------------------------------------------
	void ScreenOverlayBatched::lock()
	{
		// Lock buffers
		m_pVB->lock(0, 0, (void**)(&m_pVertices));
		m_pIVB->lock(0, 0, (void**)(&m_pIndices));
	}
//----------------------------------------------------------------------
	void ScreenOverlayBatched::unlock()
	{
		// Unlock buffers
		m_pVB->unlock();
		m_pIVB->unlock();
	}
//----------------------------------------------------------------------
	void ScreenOverlayBatched::begin()
	{
		Renderer& renderer = Renderer::getSingleton();

		// Save current render state
		RENDER_STATE renderState = renderer.getRenderState();
		StateManager::getSingleton().push(renderState);

		m_OldFillMode = renderer.getFillMode();
		m_OldCullMode = renderer.getCullingMode();
		m_nGSMask = renderer.getGlobalStatesMask();

		if(m_OldCullMode != CULL_NONE)
		{
			if(renderer.isGlobalState(GS_CULLING))
				renderer.setGlobalState(GS_CULLING, false);
			renderer.setCullingMode(CULL_NONE);
		}
		if(m_OldFillMode != FILL_SOLID)
		{
			if(renderer.isGlobalState(GS_FILLMODE))
				renderer.setGlobalState(GS_FILLMODE, false);
		}


		renderer.set2DMode();

		// Lock buffers
		lock();
	}
//----------------------------------------------------------------------
	void ScreenOverlayBatched::end()
	{
		// Draw anything left
		if(m_nCurrentIndex)
			flush();
		unlock();


		Renderer& renderer = Renderer::getSingleton();

		// Restore previous render states
		RENDER_STATE renderState = StateManager::getSingleton().pop();
		
		renderer.setCullingMode(m_OldCullMode);
		renderer.setFillMode(m_OldFillMode);
		renderer.setGlobalStatesMask(m_nGSMask);

		renderer.restore3DMode();
	}
//----------------------------------------------------------------------
	void ScreenOverlayBatched::flush()
	{
		if(!m_pMaterial)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Screen overlay does not have material assigned!");
			return;
		}


		unlock();

		m_pMaterial->bind();


		Renderer& renderer = Renderer::getSingleton();
		RENDER_STATE& renderState = StateManager::getSingleton().top();
		FILL_MODE oldFillMode = renderer.getFillMode();
		uint globalStates = renderer.getGlobalStatesMask();

		if(renderState.culling != CULL_NONE)
		{
			if(renderer.isGlobalState(GS_CULLING))
				renderer.setGlobalState(GS_CULLING, false);
			renderer.setCullingMode(CULL_NONE);
		}
		if(oldFillMode != FILL_SOLID)
		{
			if(renderer.isGlobalState(GS_FILLMODE))
				renderer.setGlobalState(GS_FILLMODE, false);
		}

		m_pVB->prepare();

		Matrix4x4 matWorld;
		renderer.setWorldMatrix(matWorld);

		RenderTechnique* pTechnique = m_pMaterial->getActiveRenderTechnique();
		for(uint i = 0; i < pTechnique->getPassesNum(); ++i)
		{
			RenderPass& pass = pTechnique->getRenderPass(i);
			// Scene pass, so skip it
			if(pass.getRenderingOrder() == DO_SCENE || !pass.isEnabled())
				continue;
			pass.bind();
			pass.update();

			m_pVB->render((m_nCurrentIndex << 2), (m_nCurrentIndex << 1));

			pass.unbind();
		}

		m_pMaterial->unbind();


		// Restore previous states
		renderer.setFillMode(oldFillMode);
		renderer.setGlobalStatesMask(globalStates);


		lock();

		m_nCurrentIndex = 0;
	}
//----------------------------------------------------------------------
	void ScreenOverlayBatched::render()
	{
	}
//----------------------------------------------------------------------
	void ScreenOverlayBatched::render(const Vector2& _position, Real _width,
		Real _height, const SRect<Real>& _src, const Colour& _colour)
	{
		if(m_nCurrentIndex == m_kBatchSize)
			flush();

		dword col = _colour.getDwordARGB();


		Real fWidthEps = m_vecOffset.x;
		Real fHeightEps = m_vecOffset.y;
		SVertex2D quad[] =
		{
			{
				Vector3(_position.x, _position.y, 0.5f),
				col,
				Vector2(_src.left + fWidthEps, _src.top + fHeightEps)
			},
			{
				Vector3(_position.x + _width, _position.y, 0.5f),
				col,
				Vector2(_src.right - fWidthEps, _src.top + fHeightEps)
			},
			{
				Vector3(_position.x + _width, _position.y + _height, 0.5f),
				col,
				Vector2(_src.right - fWidthEps, _src.bottom - fHeightEps)
			},
			{
				Vector3(_position.x, _position.y + _height, 0.5f),
				col,
				Vector2(_src.left + fWidthEps, _src.bottom - fHeightEps)
			}
		};

		uint index1 = (m_nCurrentIndex << 2);
		uint index2 = m_nCurrentIndex * 6;

		m_pVertices[index1++] = quad[0];
		m_pVertices[index1++] = quad[1];
		m_pVertices[index1++] = quad[2];
		m_pVertices[index1]   = quad[3];

		index1 = (m_nCurrentIndex << 2);

		m_pIndices[index2++] = index1;
		m_pIndices[index2++] = index1 + 2;
		m_pIndices[index2++] = index1 + 3;
		m_pIndices[index2++] = index1;
		m_pIndices[index2++] = index1 + 1;
		m_pIndices[index2]	 = index1 + 2;

		++m_nCurrentIndex;
	}
//----------------------------------------------------------------------
}
