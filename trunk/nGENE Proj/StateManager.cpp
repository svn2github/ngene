/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		StateManager.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "StateManager.h"


namespace nGENE
{
	StateManager::StateManager()
	{
	}
//----------------------------------------------------------------------
	StateManager::~StateManager()
	{
	}
//----------------------------------------------------------------------
	void StateManager::push(const RENDER_STATE& _state)
	{
		m_States.push(_state);
	}
//----------------------------------------------------------------------
	RENDER_STATE StateManager::pop()
	{
		RENDER_STATE result = m_States.top();
		m_States.pop();

		return result;
	}
//----------------------------------------------------------------------
	RENDER_STATE& StateManager::top()
	{
		return m_States.top();
	}
//----------------------------------------------------------------------
	bool StateManager::isEmpty() const
	{
		return m_States.empty();
	}
//----------------------------------------------------------------------
}
