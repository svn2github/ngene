/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TextureDX10.h
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DeviceRender.h"
#include "DX10Mapping.h"
#include "TextureDX10.h"

#include <D3DX10tex.h>


namespace nGENE
{
	// Initialize static members
	TypeInfo TextureDX10::Type(L"TextureDX10", &ITexture::Type);


	TextureDX10::TextureDX10():
		m_p1DTexture(NULL),
		m_p2DTexture(NULL),
		m_p3DTexture(NULL),
		m_pBackBuf(NULL),
		m_pSurface(NULL),
		m_nTextureUnit(0)
	{
		init();
	}
//----------------------------------------------------------------------
	TextureDX10::~TextureDX10()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void TextureDX10::init()
	{
	}
//----------------------------------------------------------------------
	void TextureDX10::cleanup()
	{
		NGENE_RELEASE(m_pSurface);
		NGENE_RELEASE(m_pBackBuf);
		NGENE_RELEASE(m_p3DTexture);
		NGENE_RELEASE(m_p2DTexture);
		NGENE_RELEASE(m_p1DTexture);
	}
//----------------------------------------------------------------------
	void TextureDX10::getUsagePoolMipMaps(dword& _dxUsage, dword& _dxPool, uint& _mips)
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
	void TextureDX10::create(uint _width, uint _height, TEXTURE_TYPE _type, TEXTURE_USAGE _usage,
		TEXTURE_FORMAT _format, uint _depth)
	{
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

		DXGI_FORMAT format = DX10Mapping::textureFormatToD3DFORMAT(_format);
		

		ID3D10Device* pDevice = reinterpret_cast<ID3D10Device*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());


		// Create appropriate texture
		HRESULT hr = 0;
/*		switch(_type)
		{
		case TT_2D: hr = D3DXCreateTexture(pDevice, _width, _height, nMipMaps, dwUsage, format, pool, &m_p2DTexture);
					if(hr != D3D_OK)
						m_p2DTexture = NULL;
					break;

		case TT_3D: hr = D3DXCreateVolumeTexture(pDevice, _width, _height, _depth, nMipMaps, dwUsage, format,
						pool, &m_p3DTexture);
					if(hr!=D3D_OK)
						m_p3DTexture = NULL;
					break;

		case TT_CUBE: hr = D3DXCreateCubeTexture(pDevice, _width, nMipMaps, dwUsage, format, pool, &m_pCubeTexture);
					if(hr != D3D_OK)
						m_pCubeTexture = NULL;
					break;
		}*/

		// Something went wrong - find out what
		if(hr != D3D_OK)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Texture creation failed. Reason: %ls", DX10Mapping::getDXErrorDescription(hr).c_str());
		}
	}
//----------------------------------------------------------------------
	void TextureDX10::createFromFile(const wstring& _fileName, TEXTURE_TYPE _type,
			TEXTURE_USAGE _usage, TEXTURE_FORMAT _format)
	{
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

		DXGI_FORMAT format = DX10Mapping::textureFormatToD3DFORMAT(m_Format);


		ID3D10Device* pDevice =	reinterpret_cast<ID3D10Device*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());


		D3DX10_IMAGE_LOAD_INFO info;
		info.Usage = D3D10_USAGE_DEFAULT;//dwUsage;
		info.MipLevels = nMipMaps;

		// Load texture file
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(_fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint size = pFile->getSize();
		void* buffer = new char[size];
		pFile->read((char*)buffer, size);
		mgr.closeFile(_fileName);

		// Create texture
		ID3D10Resource* pTexture = NULL;
		HRESULT hr = D3DX10CreateTextureFromMemory(pDevice, buffer, size, &info, NULL, &pTexture, NULL);

		NGENE_DELETE_ARRAY(buffer);


		D3D10_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;


		// Something went wrong
		if(hr != D3D_OK)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Texture creation failed. Reason: %ls", DX10Mapping::getDXErrorDescription(hr).c_str());
		}
		else
		{
			if(_type == TT_1D)
			{
				D3D10_TEXTURE1D_DESC desc;
				m_p1DTexture = (ID3D10Texture1D*)pTexture;
				m_p1DTexture->GetDesc(&desc);

				resourceViewDesc.Format = desc.Format;
				resourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE1D;
				resourceViewDesc.Texture1D.MostDetailedMip = 0;
				resourceViewDesc.Texture1D.MipLevels = desc.MipLevels;

				m_TextureDesc.width = desc.Width;
				m_TextureDesc.height = 1;
			}
			else if(_type == TT_2D || _type == TT_CUBE)
			{
				D3D10_TEXTURE2D_DESC desc;
				m_p2DTexture = (ID3D10Texture2D*)pTexture;
				m_p2DTexture->GetDesc(&desc);
				
				resourceViewDesc.Format = desc.Format;
				resourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
				resourceViewDesc.Texture2D.MostDetailedMip = 0;
				resourceViewDesc.Texture2D.MipLevels = desc.MipLevels;

				m_TextureDesc.width = desc.Width;
				m_TextureDesc.height = desc.Height;
			}
			else if(_type == TT_3D)
			{
				D3D10_TEXTURE3D_DESC desc;
				m_p3DTexture = (ID3D10Texture3D*)pTexture;
				m_p3DTexture->GetDesc(&desc);
				
				resourceViewDesc.Format = desc.Format;
				resourceViewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
				resourceViewDesc.Texture3D.MostDetailedMip = 0;
				resourceViewDesc.Texture3D.MipLevels = desc.MipLevels;

				m_TextureDesc.width = desc.Width;
				m_TextureDesc.height = desc.Height;
				m_TextureDesc.depth = desc.Depth;
			}

			// Create resource view
			ID3D10ShaderResourceView* pResourceView = NULL;
			pDevice->CreateShaderResourceView(pTexture, &resourceViewDesc, &pResourceView);
		}
	}
