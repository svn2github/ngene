/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointFactory.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTFACTORY_H_
#define __INC_JOINTFACTORY_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Base JointFactory class.
		@remarks
			Each Joint class should be accompanied by its own (derived)
			JointFactory class.
	*/
	class nGENEDLL JointFactory
	{
	public:
		virtual Joint* createJoint(NxScene* _scene, JOINT_DESC& _desc)=0;
	};
}


#endif