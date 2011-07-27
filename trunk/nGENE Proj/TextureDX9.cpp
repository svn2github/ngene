/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TextureDX9.cpp
Version:	0.18
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "TextureDX9.h"
#include "DeviceRender.h"
#include "DeviceRenderDX9.h"
#include "DX9Mapping.h"

#include <D3dx9tex.h>


namespace nGENE
{
	// Initialize static members
	TypeInfo TextureDX9::Type(L"TextureDX9", &ITexture::Type);


	TextureDX9::TextureDX9():
		m_pD3DDev(NULL),
		m_RTDesc(NULL),
		m_p2DTexture(NULL),
		m_pBackBuf(NULL),
		m_pSurface(NULL),
		m_nTextureUnit(0),
		m_bBoundAsTexture(false)
	{
		init();
	}
//----------------------------------------------------------------------
	TextureDX9::~TextureDX9()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void TextureDX9::init()
	{
		m_LockedStruct.rect = NULL;
		m_LockedStruct.box = NULL;
	}
//----------------------------------------------------------------------
	void TextureDX9::cleanup()
	{
		NGENE_RELEASE(m_pSurface);
		NGENE_RELEASE(m_pBackBuf);
		NGENE_RELEASE(m_pCubeTexture);
		NGENE_RELEASE(m_p3DTexture);
		NGENE_RELEASE(m_p2DTexture);

		NGENE_DELETE(m_LockedStruct.rect);
		NGENE_DELETE(m_LockedStruct.box);

		NGENE_DELETE(m_RTDesc);
	}
//----------------------------------------------------------------------
	void TextureDX9::getUsagePoolMipMaps(dword& _dxUsage, dword& _dxPool, uint& _mips)
	{
		if(m_TextureDesc.usage == TU_RENDERTARGET)
		{
			_dxUsage = D3DUSAGE_RENDERTARGET;
			_dxPool = 0;
			_mips = 1;
		}
		else if(m_TextureDesc.usage == TU_DYNAMIC)
		{
			_dxUsage = D3DUSAGE_DYNAMIC;
			_dxPool = 0;
			_mips = 1;
		}
	}
//----------------------------------------------------------------------
	void TextureDX9::create(uint _width, uint _height, TEXTURE_TYPE _type, TEXTURE_USAGE _usage,
		TEXTURE_FORMAT _format, uint _depth)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating empty texture");

		// Fill the texture description surface
		m_TextureDesc.format = _format;
		m_TextureDesc.type = _type;
		m_TextureDesc.usage = _usage;
		m_TextureDesc.width = _width;
		m_TextureDesc.height = _height;
		m_TextureDesc.depth = _depth;		

		// Specify DirectX settings
		dword dwUsage = 0;
		D3DPOOL pool = static_cast<D3DPOOL>(1);
		uint nMipMaps = 0;
		// We change from 'full-chain' only if texture is dynamic or serves
		// as a render target.
		getUsagePoolMipMaps(dwUsage, reinterpret_cast<dword&>(pool), nMipMaps);

		D3DFORMAT format = getFormat(_format, dwUsage);
		

