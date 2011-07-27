/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AIManager.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_AIMANAGER_H_
#define __INC_AIMANAGER_H_


#include "HashTable.h"
#include "Listener.h"
#include "ListenerRegistry.h"
#include "Log.h"
#include "Prerequisities.h"
#include "Singleton.h"
#include "Thread.h"
#include "Timer.h"
#include "TypeInfo.h"


namespace nGENE
{
namespace AI
{
	struct AIEvent;
	class AIActor;
	class FiniteStateMachine;
	class IAIActorFactory;


	#define AI_EVENT_END_LISTENING 0xffffffff


	/** Base AI event sent between actors.
	*/
	struct nGENEDLL AIEvent
	{
		int messageID;				///< Message ID

		AIActor* sender;			///< Sender of the message
		AIActor* receiver;			///< Receiver of the message

		dword delay;				///< Delay of message dispatching (in milliseconds)

		AIEvent()
		{
			messageID = 0;
			sender = NULL;
			receiver = NULL;
			delay = NULL;
		}

		friend bool operator<(const AIEvent& _evt1, const AIEvent& _evt2)
		{
			return _evt1.delay < _evt2.delay;
		}
	};

	/** A single Finite State Machine state.
	*/
	class nGENEDLL FSMState
	{
	protected:
		/** HashTable of valid transitions with key being an input
			and value a state.
		*/
		HashTable <int, FSMState*> m_States;

		/// Pointer to the FiniteStateMachine object.
		FiniteStateMachine* m_pFSM;
		
		/// Pointer to the previous state.
		FSMState* m_pPrevState;
		
	public:
		FSMState():
			m_pFSM(NULL),
			m_pPrevState(NULL)
		{
		}

		virtual void onEnter() {}
		virtual void onExit() {}

		virtual void update()=0;

		/** Adds transition between this and another state.
			@param _state pointer to ANOTHER state.
			@param _input event id which will cause transition.
		*/
		virtual void addTransition(FSMState* _state, int _input)
		{
			Assert(_state != this, "You cannot make transition to the state itself");

			m_States[_input] = _state;
		}

		/** Returns transition from this state to another one or 
			NULL if it does not exist.
		*/
		virtual FSMState* getTransition(int _input)
		{
			if(m_States.find(_input) == m_States.end())
				return NULL;

			return m_States[_input];
		}

		/// Checks for existence of a transition defined by given input.
		virtual bool hasTransition(int _input)
		{
			if(m_States.find(_input) == m_States.end())
				return false;

			return true;
		}

		/// Sets pointer to the previous state
		void setPrevState(FSMState* _state)
		{
			m_pPrevState = _state;
		}

		/// Returns pointer to the previous state.
		FSMState* getPrevState() const
		{
			return m_pPrevState;
		}

		/// Sets pointer to the FiniteStateMachine object.
		void setFSM(FiniteStateMachine* _value)
		{
			m_pFSM = _value;
		}

		/// Returns pointer to the FiniteStateMachine object.
		FiniteStateMachine* getFSM() const
		{
			return m_pFSM;
		}
	};

	/** Finite State Machine implementation.
		@remarks
			After creating FSM it is necessary to set it's
			active state by invoking setActiveState() and passing
			pointer to a valid state as an argument.
	*/
	class nGENEDLL FiniteStateMachine
	{
	protected:
		FSMState* m_pActiveState;					///< Pointer to the FSM object

	public:
		FiniteStateMachine():
			m_pActiveState(NULL)
		{
		}

		/// Changes state based on input.
		void changeState(int _input)
		{
			if(!m_pActiveState)
				return;

			FSMState* pState = m_pActiveState->getTransition(_input);
			if(!pState)
				return;

			pState->setPrevState(m_pActiveState);
			
			setActiveState(pState);
		}

		/// Updates currently active state.
		void update()
		{
			if(m_pActiveState)
				m_pActiveState->update();
		}

		/// Forces change of active state to the given one.
		void setActiveState(FSMState* _state)
		{
			if(m_pActiveState)
				m_pActiveState->onExit();

			m_pActiveState = _state;
			if(m_pActiveState)
			{
				m_pActiveState->setFSM(this);
				m_pActiveState->onEnter();
			}
		}

