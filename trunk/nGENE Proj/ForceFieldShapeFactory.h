/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapeFactory.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FORCEFIELDSHAPEFACTORY_H_
#define __INC_FORCEFIELDSHAPEFACTORY_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Base ForceFieldShapeFactory class.
		@remarks
			Each ForceFieldShape class should be accompanied by its own (derived)
			ForceFieldShapeFactory class.
	*/
	class nGENEDLL ForceFieldShapeFactory
	{
	public:
		virtual ForceFieldShape* createShape()=0;
	};
}


#endif