/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Physics.h
Version:	0.10
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PHYSICS_H_
#define __INC_PHYSICS_H_


#include "AutoPointer.h"
#include "HashTable.h"
#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "Singleton.h"
#include "Task.h"


namespace nGENE
{
	/** Physics class.
		@remarks
			Ageia PhysX is used as the underlying physics
			library.
		@par
			Physics is running on another thread (if possible).
	*/
	class nGENEDLL Physics: public Singleton <Physics>, public Task
	{
	private:
		/// Pointer to the PhysX engine
		NxPhysicsSDK* m_pPhysics;

		/// Pointer to the controller manager
		NxControllerManager* m_pControllerManager;

		/// Custom memory allocator
		PhysXAllocator* m_pAllocator;

		/// HashTable holding all worlds
		HashTable <wstring, PhysicsWorld*> m_pWorlds;

		/// Vector holding all worlds
		vector <PhysicsWorld*> m_vWorlds;

		/// Shape enumerator for physics shapes
		ShapeEnumerator* m_pShapeEnumerator;

		/// Shape enumerator for force field shapes
		ForceFieldShapeEnumerator* m_pForceFieldShapeEnumerator;

		/// Joint enumerator
		JointEnumerator* m_pJointEnumerator;


		/// Determines if Physics simulation is enabled.
		bool m_bIsEnabled;

	public:
		Physics();
		~Physics();

		void init();
		void cleanup();

		/// Updates physics.
		void update();

		/// Creates new PhysicsWorld.
		PhysicsWorld* createWorld(const wstring& _name);
		/// Returns PhysicsWorld with a given name.
		PhysicsWorld* getWorld(const wstring& _name);
		/// Returns PhysicsWorld with a given index.
		PhysicsWorld* getWorld(uint _index);

		/// Removes given physics world.
		void removeWorld(const wstring& _name);
		/// Removes given physics world.
		void removeWorld(uint _index);

		/// Returns number of worlds.
		uint getPhysicsWorldCount() const;

		NxControllerManager* getControllerManager() const;


		/// Returns shape enumerator.
		ShapeEnumerator* getShapeEnumerator() const;
		/// Returns force field shape enumerator.
		ForceFieldShapeEnumerator* getForceFieldShapeEnumerator() const;
		/// Returns joint enumerator.
		JointEnumerator* getJointEnumerator() const;


		/// Specifies whether manager is enabled.
		void setEnabled(bool _value);
		/// Checks if manager is enabled.
		bool isEnabled() const;

		NxPhysicsSDK* getPhysX() const;
	};



	inline void Physics::setEnabled(bool _value)
	{
		m_bIsEnabled = _value;
	}
//----------------------------------------------------------------------
	inline bool Physics::isEnabled() const
	{
		return m_bIsEnabled;
	}
//----------------------------------------------------------------------
	inline NxControllerManager* Physics::getControllerManager() const
	{
		return m_pControllerManager;
	}
//----------------------------------------------------------------------
	inline NxPhysicsSDK* Physics::getPhysX() const
	{
		return m_pPhysics;
	}
//----------------------------------------------------------------------
	inline uint Physics::getPhysicsWorldCount() const
	{
		return m_pWorlds.size();
	}
//----------------------------------------------------------------------
}


#endif