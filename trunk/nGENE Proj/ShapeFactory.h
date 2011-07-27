/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeFactory.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHAPEFACTORY_H_
#define __INC_SHAPEFACTORY_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Base ShapeFactory class.
		@remarks
			Each Shape class should be accompanied by its own (derived)
			ShapeFactory class.
	*/
	class nGENEDLL ShapeFactory
	{
	public:
		virtual Shape* createShape()=0;
	};
}


#endif