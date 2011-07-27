/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DX9Mapping.h
Version:	0.12
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DX9MAPPING_H_
#define __INC_DX9MAPPING_H_


#include <D3DX9Math.h>
#include <D3D9Types.h>
#include "Prerequisities.h"
#include "Renderer.h"
#include "ITexture.h"


namespace nGENE
{
	/** Provides conversions between nGENE Tech and DirectX 9.0c
		types.
		@remarks
			These conversions are necessary as nGENE uses its own
			conventions which are not entirely compatible with
			those of DirectX.
	*/
	class nGENEDLL DX9Mapping
	{
	public:
		/// Converts D3DXMATRIX of DirectX to nGENE Matrix4x4.
		static Matrix4x4 D3DXMATRIXToMatrix4(const D3DXMATRIX& _mat);
		/// Converts nGENE Matrix4x4 to D3DXMATRIX of DirectX.
		static D3DXMATRIX matrix4ToD3DXMATRIX(const Matrix4x4& _mat);

		/// Converts nGENE Vector3 to D3DXVECTOR3 of DirectX.
		static D3DXVECTOR3 vector3ToD3DXVECTOR3(const Vector3& _vec);
		/// Converts D3DXVECTOR3 of DirectX to Vector3 of nGENE.
		static Vector3 D3DXVECTOR3ToVector3(const D3DXVECTOR3& _vec);

		/// Converts nGENE Vector2 to D3DXVECTOR2 of DirectX.
		static D3DXVECTOR2 vector2ToD3DXVECTOR2(const Vector2& _vec);
		/// Converts D3DXVECTOR2 of DirectX to Vector2 of nGENE.
		static Vector2 D3DXVECTOR2ToVector2(const D3DXVECTOR2& _vec);
		
		/// Converts D3DXQUATERNION of DirectX to Quaternion of nGENE.
		static Quaternion D3DXQUATERNIONToQuaternion(const D3DXQUATERNION& _quat);

		/// Converts nGENE Colour to D3DCOLOR of DirectX.
		static D3DCOLOR colourToD3DCOLOR(const Colour& _col);
		/// Converts D3DCOLOR of DirectX to Colour of nGENE.
		static Colour D3DCOLORToColour(const D3DCOLOR& _col);

		/// Converts nGENE CULL_MODE enum to D3DCULL of DirectX.
		static D3DCULL cullingToD3DCULL(const CULL_MODE& _cull);

		/** Converts nGENE FILL_MODE to D3DFILLMODE of DirectX.
			@remarks
				Notice that as you provide a FILL_MODE
				and get back combination of D3DFILLMODEs both parameter
				and return value are unsigned integers (dwords).
		*/
		static dword fillModeToDword(const FILL_MODE& _fill);

		/// Converts nGENE Material to D3DMATERIAL9 of DirectX.
		static D3DMATERIAL9 materialToD3DMATERIAL9(const Material& _material);

		/// Converts nGENE TEXTURE_FILTER to D3DTEXTUREFILTERTYPE of DirectX.
		static D3DTEXTUREFILTERTYPE textureFilterToD3DTEXTUREFILTERTYPE(TEXTURE_FILTER _filter);

		/// Converts nGENE TEXTURE_ADDRESSING_MODE to D3DTEXTUREADDRESS of DirectX.
		static D3DTEXTUREADDRESS texureAddressingModeToD3DTEXTUREADDRESS(TEXTURE_ADDRESSING_MODE _mode);

		/// Converts nGENE TEXTURE_FORMAT to D3DFORMAT of DirectX.
		static D3DFORMAT textureFormatToD3DFORMAT(TEXTURE_FORMAT _format);

		/// Converts SVertexElement to D3DVERTEXELEMENT9.
		static D3DVERTEXELEMENT9 svertexElementToD3DVERTEXELEMENT9(const SVertexElement& _element);

		/// Converts VERTEX_ELEMENT_USAGE to D3DDECLUSAGE.
		static D3DDECLUSAGE vertexElementUsageToD3DDECLUSAGE(const VERTEX_ELEMENT_USAGE& _usage);

		/// Returns D3DDECLTYPE based on the elements count.
		static D3DDECLTYPE getD3DDECLTYPE(uint _count, const VERTEX_ELEMENT_USAGE& _usage);

		/// Converts hr result of DirectX function call to string.
		static wstring getDXErrorDescription(HRESULT _hr);
	};
}


#endif