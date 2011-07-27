/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RendererDX9.cpp
Version:	0.27
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "RendererDX9.h"
#include "AlphaStage.h"
#include "Camera.h"
#include "DeviceRenderDX9.h"
#include "DX9Mapping.h"
#include "Frustum.h"
#include "GeometryStage.h"
#include "GUIManager.h"
#include "LightStage.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "NodeVisible.h"
#include "RenderPass.h"
#include "PostStage.h"
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
	TypeInfo RendererDX9::Type(L"RendererDX9", &Renderer::Type);


	RendererDX9::RendererDX9(bool _windowed, uint _renderWndsNum, bool _multiThreaded):
		Renderer(_multiThreaded),
		m_pBackBuffer(NULL),
		m_pDeferred(NULL),
		m_pD3DDev(NULL),
		m_pAlphaStage(NULL),
		m_pPostStage(NULL),
		m_pGeometryStage(NULL),
		m_pLightStage(NULL)
	{
		// These parameters could not be placed in an initialization
		// list as they are not class members.
		m_nRenderWindows = _renderWndsNum;
		m_DisplayMode.windowed = _windowed;
	}
//----------------------------------------------------------------------
	RendererDX9::~RendererDX9()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void RendererDX9::init()
	{
		Renderer::init();

		// Create Direct3D
		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if(!m_pD3D)
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Creating Direct3D failed");
			return;
		}

		enumerateDevices();
	}
//----------------------------------------------------------------------
	void RendererDX9::cleanupDefaultResources()
	{
		// Remove all swap chains
		if(!m_pSwapChains.empty())
		{
			for(uint i = 0; i < m_pSwapChains.size(); ++i)
				NGENE_RELEASE(m_pSwapChains[i]);
			m_pSwapChains.clear();
		}
	}
//----------------------------------------------------------------------
	void RendererDX9::cleanup()
	{
		cleanupDefaultResources();

		if(!m_DisplayModes.empty())
			m_DisplayModes.clear();

		// Remove all windows
		if(!m_pWindow.empty())
		{
			for(uint i = 0; i < m_pWindow.size(); ++i)
			{
				m_pWindow[i]->closeWindow();
				NGENE_DELETE(m_pWindow[i]);
			}
			m_pWindow.clear();
		}

		// Remove stages
		NGENE_DELETE(m_pAlphaStage);
		NGENE_DELETE(m_pPostStage);
		NGENE_DELETE(m_pGeometryStage);
		NGENE_DELETE(m_pLightStage);

		NGENE_RELEASE(m_pD3D);
	}
//----------------------------------------------------------------------
	Window* RendererDX9::createRenderWindow()
	{
		m_pWindow.push_back(new WindowWin32());
		uint index = m_pWindow.size() - 1;
		m_pWindow[index]->showWindow(true);


		IDirect3DSwapChain9* pSwap = NULL;
		m_D3DParams.hDeviceWindow = (static_cast<WindowWin32*>(m_pWindow[index]))->getHandle();

		HRESULT hr;
		if(FAILED(hr = m_pD3DDev->CreateAdditionalSwapChain(&m_D3DParams, &pSwap)))
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Creating additional swap chain failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
		}
		m_pSwapChains.push_back(pSwap);


		return m_pWindow[index];
	}
//----------------------------------------------------------------------
	Window* RendererDX9::createRenderWindow(const WINDOW_DESC& _desc)
	{
		m_pWindow.push_back(new WindowWin32(_desc));
		uint index = m_pWindow.size() - 1;
		m_pWindow[index]->showWindow(true);


		IDirect3DSwapChain9* pSwap = NULL;
		m_D3DParams.hDeviceWindow = (static_cast<WindowWin32*>(m_pWindow[index]))->getHandle();
		HRESULT hr;
		if(FAILED(hr = m_pD3DDev->CreateAdditionalSwapChain(&m_D3DParams, &pSwap)))
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Creating additional swap chain failed: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
		}
		m_pSwapChains.push_back(pSwap);


		return m_pWindow[index];
	}
//----------------------------------------------------------------------
	void RendererDX9::useWindow(uint _index)
	{
		IDirect3DSurface9* pBack = NULL;
		
		// Switch rendering target
		HRESULT hr;
		if(FAILED(hr = m_pSwapChains[_index]->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBack)))
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Obtaining back buffer failed: %ls", DX9Mapping::getDXErrorDescription(hr));
		}
		m_pD3DDev->SetRenderTarget(0, pBack);

		pBack->Release();
	}
