/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Listener.h
Version:	0.55
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_LISTENER_H_
#define __INC_LISTENER_H_


#include "Prerequisities.h"


namespace nGENE
{
	class nGENEDLL Event
	{
	};

	/** Pure interface defining Listener (Observer) pattern.
		@remarks
			Listeners are waiting for event to occur and when
			it does perform specified action.
	*/
	template <typename EventType=Event>
	class Listener
	{
	private:
		/** Determines whether the Listener is valid or not.
			@remarks
				If the Listener is inactive/disabled it will
				not handle the in-coming events. Thus different
				Listeners can be used on different ocassions
				(eg. one in the main menu, one in in-game menu and
				yet another when the game is active).
			@par
				By default is set to true.
		*/
		bool m_bIsEnabled;

	public:
		Listener();

		/** Performs operation based on the received message.
			@param
				_message input message
		*/
		virtual void handleEvent(const EventType& _evt)=0;

		/// Lets you turn on/turn off frame listener.
		virtual void setEnabled(bool _value);
		/// Checks if the FrameListener is enabled.
		virtual bool isEnabled() const;
	};



	template <typename EventType>
	inline Listener <EventType>::Listener():
		m_bIsEnabled(true)
	{
	}
//----------------------------------------------------------------------
	template <typename EventType>
	inline void Listener <EventType>::setEnabled(bool _value)
	{
		m_bIsEnabled = _value;
	}
//----------------------------------------------------------------------
	template <typename EventType>
	inline bool Listener <EventType>::isEnabled() const
	{
		return m_bIsEnabled;
	}
//----------------------------------------------------------------------
}


#endif