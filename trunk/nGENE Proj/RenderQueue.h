/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RenderQueue.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RENDERQUEUE_H_
#define __INC_RENDERQUEUE_H_


#include "BaseClass.h"
#include "IRenderable.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** RenderQueue is a queue containing all IRenderables to
		be rendered.
	*/
	class nGENEDLL RenderQueue: public BaseClass
	{
		EXPOSE_TYPE
	private:
		/// Vector of Surfaces to be rendered
		vector <Surface*> m_pNodes;

	public:
		RenderQueue();
		~RenderQueue();

		void init();
		void cleanup();

		/** Adds IRenderable to the queue, making it possible to
			render this IRenderable.
			@remarks
				This method should be called when specific
				conditions have been met. For example after
				positive result of visibility/occlusion test.
		*/
		void push_back(Surface* _node);

		/// Clears the queue.
		void clear();

		/// Renders all IRenderables in the queue.
		void render(RenderPass* _pass);

		vector <Surface*>& getSurfaces();

		/** Returns number of surfaces currently in queue.
			@returns
				uint number of surfaces in the queue. It is important
				to notice that only visible and enabled surfaces count.
		*/
		uint getSurfacesNum() const;
	};



	inline vector <Surface*>& RenderQueue::getSurfaces()
	{
		return m_pNodes;
	}
//----------------------------------------------------------------------
}


#endif