//----------------------------------------------------------------------
	void RendererDX9::setWindowed(bool _value)
	{
		// Display mode has not changed so return
		if(_value == m_DisplayMode.windowed)
			return;

		++Window::s_nWindowsNum;

		// Cleanup resources
		cleanupDefaultResources();
		{
			DeviceEvent evt;
			evt.type = DET_LOST;
			ListenerRegistry <DeviceEvent>::handleEvent(evt);
		}

		m_DisplayMode.windowed = _value;
		reset();

		// Restore resources
		{
			DeviceEvent evt;
			evt.type = DET_RESET;
			ListenerRegistry <DeviceEvent>::handleEvent(evt);
		}

		// We have to manually reset frustum as it is not DirectX resource
		Camera* pCamera = Engine::getSingleton().getActiveCamera();
		if(pCamera)
		{
			Frustum& frustum = pCamera->getFrustum();
			frustum.reset();
			frustum.update();
		}

		resetRenderStates();

		--Window::s_nWindowsNum;
	}
//----------------------------------------------------------------------
	void RendererDX9::setVSync(bool _value)
	{
		// Display mode has not changed so return
		if(_value == m_DisplayMode.vsync)
			return;

		// Cleanup resources
		cleanupDefaultResources();
		{
			DeviceEvent evt;
			evt.type = DET_LOST;
			ListenerRegistry <DeviceEvent>::handleEvent(evt);
		}

		Renderer::setVSync(_value);
		reset();

		// Restore resources
		{
			DeviceEvent evt;
			evt.type = DET_RESET;
			ListenerRegistry <DeviceEvent>::handleEvent(evt);
		}

		// We have to manually reset frustum as it is not DirectX resource
		Camera* pCamera = Engine::getSingleton().getActiveCamera();
		if(pCamera)
		{
			Frustum& frustum = pCamera->getFrustum();
			frustum.reset();
			frustum.update();
		}

		resetRenderStates();
	}
//----------------------------------------------------------------------
	void RendererDX9::reset()
	{
		// Set appropriate swap chain settings
		m_D3DParams.Windowed = m_DisplayMode.windowed;
		m_D3DParams.BackBufferWidth = m_DisplayMode.width;
		m_D3DParams.BackBufferHeight= m_DisplayMode.height;
		if(m_DisplayMode.vsync)
			m_D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		else
			m_D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		
		// Reset device
		HRESULT hr = 0;

		// Reset the device itself
		if(FAILED(hr = m_pD3DDev->Reset(&m_D3DParams)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, DX9Mapping::getDXErrorDescription(hr).c_str());
			return;
		}

		// Recreate swap chains
		if(m_pWindow.size() > 1)
		{
			for(uint i = 0; i < m_pWindow.size(); ++i)
			{
				IDirect3DSwapChain9* pSwap = NULL;
				m_D3DParams.hDeviceWindow = (static_cast<WindowWin32*>(m_pWindow[i]))->getHandle();
				HRESULT hr;
				if(FAILED(hr = m_pD3DDev->CreateAdditionalSwapChain(&m_D3DParams, &pSwap)))
				{
					Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
						L"Creating additional swap chain failed: %ls",
						DX9Mapping::getDXErrorDescription(hr).c_str());
				}

				m_pSwapChains.push_back(pSwap);
			}
		}
	}
//----------------------------------------------------------------------
	void RendererDX9::addToRender(Surface* _renderable, Material* _mat)
	{
		if(!_mat || !_renderable)
			return;

		wstring stName = _mat->getName();
		if((_mat->getRenderingOrder() != DO_POST) && (_mat->getRenderingOrder() != DO_PRE))
		{
			if(_mat->isTransparent() || !_mat->isLightable() || !_renderable->isLightable())
				m_pAlphaStage->addToRender(stName, _renderable);
			else
			{
				m_pGeometryStage->addToRender(_mat, _renderable);

				// Add surface for light rendering
				if(!m_pDeferred)
					m_pDeferred = MaterialManager::getSingleton().getLibrary(L"core")->getMaterial(L"light_gbuffer");
				m_pGeometryStage->addToRender(m_pDeferred, _renderable);
			}
		}
		else if(_mat->getRenderingOrder() == DO_PRE)
		{
			m_pGeometryStage->addToRender(_mat, _renderable);
		}
		else
		{
			if(_mat->isTransparent() || !_mat->isLightable())
				m_pAlphaStage->addToRender(stName, _renderable);
		}
	}
//----------------------------------------------------------------------
	void RendererDX9::updateBackBuffer()
	{
		IDirect3DSurface9* pSurface = NULL;
		HRESULT hr;
		if(m_pWindow.size() > 1)
		{
			if(FAILED(hr = m_pSwapChains[0]->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurface)))
			{
				Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Obtaining back buffer failed: %ls",
					DX9Mapping::getDXErrorDescription(hr));
			}
		}
		else
		{
			if(FAILED(hr = m_pD3DDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface)))
			{
				Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Obtaining back buffer failed: %ls",
					DX9Mapping::getDXErrorDescription(hr));
			}
		}

		((TextureDX9*)m_pBackBuffer)->setSurface(pSurface);


		NGENE_RELEASE(pSurface);
	}
