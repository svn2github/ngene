/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PostStage.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_POSTSTAGE_H_
#define __INC_POSTSTAGE_H_


#include "Prerequisities.h"
#include "RenderStage.h"


namespace nGENE
{
	/** Post-process rendering stage.
		@par
			This stage is responsible for rendering two kinds of materials:
			* post processing materials applied as full screen quads.
			* per object post processing materials.
	*/
	class nGENEDLL PostStage: public RenderStage
	{
	private:
		/// Quad for rendering post-process effects
		ScreenOverlay* m_pPostProcessQuad;

		vector <SPriorityMaterial*> m_pPostProcess;

		/** As this renderer uses deferred shading we have to keep track
			of post process/geometry materials as well.
		*/
		HashTable <wstring, RenderQueue> m_PPRenderQueues;


		vector <Surface*> testVisibility();
		void renderGeometry(vector <Surface*>& _surfaces);

	public:
		PostStage();
		virtual ~PostStage();

		void init();
		void cleanup();

		/** Renders post processing effects and materials eg. glow, blur,
			refraction.
			@remarks
				Actually appropriate passes are rendered in this function.
		*/
		virtual void render();
		virtual void endRender();

		/// Adds post processing material to be rendered.
		virtual void addToRender(SPriorityMaterial* _mat);

		/// Adds post processing/geometry materials to be rendered.
		virtual void addToRender(const wstring& _name, Surface* _renderable);

		/// Removes post processing material from rendering.
		virtual void removeFromRender(SPriorityMaterial* _mat);

		/// Removes all materials.
		virtual void removeAllMaterials();

		/// Sort effects based on their priorities.
		void sortMaterials();
	};
}


#endif