		m_pD3DDev =	reinterpret_cast<IDirect3DDevice9*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());


		// Create appropriate texture
		HRESULT hr = 0;
		switch(_type)
		{
		// 1D texture
		case TT_1D: hr = D3DXCreateTexture(m_pD3DDev,
										   _width,
										   1,
										   nMipMaps,
										   dwUsage,
										   format,
										   pool,
										   &m_p2DTexture);
					if(hr != D3D_OK)
						m_p2DTexture = NULL;
					else
						m_LockedStruct.rect = new D3DLOCKED_RECT;
					break;

		// 2D texture
		case TT_2D: hr = D3DXCreateTexture(m_pD3DDev,
										   _width,
										   _height,
										   nMipMaps,
										   dwUsage,
										   format,
										   pool,
										   &m_p2DTexture);
					if(hr != D3D_OK)
						m_p2DTexture = NULL;
					else
						m_LockedStruct.rect = new D3DLOCKED_RECT;
					break;

		// 3D texture
		case TT_3D: hr = D3DXCreateVolumeTexture(m_pD3DDev,
												 _width,
												 _height,
												 _depth,
												 nMipMaps,
												 dwUsage,
												 format,
												 pool,
												 &m_p3DTexture);
					if(hr != D3D_OK)
						m_p3DTexture = NULL;
					else
						m_LockedStruct.box = new D3DLOCKED_BOX;
					break;

		// Cube texture
		case TT_CUBE: hr = D3DXCreateCubeTexture(m_pD3DDev,
												 _width,
												 nMipMaps,
												 dwUsage,
												 format,
												 pool,
												 &m_pCubeTexture);
					if(hr != D3D_OK)
						m_pCubeTexture = NULL;
					else
						m_LockedStruct.rect = new D3DLOCKED_RECT;
					break;
		}

		// Something went wrong - find out what
		if(hr != D3D_OK)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Texture creation failed. Reason: %ls",
				DX9Mapping::getDXErrorDescription(hr).c_str());
		}

		if(_usage == TU_RENDERTARGET)
			createRenderTarget(m_pD3DDev);
	}
//----------------------------------------------------------------------
	void TextureDX9::createFromFile(const wstring& _fileName, TEXTURE_TYPE _type,
			TEXTURE_USAGE _usage, TEXTURE_FORMAT _format)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Loading texture file: %ls", _fileName.c_str());

		// Fill the texture description surface
		m_TextureDesc.format = _format;
		m_TextureDesc.type = _type;
		m_TextureDesc.usage = _usage;
		m_TextureDesc.filename = _fileName;

		// Specify DirectX settings
		dword dwUsage = 0;
		D3DPOOL pool = static_cast<D3DPOOL>(1);
		uint nMipMaps = 0;
		// We change from 'full-chain' only if texture is dynamic or serves
		// as a render target.
		getUsagePoolMipMaps(dwUsage, reinterpret_cast<dword&>(pool), nMipMaps);

		D3DFORMAT format = getFormat(_format, dwUsage);

		// Load texture file
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(_fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint size = pFile->getSize();
		void* buffer = new char[size];
		pFile->read((char*)buffer, size);
		mgr.closeFile(_fileName);


		m_pD3DDev =	reinterpret_cast<IDirect3DDevice9*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		// Create appropriate texture
		HRESULT hr = 0;
		switch(_type)
		{
		// 1D texture
		case TT_1D: hr = D3DXCreateTextureFromFileInMemoryEx(m_pD3DDev,
															 buffer,
															 size,
															 0,
															 1,
															 nMipMaps,
															 dwUsage,
															 format,
															 pool,
															 D3DX_DEFAULT,
															 D3DX_DEFAULT,
															 0,
															 NULL,
															 NULL,
															 &m_p2DTexture);
					if(hr != D3D_OK)
						m_p2DTexture = NULL;
					break;

		// 2D texture
		case TT_2D: hr = D3DXCreateTextureFromFileInMemoryEx(m_pD3DDev,
															 buffer,
														     size,
															 0,
															 0,
															 nMipMaps,
															 dwUsage,
															 format,
															 pool,
															 D3DX_DEFAULT,
															 D3DX_DEFAULT,
															 0,
															 NULL,
															 NULL,
															 &m_p2DTexture);
					if(hr != D3D_OK)
						m_p2DTexture = NULL;
					break;

		// 3D texture
		case TT_3D: hr = D3DXCreateVolumeTextureFromFileInMemoryEx(m_pD3DDev,
																   buffer,
																   size,
																   0,
																   0,
																   0,
																   nMipMaps,
																   dwUsage,
																   format,
																   pool,
																   D3DX_DEFAULT,
																   0,
																   0,
																   NULL,
																   NULL,
																   &m_p3DTexture);
					if(hr != D3D_OK)
						m_p3DTexture = NULL;
					break;

		// Cube texture
		case TT_CUBE: hr = D3DXCreateCubeTextureFromFileInMemoryEx(m_pD3DDev,
																   buffer,
																   size,
																   0,
																   nMipMaps,
																   dwUsage,
																   format,
																   pool,
																   D3DX_DEFAULT,
																   0,
																   0,
																   NULL,
																   NULL,
																   &m_pCubeTexture);
					if(hr != D3D_OK)
						m_pCubeTexture = NULL;
					break;
		}

		// Something went wrong
		if(hr != D3D_OK)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Texture creation failed. Reason: %ls", DX9Mapping::getDXErrorDescription(hr).c_str());
		}
		else
		{
			if(_type == TT_1D || _type == TT_2D)
			{
				D3DSURFACE_DESC desc;
				m_p2DTexture->GetLevelDesc(0, &desc);

				m_TextureDesc.width = desc.Width;
				m_TextureDesc.height = desc.Height;

				m_LockedStruct.rect = new D3DLOCKED_RECT;
			}
			else if(_type == TT_3D)
			{
				D3DVOLUME_DESC desc;
				m_p3DTexture->GetLevelDesc(0, &desc);

				m_TextureDesc.width = desc.Width;
				m_TextureDesc.height = desc.Height;
				m_TextureDesc.depth = desc.Depth;

				m_LockedStruct.box = new D3DLOCKED_BOX;
			}
			else if(_type == TT_CUBE)
			{
				D3DSURFACE_DESC desc;
				m_pCubeTexture->GetLevelDesc(0, &desc);

				m_TextureDesc.width = desc.Width;
				m_TextureDesc.height = desc.Height;

				m_LockedStruct.rect = new D3DLOCKED_RECT;
			}

			if(_usage == TU_RENDERTARGET)
				createRenderTarget(m_pD3DDev);
		}

		NGENE_DELETE_ARRAY(buffer);
	}
