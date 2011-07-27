/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Factory.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FACTORY_H_
#define __INC_FACTORY_H_


#include "Singleton.h"


namespace nGENE
{
	/** Factory design pattern implemented using templates.
	*/
	template <typename T>
	class Factory: public Singleton <Factory <T> >
	{
	public:
		Factory() {}
		virtual ~Factory() {}

		static T* cloneObject(const T& _obj) {return &_obj;}
		static T* copyObject(const T& _obj) {return new T(_obj);}
		/** Creates new object of type T
			@remarks
				It is important to notice that the T type
				has to have default constuctor as it is used
				to create new object.
		*/
		static T* createObject() {return new T();}
		static void deleteObject() {}
	};
}


#endif