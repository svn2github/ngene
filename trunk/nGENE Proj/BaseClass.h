/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		BaseClass.h
Version:	0.53
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_BASECLASS_H_
#define __INC_BASECLASS_H_


#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** A base class for almost all nGENE Tech classes.
		@remarks
			It provides basic functionality: initializing, cleaning up.
	*/
	class nGENEDLL BaseClass
	{
		EXPOSE_TYPE
	public:
		BaseClass();
		virtual ~BaseClass();

		/// Initializes class.
		virtual void init()=0;
		/// Releases all resources.
		virtual void cleanup()=0;
	};
}


#endif