//----------------------------------------------------------------------
	void TextureDX9::createRenderTarget(IDirect3DDevice9* _device)
	{
		Renderer& renderer = Renderer::getSingleton();
		m_RTDesc = new RT_DESC();

		if(m_TextureDesc.width != renderer.getDisplayMode().width ||
		   m_TextureDesc.height != renderer.getDisplayMode().height)
		{
			_device->CreateDepthStencilSurface(m_TextureDesc.width,
											   m_TextureDesc.height,
											   D3DFMT_D16,
											   D3DMULTISAMPLE_NONE,
											   0,
											   TRUE,
											   &m_RTDesc->zbuffer,
											   NULL);

			/// @todo Introduce own viewport class.
			m_RTDesc->viewport.X      = 0;
			m_RTDesc->viewport.Y      = 0;
			m_RTDesc->viewport.Width  = m_TextureDesc.width;
			m_RTDesc->viewport.Height = m_TextureDesc.height;
			m_RTDesc->viewport.MinZ   = 0.0f;
			m_RTDesc->viewport.MaxZ   = 1.0f;

			m_RTDesc->isViewportChange = true;
		}
	}
//----------------------------------------------------------------------
	D3DFORMAT TextureDX9::getFormat(TEXTURE_FORMAT& _format, dword& _usage)
	{
		D3DFORMAT format = DX9Mapping::textureFormatToD3DFORMAT(_format);
		if(_format == TF_DEFAULT)
			return format;

		D3DRESOURCETYPE resType = D3DRTYPE_TEXTURE;
		switch(m_TextureDesc.type)
		{
		case TT_1D: resType = D3DRTYPE_TEXTURE;
					break;
		case TT_2D: resType = D3DRTYPE_TEXTURE;
					break;
		case TT_3D: resType = D3DRTYPE_VOLUMETEXTURE;
					break;
		case TT_CUBE: resType = D3DRTYPE_CUBETEXTURE;
					break;
		}

		// Check for device support
		D3DFORMAT adapterFormat = D3DFMT_X8R8G8B8;
		IDirect3D9* pD3D = ((DeviceRenderDX9&)(Renderer::getSingleton().getDeviceRender())).getD3DPtr();
		if(FAILED(pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
										  D3DDEVTYPE_HAL,
										  adapterFormat,
										  _usage,
										  resType,
										  format)))
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Texture format: %d is not supported by this adapter", (int)format);
			format = D3DFMT_A8R8G8B8;
		}

		return format;
	}
