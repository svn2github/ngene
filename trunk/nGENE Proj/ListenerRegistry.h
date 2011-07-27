/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ListenerRegistry.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_LISTENERREGISTRY_H_
#define __INC_LISTENERREGISTRY_H_


#include "HashTable.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Lets you add listeners and link them to the messages.
		@remarks
			All of the methods of this class are marked static. If
			you want to add new listener, listening for events of
			type EventType, you have to write:
			ListenerRegistry <EventType>::addListener(&listener).
		@par
			If you want to handle an event call:
			ListenerRegistry <EventType>::handleEvent(event).
		@par
			Note that you cannot instantiate this class.
	*/
	template <typename EventType>
	class ListenerRegistry
	{
	private:
		typedef list <Listener <EventType>* > ListenersList;

		/// Static list containing all listeners.
		static ListenersList s_Listeners;


		ListenerRegistry() {}
		~ListenerRegistry() {}

	public:
		static void cleanup();

		/** Registers new listener.
			@param
				_listener pointer to the listener being added.
		*/
		static void addListener(Listener <EventType>* _listener);

		/** Removes listener from the list.
			@param
				_listener listener to remove.
		*/
		static void removeListener(Listener <EventType>* _listener);

		/** Handles given event. It calls appropriate method (handleEvent())
			of all active listeners related to the event.
			@param
				_event an event to be handled.
		*/
		static void handleEvent(const EventType& _event);
	};


	template <typename EventType> list <Listener <EventType>*> ListenerRegistry <EventType>::s_Listeners;



	template <typename EventType>
	void ListenerRegistry <EventType>::addListener(Listener<EventType>* _listener)
	{
		s_Listeners.push_back(_listener);
	}
//----------------------------------------------------------------------
	template <typename EventType>
	void ListenerRegistry <EventType>::removeListener(Listener<EventType>* _listener)
	{
		ListenersList::iterator iter = s_Listeners.begin();
		ListenersList::iterator iterEnd = s_Listeners.end();
		bool deleted = false;
		while(iter != iterEnd && !deleted)
		{
			if((*iter) == _listener)
			{
				ListenersList::iterator iterTemp = iter++;
				s_Listeners.erase(iterTemp);
				deleted = true;
			}
			else
				++iter;
		}
	}
//----------------------------------------------------------------------
	template <typename EventType>
	void ListenerRegistry <EventType>::handleEvent(const EventType& _event)
	{
		ListenersList::iterator iter;
		ListenersList::iterator iterEnd = s_Listeners.end();
		for(iter = s_Listeners.begin(); iter != iterEnd; ++iter)
		{
			if((*iter)->isEnabled())
			{
				(*iter)->handleEvent(_event);
			}
		}
	}
//----------------------------------------------------------------------
}


#endif