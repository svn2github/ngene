/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		EngineUpdateListener.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ENGINEUPDATELISTENER_H_
#define __INC_ENGINEUPDATELISTENER_H_


#include "Listener.h"
#include "Prerequisities.h"


namespace nGENE
{
	/// Frame event
	struct EngineUpdateEvent
	{
	};

	/** Interface providing Listener for a Renderer
		@remarks
			EngineUpdateListener allows you to control update process.
			This is called once per engine update.
		@par
			To use it, you have to derive this class and provide
			desired functionality in the subclass.
	*/
	class nGENEDLL EngineUpdateListener: public Listener <EngineUpdateEvent>
	{
	public:
		EngineUpdateListener();
		virtual ~EngineUpdateListener();

		virtual void handleEvent(const EngineUpdateEvent& _event)=0;
	};
}


#endif