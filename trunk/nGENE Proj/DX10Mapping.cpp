/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DX10Mapping.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DX10Mapping.h"

#include <dxerr.h>


namespace nGENE
{
	wstring DX10Mapping::getDXErrorDescription(HRESULT _hr)
	{
		string stTemp = DXGetErrorDescription(_hr);
		wstring stMessage(stTemp.begin(), stTemp.end());
		return stMessage;
	}
//----------------------------------------------------------------------
	DXGI_FORMAT DX10Mapping::textureFormatToD3DFORMAT(TEXTURE_FORMAT _format)
	{
		switch(_format)
		{
		case TF_A8R8G8B8: return DXGI_FORMAT_R8G8B8A8_TYPELESS;
		case TF_R8G8B8: return DXGI_FORMAT_B8G8R8X8_UNORM;
		case TF_G16R16F: return DXGI_FORMAT_R16G16_FLOAT;
		case TF_A16B16G16R16F: DXGI_FORMAT_R16G16B16A16_FLOAT;
		default: return DXGI_FORMAT_UNKNOWN;
		}
	}
//----------------------------------------------------------------------
}
