/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsTriggerListener.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PHYSICSTRIGGERLISTENER_H_
#define __INC_PHYSICSTRIGGERLISTENER_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"

#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxUserContactReport.h"
#pragma warning(pop)


namespace nGENE
{
	/// Collision listener for PhysX.
	class nGENEDLL PhysicsTriggerListener: public NxUserTriggerReport
	{
	public:
		void onTrigger(NxShape& _triggerShape, NxShape& _otherShape,
			NxTriggerFlag _status);
	};
}


#endif