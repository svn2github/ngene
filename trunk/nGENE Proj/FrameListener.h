/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FrameListener.h
Version:	0.54
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRAMELISTENER_H_
#define __INC_FRAMELISTENER_H_


#include "Listener.h"
#include "Prerequisities.h"


namespace nGENE
{
	/// Frame event
	struct FrameEvent
	{
		bool isFrameBegin;

		FrameEvent():
			isFrameBegin(false)
		{
		}
	};

	/** Interface providing Listener for a Renderer
		@remarks
			FrameListener allows you to control rendering process.
			You can setup application before the frame is rendered
			using onFrameBegin() function and after the frame is
			rendered using onFrameEnd(). This can be used to
			update data or influence rendering.
		@par
			To use it, you have to derive this class and provide
			desired functionality in the subclass.
	*/
	class nGENEDLL FrameListener: public Listener <FrameEvent>
	{
	public:
		FrameListener();
		virtual ~FrameListener();

		/** Actions performed when rendering frame starts.
		*/
		virtual void onFrameBegin() {}
		/** Actions performed after rendering frame finishes.
		*/
		virtual void onFrameEnd() {}

		virtual void handleEvent(const FrameEvent& _event);
	};
}


#endif