/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsTriggerListener.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PhysicsTriggerListener.h"
#include "TriggerListener.h"
#include "Shape.h"

#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxActor.h"
#pragma warning(pop)


namespace nGENE
{
	void PhysicsTriggerListener::onTrigger(NxShape& _triggerShape, NxShape& _otherShape, NxTriggerFlag _status)
	{
		Shape* pTrigger = static_cast<Shape*>(_triggerShape.userData);
		Shape* pOther = static_cast<Shape*>(_otherShape.userData);

		// No shapes
		if(!pTrigger || !pOther)
			return;

		// Two triggers cannot collide with each other
		if(pOther->isTrigger())
			return;

		TriggerEvent evt;

		evt.triggerShape = pTrigger;
		evt.otherShape = pOther;
		evt.type = 0;

		if(_status & NX_TRIGGER_ON_ENTER)
			evt.type |= TT_START;
		if(_status & NX_TRIGGER_ON_LEAVE)
			evt.type |= TT_END;
		if(_status & NX_TRIGGER_ON_STAY)
			evt.type |= TT_ON;
		
		// Handle event
		ListenerRegistry <TriggerEvent>::handleEvent(evt);
	}
//----------------------------------------------------------------------
}
