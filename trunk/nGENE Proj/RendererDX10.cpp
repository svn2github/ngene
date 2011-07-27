/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RendererDX10.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "RendererDX10.h"
#include "AlphaStage.h"
#include "Camera.h"
#include "DeviceRenderDX9.h"
#include "DX9Mapping.h"
#include "Frustum.h"
#include "GeometryStage.h"
#include "LightStage.h"
#include "Material.h"
#include "NodeVisible.h"
#include "RenderPass.h"
#include "RenderStage.h"
#include "ScreenOverlay.h"
#include "Surface.h"
#include "TextureDX9.h"
#include "TextureManager.h"
#include "VertexBufferManager.h"
#include "WindowWin32.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo RendererDX10::Type(L"RendererDX10", &Renderer::Type);


	RendererDX10::RendererDX10(bool _windowed, uint _renderWndsNum, bool _multiThreaded):
		Renderer(_multiThreaded),
		m_matWorld(new Matrix4x4()),
		m_matView(new Matrix4x4()),
		m_matProj(new Matrix4x4()),
		m_pAlphaStage(NULL),
		m_pGeometryStage(NULL),
		m_pLightStage(NULL)
	{
		// These parameters could not be placed in an initialization
		// list as they are not class members.
		m_nRenderWindows = _renderWndsNum;
		m_DisplayMode.windowed = _windowed;
	}
//----------------------------------------------------------------------
	RendererDX10::~RendererDX10()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void RendererDX10::init()
	{
		Renderer::init();
	}
//----------------------------------------------------------------------
	void RendererDX10::cleanupManagedResources()
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::cleanup()
	{
	}
//----------------------------------------------------------------------
	Window* RendererDX10::createRenderWindow()
	{
		m_pWindow.push_back(new WindowWin32());
		uint index = m_pWindow.size() - 1;
		m_pWindow[index]->showWindow(true);

		return m_pWindow[index];
	}
//----------------------------------------------------------------------
	Window* RendererDX10::createRenderWindow(const WINDOW_DESC& _desc)
	{
		m_pWindow.push_back(new WindowWin32(_desc));
		uint index = m_pWindow.size() - 1;
		m_pWindow[index]->showWindow(true);

		return m_pWindow[index];
	}
//----------------------------------------------------------------------
	void RendererDX10::useWindow(uint _index)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setWindowed(bool _value)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::reset()
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::addToRender(Surface* _renderable, Material* _mat)
	{
		if(!_mat || !_renderable)
			return;

		wstring stName = _mat->getName();
		if((_mat->getRenderingOrder() != DO_POST) && (_mat->getRenderingOrder() != DO_PRE))
		{
			if(_mat->isTransparent() || !_mat->isLightable())
				m_pAlphaStage->addToRender(stName, _renderable);
			else
				m_pGeometryStage->addToRender(_mat, _renderable);
		}
		else if(_mat->getRenderingOrder() == DO_PRE)
			m_pGeometryStage->addToRender(_mat, _renderable);
		else
		{
			if(_mat->isTransparent() || !_mat->isLightable())
				m_pAlphaStage->addToRender(stName, _renderable);
		}
	}
//----------------------------------------------------------------------
	void RendererDX10::beginRender()
	{
		Renderer::beginRender();
	}
//----------------------------------------------------------------------
	void RendererDX10::endRender()
	{
		Renderer::endRender();
	}
//----------------------------------------------------------------------
	void RendererDX10::renderGeometry()
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::renderLight()
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::renderPostProcessing()
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::render()
	{
		// Pre-rendering configuration
		beginRender();

		m_pDeviceRender->beginRender();

			m_pDeviceRender->beginRender();

				renderStages();

			m_pDeviceRender->endRender();

		m_pDeviceRender->endRender();

		// Post-rendering configuration
		endRender();
	}
//----------------------------------------------------------------------
	void RendererDX10::renderStages()
	{
		RenderStage* pStage = NULL;
		for(uint i = 0; i < m_vRenderStages.size(); ++i)
		{
			pStage = m_vRenderStages[i];
			if(pStage->isEnabled())
				pStage->render();
		}
	}
//----------------------------------------------------------------------
	void RendererDX10::enumerateDevices()
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::createDevice()
	{
		m_pWindow.push_back(new WindowWin32());
		m_pWindow[0]->showWindow(true);

		ZeroMemory(&m_D3DParams, sizeof(DXGI_SWAP_CHAIN_DESC));
		m_D3DParams.BufferCount = 1;
		m_D3DParams.BufferDesc.Width = m_DisplayMode.width;
		m_D3DParams.BufferDesc.Height = m_DisplayMode.height;
		m_D3DParams.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_D3DParams.BufferDesc.RefreshRate.Numerator = 60;
		m_D3DParams.BufferDesc.RefreshRate.Denominator = 1;
		m_D3DParams.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_D3DParams.OutputWindow = (static_cast<WindowWin32*>(m_pWindow[0]))->getHandle();
		m_D3DParams.SampleDesc.Count = 1;
		m_D3DParams.SampleDesc.Quality = 0;
		m_D3DParams.Windowed = m_DisplayMode.windowed;

		// Create DirectX 10.0 capable rendering device
		m_pDeviceRender.setPointer(new DeviceRenderDX10(m_D3DParams));
		m_pD3DDev = (ID3D10Device*)m_pDeviceRender->getDevicePtr();

		// Initialize rendering
		initRendering();

		DeviceEvent evt;
		evt.type = DET_CREATED;
		ListenerRegistry <DeviceEvent>::handleEvent(evt);
	}