//----------------------------------------------------------------------
	ITexture* RendererDX9::getBackBuffer() const
	{
		return m_pBackBuffer;
	}
//----------------------------------------------------------------------
	void RendererDX9::setBackBuffer(ITexture* _texture)
	{
		m_pBackBuffer = _texture;
	}
//----------------------------------------------------------------------
	void RendererDX9::beginRender()
	{
		Renderer::beginRender();

		// Check if device is still valid
		if(m_pD3DDev->TestCooperativeLevel() != D3D_OK)
			m_pDeviceRender->onDeviceLost();
	}
//----------------------------------------------------------------------
	void RendererDX9::endRender()
	{
		Renderer::endRender();
	}
//----------------------------------------------------------------------
	void RendererDX9::render()
	{
		// Pre-rendering configuration
		beginRender();

		// Device is lost, so there is no aim in rendering
		if(m_pDeviceRender->isDeviceLost())
			return;

		// Is windowed mode active?
		if((m_DisplayMode.windowed) && (m_pWindow.size() > 1))
		{
			for(uint j = 0; j < m_pWindow.size(); ++j)
			{
				/** Remember that RenderQueue cannot be simply cleared
					while rendering to one window (or pass) as it will
					contain nothing when rendering to the latter.
				*/
				useWindow(j);

				clearBuffer(CB_DEPTH | CB_TARGET);

				m_pD3DDev->BeginScene();

					if(onRender)
						onRender();
					else
						renderStages();

				m_pD3DDev->EndScene();

				m_pSwapChains[j]->Present(NULL, NULL, NULL, NULL, NULL);
			}
		}
		else
		{
			clearBuffer(CB_DEPTH | CB_TARGET);

			m_pDeviceRender->beginRender();

				if(onRender)
					onRender();
				else
					renderStages();

			m_pDeviceRender->endRender();
		}

		// Post-rendering configuration
		endRender();
	}
//----------------------------------------------------------------------
	void RendererDX9::renderStages()
	{
		RenderStage* pStage = NULL;
		for(uint i = 0; i < m_vRenderStages.size(); ++i)
		{
			pStage = m_vRenderStages[i];
			if(pStage->isEnabled())
			{
				pStage->onBeginFrame();
				pStage->render();
			}
		}
	}
//----------------------------------------------------------------------
	void RendererDX9::enumerateDevices()
	{
		uint nAdaptersNum = m_pD3D->GetAdapterCount();
		
		// List all available display modes, so the user will
		// be able to choose from them later on
		for(uint i = 0; i < nAdaptersNum; ++i)
		{
			uint nModesNum = m_pD3D->GetAdapterModeCount(i, D3DFMT_X8R8G8B8);

			for(uint j = 0; j < nModesNum; ++j)
			{
				D3DDISPLAYMODE mode;
				m_pD3D->EnumAdapterModes(i, D3DFMT_X8R8G8B8, j, &mode);
				uint temp = mode.Width;
				m_DisplayModes[temp].width = temp;
				m_DisplayModes[temp].height= mode.Height;
			}
		}
	}
//----------------------------------------------------------------------
	void RendererDX9::createDevice()
	{
		// Allocate memory for windows
		WINDOW_DESC desc;
		desc.height = m_DisplayMode.height;
		desc.width = m_DisplayMode.width;
		desc.name = L"nGENE Tech";
		for(uint i = 0; i < m_nRenderWindows; ++i)
		{
			m_pWindow.push_back(new WindowWin32(desc));
			m_pWindow[i]->showWindow(true);
		}

		HWND hWnd = (static_cast<WindowWin32*>(m_pWindow[0]))->getHandle();
		Engine::getSingleton().setTopLevelWndHandle(hWnd);


		// Set swap chain parameters
		ZeroMemory(&m_D3DParams, sizeof(D3DPRESENT_PARAMETERS));
		m_D3DParams.Windowed = m_DisplayMode.windowed;
		m_D3DParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_D3DParams.BackBufferFormat = D3DFMT_X8R8G8B8;
		m_D3DParams.BackBufferCount = 1;
		m_D3DParams.BackBufferWidth = m_DisplayMode.width;
		m_D3DParams.BackBufferHeight = m_DisplayMode.height;
		m_D3DParams.hDeviceWindow = hWnd;
		m_D3DParams.EnableAutoDepthStencil = true;
		m_D3DParams.AutoDepthStencilFormat = D3DFMT_D16;
		if(m_DisplayMode.vsync)
			m_D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		else
			m_D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


		// Create DirectX 9.0c capable rendering device
		m_pDeviceRender.setPointer(new DeviceRenderDX9(m_pD3D,
													   m_D3DParams,
													   NULL,
													   m_DisplayMode.processingType,
													   m_bProfile,
													   m_bMultiThreaded));
		m_pD3DDev = (IDirect3DDevice9*)m_pDeviceRender->getDevicePtr();

		// Create more swap chains
		if(m_pWindow.size() > 1)
		{
			for(uint i = 0; i < m_pWindow.size(); ++i)
			{
				IDirect3DSwapChain9* pSwap = NULL;
				m_D3DParams.hDeviceWindow = (static_cast<WindowWin32*>(m_pWindow[i]))->getHandle();
				HRESULT hr;
				if(FAILED(hr = m_pD3DDev->CreateAdditionalSwapChain(&m_D3DParams, &pSwap)))
				{
					Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
						L"Creating additional swap chain failed: %ls",
						DX9Mapping::getDXErrorDescription(hr).c_str());
				}

				m_pSwapChains.push_back(pSwap);
			}
		}


		// Initialize rendering
		initRendering();


		DeviceEvent evt;
		evt.type = DET_CREATED;
		ListenerRegistry <DeviceEvent>::handleEvent(evt);
	}