		/// Reverts to the previous state.
		void revertToPreviousState()
		{
			if(m_pActiveState)
				setActiveState(m_pActiveState->getPrevState());
		}
	};

	/** It is abstract brain interface.
		@par
			AI actor is entity performing some actions and the brain
			is the class which performs all the thinking, environment
			perceiving etc.
	*/
	class nGENEDLL IBrain
	{
	public:
		virtual void update()=0;
		virtual void handleEvent(const AIEvent& _evt)=0;
	};

	/** Finite State Machine based brain.
		@par
			Behaviour is determined by making transitions between
			states.
	*/
	class nGENEDLL FSMBrain: public IBrain
	{
	private:
		FiniteStateMachine* m_pFSM;

	public:
		FSMBrain():
			m_pFSM(NULL)
		{
		}

		FSMBrain(FiniteStateMachine* _fsm):
			m_pFSM(_fsm)
		{
		}

		/// Sets FSM for this brain.
		void setFSM(FiniteStateMachine* _fsm)
		{
			m_pFSM = _fsm;
		}

		/// Returns FSM of this brain.
		FiniteStateMachine* getFSM() const
		{
			return m_pFSM;
		}

		virtual void update()
		{
			if(m_pFSM)
				m_pFSM->update();
		}

		virtual void handleEvent(const AIEvent& _evt)
		{
			if(m_pFSM)
				m_pFSM->changeState(_evt.messageID);
		}
	};

	/** It is a base AI Actor (or agent) class for nGENE Tech.
		@remarks	
			This class provides only update() method which updates actor's
			brain.
	*/
	class nGENEDLL AIActor
	{
		EXPOSE_TYPE
	private:
		IBrain* m_pBrain;			///< Pointer to the brain object
		wstring m_stName;			///< Name of the actor

		IAIActorFactory* m_pFactory;///< Pointer to the actor's factory
		
	public:
		AIActor():
			m_pBrain(NULL)
		{
		}

		AIActor(IBrain* _brain):
			m_pBrain(_brain)
		{
		}

		/// Sets actor's brain.
		void setBrain(IBrain* _brain)
		{
			m_pBrain = _brain;
		}

		/// Returns actor's brain.
		IBrain* getBrain() const
		{
			return m_pBrain;
		}

		/// Updates actor's brain.
		virtual void update()
		{
			if(m_pBrain)
				m_pBrain->update();
		}

		/// Handles events received by this actor.
		virtual void handleEvent(const AIEvent& _evt)
		{
			if(m_pBrain)
				m_pBrain->handleEvent(_evt);
		}

		/// Sets actor's name.
		void setName(const wstring& _value)
		{
			m_stName = _value;
		}

		/// Returns actor's name.
		wstring& getName()
		{
			return m_stName;
		}

		void setCreator(IAIActorFactory* _factory)
		{
			m_pFactory = _factory;
		}

		IAIActorFactory* getCreator() const
		{
			return m_pFactory;
		}
	};

	/** FSM state to be used by AI actors.
	*/
	class nGENEDLL AIActorFSMState: public FSMState
	{
	protected:
		AIActor* m_pActor;				///< Pointer to the actor object
		
	public:
		AIActorFSMState():
			m_pActor(NULL)
		{
		}

		AIActorFSMState(AIActor* _actor):
			m_pActor(_actor)
		{
		}

		/// Sets pointer to the actor object.
		void setActor(AIActor* _value)
		{
			m_pActor = _value;
		}

		/// Returns pointer to the actor object.
		AIActor* getActor() const
		{
			return m_pActor;
		}
	};

	/** AI Actor to be used for flock simulation.
	*/
	class nGENEDLL BoidAIActor: public AIActor
	{
	public:
		virtual void separation() {}
		virtual void alignment() {}
		virtual void cohesion() {}
	};

	/** 
	*/
	class DNAAIActor: public AIActor
	{
	public:
		virtual void mutate() {}
	};

