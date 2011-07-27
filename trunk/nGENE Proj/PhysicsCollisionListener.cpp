/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsCollisionListener.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PhysicsCollisionListener.h"
#include "CollisionListener.h"

#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxActor.h"
#pragma warning(pop)


namespace nGENE
{
	void PhysicsCollisionListener::onContactNotify(NxContactPair& _pair, NxU32 _events)
	{
		if(_pair.actors[0] && _pair.actors[1] &&
		   !_pair.isDeletedActor[0] && !_pair.isDeletedActor[1])
		{
			CollisionEvent evt;
			evt.actor1 = (PhysicsActor*)_pair.actors[0]->userData;
			evt.actor2 = (PhysicsActor*)_pair.actors[1]->userData;
			evt.type = 0;

			// Set event flags
			if(_events & NX_NOTIFY_ON_START_TOUCH)
				evt.type |= CT_START;
			if(_events & NX_NOTIFY_ON_TOUCH)
				evt.type |= CT_ON;
			if(_events & NX_NOTIFY_ON_END_TOUCH)
				evt.type |= CT_END;

			// Get contact points
			NxContactStreamIterator i(_pair.stream);
			while(i.goNextPair())
			{
				while(i.goNextPatch())
				{
					while(i.goNextPoint())
					{
						const NxVec3& contactPoint = i.getPoint();
						Vector3 vecContact(contactPoint.x, contactPoint.y, contactPoint.z);

						evt.contactPoints.push_back(vecContact);
					}
				}
			}

			// Handle event
			ListenerRegistry <CollisionEvent>::handleEvent(evt);
		}
	}
//----------------------------------------------------------------------
}