//----------------------------------------------------------------------
	void RendererDX9::createDevice(HWND* _hwnd)
	{
		// Set swap chain parameters
		ZeroMemory(&m_D3DParams, sizeof(D3DPRESENT_PARAMETERS));
		m_D3DParams.Windowed = m_DisplayMode.windowed;
		m_D3DParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_D3DParams.BackBufferFormat = D3DFMT_X8R8G8B8;
		m_D3DParams.BackBufferCount = 1;
		m_D3DParams.BackBufferWidth = m_DisplayMode.width;
		m_D3DParams.BackBufferHeight = m_DisplayMode.height;
		m_D3DParams.hDeviceWindow = *_hwnd;
		m_D3DParams.EnableAutoDepthStencil = true;
		m_D3DParams.AutoDepthStencilFormat = D3DFMT_D16;
		if(m_DisplayMode.vsync)
			m_D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		else
			m_D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


		// Create DirectX 9.0c capable rendering device
		m_pDeviceRender.setPointer(new DeviceRenderDX9(m_pD3D,
													   m_D3DParams,
													   *_hwnd,
													   m_DisplayMode.processingType,
													   m_bProfile,
													   m_bMultiThreaded));
		m_pD3DDev = (IDirect3DDevice9*)m_pDeviceRender->getDevicePtr();
		

		// Initialize rendering states
		initRendering();


		DeviceEvent evt;
		evt.type = DET_CREATED;
		ListenerRegistry <DeviceEvent>::handleEvent(evt);
	}
//----------------------------------------------------------------------
	void RendererDX9::initRendering()
	{
		resetRenderStates();

		// Init vertex buffer manager
		m_pVBManager->init();

		m_pBackBuffer = TextureManager::getSingleton().createTexture(L"BackBuffer",
																	 m_DisplayMode.width,
																	 m_DisplayMode.height,
																	 TT_2D,
																	 TU_RENDERTARGET,
																	 TF_R8G8B8);


		// Create required stages
		m_pAlphaStage = new AlphaStage();
		addRenderStage(L"Alpha", m_pAlphaStage);
		m_pAlphaStage->init();
		m_pAlphaStage->setPriority(20);

		m_pPostStage = new PostStage();
		addRenderStage(L"PostProcess", m_pPostStage);
		m_pPostStage->init();
		m_pPostStage->setPriority(100);

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
	void RendererDX9::clearBuffer(dword _buffer)
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

		HRESULT hr;
		if(FAILED(hr = m_pD3DDev->Clear(0,
										NULL,
										clearBuffer,
										m_ClearColour.getDwordARGB(),
										1.0f,
										0)))
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Clearing back buffer failed: %ls",
				DX9Mapping::getDXErrorDescription(hr));
		}
	}
//----------------------------------------------------------------------
	void RendererDX9::showCursor(bool _value)
	{
		GUIManager::getSingleton().setCursorEnabled(_value);

		SetCursor(NULL);
		if(_value && !GUIManager::getSingleton().getCursor())
		{
			HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW);
			setCursor(hCursor);
		}

		// Finally, call appropriate DirectX function
		m_pD3DDev->ShowCursor(_value);
		if(m_DisplayMode.windowed)
		{
			HWND hWnd = Engine::getSingleton().getTopLevelWndHandle();
			RECT rectClient;
			POINT pt;

			GetClientRect(hWnd, &rectClient);
			
			pt.x = rectClient.left;
			pt.y = rectClient.top;
			ClientToScreen(hWnd, &pt);
			rectClient.left = pt.x;
			rectClient.top = pt.y;

			pt.x = rectClient.right;
			pt.y = rectClient.bottom;
			ClientToScreen(hWnd, &pt);
			rectClient.right = pt.x;
			rectClient.bottom = pt.y;

			ClipCursor(&rectClient);
		}
		else
			ClipCursor(NULL);
	}
