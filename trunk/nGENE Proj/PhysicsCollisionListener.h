/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsCollisionListener.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PHYSICSCOLLISIONLISTENER_H_
#define __INC_PHYSICSCOLLISIONLISTENER_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"

#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxUserContactReport.h"
#pragma warning(pop)


namespace nGENE
{
	/// Collision listener for PhysX.
	class nGENEDLL PhysicsCollisionListener: public NxUserContactReport
	{
	public:
		void onContactNotify(NxContactPair& _pair, NxU32 _events);
	};
}


#endif