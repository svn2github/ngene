/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InputListener.cpp
Version:	0.51
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "InputListener.h"


namespace nGENE
{
	InputListener::InputListener()
	{
	}
//----------------------------------------------------------------------
	InputListener::~InputListener()
	{
	}
//----------------------------------------------------------------------
	void InputListener::handleEvent(const MouseEvent& _event)
	{
	}
//----------------------------------------------------------------------
	void InputListener::handleEvent(const KeyboardEvent& _event)
	{
	}
//----------------------------------------------------------------------
	void InputListener::setEnabled(bool _value)
	{
		Listener <MouseEvent>::setEnabled(_value);
		Listener <KeyboardEvent>::setEnabled(_value);
	}
//----------------------------------------------------------------------
	bool InputListener::isEnabled() const
	{
		return (Listener <MouseEvent>::isEnabled() && Listener <KeyboardEvent>::isEnabled());
	}
//----------------------------------------------------------------------
}