//----------------------------------------------------------------------
	void TextureDX10::bind(uint _textureUnit, bool _forceSet, dword _clearFlags)
	{
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
			case TU_RENDERTARGET: bindRenderTarget();
				break;
			}
		}
	}
//----------------------------------------------------------------------
	void TextureDX10::unbind()
	{
		if(m_bIsForced)
		{
			unbindNormal();
		}
		else
		{
			switch(m_TextureDesc.usage)
			{
			case TU_NORMAL: unbindNormal();
				break;
			case TU_RENDERTARGET: unbindRenderTarget();
				break;
			}
		}
	}
//----------------------------------------------------------------------
	void TextureDX10::bindRenderTarget()
	{
		IDirect3DDevice9* pDevice =
			reinterpret_cast<IDirect3DDevice9*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		pDevice->GetRenderTarget(m_nTextureUnit, &m_pBackBuf);


//		m_p2DTexture->GetSurfaceLevel(0, &m_pSurface);

		// Check if the render target index is valid for this device
		uint nTarget = m_nTextureUnit;
		uint nMaxRTs = 0;
		Renderer::getSingleton().getDeviceRender().testCapabilities(CAPS_MAX_RTS, &nMaxRTs);
		Maths::clamp <uint> (nTarget, 0, nMaxRTs);

		pDevice->SetRenderTarget(nTarget, m_pSurface);

		/// @todo Add more control over clearing.
		Renderer::getSingleton().clearBuffer(CB_TARGET | CB_DEPTH);
	}
//----------------------------------------------------------------------
	void TextureDX10::bindNormal()
	{
		ID3D10Device* pDevice =	reinterpret_cast<ID3D10Device*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());
		//pDevice->SetTexture(m_nTextureUnit, m_p2DTexture);
		//pDevice->VSSetShaderResources(
	}
//----------------------------------------------------------------------
	void TextureDX10::unbindRenderTarget()
	{
		// Check if render target index is valid for this device
		uint nTarget = m_nTextureUnit;
		uint nMaxRTs = 0;
		Renderer::getSingleton().getDeviceRender().testCapabilities(CAPS_MAX_RTS, &nMaxRTs);
		Maths::clamp <uint> (nTarget, 0, nMaxRTs);

		IDirect3DDevice9* pDevice =
			reinterpret_cast<IDirect3DDevice9*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());
		pDevice->SetRenderTarget(nTarget, m_pBackBuf);

		// Remember to free surface
		NGENE_RELEASE(m_pSurface);
		NGENE_RELEASE(m_pBackBuf);
	}
//----------------------------------------------------------------------
	void TextureDX10::unbindNormal()
	{
		IDirect3DDevice9* pDevice =
			reinterpret_cast<IDirect3DDevice9*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());
		pDevice->SetTexture(m_nTextureUnit, NULL);
	}
//----------------------------------------------------------------------
	void* TextureDX10::getData(uint _level) const
	{
		D3DLOCKED_RECT rect;
		byte r, g, b;

/*		if(FAILED(m_p2DTexture->LockRect(0, &rect, NULL, 0)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Locking texture failed");
			return NULL;
		}*/

		dword dwBytes = rect.Pitch * m_TextureDesc.height;
		void* temp = new byte[dwBytes];

		memcpy(temp, rect.pBits, dwBytes);

//		m_p2DTexture->UnlockRect(0);


		return temp;
	}
//----------------------------------------------------------------------
	void TextureDX10::onDeviceLost()
	{
		if(m_TextureDesc.usage == TU_NORMAL)
			return;
		cleanup();
	}
//----------------------------------------------------------------------
	void TextureDX10::onDeviceReset()
	{
		if(m_TextureDesc.usage == TU_NORMAL)
			return;
		
		if(m_TextureDesc.filename.compare(L"") == 0)
		{
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
}