	/** Listener for AI events.
		@remarks	
			Runs on a separate thread.
	*/
	class nGENEDLL AIListener: public Listener <AIEvent>,
		public Thread
	{
	private:
		priority_queue <AIEvent> m_DelayedMsgsQueue;

		Mutex m_Mutex;
		Semaphore m_FillCount;
		Timer m_Timer;

	protected:
		virtual void run()
		{
			Thread::setName("AIListener");

			while(isRunning())
			{
				m_FillCount.down();

				bool bOK = true;

				m_Mutex.lock();

					AIEvent evt = m_DelayedMsgsQueue.top();

					if(evt.messageID == AI_EVENT_END_LISTENING)
					{
						m_Mutex.unlock();
						break;
					}

					if(evt.delay < m_Timer.getMilliseconds())
						m_DelayedMsgsQueue.pop();
					else
						bOK = false;

				m_Mutex.unlock();

				if(bOK)
					evt.receiver->handleEvent(evt);
			}

			signal();
		}

	public:
		AIListener():
			m_FillCount(0, 1000),
			m_Timer(false)
		{
		}

		virtual void handleEvent(const AIEvent& _evt)
		{
			if(!_evt.delay)
				_evt.receiver->handleEvent(_evt);
			else
			{
				AIEvent evt = _evt;
				evt.delay += m_Timer.getMilliseconds();
				
				m_Mutex.lock();
					m_DelayedMsgsQueue.push(evt);
				m_Mutex.unlock();
				m_FillCount.up(1);
			}
		}
	};

	/// Factory to be used for AIActor class objects creation.
	class nGENEDLL IAIActorFactory
	{
	private:
		bool m_bOnHeap;

	public:
		virtual AIActor* createAIActor()=0;
		virtual AIActor* createAIActor(IBrain* _brain)=0;

		virtual void removeAIActor(AIActor* _actor)
		{
			NGENE_DELETE(_actor);
		}

		void setAllocatedOnHeap(bool _value)
		{
			m_bOnHeap = _value;
		}

		bool isAllocatedOnHeap() const
		{
			return m_bOnHeap;
		}
	};

	/// Factory to be used for AIActor class objects creation.
	class nGENEDLL AIActorFactory: public IAIActorFactory
	{
	public:
		virtual AIActor* createAIActor()
		{
			return new AIActor();
		}

		virtual AIActor* createAIActor(IBrain* _brain)
		{
			return new AIActor(_brain);
		}
	};

	/** AIActor Enumerator class.
		@remarks
			Enumerators lets you easily extend nGENE Tech by just adding
			factories of new type to the enumerator. Then by specifying
			the type name you can create objects of any available type
			in the enumerator.
	*/
	class nGENEDLL AIActorEnumerator: public Singleton <AIActorEnumerator>
	{
	private:
		/// Hash table containing all factories
		HashTable <wstring, IAIActorFactory*> m_pFactories;

		// Default factories:

		/// Base AIActor factory
		AIActorFactory* m_pAIActor;

	public:
		AIActorEnumerator()
		{
			init();
		}

		virtual ~AIActorEnumerator()
		{
			cleanup();
		}

		void init()
		{
			// Create factories
			m_pAIActor = new AIActorFactory();

			// Add factories
			addFactory(m_pAIActor, AIActor::Type.Type());
		}

		/// Frees up memory.
		void cleanup()
		{
			if(!m_pFactories.empty())
			{
				HashTable <wstring, IAIActorFactory*>::iterator iter;
				for(iter = m_pFactories.begin(); iter != m_pFactories.end(); ++iter)
				{
					if(iter->second->isAllocatedOnHeap())
						NGENE_DELETE(iter->second);
				}
				m_pFactories.clear();
			}
		}

		/// Adds factory to the enumerator.
		void addFactory(IAIActorFactory& _factory, const wstring& _typeName)
		{
			// If factory already exists, return
			if(m_pFactories.find(_typeName) != m_pFactories.end())
				return;

			_factory.setAllocatedOnHeap(false);
			m_pFactories[_typeName] = &_factory;
		}

		/// Adds factory to the enumerator.
		void addFactory(IAIActorFactory* _factory, const wstring& _typeName)
		{
			// If factory already exists, return
			if(m_pFactories.find(_typeName) != m_pFactories.end())
				return;

			_factory->setAllocatedOnHeap(true);
			m_pFactories[_typeName] = _factory;
		}

