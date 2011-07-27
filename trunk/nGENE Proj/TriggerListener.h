/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TriggerListener.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TRIGGERLISTENER_H_
#define __INC_TRIGGERLISTENER_H_


#include "Prerequisities.h"
#include "Listener.h"
#include "Vector3.h"


namespace nGENE
{
	/// Type of the trigger event
	enum TRIGGER_TYPE
	{
		TT_START = (1 << 0),					///< Shape entered trigger area
		TT_ON	 = (1 << 1),					///< Shape is still in trigger area
		TT_END   = (1 << 2)						///< Shape left trigger area
	};

	/// Trigger event.
	struct nGENEDLL TriggerEvent
	{
		Shape* triggerShape;					///< Pointer to the trigger shape
		Shape* otherShape;						///< Pointer to the other shape

		uint type;								///< Combination of TRIGGER_TYPE flags
	};

	/// nGENE Tech trigger listener.
	class nGENEDLL TriggerListener: public Listener <TriggerEvent>
	{
	public:
		/// Handles trigger events between actors.
		virtual void handleEvent(const TriggerEvent& _event) {}
	};
}


#endif