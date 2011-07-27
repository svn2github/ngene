/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DX9Mapping.cpp
Version:	0.17
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DX9Mapping.h"
#include "Material.h"
#include "Quaternion.h"
#include "Vector2.h"
#include "Vertex.h"
#include "VertexDeclaration.h"

#include <dxerr.h>


namespace nGENE
{
	Matrix4x4 DX9Mapping::D3DXMATRIXToMatrix4(const D3DXMATRIX& _mat)
	{
		return (Matrix4x4(_mat._11, _mat._12, _mat._13, _mat._14,
						  _mat._21, _mat._22, _mat._23, _mat._24,
						  _mat._31, _mat._32, _mat._33, _mat._34,
						  _mat._41, _mat._42, _mat._43, _mat._44));
	}
//----------------------------------------------------------------------
	D3DXMATRIX DX9Mapping::matrix4ToD3DXMATRIX(const Matrix4x4& _mat)
	{
		return (D3DXMATRIX(_mat[0][0], _mat[0][1], _mat[0][2], _mat[0][3],
						   _mat[1][0], _mat[1][1], _mat[1][2], _mat[1][3],
						   _mat[2][0], _mat[2][1], _mat[2][2], _mat[2][3],
						   _mat[3][0], _mat[3][1], _mat[3][2], _mat[3][3]));
	}
//----------------------------------------------------------------------
	D3DXVECTOR3 DX9Mapping::vector3ToD3DXVECTOR3(const Vector3& _vec)
	{
		return (D3DXVECTOR3(_vec.x, _vec.y, _vec.z));
	}
//----------------------------------------------------------------------
	Vector3 DX9Mapping::D3DXVECTOR3ToVector3(const D3DXVECTOR3& _vec)
	{
		return (Vector3(_vec.x, _vec.y, _vec.z));
	}
//----------------------------------------------------------------------
	D3DXVECTOR2 DX9Mapping::vector2ToD3DXVECTOR2(const Vector2& _vec)
	{
		return (D3DXVECTOR2(_vec.x, _vec.y));
	}
//----------------------------------------------------------------------
	Vector2 DX9Mapping::D3DXVECTOR2ToVector2(const D3DXVECTOR2& _vec)
	{
		return (Vector2(_vec.x, _vec.y));
	}
//----------------------------------------------------------------------
	Quaternion DX9Mapping::D3DXQUATERNIONToQuaternion(const D3DXQUATERNION& _quat)
	{
		return (Quaternion(_quat.x, _quat.y, _quat.z, _quat.w));
	}
//----------------------------------------------------------------------
	D3DCOLOR DX9Mapping::colourToD3DCOLOR(const Colour& _col)
	{
		return (D3DCOLOR(_col.getDwordARGB()));
	}
//----------------------------------------------------------------------
	Colour DX9Mapping::D3DCOLORToColour(const D3DCOLOR& _col)
	{
		return (Colour(_col));
	}
//----------------------------------------------------------------------
	D3DCULL DX9Mapping::cullingToD3DCULL(const CULL_MODE& _cull)
	{
		return ((D3DCULL)(_cull + 1));
	}
//----------------------------------------------------------------------
	dword DX9Mapping::fillModeToDword(const FILL_MODE& _fill)
	{
		dword result = 0;

		// Find out D3DFILLMODE combination
		if(_fill == FILL_DOT)
			result = D3DFILL_POINT;
		else if(_fill == FILL_WIREFRAME)
			result = D3DFILL_WIREFRAME;
		else if(_fill == FILL_SOLID)
			result = D3DFILL_SOLID;

		return result;
	}
//----------------------------------------------------------------------
	D3DMATERIAL9 DX9Mapping::materialToD3DMATERIAL9(const Material& _material)
	{
		D3DMATERIAL9 mat;
		Material material = _material;
		float inv = 1.0f / 255.0f;

		Colour* col = &material.getAmbient();
		mat.Ambient.r = (float)col->getRed() * inv;
		mat.Ambient.g = (float)col->getGreen() * inv;
		mat.Ambient.b = (float)col->getBlue() * inv;
		mat.Ambient.a = (float)col->getAlpha() * inv;

		col = &material.getDiffuse();
		mat.Diffuse.r = (float)col->getRed() * inv;
		mat.Diffuse.g = (float)col->getGreen() * inv;
		mat.Diffuse.b = (float)col->getBlue() * inv;
		mat.Diffuse.a = (float)col->getAlpha() * inv;

		col = &material.getSpecular();
		mat.Specular.r = (float)col->getRed() * inv;
		mat.Specular.g = (float)col->getGreen() * inv;
		mat.Specular.b = (float)col->getBlue() * inv;
		mat.Specular.a = (float)col->getAlpha() * inv;

		float emissive = (float)material.getEmissive() * inv;
		mat.Emissive.r = emissive;
		mat.Emissive.g = emissive;
		mat.Emissive.b = emissive;
		mat.Emissive.a = emissive;

		return mat;
	}
//----------------------------------------------------------------------
	D3DTEXTUREFILTERTYPE DX9Mapping::textureFilterToD3DTEXTUREFILTERTYPE(TEXTURE_FILTER _filter)
	{
		switch(_filter)
		{
		case FILTER_UNKNOWN		: Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
									  L"Unknown texture filter used");
								  return D3DTEXF_NONE;
		case FILTER_NONE		: return D3DTEXF_NONE;
		case FILTER_NEAREST		: return D3DTEXF_POINT;
		case FILTER_TRILINEAR	: return D3DTEXF_LINEAR;
		case FILTER_ANISOTROPIC	: return D3DTEXF_ANISOTROPIC;
		}

