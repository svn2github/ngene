/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Singleton.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SINGLETON_H_
#define __INC_SINGLETON_H_


#include "Prerequisities.h"
#include "Assertion.h"


namespace nGENE
{
	/** Singleton pattern implemented using templates.
		@remarks
			Using singleton is useful for objects such as engine
			or managers which should have no more than a single
			instance in the whole application.
		@remarks
			If you want to make object of some class singleton you
			have to	derive this class from Singleton class.
		@remarks
			If something goes wrong during singleton object creation,
			deletion or on attempt to access it, assertion arises.
			By "something going wrong" I also mean attempts to create
			more than single object of class marked singleton.
	*/
	template <typename T>
	class Singleton
	{
	public:
		/// A static pointer to an object of T type.
		/// This is the core member of the singleton. 
		/// As it is declared as static no more than
		/// one object of this type can exist at the time.
		static T* s_pSingleton;

	public:
		/** Creates singleton of type T.
		*/
		Singleton()
		{
			Assert(!s_pSingleton, "Singleton class already instantiated.");

			#if defined( _MSC_VER ) && _MSC_VER < 1200
				int offset=(int)(T*)1-(int)(Singleton <T>*)(T*)1;
				s_pSingleton=(T*)((int)this+offset);
			#else
				s_pSingleton=static_cast<T*>(this);
			#endif
		}

		virtual ~Singleton()
		{
			Assert(s_pSingleton, "Singleton class not instantiated.");
			s_pSingleton=0;
		}

		/** Returns reference to a singleton.
		*/
		static T& getSingleton()
		{
			Assert(s_pSingleton, "Singleton class not instantiated.");
			return(*s_pSingleton);
		}

		/** Returns pointer to a singleton.
		*/
		static T* getSingletonPtr()
		{
			return s_pSingleton;
		}
	};


	template <typename T> T* Singleton <T>::s_pSingleton=0;
}


#endif