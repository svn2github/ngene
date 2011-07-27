/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RendererTypes.h
Version:	0.12
Info:		This file contains various types definitions which
			are used by the renderer.
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RENDERERTYPES_H_
#define __INC_RENDERERTYPES_H_


#include "Prerequisities.h"
#include "DeviceRenderTypes.h"
#include "TextureTypes.h"
#include "UtilitiesTypes.h"


namespace nGENE
{
	/** Type of the API used in rendering process
		@todo
			Change this into enumerator.
	*/
	enum API_TYPE
	{
		API_UNKNOWN,
		API_DIRECTX9C,
		API_DIRECTX10,
		API_OPENGL
	};


	/// Clipping planes flags
	enum CLIPPING_PLANES
	{
		PLANE_NONE = 0,
		PLANE_0_ENABLED = (1<<0),
		PLANE_1_ENABLED = (1<<1),
		PLANE_2_ENABLED = (1<<2),
		PLANE_3_ENABLED = (1<<3),
		PLANE_4_ENABLED = (1<<4),
		PLANE_5_ENABLED = (1<<5)
	};


	/// Type of blending
	enum BLEND_TYPE
	{
		BLEND_ZERO = 1,
		BLEND_ONE = 2,
		BLEND_SRCCOLOR = 3,
		BLEND_INVSRCCOLOR = 4,
		BLEND_SRCALPHA = 5,
		BLEND_INVSRCALPHA = 6,
		BLEND_DESTALPHA = 7,
		BLEND_INVDESTALPHA = 8,
		BLEND_DESTCOLOR = 9,
		BLEND_INVDESTCOLOR = 10,
		BLEND_SRCALPHASAT = 11,
		BLEND_BOTHSRCALPHA = 12,
		BLEND_BOTHINVSRCALPHA = 13,
		BLEND_BLENDFACTOR = 14,
		BLEND_INVBLENDFACTOR = 15,
	};

	/// Blending method
	enum BLEND_OPERATION
	{
		BO_ADD = 1,
		BO_SUBTRACT = 2,
		BO_REVSUBTRACT = 3,
		BO_MIN = 4,
		BO_MAX = 5
	};


	/** Type of the buffer
	*/
	enum CLEAR_BUFFER_TYPE
	{
		CB_UNKNOWN	= (1<<0),
		CB_DEPTH	= (1<<1),
		CB_STENCIL	= (1<<2),
		CB_TARGET	= (1<<3)
	};


	/** Drawing mode
	*/
	enum FILL_MODE
	{
		FILL_UNKNOWN	= (1<<0),
		FILL_DOT		= (1<<1),
		FILL_WIREFRAME	= (1<<2),
		FILL_SOLID		= (1<<3),
		FILL_TEXTURED	= (1<<4)
	};


	/** Culling mode of back faces
	*/
	enum CULL_MODE
	{
		CULL_NONE,						///< No culling
		CULL_CW,						///< Cull clockwise
		CULL_CCW						///< Cull counter-clockwise
	};


	/// Specifies rendering mode.
	enum RENDER_MODE
	{
		RM_2D,							///< 2D mode
		RM_3D							///< 3D mode
	};


	/// Used to make global states
	enum GLOBAL_STATE
	{
		GS_NONE =					0,
		GS_ZBIAS =					(1<<0),
		GS_GAMMA =					(1<<1),
		GS_LIGHT =					(1<<2),
		GS_ALPHABLEND =				(1<<3),
		GS_SEPARATEALPHABLEND =		(1<<4),
		GS_ZWRITE =					(1<<5),
		GS_ZBUFFER =				(1<<6),
		GS_ALPHATEST =				(1<<7),
		GS_SCISSORTEST =			(1<<8),
		GS_ALPHATESTREF =			(1<<9),
		GS_SOURCE =					(1<<10),
		GS_DESTINATION =			(1<<11),
		GS_ALPHABLENDSOURCE =		(1<<12),
		GS_ALPHABLENDDESTINATION =	(1<<13),
		GS_BLENDOP =				(1<<14),
		GS_CULLING =				(1<<15),
		GS_FILLMODE =				(1<<16),
		GS_AMBIENT =				(1<<17)
	};


