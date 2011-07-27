/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceRenderTypes.h
Version:	0.01
Info:		This file contains various types definitions which
			are used by the device render.
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DEVICERENDERTYPES_H_
#define __INC_DEVICERENDERTYPES_H_


#include "Prerequisities.h"
#include "UtilitiesTypes.h"


namespace nGENE
{
	/// Type of the rendering device event
	enum DEVICE_EVENT_TYPE
	{
		DET_UNKNOWN = 0,
		DET_LOST,
		DET_RESET,
		DET_CREATED,
		DET_DESTROYED,
		DET_CHANGED
	};

	/// Render device event
	struct DeviceEvent
	{
		DEVICE_EVENT_TYPE type;

		DeviceEvent():
			type(DET_UNKNOWN)
		{
		}
	};

	/// Enumeration of different capabilities engine could be tested against.
	enum nGENE_CAPABILITIES
	{
		CAPS_UNKNOWN,
		CAPS_MAX_ANISOTROPY,				/**< Maximum anisotropy for anisotropic
												 texture filtering */
		CAPS_MIN_ANISOTROPY,				/**< Anisotropy for minifying textures is
												 supported. */
		CAPS_MAG_ANISOTROPY,				/**< Anisotropy for magnifying textures is
											 	 supported. */
		CAPS_MAX_CLIPPLANES,				/**< How many clipping planes could
												 be defined and used */
		CAPS_MAX_INDICES_COUNT,				/**< How many vertex indices can be used
												 in a buffer */
		CAPS_MAX_PRIMITIVES_COUNT,			/**< How many primitives can be used in
												 one draw call. */
		CAPS_MAX_LIGHTS,					/**< Maximum number of active lights
												 being active at the same time */
		CAPS_MAX_RTS,						/**< How many simultanous render targets
												 can be used */
		CAPS_MAX_TEXTURE_HEIGHT,			///< Maximum texture height
		CAPS_MAX_TEXTURE_WIDTH,				///< Maximum texture width
		CAPS_VERTEX_SHADER,					///< What version of vertex shaders is supported?
		CAPS_GEOMETRY_SHADER,				///< What version of geometry shaders is supported?
		CAPS_PIXEL_SHADER					///< What version of pixel shaders is supported?
	};


	/// Determines the way vertices are processed.
	enum PROCESSING_TYPE
	{
		PROCESSING_UNKNOWN,
		PROCESSING_SOFTWARE,					///< Vertices are processed by software only.
		PROCESSING_MIXED,						/**< Vertices are processed both by software
													 and hardware. */
		PROCESSING_HARDWARE,					///< Vertices are processed by hardware only.
		PROCESSING_HARDWARE_PURE				///< Vertices are PUREly processed by hardware.
	};
}


#endif