/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScriptRuby.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCRIPTRUBY_H_
#define __INC_SCRIPTRUBY_H_


#include "BaseClass.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** ScriptRuby is a way to embed Ruby in your applications.
		@todo
			Implement this
	*/
	class nGENEDLL ScriptRuby: public BaseClass
	{
		EXPOSE_TYPE
	private:

	public:
		ScriptRuby();
		virtual ~ScriptRuby();

		void init();
		void cleanup();

		void runScript(const string& _fileName) {}
	};
}


#endif