//----------------------------------------------------------------------
	void RendererDX10::createDevice(HWND* _hwnd)
	{
		ZeroMemory(&m_D3DParams, sizeof(DXGI_SWAP_CHAIN_DESC));
		m_D3DParams.BufferCount = 1;
		m_D3DParams.BufferDesc.Width = m_DisplayMode.width;
		m_D3DParams.BufferDesc.Height = m_DisplayMode.height;
		m_D3DParams.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_D3DParams.BufferDesc.RefreshRate.Numerator = 60;
		m_D3DParams.BufferDesc.RefreshRate.Denominator = 1;
		m_D3DParams.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_D3DParams.OutputWindow = *_hwnd;
		m_D3DParams.SampleDesc.Count = 1;
		m_D3DParams.SampleDesc.Quality = 0;
		m_D3DParams.Windowed = m_DisplayMode.windowed;

		// Create DirectX 10.0 capable rendering device
		m_pDeviceRender.setPointer(new DeviceRenderDX10(m_D3DParams));
		m_pD3DDev = (ID3D10Device*)m_pDeviceRender->getDevicePtr();

		// Initialize rendering
		initRendering();

		DeviceEvent evt;
		evt.type = DET_CREATED;
		ListenerRegistry <DeviceEvent>::handleEvent(evt);
	}
//----------------------------------------------------------------------
	void RendererDX10::initRendering()
	{
		resetRenderStates();

		// Init vertex buffer manager
		m_pVBManager->init();

/*		m_pBackBuffer = TextureManager::getSingleton().createTexture(L"BackBuffer",
																	 m_DisplayMode.width,
																	 m_DisplayMode.height,
																	 TT_2D,
																	 TU_RENDERTARGET,
																	 TF_R8G8B8);*/


		// Create required stages
		m_pAlphaStage = new AlphaStage();
		addRenderStage(L"Alpha", m_pAlphaStage);
		m_pAlphaStage->init();
		m_pAlphaStage->setPriority(10);

		m_pGeometryStage = new GeometryStage();
		addRenderStage(L"Geometry", m_pGeometryStage);
		m_pGeometryStage->init();
		m_pGeometryStage->setPriority(1);

		m_pLightStage = new LightStage();
		addRenderStage(L"Light", m_pLightStage);
		m_pLightStage->init();
		m_pLightStage->setPriority(2);
	}
//----------------------------------------------------------------------
	void RendererDX10::clearBuffer(dword _buffer)
	{
		dword clearBuffer = 0;

		// Check, which buffers need to be cleared and
		// then perform clearing on them
		if(_buffer & CB_DEPTH)
			clearBuffer |= D3DCLEAR_ZBUFFER;
		if(_buffer & CB_STENCIL)
			clearBuffer |= D3DCLEAR_STENCIL;
		if(_buffer & CB_TARGET)
			clearBuffer |= D3DCLEAR_TARGET;
	}
//----------------------------------------------------------------------
	void RendererDX10::showCursor(bool _value)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setWorldMatrix(const Matrix4x4& _mat)
	{
		(*m_matWorld) = _mat;
	}
//----------------------------------------------------------------------
	Matrix4x4 RendererDX10::getWorldMatrix() const
	{
		return (*m_matWorld);
	}
//----------------------------------------------------------------------
	void RendererDX10::setViewMatrix(const Matrix4x4& _mat)
	{
		(*m_matView) = _mat;
	}
//----------------------------------------------------------------------
	Matrix4x4 RendererDX10::getViewMatrix() const
	{
		return (*m_matView);
	}
//----------------------------------------------------------------------
	void RendererDX10::setProjectionMatrix(const Matrix4x4& _mat)
	{
		(*m_matProj) = _mat;
	}
//----------------------------------------------------------------------
	Matrix4x4 RendererDX10::getProjectionMatrix() const
	{
		return (*m_matProj);
	}
//----------------------------------------------------------------------
	void RendererDX10::setMaterial(Material& _material)
	{
		Renderer::setMaterial(_material);
	}
//----------------------------------------------------------------------
	void RendererDX10::setLightEnabled(bool _value)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setAlphaTestEnabled(bool _value)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setAlphaBlendEnabled(bool _value)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setZWriteEnabled(bool _value)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setSourceBlending(BLEND_TYPE _mode)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setDestinationBlending(BLEND_TYPE _mode)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setCullingMode(CULL_MODE _cull)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setFillMode(FILL_MODE _fill)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setZBias(float _bias)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setAmbientLight(Colour& _ambient)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setTextureFilter(uint _sampler, dword _type, TEXTURE_FILTER _filter)
	{
	}
//----------------------------------------------------------------------
	void RendererDX10::setAnisotropyLevel(uint _sampler, uint _anisotropy)
	{
	}
//----------------------------------------------------------------------
//	void RendererDX10::setRenderState(D3DRENDERSTATETYPE _rs, int _value) const
//	{
//	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	Renderer* RendererDX10Factory::createRenderer(bool _windowed, uint _renderWndsNum,
		bool _multiThreading)
	{
		return (new RendererDX10(_windowed, _renderWndsNum, _multiThreading));
	}
//----------------------------------------------------------------------
}