/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TextureDX10.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TEXTUREDX10_H_
#define __INC_TEXTUREDX10_H_


#include "IResourceDX9.h"
#include "ITexture.h"
#include "TypeInfo.h"

#include <d3d10.h>
#include <d3d9.h>


namespace nGENE
{
	/** DirectX 10.0 compatible texture.
		@todo
			It is worth considering whether render target should not
			be separate class deriving from texture class.
	*/
	class TextureDX10: public ITexture//, public IResourceDX10
	{
		EXPOSE_TYPE
	private:
		/// DirectX texture
		union
		{
			ID3D10Texture1D* m_p1DTexture;
			ID3D10Texture2D* m_p2DTexture;
			ID3D10Texture3D* m_p3DTexture;
		};
		
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


		void bindNormal();
		void bindRenderTarget();
		void unbindNormal();
		void unbindRenderTarget();
		void getUsagePoolMipMaps(dword& _dxUsage, dword& _dxPool, uint& _mips);


		void onDeviceLost();
		void onDeviceReset();

	public:
		TextureDX10();
		virtual ~TextureDX10();

		void init();
		void cleanup();

		/// Creates empty texture.
		void create(uint _width, uint _height, TEXTURE_TYPE _type, TEXTURE_USAGE _usage,
			TEXTURE_FORMAT _format, uint _depth);

		/// Creates texture from file.
		void createFromFile(const wstring& _fileName, TEXTURE_TYPE _type, TEXTURE_USAGE _usage,
			TEXTURE_FORMAT _format);

		void bind(uint _textureUnit, bool _forceSet, dword _clearFlags);
		void unbind();

		void setData(void* _data, uint _level) const {}
		void setDataFloat(void* _data,uint size, uint _level) const {}
		void* getData(uint _level) const;

		/// Returns texture bit depth.
		uint getBitDepth(uint _level) const {return 0;}

		void clearRenderTarget() {};
	};
}


#endif