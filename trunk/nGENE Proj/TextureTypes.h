/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TextureTypes.h
Version:	0.05
Info:		This file contains various types definitions concerning
			texturing which	are used by the renderer.
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TEXTURETYPES_H_
#define __INC_TEXTURETYPES_H_


#include "Prerequisities.h"


namespace nGENE
{
	/// Format of the texture
	enum TEXTURE_FORMAT
	{
		TF_DEFAULT,						///< Default format (taken from file or unknown)
		TF_A8R8G8B8,					///< Default - 32 bit. 8 bits per channel
		TF_A2R10G10B10,					///< 32 bit, 2 bit alpha and 10 bit RGB channels
		TF_R8G8B8,						///< 32 bit without alpha - alpha channel inaccessible
		TF_A1R5G5B5,					///< 16 bit - 1 per alpha and 5 per other channels
		TF_R5G5B5,						///< 16 bit - 5 per colour channels. No alpha
		TF_R5G6B5,						///< 16 bit - 5 bits per red and blue and 6 per green
		TF_G16R16F,						///< 32 bit floating point. Only red and green channels
		TF_A16B16G16R16F,				///< 64 bit, 4 channel floating point texture
		TF_R16F,						///< 16 bit, 1 channel floating point texture
		TF_G32R32F,						///< 64 bit, 2 channel floating point texture
		TF_R32F,						///< 32 bit, 1 channel floating point texture
		TF_A32B32G32R32F,				///< 128 bit, 4 channel floating point texture
		TF_A8							///< 8 bit, 1-channel texture
	};

	/// Type of the texture.
	enum TEXTURE_TYPE
	{
		TT_UNKNOWN,
		TT_1D,							///< 1D Texture
		TT_2D,							///< 2D Texture - you will use it in most cases
		TT_3D,							///< 3D Texture - memory usage expensive
		TT_CUBE							///< Cube texture
	};

	/// Texture usage.
	enum TEXTURE_USAGE
	{
		TU_UNKNOWN,
		TU_NORMAL,						///< Regular texture
		TU_RENDERTARGET,				///< Texture will be used as a render target
		TU_DYNAMIC						///< Texture is dynamic, you can write to it
	};

	/** Method of filtering textures
	*/
	enum TEXTURE_FILTER
	{
		FILTER_UNKNOWN,
		FILTER_NONE,
		FILTER_NEAREST,					///< Point filtering
		FILTER_TRILINEAR,				///< Bilinear filtering
		FILTER_ANISOTROPIC				///< Anisotropic filtering
	};

	// Type which let you differentiate between filtering target
	enum FILTER_TYPE
	{
		FILTER_MIN	= (1<<0),			///< Minimize filter
		FILTER_MAG  = (1<<1),			///< Magnification filter
		FILTER_MIP	= (1<<2)			///< Mip-mapping filter
	};


	/// Method of texture addressing
	enum TEXTURE_ADDRESSING_MODE
	{
		TAM_UNKNOWN,
		TAM_CLAMP,
		TAM_WRAP,
		TAM_BORDER,
		TAM_MIRROR
	};

	/** Structure describing texture objects.
	*/
	typedef struct STextureDescription
	{
		TEXTURE_FORMAT format;			///< Format of the texture
		TEXTURE_TYPE type;				///< Type of the texture
		TEXTURE_USAGE usage;			///< Usage of the texture
		uint width;						///< Width of the texture
		uint height;					///< Height of the texture
		uint depth;						///< Depth of the texture

		wstring filename;				/** Image file for the texture.
										    It can be "" what states for
											texture existing in the memory
											only. */

		STextureDescription():
			format(TF_A8R8G8B8),
			type(TT_2D),
			usage(TU_NORMAL),
			width(0),
			height(0),
			depth(0),
			filename(L"")
		{
		}
	} TEXTURE_DESC;
}


#endif