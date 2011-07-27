/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TextureDX9.h
Version:	0.10
---------------------------------------------------------------------------
*/
#pragma once
#ifndef __INC_TEXTUREDX9_H_
#define __INC_TEXTUREDX9_H_


#include "IResourceDX9.h"
#include "ITexture.h"
#include "TypeInfo.h"

#include <d3d9.h>
#include <D3dx9tex.h>


namespace nGENE
{
	/// Description of the render target
	typedef struct SRenderTargetDesc
	{
		D3DVIEWPORT9 viewport;						///< Current viewport
		D3DVIEWPORT9 oldViewport;					///< Pointer to the previous viewport
		IDirect3DSurface9* zbuffer;					///< Current z-buffer
		IDirect3DSurface9* oldZbuffer;				///< Previous z-buffer

		bool isViewportChange;						///< Is viewport change necessary?

		SRenderTargetDesc():
			zbuffer(NULL),
			oldZbuffer(NULL),
			isViewportChange(false)
		{}

		~SRenderTargetDesc()
		{
			NGENE_RELEASE(zbuffer);
			NGENE_RELEASE(oldZbuffer);
		}
	} RT_DESC;

	/// DirectX 9.0c compatible texture.
	class nGENEDLL TextureDX9: public ITexture, public IResourceDX9
	{
		EXPOSE_TYPE
	private:
		IDirect3DDevice9* m_pD3DDev;
		RT_DESC* m_RTDesc;

		/// DirectX texture
		union
		{
			IDirect3DTexture9* m_p2DTexture;
			IDirect3DVolumeTexture9* m_p3DTexture;
			IDirect3DCubeTexture9* m_pCubeTexture;
		};

		union LOCKED_STRUCT
		{
			D3DLOCKED_RECT* rect;
			D3DLOCKED_BOX* box;
		} m_LockedStruct;
		
		/// Previous render target
		IDirect3DSurface9* m_pBackBuf;
		/// Current render target
		IDirect3DSurface9* m_pSurface;
	
		/// For what purpose texture is going to be used
		dword m_dwUsage;

		/// Format of the texture
		TEXTURE_FORMAT m_Format;

		/// Texture unit used by the texture.
		uint m_nTextureUnit;

		bool m_bIsForced;
		bool m_bBoundAsTexture;


		void bindNormal();
		void bindRenderTarget(dword _clearFlag);
		void unbindNormal();
		void unbindRenderTarget();
		void getUsagePoolMipMaps(dword& _dxUsage, dword& _dxPool, uint& _mips);
		D3DFORMAT getFormat(TEXTURE_FORMAT& _format, dword& _usage);

		void createRenderTarget(IDirect3DDevice9* _device);


		void onDeviceLost();
		void onDeviceReset();

		/// Locks texture surface if possible.
		bool lock(uint _level) const;

	public:
		TextureDX9();
		virtual ~TextureDX9();

		void init();
		void cleanup();

		/// Creates empty texture.
		void create(uint _width, uint _height, TEXTURE_TYPE _type, TEXTURE_USAGE _usage,
			TEXTURE_FORMAT _format, uint _depth);

		/// Creates texture from file.
		void createFromFile(const wstring& _fileName, TEXTURE_TYPE _type, TEXTURE_USAGE _usage,
			TEXTURE_FORMAT _format);

		void bind(uint _textureUnit, bool _forceSet, dword _clearFlag);
		void unbind();

		void setDataFloat(void* _data, uint size, uint _level) const;
		void setData(void* _data, uint _level) const;
		void* getData(uint _level) const;

		/// Returns texture bit depth.
		uint getBitDepth(uint _level) const;


		/// Lets you change default texture surface.
		void setSurface(IDirect3DSurface9* _surface);
		/** Returns texture surface.
			@remarks
				You have to manually release memory reserved for surface or else
				memory leak will occur.
		*/
		IDirect3DSurface9* getSurface(uint _level=0);

		/// Clears render target.
		void clearRenderTarget();
	};



	inline void TextureDX9::setSurface(IDirect3DSurface9* _surface)
	{
		IDirect3DSurface9* surf;
		m_p2DTexture->GetSurfaceLevel(0, &surf);
		m_pD3DDev->StretchRect(_surface, NULL, surf, NULL, D3DTEXF_NONE);

		NGENE_RELEASE(surf);
	}
//----------------------------------------------------------------------
	inline IDirect3DSurface9* TextureDX9::getSurface(uint _level)
	{
		IDirect3DSurface9* surf;
		m_p2DTexture->GetSurfaceLevel(_level, &surf);

		return surf;
	}
//----------------------------------------------------------------------
}


#endif