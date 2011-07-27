/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScreenOverlay.cpp
Version:	0.16
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ScreenOverlay.h"
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
	TypeInfo ScreenOverlay::Type(L"ScreenOverlay", NULL);


	ScreenOverlay::ScreenOverlay(const RECT_FLOAT& _rect):
		m_vecPosition(_rect.left, _rect.top),
		m_fWidth(_rect.right - _rect.left),
		m_fHeight(_rect.bottom - _rect.top),
		m_nGSMask(0),
		m_pVB(NULL),
		m_pMaterial(NULL)
	{
		init();
	}
//----------------------------------------------------------------------
	ScreenOverlay::ScreenOverlay(float _x, float _y, float _width, float _height):
		m_vecPosition(_x, _y),
		m_fWidth(_width),
		m_fHeight(_height),
		m_nGSMask(0),
		m_pVB(NULL),
		m_pMaterial(NULL)
	{
		init();
	}
//----------------------------------------------------------------------
	ScreenOverlay::~ScreenOverlay()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ScreenOverlay::cleanup()
	{
		m_pMaterial = NULL;
		m_pVB = NULL;
	}
//----------------------------------------------------------------------
	void ScreenOverlay::init()
	{
		Real fWidthEps = 0.5f / m_fWidth;
		Real fHeightEps = 0.5f / m_fHeight;

		SVertex2D quad[] =
		{
			{
				Vector3(m_vecPosition.x, m_vecPosition.y, 0.5f),
				0xffffffff,
				Vector2(0.0f + fWidthEps, 0.0f + fHeightEps)
			},
			{
				Vector3(m_vecPosition.x + 1.0f, m_vecPosition.y, 0.5f),
				0xffffffff,
				Vector2(1.0f + fWidthEps, 0.0f + fHeightEps)
			},
			{
				Vector3(m_vecPosition.x + 1.0f, m_vecPosition.y + 1.0f, 0.5f),
				0xffffffff,
				Vector2(1.0f + fWidthEps, 1.0f + fHeightEps)
			},
			{
				Vector3(m_vecPosition.x, m_vecPosition.y + 1.0f, 0.5f),
				0xffffffff,
				Vector2(0.0f + fWidthEps, 1.0f + fHeightEps)
			}
		};
		
		VertexBufferManager& manager = VertexBufferManager::getSingleton();
		
		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = quad;
		vbDesc.verticesNum = 4;
		vbDesc.primitivesNum = 2;
		vbDesc.primitivesType = PT_TRIANGLEFAN;
		vbDesc.vertexSize = sizeof(SVertex2D);
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Declaration2D");
		m_pVB = manager.createVertexBuffer(vbDesc);
	}
//----------------------------------------------------------------------
	void ScreenOverlay::begin()
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
	}
//----------------------------------------------------------------------
	void ScreenOverlay::end()
	{
		Renderer& renderer = Renderer::getSingleton();

		// Restore previous render states
		RENDER_STATE renderState = StateManager::getSingleton().pop();

		renderer.setCullingMode(m_OldCullMode);
		renderer.setFillMode(m_OldFillMode);
		renderer.setGlobalStatesMask(m_nGSMask);

		renderer.restore3DMode();
	}
//----------------------------------------------------------------------
	void ScreenOverlay::render()
	{
		if(!m_pMaterial)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Screen overlay does not have material assigned!");
			return;
		}

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

		Matrix4x4 newWorld;
		Vector3 temp(m_fWidth, m_fHeight, 1.0f);
		newWorld.scale(temp);
		Matrix4x4 matTrans;
		matTrans.translate(m_vecPosition.x, m_vecPosition.y, 0.0f);
		newWorld.multiply(matTrans);
		renderer.setWorldMatrix(newWorld);


		m_pVB->prepare();
		RenderTechnique* pTechnique = m_pMaterial->getActiveRenderTechnique();
		for(uint i = 0; i < pTechnique->getPassesNum(); ++i)
		{
			RenderPass& pass = pTechnique->getRenderPass(i);
			// Scene pass, so skip it
			if(pass.getRenderingOrder() == DO_SCENE || !pass.isEnabled())
				continue;
			pass.bind();
			pass.update();

			m_pVB->render();

			pass.unbind();
		}

		m_pMaterial->unbind();

		// Restore previous states
		renderer.setFillMode(oldFillMode);
		renderer.setGlobalStatesMask(globalStates);
	}
//----------------------------------------------------------------------
	void ScreenOverlay::render(const Vector2& _position, Real _width, Real _height,
		const SRect<Real>& _src, const Colour& _colour)
	{
		if(!m_pMaterial)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Screen overlay does not have material assigned!");
			return;
		}

		dword col = _colour.getDwordARGB();


		SVertex2D quad[] =
		{
			{Vector3(0, 0, 0.5f), col, Vector2(_src.left, _src.top)},
			{Vector3(0 + 1.0f, 0, 0.5f), col, Vector2(_src.right, _src.top)},
			{Vector3(0 + 1.0f, 0 + 1.0f, 0.5f), col, Vector2(_src.right, _src.bottom)},
			{Vector3(0, 0 + 1.0f, 0.5f), col, Vector2(_src.left, _src.bottom)}
		};

		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = quad;
		vbDesc.verticesNum = 4;
		vbDesc.primitivesNum = 2;
		vbDesc.primitivesType = PT_TRIANGLEFAN;
		vbDesc.vertexSize = sizeof(SVertex2D);
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Declaration2D");

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

		Matrix4x4 newWorld;
		Vector3 temp(_width, _height, 1.0f);
		newWorld.scale(temp);
		Matrix4x4 matTrans;
		matTrans.translate(_position.x, _position.y, 0.0f);
		newWorld.multiply(matTrans);
		renderer.setWorldMatrix(newWorld);


		VertexBufferManager::getSingleton().bindVertexDeclaration(vbDesc.vertexDeclaration);
		RenderTechnique* pTechnique = m_pMaterial->getActiveRenderTechnique();
		for(uint i = 0; i < pTechnique->getPassesNum(); ++i)
		{
			RenderPass& pass = pTechnique->getRenderPass(i);
			// Scene pass, so skip it
			if(pass.getRenderingOrder() == DO_SCENE || !pass.isEnabled())
				continue;
			pass.bind();
			pass.update();

			m_pVB->render(vbDesc);
			VertexBufferManager::getSingleton().bindVertexBuffer(NULL);

			pass.unbind();
		}


		m_pMaterial->unbind();

		// Restore previous states
		renderer.setFillMode(oldFillMode);
		renderer.setGlobalStatesMask(globalStates);
	}
//----------------------------------------------------------------------
	void ScreenOverlay::setMaterial(Material* _material)
	{
		m_pMaterial = _material;
	}
//----------------------------------------------------------------------
	Material* ScreenOverlay::getMaterial() const
	{
		return m_pMaterial;
	}
//----------------------------------------------------------------------
}
