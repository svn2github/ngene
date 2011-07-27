/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DX10Mapping.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DX10MAPPING_H_
#define __INC_DX10MAPPING_H_


#include <DXGI.h>

#include "Prerequisities.h"
#include "Renderer.h"
#include "ITexture.h"


namespace nGENE
{
	/** Provides conversions between nGENE Tech and DirectX 10.0
		types.
		@remarks
			These conversions are necessary as nGENE uses its own
			conventions which are not entirely compatible with
			those of DirectX.
	*/
	class nGENEDLL DX10Mapping
	{
	public:
		/// Converts hr result of DirectX function call to string.
		static wstring getDXErrorDescription(HRESULT _hr);

		/// Converts nGENE TEXTURE_FORMAT to DXGI_FORMAT of DirectX.
		static DXGI_FORMAT textureFormatToD3DFORMAT(TEXTURE_FORMAT _format);
	};
}


#endif