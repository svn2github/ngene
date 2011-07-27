/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CollisionListener.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_COLLISION_LISTENER_H_
#define __INC_COLLISION_LISTENER_H_


#include "Prerequisities.h"
#include "Listener.h"
#include "Vector3.h"


namespace nGENE
{
	/// Type of the collision event
	enum COLLISION_TYPE
	{
		CT_START = (1 << 0),
		CT_ON	 = (1 << 1),
		CT_END   = (1 << 2)
	};

	/// Collision event.
	struct nGENEDLL CollisionEvent
	{
		PhysicsActor* actor1;					///< Pointer to the first actor
		PhysicsActor* actor2;					///< Pointer to the second actor

		uint type;								///< Combination of COLLISION_TYPE flags

		vector <Vector3> contactPoints;			///< Points in which collision occurred
	};

	/// Collision event between character controller and scene.
	struct nGENEDLL ControllerCollisionEvent
	{
		CharacterController* controller;		///< Pointer to the character controller
		PhysicsActor* actor;					///< Pointer to the actor

		Vector3 motionDir;						///< Movement direction
		Real motionLength;						///< Movement length

		vector <Vector3> contactPoints;			///< Points in which collision occurred
		vector <Vector3> contactNormals;		///< Normals at collision points
	};

	/// Collision event between two character controllers.
	struct nGENEDLL ControllerControllerCollisionEvent
	{
		CharacterController* controller1;		///< Pointer to the first controller
		CharacterController* controller2;		///< Pointer to the second controller
	};


	/// nGENE Tech collision listener.
	class nGENEDLL CollisionListener: public Listener <CollisionEvent>,
		public Listener <ControllerCollisionEvent>,
		public Listener <ControllerControllerCollisionEvent>
	{
	public:
		/// Handles collision events between actors.
		virtual void handleEvent(const CollisionEvent& _event) {}

		/// Handles collision events between controllers and scene.
		virtual void handleEvent(const ControllerCollisionEvent& _event) {}

		/// Handles collision event between two character controllers.
		virtual void handleEvent(const ControllerControllerCollisionEvent& _event) {}
	};
}


#endif