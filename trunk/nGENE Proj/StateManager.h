/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		StateManager.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_STATEMANAGER_H_
#define __INC_STATEMANAGER_H_


#include "Prerequisities.h"
#include "RendererTypes.h"
#include "Singleton.h"


namespace nGENE
{
	/** Provides management of the render states.
	*/
	class nGENEDLL StateManager: public Singleton <StateManager>
	{
	private:
		stack <RENDER_STATE> m_States;

	public:
		StateManager();
		~StateManager();

		/// Pushes state.
		void push(const RENDER_STATE& _state);

		/// Pops top element and returns it.
		RENDER_STATE pop();
		/// Returns top element.
		RENDER_STATE& top();

		/// Checks if there are no states.
		bool isEmpty() const;
	};
}


#endif