//----------------------------------------------------------------------
	void TextureDX9::bind(uint _textureUnit, bool _forceSet, dword _clearFlag)
	{
		// If is currently bound as texture
		if(m_bBoundAsTexture && !_forceSet)
		{
			IDirect3DBaseTexture9* pTexture = NULL;
			m_pD3DDev->GetTexture(m_nTextureUnit, &pTexture);
			if(pTexture == m_p2DTexture)
				unbindNormal();
			NGENE_RELEASE(pTexture);
			m_bBoundAsTexture = false;
		}


		m_nTextureUnit = _textureUnit;
		m_bIsForced = _forceSet;

		if(_forceSet)
		{
			bindNormal();
		}
		else
		{
			switch(m_TextureDesc.usage)
			{
			case TU_NORMAL: bindNormal();
				break;
			case TU_RENDERTARGET: bindRenderTarget(_clearFlag);
				break;
			}
		}
	}
//----------------------------------------------------------------------
	void TextureDX9::unbind()
	{
		if(m_bIsForced)
		{
			switch(m_TextureDesc.usage)
			{
			case TU_RENDERTARGET: unbindRenderTarget();
				break;
			}
		}
		else
		{
			switch(m_TextureDesc.usage)
			{
			case TU_RENDERTARGET: unbindRenderTarget();
				break;
			}
		}
	}
//----------------------------------------------------------------------
	void TextureDX9::bindRenderTarget(dword _clearFlag)
	{
		Renderer& renderer = Renderer::getSingleton();

		if(!m_nTextureUnit)
			m_pD3DDev->GetRenderTarget(m_nTextureUnit, &m_pBackBuf);

		if(m_RTDesc->isViewportChange)
		{
			m_pD3DDev->GetDepthStencilSurface(&m_RTDesc->oldZbuffer);
			m_pD3DDev->GetViewport(&m_RTDesc->oldViewport);
		}

		m_p2DTexture->GetSurfaceLevel(0, &m_pSurface);

		// Check if the render target index is valid for this device
		uint nTarget = m_nTextureUnit;
		uint nMaxRTs = 0;
		renderer.getDeviceRender().testCapabilities(CAPS_MAX_RTS, &nMaxRTs);
		Maths::clamp <uint> (nTarget, 0, nMaxRTs);


		// Change viewport
		if(m_RTDesc->isViewportChange && !nTarget)
		{
			m_pD3DDev->SetViewport(&m_RTDesc->viewport);

			// Change depth/stencil surface
			if(_clearFlag & CB_DEPTH)
				m_pD3DDev->SetDepthStencilSurface(m_RTDesc->zbuffer);
			else
				m_pD3DDev->SetDepthStencilSurface(NULL);
		}

		m_pD3DDev->SetRenderTarget(nTarget, m_pSurface);

		// We have to also reset scissor rectangle
		renderer.setScissorRectangle(renderer.getScissorRectangle());


		if(_clearFlag & CB_TARGET || _clearFlag & CB_DEPTH)
			renderer.clearBuffer(_clearFlag);
	}
//----------------------------------------------------------------------
	void TextureDX9::bindNormal()
	{
		switch(m_TextureDesc.type)
		{
		case TT_1D: m_pD3DDev->SetTexture(m_nTextureUnit, m_p2DTexture);
					break;
		case TT_2D: m_pD3DDev->SetTexture(m_nTextureUnit, m_p2DTexture);
					break;
		case TT_3D: m_pD3DDev->SetTexture(m_nTextureUnit, m_p3DTexture);
					break;
		case TT_CUBE: m_pD3DDev->SetTexture(m_nTextureUnit, m_pCubeTexture);
					break;
		}

		if(m_TextureDesc.usage == TU_RENDERTARGET)
			m_bBoundAsTexture = true;
	}