//----------------------------------------------------------------------
	void RendererDX9::setCursor(HCURSOR _cursor)
	{
		ICONINFO cursorInfo;
		BITMAP cursorBM;
		dword dwWidth;
		dword dwHeightSrc;
		dword dwHeightDest;
		dword* pBitmap;
		bool isBlackAndWhite;
		IDirect3DSurface9* pCursorSurf = NULL;
		COLORREF* pArrayMask = NULL;
		COLORREF* pArrayColor = NULL;
		BITMAPINFO bitmapInfo;
		HDC hdcScreen = NULL;
		HDC hdcMask = NULL;
		HDC hdcColor = NULL;
		HGDIOBJ hGDIOld;
		COLORREF color;
		COLORREF mask;

		if(!GetIconInfo(_cursor, &cursorInfo))
			return;

		if(!GetObject((HGDIOBJ)cursorInfo.hbmMask, sizeof(BITMAP), (LPVOID)&cursorBM))
			return;

		dwWidth = cursorBM.bmWidth;
		dwHeightSrc = cursorBM.bmHeight;

		if(!cursorInfo.hbmColor)
		{
			isBlackAndWhite = true;
			dwHeightDest = dwHeightSrc / 2;
		}
		else
		{
			isBlackAndWhite = false;
			dwHeightDest = dwHeightSrc;
		}

		if(FAILED(m_pD3DDev->CreateOffscreenPlainSurface(dwWidth, dwHeightDest, D3DFMT_A8R8G8B8,
			D3DPOOL_SCRATCH, &pCursorSurf, NULL)))
		{
			return;
		}

		pArrayMask = new dword[dwWidth * dwHeightSrc];
		
		memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
		bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biWidth = dwWidth;
		bitmapInfo.bmiHeader.biHeight = dwHeightSrc;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;


		hdcScreen = GetDC(NULL);
		hdcMask = CreateCompatibleDC(hdcScreen);
		if(!hdcMask)
			return;

		hGDIOld = SelectObject(hdcMask, cursorInfo.hbmMask);
		GetDIBits(hdcMask, cursorInfo.hbmMask, 0, dwHeightSrc, pArrayMask, &bitmapInfo, DIB_RGB_COLORS);
		SelectObject(hdcMask, hGDIOld);

		if(!isBlackAndWhite)
		{
			pArrayColor = new dword[dwWidth * dwHeightDest];
			hdcColor = CreateCompatibleDC(hdcScreen);
			if(!hdcColor)
				return;
			SelectObject(hdcColor, cursorInfo.hbmColor);
			GetDIBits(hdcMask, cursorInfo.hbmColor, 0, dwHeightDest, pArrayColor, &bitmapInfo, DIB_RGB_COLORS);
		}

		D3DLOCKED_RECT rect;
		pCursorSurf->LockRect(&rect, NULL, 0);
		pBitmap = (dword*)rect.pBits;
		for(uint y = 0; y < dwHeightDest; ++y)
		{
			for(uint x = 0; x < dwWidth; ++x)
			{
				if(isBlackAndWhite)
				{
					color = pArrayMask[dwWidth * (dwHeightDest - 1 - y) + x];
					mask = pArrayMask[dwWidth * (dwHeightSrc - 1 - y) + x];
				}
				else
				{
					color = pArrayColor[dwWidth * (dwHeightDest - 1 - y) + x];
					mask = pArrayMask[dwWidth * (dwHeightDest - 1 - y) + x];
				}

				if(!mask)
					pBitmap[dwWidth * y + x] = 0xff000000 | color;
				else
					pBitmap[dwWidth * y + x] = 0x00000000;
			}
		}

		pCursorSurf->UnlockRect();

		if(FAILED(m_pD3DDev->SetCursorProperties(cursorInfo.xHotspot, cursorInfo.yHotspot, pCursorSurf)))
		{
			return;
		}

		if(cursorInfo.hbmMask)
			DeleteObject(cursorInfo.hbmMask);
		if(cursorInfo.hbmColor)
			DeleteObject(cursorInfo.hbmColor);
		if(hdcScreen)
			ReleaseDC(NULL, hdcScreen);
		if(hdcColor)
			DeleteDC(hdcColor);
		if(hdcMask)
			DeleteDC(hdcMask);


		NGENE_DELETE_ARRAY(pArrayColor);
		NGENE_DELETE_ARRAY(pArrayMask);
		NGENE_RELEASE(pCursorSurf);
	}
//----------------------------------------------------------------------
	void RendererDX9::setWorldMatrix(const Matrix4x4& _mat)
	{
		D3DXMATRIX matWorld = DX9Mapping::matrix4ToD3DXMATRIX(_mat);
		m_pD3DDev->SetTransform(D3DTS_WORLD, &matWorld);
	}
