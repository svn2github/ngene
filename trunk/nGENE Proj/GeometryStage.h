/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GeometryStage.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GEOMETRYSTAGE_H_
#define __INC_GEOMETRYSTAGE_H_


#include "Prerequisities.h"
#include "RenderStage.h"


namespace nGENE
{
	/** Geometry rendering stage.
	*/
	class nGENEDLL GeometryStage: public RenderStage
	{
	private:
		/// HashTable containing all render queues.
		HashTable <wstring, RenderQueue> m_RenderQueues;
		HashTable <wstring, RenderQueue> m_PreRenderQueues;

		/// All materials in this stage.
		HashTable <wstring, Material*> m_pMaterials;


		void testVisibility();

	public:
		GeometryStage();
		virtual ~GeometryStage();

		void init();

		void onBeginFrame();

		/** This method is used for geometry stage of deferred shading.
			@remarks
				The standard material plays role of diffuse buffer, but
				several other rendering passes are applied (eg. normals,
				specular) to enhance visual quality of the final image.
		*/
		virtual void render();
		virtual void endRender();

		/// Adds materials to be rendered.
		virtual void addToRender(Material* _mat, Surface* _renderable);
	};
}


#endif