//----------------------------------------------------------------------
	void TextureDX9::unbindRenderTarget()
	{
		if(!m_pBackBuf && !m_nTextureUnit)
			return;

		Renderer& renderer = Renderer::getSingleton();

		// Check if render target index is valid for this device
		uint nTarget = m_nTextureUnit;
		uint nMaxRTs = 0;
		renderer.getDeviceRender().testCapabilities(CAPS_MAX_RTS, &nMaxRTs);
		Maths::clamp <uint> (nTarget, 0, nMaxRTs);

		
		m_pD3DDev->SetRenderTarget(nTarget, m_pBackBuf);

		if(m_RTDesc->isViewportChange && !nTarget)
		{
			m_pD3DDev->SetDepthStencilSurface(m_RTDesc->oldZbuffer);
			m_pD3DDev->SetViewport(&m_RTDesc->oldViewport);
		}

		// Remember to free surface
		NGENE_RELEASE(m_pSurface);
		NGENE_RELEASE(m_pBackBuf);
		NGENE_RELEASE(m_RTDesc->oldZbuffer);
	}
//----------------------------------------------------------------------
	void TextureDX9::unbindNormal()
	{
		m_pD3DDev->SetTexture(m_nTextureUnit, NULL);
	}
//----------------------------------------------------------------------
	void TextureDX9::setData(void* _data, uint _level) const
	{
		if(!lock(_level))
			return;

		if(m_TextureDesc.type == TT_3D)
		{
			dword dwBytes = m_LockedStruct.box->RowPitch * m_LockedStruct.box->SlicePitch;
			memcpy(m_LockedStruct.box->pBits, _data, dwBytes);
		}
		else
		{
			dword dwBytes = m_LockedStruct.rect->Pitch * m_TextureDesc.height;
			memcpy(m_LockedStruct.rect->pBits, _data, dwBytes);
		}

		switch(m_TextureDesc.type)
		{
		case TT_1D: m_p2DTexture->UnlockRect(_level);
					break;
		case TT_2D: m_p2DTexture->UnlockRect(_level);
					break;
		case TT_3D: m_p3DTexture->UnlockBox(_level);
					break;
		case TT_CUBE: m_pCubeTexture->UnlockRect(static_cast<D3DCUBEMAP_FACES>(0), _level);
					break;
		}
	}
//----------------------------------------------------------------------
	void TextureDX9::setDataFloat(void* _data, uint size, uint _level) const
	{
		if(!lock(_level))
			return;

		if(m_TextureDesc.type == TT_3D)
		{
			memcpy(m_LockedStruct.box->pBits, _data, size);
		}
		else
		{
			byte* bytes = (byte*)m_LockedStruct.rect->pBits;
			Vector4* data = (Vector4*)_data;
			int bitDepth = m_LockedStruct.rect->Pitch / m_TextureDesc.width;

			uint ii = 0;
			for(uint y = 0; y < m_TextureDesc.height; y++, ii++)
			{
				//Copy one line at a time
				memcpy(&bytes[y * m_LockedStruct.rect->Pitch], &data[ii], sizeof(Vector4) * m_TextureDesc.width); //or sizeof(float)*4				
			}
		}

		switch(m_TextureDesc.type)
		{
		case TT_1D: m_p2DTexture->UnlockRect(_level);
					break;
		case TT_2D: m_p2DTexture->UnlockRect(_level);
					break;
		case TT_3D: m_p3DTexture->UnlockBox(_level);
					break;
		case TT_CUBE: m_pCubeTexture->UnlockRect(static_cast<D3DCUBEMAP_FACES>(0), _level);
					break;
		}
	}
//----------------------------------------------------------------------
	void* TextureDX9::getData(uint _level) const
	{
		if(!lock(_level))
			return NULL;

		void* temp = NULL;
		if(m_TextureDesc.type == TT_3D)
		{
			dword dwBytes = m_LockedStruct.box->RowPitch * m_LockedStruct.box->SlicePitch;
			temp = new byte[dwBytes];

			memcpy(temp, m_LockedStruct.box->pBits, dwBytes);
		}
		else
		{
			dword dwBytes = m_LockedStruct.rect->Pitch * m_TextureDesc.height;
			temp = new byte[dwBytes];

			memcpy(temp, m_LockedStruct.rect->pBits, dwBytes);
		}

		switch(m_TextureDesc.type)
		{
		case TT_1D: m_p2DTexture->UnlockRect(_level);
					break;
		case TT_2D: m_p2DTexture->UnlockRect(_level);
					break;
		case TT_3D: m_p3DTexture->UnlockBox(_level);
					break;
		case TT_CUBE: m_pCubeTexture->UnlockRect(static_cast<D3DCUBEMAP_FACES>(0), _level);
					break;
		}

		return temp;
	}