//----------------------------------------------------------------------
	Matrix4x4 RendererDX9::getWorldMatrix() const
	{
		D3DXMATRIX matTemp;
		m_pD3DDev->GetTransform(D3DTS_WORLD, &matTemp);

		return DX9Mapping::D3DXMATRIXToMatrix4(matTemp);
	}
//----------------------------------------------------------------------
	void RendererDX9::setViewMatrix(const Matrix4x4& _mat)
	{
		D3DXMATRIX matView = DX9Mapping::matrix4ToD3DXMATRIX(_mat);
		m_pD3DDev->SetTransform(D3DTS_VIEW, &matView);
	}
//----------------------------------------------------------------------
	Matrix4x4 RendererDX9::getViewMatrix() const
	{
		D3DXMATRIX matTemp;
		m_pD3DDev->GetTransform(D3DTS_VIEW, &matTemp);

		return DX9Mapping::D3DXMATRIXToMatrix4(matTemp);
	}
//----------------------------------------------------------------------
	void RendererDX9::setProjectionMatrix(const Matrix4x4& _mat)
	{
		D3DXMATRIX matProj = DX9Mapping::matrix4ToD3DXMATRIX(_mat);
		m_pD3DDev->SetTransform(D3DTS_PROJECTION, &matProj);
	}
//----------------------------------------------------------------------
	Matrix4x4 RendererDX9::getProjectionMatrix() const
	{
		D3DXMATRIX matTemp;
		m_pD3DDev->GetTransform(D3DTS_PROJECTION, &matTemp);

		return DX9Mapping::D3DXMATRIXToMatrix4(matTemp);
	}
//----------------------------------------------------------------------
	void RendererDX9::setMaterial(Material& _material)
	{
		Renderer::setMaterial(_material);
	}
//----------------------------------------------------------------------
	void RendererDX9::setDisplayMode(SDisplayMode& _mode)
	{
		if((_mode.height != m_DisplayMode.height ||
		   _mode.width != m_DisplayMode.width ||
		   _mode.windowed != m_DisplayMode.windowed ||
		   _mode.vsync != m_DisplayMode.vsync) &&
		   m_pD3DDev)
		{
			// Cleanup resources
			cleanupDefaultResources();
			{
				DeviceEvent evt;
				evt.type = DET_LOST;
				ListenerRegistry <DeviceEvent>::handleEvent(evt);
			}

			Renderer::setDisplayMode(_mode);
			reset();

			// Restore resources
			{
				DeviceEvent evt;
				evt.type = DET_RESET;
				ListenerRegistry <DeviceEvent>::handleEvent(evt);
			}

			// We have to manually reset frustum as it is not DirectX resource
			if(Engine::getSingleton().hasActiveCamera())
			{
				Engine::getSingleton().getActiveCamera()->getFrustum().reset();
				Engine::getSingleton().getActiveCamera()->getFrustum().update();
			}

			resetRenderStates();

			// Reset post-process full-screen quad
			for(uint i = 0; i < m_vRenderStages.size(); ++i)
			{
				m_vRenderStages[i]->cleanup();
				m_vRenderStages[i]->init();
			}
		}
		else
			Renderer::setDisplayMode(_mode);
	}
//----------------------------------------------------------------------
	void RendererDX9::setLightEnabled(bool _value)
	{
		Renderer::setLightEnabled(_value);
		// It is necessary as nGENE Tech doesn't work on fixed-pipeline
		setRenderState(D3DRS_LIGHTING, false);
	}
