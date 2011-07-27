/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RendererFactory.h
Version:	0.11
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RENDERERFACTORY_H_
#define __INC_RENDERERFACTORY_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Type of the API used in rendering process
		@todo
			Change this into enumerator.
	*/
	/*enum API_TYPE
	{
		API_UNKNOWN,
		API_DIRECTX9C,
		API_DIRECTX10,
		API_OPENGL
	};*/

	/** Factory used to create renderer matching demanded API.
	*/
	class nGENEDLL RendererFactory
	{
	public:
		RendererFactory();
		virtual ~RendererFactory();

		/** Creates demanded renderer.
			@param
				api rendering api. As for now only API_DIRECTX9C,
				standing for DirectX 9.0c is supported, but in
				the future also DirectX 10 and OpenGL will be
				available.
			@param
				renderWindows as nGENE can handle rendering to multiple
				windows you have to specify how many of them is to be used.
			@returns
				Renderer* new renderer
		*/
		static Renderer* createRenderer(API_TYPE _api=API_DIRECTX9C,
			bool _windowed=true, uint _renderWindows=1);

		static wstring getRenderingAPIName(API_TYPE _api);
	};
}


#endif