	/// Structure describing single rendering state.
	typedef struct nGENEDLL SRenderState
	{
		uint globalStates;

		/// Z-bias value
		float zBias;
		/// Gamma aspect
		float gamma;

		/// Is lighting enabled? If not, no lighting model will
		/// be rendered.
		bool light;
		/// Is alpha blending enabled?
		bool alphaBlend;
		/// Is separate alpha blending enabled?
		bool separateAlphaBlend;

		/// Is writing to Z-buffer enabled?
		bool zWrite;
		/// Is Z-buffer enabled?
		bool zBuffer;

		/// Is alpha test enabled?
		bool isAlphaTest;

		/// Is scissor test enabled?
		bool scissorTest;

		/// Alpha test reference value
		dword alphaTestRef;

		/// Source blending type
		BLEND_TYPE source;
		/// Destination blending type
		BLEND_TYPE destination;

		/// Source blending type
		BLEND_TYPE alphaBlendSource;
		/// Destination blending type
		BLEND_TYPE alphaBlendDestination;

		/// Type of the operation
		BLEND_OPERATION blendOP;

		/// Back face culling
		CULL_MODE culling;
		/// Fill mode
		FILL_MODE fillMode;
		/// Ambient light colour
		Colour ambient;


		SRenderState():
			globalStates(0),
			gamma(1.0f),
			light(true),
			alphaBlend(false),
			separateAlphaBlend(false),
			zWrite(true),
			zBuffer(true),
			isAlphaTest(false),
			scissorTest(false),
			alphaTestRef(1),
			source(BLEND_ONE),
			destination(BLEND_ZERO),
			alphaBlendSource(BLEND_ONE),
			alphaBlendDestination(BLEND_ZERO),
			blendOP(BO_ADD),
			culling(CULL_CW),
			fillMode(FILL_SOLID),
			zBias(0.0f),
			ambient(Colour::COLOUR_BLACK)
		{
		}
	} RENDER_STATE;


	/** Structure describing a display mode.
		@remarks
			Apart from regular display modes it contains all
			graphics related settings of the application.
	*/
	struct nGENEDLL SDisplayMode
	{
		/// Width of the screen in pixels
		uint width;
		/// Height of the screen in pixels
		uint height;

		/** Specifies which clipping planes are enabled. Each bit stays
			for one user defined clipping plane.
		*/
		uint clippingPlanesFlag;

		/// Determines whether an application is run in windowed
		/// or full screen mode: if true runs in window, otherwise
		/// in full-screen.
		bool windowed;
		/// Determines whether VSync is enabled
		bool vsync;

		RENDER_STATE state;
		SRect <uint> scissorRectangle;

		RENDER_MODE mode;

		/// Texture filtering
		TEXTURE_FILTER filterMin[8];
		TEXTURE_FILTER filterMag[8];
		TEXTURE_FILTER filterMip[8];

		TEXTURE_ADDRESSING_MODE addressing[8];

		/// Anisotropy level (if anisotropic filtering is used)
		uint anisotropyLevel[8];

		/// Vertices processing mode
		PROCESSING_TYPE processingType;


		SDisplayMode():
			width(800),
			height(600),
			clippingPlanesFlag(0),
			windowed(true),
			vsync(true),
			mode(RM_3D),
			processingType(PROCESSING_HARDWARE)
		{
			for(uint i = 0; i < 8; ++i)
			{
				filterMin[i] =
				filterMag[i] =
				filterMip[i] = FILTER_TRILINEAR;

				anisotropyLevel[i] = 1;

				addressing[i] = TAM_CLAMP;
			}
		}
	};


	/** Representation of the post-process effect.
		@remarks
			As often order of post-process effects play important
			role, you can use priority feature. The higher the priority
			is, the later the effect will be applied.
	*/
	struct nGENEDLL SPriorityMaterial
	{
		uint priority;
		Material* material;
		bool enabled;

		SPriorityMaterial():
			priority(0),
			material(NULL),
			enabled(true)
		{

		}

		friend bool operator<(const SPriorityMaterial& _mat1, const SPriorityMaterial& _mat2)
		{
			return _mat1.priority < _mat2.priority;
		}
	};
}


#endif