//----------------------------------------------------------------------
	void RendererDX9::setAlphaTestEnabled(bool _value)
	{
		Renderer::setAlphaTestEnabled(_value);
		setRenderState(D3DRS_ALPHATESTENABLE, m_DisplayMode.state.isAlphaTest);
		dword function = 0;
		m_pD3DDev->GetRenderState(D3DRS_ALPHAFUNC, &function);
		if(function != D3DCMP_GREATEREQUAL)
			m_pD3DDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
//----------------------------------------------------------------------
	void RendererDX9::setAlphaTestReference(dword _value)
	{
		Renderer::setAlphaTestReference(_value);
		m_pD3DDev->SetRenderState(D3DRS_ALPHAREF,
			m_DisplayMode.state.alphaTestRef);
	}
//----------------------------------------------------------------------
	void RendererDX9::setSeparateAlphaBlendEnabled(bool _value)
	{
		Renderer::setSeparateAlphaBlendEnabled(_value);
		setRenderState(D3DRS_SEPARATEALPHABLENDENABLE,
			m_DisplayMode.state.separateAlphaBlend);
	}
//----------------------------------------------------------------------
	void RendererDX9::setAlphaBlendEnabled(bool _value)
	{
		Renderer::setAlphaBlendEnabled(_value);
		setRenderState(D3DRS_ALPHABLENDENABLE,
			m_DisplayMode.state.alphaBlend);
	}
//----------------------------------------------------------------------
	void RendererDX9::setZWriteEnabled(bool _value)
	{
		Renderer::setZWriteEnabled(_value);
		setRenderState(D3DRS_ZWRITEENABLE,
			m_DisplayMode.state.zWrite);
	}
//----------------------------------------------------------------------
	void RendererDX9::setZEnabled(bool _value)
	{
		Renderer::setZEnabled(_value);
		setRenderState(D3DRS_ZENABLE,
			m_DisplayMode.state.zBuffer);
	}
//----------------------------------------------------------------------
	void RendererDX9::setSourceBlending(BLEND_TYPE _mode)
	{
		Renderer::setSourceBlending(_mode);
		setRenderState(D3DRS_SRCBLEND,
			m_DisplayMode.state.source);
	}
//----------------------------------------------------------------------
	void RendererDX9::setDestinationBlending(BLEND_TYPE _mode)
	{
		Renderer::setDestinationBlending(_mode);
		setRenderState(D3DRS_DESTBLEND,
			m_DisplayMode.state.destination);
	}
//----------------------------------------------------------------------
	void RendererDX9::setSeparateAlphaSourceBlending(BLEND_TYPE _mode)
	{
		Renderer::setSeparateAlphaSourceBlending(_mode);
		dword mode = 0;
		m_pD3DDev->GetRenderState(D3DRS_SRCBLENDALPHA, &mode);
		if((BLEND_TYPE)mode != m_DisplayMode.state.alphaBlendSource)
		{
			setRenderState(D3DRS_SRCBLENDALPHA,
			m_DisplayMode.state.alphaBlendSource);
		}
	}
//----------------------------------------------------------------------
	void RendererDX9::setSeparateAlphaDestinationBlending(BLEND_TYPE _mode)
	{
		Renderer::setSeparateAlphaDestinationBlending(_mode);
		dword mode = 0;
		m_pD3DDev->GetRenderState(D3DRS_DESTBLENDALPHA, &mode);
		if((BLEND_TYPE)mode != m_DisplayMode.state.alphaBlendDestination)
		{
			setRenderState(D3DRS_DESTBLENDALPHA,
				m_DisplayMode.state.alphaBlendDestination);
		}
	}
//----------------------------------------------------------------------
	void RendererDX9::setCullingMode(CULL_MODE _cull)
	{
		Renderer::setCullingMode(_cull);
		dword mode = 0;
		dword cullMode = DX9Mapping::cullingToD3DCULL(m_DisplayMode.state.culling);
		m_pD3DDev->GetRenderState(D3DRS_CULLMODE, &mode);
		if(mode != cullMode)
			setRenderState(D3DRS_CULLMODE, cullMode);
	}
//----------------------------------------------------------------------
	void RendererDX9::setFillMode(FILL_MODE _fill)
	{
		Renderer::setFillMode(_fill);
		dword mode = 0;
		dword fillMode = DX9Mapping::fillModeToDword(m_DisplayMode.state.fillMode);
		m_pD3DDev->GetRenderState(D3DRS_FILLMODE, &mode);
		if(mode != fillMode)
			setRenderState(D3DRS_FILLMODE, fillMode);
	}
//----------------------------------------------------------------------
	void RendererDX9::setZBias(float _bias)
	{
		Renderer::setZBias(_bias);
		Maths::clamp <float> (m_DisplayMode.state.zBias, 0.0f, 16.0f);
		setRenderState(D3DRS_DEPTHBIAS,
			m_DisplayMode.state.zBias);
	}
//----------------------------------------------------------------------
	void RendererDX9::setAmbientLight(Colour& _ambient)
	{
		Renderer::setAmbientLight(_ambient);
		setRenderState(D3DRS_AMBIENT,
			m_DisplayMode.state.ambient.getDwordARGB());
	}
//----------------------------------------------------------------------
	void RendererDX9::setTextureFilter(uint _sampler, dword _type, TEXTURE_FILTER _filter)
	{
		uint sampler = _sampler;
		Maths::clamp <uint> (sampler, 0, 7);


		TEXTURE_FILTER filter = _filter;
		if(_type & FILTER_MIN)
		{
			if(filter == FILTER_ANISOTROPIC)
			{
				bool value = true;
				m_pDeviceRender->testCapabilities(nGENE::CAPS_MIN_ANISOTROPY, &value);
				if(!value)
					filter = FILTER_TRILINEAR;
			}
			else if(filter == FILTER_NONE)
			{
				Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"FILTER_NONE is invalid for FILTER_MIN");
				filter = FILTER_TRILINEAR;
			}

			m_pD3DDev->SetSamplerState(sampler,
									   D3DSAMP_MINFILTER,
									   DX9Mapping::textureFilterToD3DTEXTUREFILTERTYPE(filter));
		}
		if(_type & FILTER_MAG)
		{
			if(filter == FILTER_ANISOTROPIC)
			{
				bool value = true;
				m_pDeviceRender->testCapabilities(nGENE::CAPS_MAG_ANISOTROPY, &value);
				if(!value)
					filter = FILTER_TRILINEAR;
			}
			else if(filter == FILTER_NONE)
			{
				Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"FILTER_NONE is invalid for FILTER_MAG");
				filter = FILTER_TRILINEAR;
			}

			m_pD3DDev->SetSamplerState(sampler,
									   D3DSAMP_MAGFILTER,
									   DX9Mapping::textureFilterToD3DTEXTUREFILTERTYPE(filter));
		}
		if(_type & FILTER_MIP)
		{
			// You cannot use anisotropic filtering with mip-maps!
			if(filter == FILTER_ANISOTROPIC)
			{
				Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Mip-maps anisotropic filtering is not supported");
				filter = FILTER_TRILINEAR;
			}

			m_pD3DDev->SetSamplerState(sampler,
									   D3DSAMP_MIPFILTER,
									   DX9Mapping::textureFilterToD3DTEXTUREFILTERTYPE(filter));
		}

		// Set this as current state
		Renderer::setTextureFilter(sampler, _type, filter);
	}
