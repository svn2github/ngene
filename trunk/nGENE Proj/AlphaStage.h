/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AlphaStage.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ALPHASTAGE_H_
#define __INC_ALPHASTAGE_H_


#include "Prerequisities.h"
#include "HashTable.h"
#include "RenderStage.h"


namespace nGENE
{
	/** Alpha rendering stage.
		@par
			Separate alpha rendering stage is necessary as nGENE
			uses DirectX 9.0c deferred renderer by default.
	*/
	class nGENEDLL AlphaStage: public RenderStage
	{
	private:
		/// Quad for rendering post-process effects
		ScreenOverlay* m_pPostProcessQuad;

		/** As this renderer uses deferred shading we have to keep track
			of semi-transparent materials as well.
		*/
		HashTable <wstring, RenderQueue> m_AlphaRenderQueues;

	public:
		AlphaStage();
		virtual ~AlphaStage();

		void init();
		void cleanup();

		/// This method is used for semi-transparent geometry rendering.
		virtual void render();
		virtual void endRender();

		/// Adds semi-transparent materials to be rendered.
		virtual void addToRender(const wstring& _name, Surface* _renderable);
	};
}


#endif