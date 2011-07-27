/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceRenderDX9.cpp
Version:	0.11
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DeviceRenderDX9.h"
#include "DX9Mapping.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo DeviceRenderDX9::Type(L"DeviceRenderDX9", &DeviceRender::Type);


	DeviceRenderDX9::DeviceRenderDX9(IDirect3D9* _d3d,
		D3DPRESENT_PARAMETERS& _params,
		HWND _hwnd,
		PROCESSING_TYPE _type,
		bool _profile,
		bool _multiThreaded):
			m_pD3DDev(NULL),
			DeviceRender(_type),
			m_Params(_params),
			m_pD3D(_d3d),
			m_hWnd(_hwnd),
			m_bProfile(_profile),
			m_bMultiThreaded(_multiThreaded)
	{
		init();
	}
//----------------------------------------------------------------------
	DeviceRenderDX9::~DeviceRenderDX9()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void DeviceRenderDX9::init()
	{
		// Determine DirectX vertex processing method, based on the
		// nGENE's one.
		dword create = 0;
		switch(m_Processing)
		{
		case PROCESSING_SOFTWARE: create = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
								  break;
		case PROCESSING_MIXED	: create = D3DCREATE_MIXED_VERTEXPROCESSING;
								  break;
		case PROCESSING_HARDWARE: create = D3DCREATE_HARDWARE_VERTEXPROCESSING;
								  break;
		case PROCESSING_HARDWARE_PURE: create = D3DCREATE_PUREDEVICE;
								  break;

		default:
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Unknown vertex processing type. Defaulting to mixed.");
			create = D3DCREATE_MIXED_VERTEXPROCESSING;
			break;
		}

		// Set multi-threading flag
		if(m_bMultiThreaded)
			create |= D3DCREATE_MULTITHREADED;

		uint adapter = D3DADAPTER_DEFAULT;
		D3DDEVTYPE devType = D3DDEVTYPE_HAL;

		if(m_bProfile)
		{
			// Look for PerfHUD
			for(uint i = 0; i < m_pD3D->GetAdapterCount(); ++i)
			{
				D3DADAPTER_IDENTIFIER9 adapterID;
				HRESULT result;
				result = m_pD3D->GetAdapterIdentifier(i, 0, &adapterID);
				if(strstr(adapterID.Description, "PerfHUD") != 0)
				{
					adapter = i;
					devType = D3DDEVTYPE_REF;
					break;
				}
			}
		}

		// Create device
		m_hWnd = (m_hWnd ? m_hWnd : Engine::getSingleton().getTopLevelWndHandle());
		HRESULT hr = 0;
		if(FAILED(hr = m_pD3D->CreateDevice(adapter,
											devType,
											m_hWnd,
											create,
											&m_Params,
											&m_pD3DDev)))
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Creating 3D Device failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}

		// Store caps to avoid calling this method again
		if(FAILED(hr = m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_DeviceCaps)))
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"DirectX 9c device caps could not be retrieved: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}
	}
//----------------------------------------------------------------------
	void DeviceRenderDX9::cleanup()
	{
		NGENE_RELEASE(m_pD3DDev);
		m_pD3D = NULL;
	}
//----------------------------------------------------------------------
	void DeviceRenderDX9::onDeviceLost()
	{
		HRESULT res = m_pD3DDev->TestCooperativeLevel();

		if(res == D3DERR_DEVICELOST)
		{
			// Device is lost and could not be reset... so let's wait a while
			if(!m_bIsLost)
			{
				m_bIsLost = true;
				Renderer::getSingleton().cleanupDefaultResources();

				// Handle event
				DeviceEvent evt;
				evt.type = DET_LOST;
				ListenerRegistry <DeviceEvent>::handleEvent(evt);

				Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__,
					__LINE__, L"Rendering device lost");
			}

			Sleep(100);

			return;
		}
		else if(res == D3DERR_DEVICENOTRESET)
		{
			// Device is lost but we can reset it now
			m_bIsLost = false;
			HRESULT hr = 0;


			// Reset the device itself
			if(FAILED(hr = m_pD3DDev->Reset(&m_Params)))
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
					__LINE__, DX9Mapping::getDXErrorDescription(hr).c_str());
				return;
			}

			// Handle event
			DeviceEvent evt;
			evt.type = DET_RESET;
			ListenerRegistry <DeviceEvent>::handleEvent(evt);

			// We have to manually reset frustum as it is not DirectX resource
			Engine::getSingleton().resetCameraAndFrustum();

			Renderer::getSingleton().resetRenderStates();
	
			Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Rendering device successfully restored");
		}
	}
//----------------------------------------------------------------------
	bool DeviceRenderDX9::testCapabilities(nGENE_CAPABILITIES _caps, void* _param)
	{
		// Find the cap
		switch(_caps)
		{
		case CAPS_MAX_ANISOTROPY: memcpy(_param, &m_DeviceCaps.MaxAnisotropy, sizeof(dword));
								  break;

		case CAPS_MIN_ANISOTROPY: memset(_param, (m_DeviceCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC ? true : false),
									  sizeof(bool));
								  break;

		case CAPS_MAG_ANISOTROPY: memset(_param, (m_DeviceCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC ? true : false),
									  sizeof(bool));
								  break;

		case CAPS_MAX_CLIPPLANES: memcpy(_param, &m_DeviceCaps.MaxUserClipPlanes, sizeof(dword));
								  break;

		case CAPS_MAX_INDICES_COUNT: memcpy(_param, &m_DeviceCaps.MaxVertexIndex, sizeof(dword));
								  break;

		case CAPS_MAX_PRIMITIVES_COUNT: memcpy(_param, &m_DeviceCaps.MaxPrimitiveCount, sizeof(dword));
								  break;

		case CAPS_MAX_LIGHTS:	  memcpy(_param, &m_DeviceCaps.MaxActiveLights, sizeof(dword));
								  break;

		case CAPS_MAX_RTS:		  memcpy(_param, &m_DeviceCaps.NumSimultaneousRTs, sizeof(dword));
								  break;

		case CAPS_MAX_TEXTURE_HEIGHT: memcpy(_param, &m_DeviceCaps.MaxTextureHeight, sizeof(dword));
								  break;

		case CAPS_MAX_TEXTURE_WIDTH: memcpy(_param, &m_DeviceCaps.MaxTextureWidth, sizeof(dword));
								  break;

		case CAPS_VERTEX_SHADER:  strcpy((char*)_param, D3DXGetVertexShaderProfile(m_pD3DDev));
								  break;

		case CAPS_GEOMETRY_SHADER:strcpy((char*)_param, "no support");
								  break;

		case CAPS_PIXEL_SHADER:	  strcpy((char*)_param, D3DXGetPixelShaderProfile(m_pD3DDev));
								  break;

		default:
			// Cap does not exist
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Unknown or unused device cap: %d", static_cast<int>(_caps));
			break;
		}

		return true;
	}
//----------------------------------------------------------------------
	void DeviceRenderDX9::reset(D3DPRESENT_PARAMETERS* _params)
	{
		m_Params = *_params;
		m_pD3DDev->Reset(&m_Params);
	}
//----------------------------------------------------------------------
	void DeviceRenderDX9::beginRender()
	{
		m_pD3DDev->BeginScene();
	}
//----------------------------------------------------------------------
	void DeviceRenderDX9::endRender()
	{
		m_pD3DDev->EndScene();
		m_pD3DDev->Present(NULL, NULL, NULL, NULL);
	}
//----------------------------------------------------------------------
}
