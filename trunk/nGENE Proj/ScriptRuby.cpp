/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScriptRuby.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ScriptRuby.h"

//#include <ruby.h>


namespace nGENE
{
	// Initialize static members
	TypeInfo ScriptRuby::Type(L"ScriptRuby", &BaseClass::Type);


	ScriptRuby::ScriptRuby()
	{
	}
//----------------------------------------------------------------------
	ScriptRuby::~ScriptRuby()
	{
	}
//----------------------------------------------------------------------
	void ScriptRuby::init()
	{
//		RUBY_INIT_STACK
//		ruby_init();
//		ruby_script("nGENE Tech");
	}
//----------------------------------------------------------------------
	void ScriptRuby::cleanup()
	{
	}
//----------------------------------------------------------------------
}