//----------------------------------------------------------------------
	void RendererDX9::setTextureAddressingMode(uint _sampler, TEXTURE_ADDRESSING_MODE _mode)
	{
		Renderer::setTextureAddressingMode(_sampler, _mode);

		D3DTEXTUREADDRESS address = DX9Mapping::texureAddressingModeToD3DTEXTUREADDRESS(_mode);

		m_pD3DDev->SetSamplerState(_sampler, D3DSAMP_ADDRESSU, address);
		m_pD3DDev->SetSamplerState(_sampler, D3DSAMP_ADDRESSV, address);
		m_pD3DDev->SetSamplerState(_sampler, D3DSAMP_ADDRESSW, address);
	}
//----------------------------------------------------------------------
	void RendererDX9::setAnisotropyLevel(uint _sampler, uint _anisotropy)
	{
		uint sampler = _sampler;
		Maths::clamp <uint> (sampler, 0, 7);

		Renderer::setAnisotropyLevel(sampler, _anisotropy);
		m_pD3DDev->SetSamplerState(sampler, D3DSAMP_MAXANISOTROPY, _anisotropy); 
	}
//----------------------------------------------------------------------
	void RendererDX9::setScissorTestEnabled(bool _value)
	{
		Renderer::setScissorTestEnabled(_value);
		setRenderState(D3DRS_SCISSORTESTENABLE,
			m_DisplayMode.state.scissorTest);
	}
//----------------------------------------------------------------------
	void RendererDX9::setScissorRectangle(SRect <uint>& _rect)
	{
		Renderer::setScissorRectangle(_rect);

		RECT rect;
		rect.left = _rect.left;
		rect.right = _rect.right;
		rect.bottom = _rect.bottom;
		rect.top = _rect.top;

		m_pD3DDev->SetScissorRect(&rect);
	}
//----------------------------------------------------------------------
	void RendererDX9::setClippingPlanesFlag(uint _flag)
	{
		Renderer::setClippingPlanesFlag(_flag);
		m_pD3DDev->SetRenderState(D3DRS_CLIPPLANEENABLE, _flag);
	}
//----------------------------------------------------------------------
	void RendererDX9::setRenderState(D3DRENDERSTATETYPE _rs, int _value) const
	{
		m_pD3DDev->SetRenderState(_rs, _value);
	}
//----------------------------------------------------------------------
	void RendererDX9::setUserClipPlane(uint _index, const Plane& _plane)
	{
		float plane[] = {_plane.getPlaneNormal().x,
						 _plane.getPlaneNormal().y,
						 _plane.getPlaneNormal().z,
						 _plane.getPlaneD()};

		m_pD3DDev->SetClipPlane(_index, plane);
	}
//----------------------------------------------------------------------
	void RendererDX9::setBlendOperation(BLEND_OPERATION _operation)
	{
		Renderer::setBlendOperation(_operation);
		m_pD3DDev->SetRenderState(D3DRS_BLENDOP,
			(dword)m_DisplayMode.state.blendOP);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	Renderer* RendererDX9Factory::createRenderer(bool _windowed, uint _renderWndsNum,
		bool _multiThreading)
	{
		return (new RendererDX9(_windowed, _renderWndsNum, _multiThreading));
	}
//----------------------------------------------------------------------
}