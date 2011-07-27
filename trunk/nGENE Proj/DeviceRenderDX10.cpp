/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceRenderDX10.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DeviceRenderDX10.h"
#include "DX9Mapping.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo DeviceRenderDX10::Type(L"DeviceRenderDX10", &DeviceRender::Type);


	DeviceRenderDX10::DeviceRenderDX10(DXGI_SWAP_CHAIN_DESC& _params, HWND _hwnd, PROCESSING_TYPE _type):
			DeviceRender(_type),
			m_pRenderTargetView(NULL),
			m_Params(_params),
			m_hWnd(_hwnd)
	{
		init();
	}
//----------------------------------------------------------------------
	DeviceRenderDX10::~DeviceRenderDX10()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void DeviceRenderDX10::init()
	{
		// Determine DirectX vertex processing method, based on the
		// nGENE's one.
		D3D10_DRIVER_TYPE create = D3D10_DRIVER_TYPE_NULL;
		switch(m_Processing)
		{
		case PROCESSING_SOFTWARE: create = D3D10_DRIVER_TYPE_REFERENCE;
								  break;
		case PROCESSING_MIXED	: create = D3D10_DRIVER_TYPE_HARDWARE;
								  break;
		case PROCESSING_HARDWARE: create = D3D10_DRIVER_TYPE_HARDWARE;
								  break;
		default:
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Unknown vertex processing type. Defaulting to mixed.");
			create = D3D10_DRIVER_TYPE_HARDWARE;
			break;
		}

		// Create device
		m_hWnd = (m_hWnd ? m_hWnd : Engine::getSingleton().getTopLevelWndHandle());
		HRESULT hr = 0;
		dword dwFlags = 0;

		if(FAILED(D3D10CreateDeviceAndSwapChain(NULL,
												D3D10_DRIVER_TYPE_HARDWARE,
												NULL,
												dwFlags,
												D3D10_SDK_VERSION,
												&m_Params,
												&m_pSwapChain,
												&m_pD3DDev)))
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Creating 3D Device failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}


		// Create a render target view
		ID3D10Texture2D *pBackBuffer;
		m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**)&pBackBuffer);
		

		m_pD3DDev->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
		pBackBuffer->Release();


		m_pD3DDev->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);


		D3D10_VIEWPORT viewport;
		viewport.Width = m_Params.BufferDesc.Width;
		viewport.Height = m_Params.BufferDesc.Height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		m_pD3DDev->RSSetViewports(1, &viewport);
	}
//----------------------------------------------------------------------
	void DeviceRenderDX10::cleanup()
	{
		m_pD3DDev->ClearState();

		NGENE_RELEASE(m_pRenderTargetView);
		NGENE_RELEASE(m_pSwapChain);
		NGENE_RELEASE(m_pD3DDev);
	}
//----------------------------------------------------------------------
	void DeviceRenderDX10::onDeviceLost()
	{
/*		HRESULT res = m_pD3DDev->TestCooperativeLevel();

		if(res == D3DERR_DEVICELOST)
		{
			// Device is lost and could not be reset... so let's wait a while
			m_bIsLost = true;
			Renderer::getSingleton().cleanupManagedResources();
			Sleep(100);

			return;
		}
		else if(res == D3DERR_DEVICENOTRESET)
		{
			// Device is lost but we can reset it now
			m_bIsLost = false;
			m_pD3DDev->Reset(&m_Params);
		}*/
	}
//----------------------------------------------------------------------
	bool DeviceRenderDX10::testCapabilities(nGENE_CAPABILITIES _caps, void* _param)
	{
		/*D3DCAPS9 Caps;
		m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps);

		// Find the cap
		switch(_caps)
		{
		case CAPS_MAX_ANISOTROPY: memcpy(_param, &Caps.MaxAnisotropy, sizeof(dword));
								  break;
		case CAPS_MAX_CLIPPLANES: memcpy(_param, &Caps.MaxUserClipPlanes, sizeof(dword));
								  break;
		case CAPS_MAX_INDICES_COUNT: memcpy(_param, &Caps.MaxVertexIndex, sizeof(dword));
								  break;
		case CAPS_MAX_LIGHTS:	  memcpy(_param, &Caps.MaxActiveLights, sizeof(dword));
								  break;
		case CAPS_MAX_RTS:		  memcpy(_param, &Caps.NumSimultaneousRTs, sizeof(dword));
								  break;
		case CAPS_MAX_TEXTURE_HEIGHT: memcpy(_param, &Caps.MaxTextureHeight, sizeof(dword));
								  break;
		case CAPS_MAX_TEXTURE_WIDTH: memcpy(_param, &Caps.MaxTextureWidth, sizeof(dword));
								  break;
		default:
			// Cap does not exist
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Unknown or unused device cap: %d", static_cast<int>(_caps));
			break;
		}*/

		return true;
	}
//----------------------------------------------------------------------
	void DeviceRenderDX10::reset()
	{
	}
//----------------------------------------------------------------------
	void DeviceRenderDX10::beginRender()
	{
		float ClearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f}; //red,green,blue,alpha
		m_pD3DDev->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	}
//----------------------------------------------------------------------
	void DeviceRenderDX10::endRender()
	{
		m_pSwapChain->Present(0, 0);
	}
//----------------------------------------------------------------------
}
