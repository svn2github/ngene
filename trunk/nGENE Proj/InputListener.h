/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InputListener.h
Version:	0.51
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_INPUTLISTENER_H_
#define __INC_INPUTLISTENER_H_


#include "Listener.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Interface providing Listener for input system.
		@par
			It listens to the mouse and keyboard events.
	*/
	class nGENEDLL InputListener: public Listener <MouseEvent>,
		public Listener <KeyboardEvent>
	{
	public:
		InputListener();
		virtual ~InputListener();

		/// Handles mouse events.
		virtual void handleEvent(const MouseEvent& _event);
		/// Handles keyboard events.
		virtual void handleEvent(const KeyboardEvent& _event);

		/// Lets you turn on/turn off frame listener.
		virtual void setEnabled(bool _value);
		/// Checks if the FrameListener is enabled.
		virtual bool isEnabled() const;
	};
}


#endif