//----------------------------------------------------------------------
	uint TextureDX9::getBitDepth(uint _level) const
	{
		if(!lock(_level))
			return 0;

		dword bitDepth = 0;
		if(m_TextureDesc.type == TT_3D)
		{
			bitDepth = (m_LockedStruct.box->RowPitch * m_LockedStruct.box->SlicePitch) /
					   (m_TextureDesc.width * m_TextureDesc.height);
		}
		else
		{
			bitDepth = m_LockedStruct.rect->Pitch / m_TextureDesc.width;
		}

		switch(m_TextureDesc.type)
		{
		case TT_1D: m_p2DTexture->UnlockRect(_level);
					break;
		case TT_2D: m_p2DTexture->UnlockRect(_level);
					break;
		case TT_3D: m_p3DTexture->UnlockBox(_level);
					break;
		case TT_CUBE: m_pCubeTexture->UnlockRect(static_cast<D3DCUBEMAP_FACES>(0), _level);
					break;
		}

		return bitDepth;
	}
//----------------------------------------------------------------------
	void TextureDX9::onDeviceLost()
	{
		if(m_TextureDesc.usage == TU_NORMAL)
			return;
		cleanup();
	}
//----------------------------------------------------------------------
	void TextureDX9::onDeviceReset()
	{
		if(m_TextureDesc.usage == TU_NORMAL)
			return;
		
		if(m_TextureDesc.filename.compare(L"") == 0)
		{
			if(m_fRatioWidth > 0.0f && m_fRatioHeight > 0.0f && m_TextureDesc.usage == TU_RENDERTARGET)
			{
				const SDisplayMode& mode = Renderer::getSingleton().getDisplayMode();
				m_TextureDesc.width = mode.width * m_fRatioWidth;
				m_TextureDesc.height = mode.height * m_fRatioHeight;
			}


			create(m_TextureDesc.width,
				   m_TextureDesc.height,
				   m_TextureDesc.type,
				   m_TextureDesc.usage,
				   m_TextureDesc.format,
				   m_TextureDesc.depth);
		}
		else
		{
			createFromFile(m_TextureDesc.filename,
						   m_TextureDesc.type,
						   m_TextureDesc.usage,
						   m_TextureDesc.format);
		}
	}
//----------------------------------------------------------------------
	bool TextureDX9::lock(uint _level) const
	{
		// Render targets are not lockable!
		if(m_TextureDesc.usage == TU_RENDERTARGET)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Locking texture failed. Render target cannot be locked");
			return false;
		}


		if(m_TextureDesc.type == TT_1D || m_TextureDesc.type == TT_2D)
		{
			if(FAILED(m_p2DTexture->LockRect(_level, m_LockedStruct.rect, NULL, 0)))
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
					__LINE__, L"Locking 2D texture failed");
				return false;
			}
		}
		else if(m_TextureDesc.type == TT_3D)
		{
			if(FAILED(m_p3DTexture->LockBox(_level, m_LockedStruct.box, NULL, 0)))
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
					__LINE__, L"Locking 3D texture failed");
				return false;
			}
		}
		else if(m_TextureDesc.type == TT_CUBE)
		{
			if(FAILED(m_pCubeTexture->LockRect(static_cast<D3DCUBEMAP_FACES>(0), _level, m_LockedStruct.rect, NULL, 0)))
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
					__LINE__, L"Locking cube texture failed");
				return false;
			}
		}

		return true;
	}
//----------------------------------------------------------------------
	void TextureDX9::clearRenderTarget()
	{
		if(m_TextureDesc.usage == TU_RENDERTARGET)
		{
			bindRenderTarget(CB_TARGET);
			unbindRenderTarget();
		}
	}
//----------------------------------------------------------------------
}