		return D3DTEXF_NONE;
	}
//----------------------------------------------------------------------
	D3DTEXTUREADDRESS DX9Mapping::texureAddressingModeToD3DTEXTUREADDRESS(TEXTURE_ADDRESSING_MODE _mode)
	{
		switch(_mode)
		{
		case TAM_UNKNOWN	: Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
									  L"Unknown addressing mode used");
							  return D3DTADDRESS_WRAP;
		case TAM_WRAP		: return D3DTADDRESS_WRAP;
		case TAM_CLAMP		: return D3DTADDRESS_CLAMP;
		case TAM_BORDER		: return D3DTADDRESS_BORDER;
		case TAM_MIRROR		: return D3DTADDRESS_MIRROR;
		default: return D3DTADDRESS_WRAP;
		}
	}
//----------------------------------------------------------------------
	D3DFORMAT DX9Mapping::textureFormatToD3DFORMAT(TEXTURE_FORMAT _format)
	{
		switch(_format)
		{
		case TF_DEFAULT: return D3DFMT_UNKNOWN;
		case TF_A8R8G8B8: return D3DFMT_A8R8G8B8;
		case TF_A2R10G10B10: return D3DFMT_A2R10G10B10;
		case TF_R8G8B8: return D3DFMT_X8R8G8B8;
		case TF_A1R5G5B5: return D3DFMT_A1R5G5B5;
		case TF_R5G5B5: return D3DFMT_X1R5G5B5;
		case TF_R5G6B5: return D3DFMT_R5G6B5;
		case TF_G16R16F: return D3DFMT_G16R16F;
		case TF_A16B16G16R16F: return D3DFMT_A16B16G16R16F;
		case TF_R16F: return D3DFMT_R16F;
		case TF_G32R32F: return D3DFMT_G32R32F;
		case TF_R32F: return D3DFMT_R32F;
		case TF_A32B32G32R32F: return D3DFMT_A32B32G32R32F;
		case TF_A8: return D3DFMT_A8;
		default: return D3DFMT_UNKNOWN;
		}
	}
//----------------------------------------------------------------------
	D3DVERTEXELEMENT9 DX9Mapping::svertexElementToD3DVERTEXELEMENT9(const SVertexElement& _element)
	{
		D3DVERTEXELEMENT9 result;

		result.Stream = _element.stream;
		result.Offset = _element.offset;
		result.Type = getD3DDECLTYPE(_element.size, _element.usage);
		result.Method = D3DDECLMETHOD_DEFAULT;
		result.Usage = vertexElementUsageToD3DDECLUSAGE(_element.usage);
		result.UsageIndex = _element.elementIndex;

		return result;
	}
//----------------------------------------------------------------------
	D3DDECLUSAGE DX9Mapping::vertexElementUsageToD3DDECLUSAGE(const VERTEX_ELEMENT_USAGE& _usage)
	{
		switch(_usage)
		{
		case VEU_POSITION: return D3DDECLUSAGE_POSITION;
		case VEU_NORMAL: return D3DDECLUSAGE_NORMAL;
		case VEU_BINORMAL: return D3DDECLUSAGE_BINORMAL;
		case VEU_TANGENT: return D3DDECLUSAGE_TANGENT;
		case VEU_TEXCOORD: return D3DDECLUSAGE_TEXCOORD;
		case VEU_COLOUR: return D3DDECLUSAGE_COLOR;
		case VEU_FOG: return D3DDECLUSAGE_FOG;
		case VEU_DEPTH: return D3DDECLUSAGE_DEPTH;
		case VEU_SAMPLE: return D3DDECLUSAGE_SAMPLE;
		case VEU_BLEND_WEIGHTS: return D3DDECLUSAGE_BLENDWEIGHT;
		case VEU_BLEND_INDICES: return D3DDECLUSAGE_BLENDINDICES;
		default: return D3DDECLUSAGE_POSITION;
		}
	}
//----------------------------------------------------------------------
	D3DDECLTYPE DX9Mapping::getD3DDECLTYPE(uint _count, const VERTEX_ELEMENT_USAGE& _usage)
	{
		Maths::clamp <uint> (_count, 1, 4);

		switch(_count)
		{
		case 1: return D3DDECLTYPE_FLOAT1;
		case 2: return D3DDECLTYPE_FLOAT2;
		case 3: return D3DDECLTYPE_FLOAT3;
		case 4:
			{
				if(_usage == VEU_POSITION)
					return D3DDECLTYPE_FLOAT4;
				else if(_usage == VEU_COLOUR)
					return D3DDECLTYPE_D3DCOLOR;
				else if(_usage == VEU_BLEND_INDICES)
					return D3DDECLTYPE_D3DCOLOR;
				else
					return D3DDECLTYPE_FLOAT4;
			}
		default: return D3DDECLTYPE_FLOAT1;
		}
	}
//----------------------------------------------------------------------
	wstring DX9Mapping::getDXErrorDescription(HRESULT _hr)
	{
		string stTemp = DXGetErrorDescription(_hr);
		wstring stMessage(stTemp.begin(), stTemp.end());

		return stMessage;
	}
//----------------------------------------------------------------------
}