		/// Removes factory with a given name.
		void removeFactory(const wstring& _typename)
		{
			// Factory doesn't exist, so return
			if(m_pFactories.find(_typename) == m_pFactories.end())
				return;

			m_pFactories.erase(_typename);
		}

		/// Creates object of a given type.
		AIActor* create(const wstring& _typeName)
		{
			// Appropriate factory does not exist, so return NULL
			if(m_pFactories.find(_typeName) == m_pFactories.end())
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Factory for type: %ls does not exist!", _typeName.c_str());
				return NULL;
			}

			AIActor* pActor = (m_pFactories[_typeName]->createAIActor());
			pActor->setCreator(m_pFactories[_typeName]);

			// Create new actor
			return pActor;
		}

		/// Creates object of a given type with a given brain.
		AIActor* create(const wstring& _typeName, IBrain* _brain)
		{
			// Appropriate factory does not exist
			if(m_pFactories.find(_typeName) == m_pFactories.end())
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Factory for type: %ls does not exist!", _typeName.c_str());
				return NULL;
			}

			AIActor* pActor = (m_pFactories[_typeName]->createAIActor(_brain));
			pActor->setCreator(m_pFactories[_typeName]);

			// Create new actor
			return pActor;
		}

		/// Destroys given object.
		void destroy(AIActor* _actor)
		{
			NGENE_DELETE(_actor);
		}
	};

	/** Manages all AI actors.
	*/
	class nGENEDLL AIManager: public Singleton <AIManager>
	{
	private:
		HashTable <wstring, AIActor*> m_Actors;

		AIListener m_AIListener;
		AIActorEnumerator m_AIActorEnumerator;

	public:
		AIManager()
		{
			ListenerRegistry <AIEvent>::addListener(&m_AIListener);
			m_AIListener.start();
		}

		virtual ~AIManager()
		{
			AIEvent evt;
			evt.delay = 1;
			evt.messageID = AI_EVENT_END_LISTENING;
			handleEvent(evt);

			ListenerRegistry <AIEvent>::removeListener(&m_AIListener);
			m_AIListener.wait();
			cleanup();
		}

		void handleEvent(AIEvent& _evt)
		{
			ListenerRegistry <AIEvent>::handleEvent(_evt);
		}

		/// Frees up allocated memory.
		virtual void cleanup()
		{
			removeAllActors();
		}

		/** Creates new AI Actor.
			@param _typename actor type (default - AIActor).
			@param _name desired and unique name of the actor.
			@returns
				AIActor* pointer to the newly created actor or to the
				actor with the _name if such an actor already exists.
		*/
		virtual AIActor* createActor(const wstring& _typename, const wstring& _name)
		{
			if(m_Actors.find(_name) != m_Actors.end())
				return m_Actors[_name];

			AIActor* pActor = m_AIActorEnumerator.create(_typename);
			pActor->setName(_name);
			m_Actors[_name] = pActor;

			return pActor;
		}

		/** Creates new AI Actor.
			@param _type actor type (default - AIActor).
			@param _name desired and unique name of the actor.
			@returns
				AIActor* pointer to the newly created actor or to the
				actor with the _name if such an actor already exists.
		*/
		virtual AIActor* createActor(const TypeInfo& _type, const wstring& _name)
		{
			if(m_Actors.find(_name) != m_Actors.end())
				return m_Actors[_name];

			AIActor* pActor = m_AIActorEnumerator.create(_type.Type());
			pActor->setName(_name);
			m_Actors[_name] = pActor;

			return pActor;
		}

		/** Creates new AI Actor.
			@param _name desired and unique name of the actor.
			@returns
				AIActor* pointer to the newly created actor or to the
				actor with the _name if such an actor already exists.
		*/
		template <typename T>
		T* createActor(const wstring& _name)
		{
			if(m_Actors.find(_name) != m_Actors.end())
				return (T*)m_Actors[_name];

			T* pActor = (T*)m_AIActorEnumerator.create(T::Type.Type());
			pActor->setName(_name);
			m_Actors[_name] = pActor;

			return pActor;
		}

		/** Creates new AI Actor.
			@param _typename actor type (default - AIActor).
			@param _name desired and unique name of the actor.
			@param _brain brain with which actor should be created.
			@returns
				AIActor* pointer to the newly created actor or to the
				actor with the _name if such an actor already exists.
		*/
		virtual AIActor* createActor(const wstring& _typename, const wstring& _name, IBrain* _brain)
		{
			/// Actor already created, so return it
			if(m_Actors.find(_name) != m_Actors.end())
				return m_Actors[_name];
			
			AIActor* pActor = m_AIActorEnumerator.create(_typename, _brain);
			pActor->setName(_name);
			m_Actors[_name] = pActor;

			return pActor;
		}

		/** Creates new AI Actor.
			@param _typename actor type (default - AIActor).
			@param _name desired and unique name of the actor.
			@param _brain brain with which actor should be created.
			@returns
				AIActor* pointer to the newly created actor or to the
				actor with the _name if such an actor already exists.
		*/
		virtual AIActor* createActor(const TypeInfo& _type, const wstring& _name, IBrain* _brain)
		{
			/// Actor already created, so return it
			if(m_Actors.find(_name) != m_Actors.end())
				return m_Actors[_name];

			AIActor* pActor = m_AIActorEnumerator.create(_type.Type(), _brain);
			pActor->setName(_name);
			m_Actors[_name] = pActor;

			return pActor;
		}

		/** Creates new AI Actor.
			@param _name desired and unique name of the actor.
			@param _brain brain with which actor should be created.
			@returns
				AIActor* pointer to the newly created actor or to the
				actor with the _name if such an actor already exists.
		*/
		template <typename T>
		T* createActor(const wstring& _name, IBrain* _brain)
		{
			if(m_Actors.find(_name) != m_Actors.end())
				return (T*)m_Actors[_name];

			T* pActor = (T*)m_AIActorEnumerator.create(T::Type.Type(), _brain);
			pActor->setName(_name);
			m_Actors[_name] = pActor;

			return pActor;
		}

		/** Adds existing AI actor.
			@param _name desired and unique name of the actor.
			@param _brain brain with which actor should be created.
		*/
		template <typename T>
		void addActor(const wstring& _name, T* _actor)
		{
			if(m_Actors.find(_name) == m_Actors.end())
			{
				_actor->setName(_name);
				m_Actors[_name] = _actor;
			}
		}

		/// Returns actor with the given name or null if it does not exist.
		AIActor* getActor(const wstring& _name)
		{
			if(m_Actors.find(_name) == m_Actors.end())
				return NULL;

			return m_Actors[_name];
		}

		/** Checks if an actor with the given name exists and returns true in
			such a case and false otherwise.
		*/
		bool hasActor(const wstring& _name)
		{
			if(m_Actors.find(_name) == m_Actors.end())
				return false;

			return true;
		}

		/// Removes actor with a given name.
		void removeActor(const wstring& _name)
		{
			// Actor doesn't exist, so exit
			if(m_Actors.find(_name) == m_Actors.end())
				return;

			// @todo do something about this!
			//NGENE_DELETE(m_Actors[_name]);
			m_Actors.erase(_name);
		}

		/// Removes actor with a given address.
		void removeActor(AIActor* _actor)
		{
			HashTable <wstring, AIActor*>::iterator iter;
			for(iter = m_Actors.begin(); iter != m_Actors.end(); ++iter)
			{
				if(iter->second == _actor)
					break;
			}

			if(iter != m_Actors.end())
			{
				// @todo do something about this!
				//NGENE_DELETE(iter->second);
				m_Actors.erase(iter);
			}
		}

		/// Removes all actors.
		void removeAllActors()
		{
			if(m_Actors.size())
			{
				HashTable <wstring, AIActor*>::iterator iter;
				for(iter = m_Actors.begin(); iter != m_Actors.end(); ++iter)
				{
					IAIActorFactory* pFactory = iter->second->getCreator();
					if(pFactory)
						pFactory->removeAIActor(iter->second);
				}
				m_Actors.clear();
			}
		}

		/// Updates all the actors.
		void update()
		{
			HashTable <wstring, AIActor*>::iterator iter;
			for(iter = m_Actors.begin(); iter != m_Actors.end(); ++iter)
			{
				iter->second->update();
			}
		}
	};
}
}


#endif