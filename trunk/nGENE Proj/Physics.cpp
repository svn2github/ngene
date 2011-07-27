/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Physics.cpp
Version:	0.12
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Physics.h"
#include "ForceFieldShapeEnumerator.h"
#include "JointEnumerator.h"
#include "PhysicsWorld.h"
#include "PhysXAllocator.h"
#include "ShapeEnumerator.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxControllerManager.h"
	#include "PhysXLoader.h"
#pragma warning(pop)


namespace nGENE
{
	Physics::Physics():
		m_pPhysics(NULL),
		m_pShapeEnumerator(new ShapeEnumerator()),
		m_pForceFieldShapeEnumerator(new ForceFieldShapeEnumerator()),
		m_pJointEnumerator(new JointEnumerator()),
		m_pControllerManager(NULL),
		m_pAllocator(NULL),
		m_bIsEnabled(true)
	{
		init();
	}
//----------------------------------------------------------------------
	Physics::~Physics()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Physics::init()
	{
		// Initialize Ageia PhysX
		m_pPhysics = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, m_pAllocator);

		if(!m_pPhysics)
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Initializing Ageia PhysX engine failed");
			return;
		}

		m_pPhysics->setParameter(NX_SKIN_WIDTH, 0.01f);

		// Initialize character controller manager
		m_pAllocator = new PhysXAllocator();
		m_pControllerManager = NxCreateControllerManager(m_pAllocator);
	}
//----------------------------------------------------------------------
	void Physics::cleanup()
	{
		NGENE_DELETE(m_pForceFieldShapeEnumerator);
		NGENE_DELETE(m_pShapeEnumerator);

		// Remove all worlds
		if(!m_pWorlds.empty())
		{
			m_vWorlds.clear();

			HashTable <wstring, PhysicsWorld*>::iterator iter;
			for(iter = m_pWorlds.begin(); iter != m_pWorlds.end(); ++iter)
				NGENE_DELETE(iter->second);

			m_pWorlds.clear();
		}

		// Remove characters
		m_pControllerManager->purgeControllers();
		NxReleaseControllerManager(m_pControllerManager);

		m_pPhysics->release();
		m_pPhysics = NULL;

		NGENE_DELETE(m_pAllocator);
	}
//----------------------------------------------------------------------
	void Physics::update()
	{
		if(m_bIsEnabled)
		{
			// Update all worlds
			HashTable <wstring, PhysicsWorld*>::iterator iter = m_pWorlds.begin();
			for(iter = m_pWorlds.begin(); iter != m_pWorlds.end(); ++iter)
			{
				PhysicsWorld* pWorld = iter->second;
				if(pWorld->isEnabled())
					pWorld->update();
			}

			// Update character controllers
			m_pControllerManager->updateControllers();
		}
	}
//----------------------------------------------------------------------
	PhysicsWorld* Physics::createWorld(const wstring& _name)
	{
		// World already exists, so exit
		if(m_pWorlds.find(_name) != m_pWorlds.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"World with the name: %ls already exists", _name.c_str());
			return m_pWorlds[_name];
		}

		PhysicsWorld* pWorld = new PhysicsWorld(m_pPhysics);
		pWorld->setName(_name);

		// Add to containers
		m_pWorlds[_name] = pWorld;
		m_vWorlds.push_back(pWorld);

		return pWorld;
	}
//----------------------------------------------------------------------
	PhysicsWorld* Physics::getWorld(const wstring& _name)
	{
		// World doesn't exist, so exit
		if(m_pWorlds.find(_name) == m_pWorlds.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"World: %ls does not exist", _name.c_str());
			return NULL;
		}

		return m_pWorlds[_name];
	}
//----------------------------------------------------------------------
	PhysicsWorld* Physics::getWorld(uint _index)
	{
		if(_index >= m_vWorlds.size())
			return NULL;

		return m_vWorlds[_index];
	}
//----------------------------------------------------------------------
	void Physics::removeWorld(const wstring& _name)
	{
		// World doesn't exist, so exit
		if(m_pWorlds.find(_name) == m_pWorlds.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"World: %ls does not exist", _name.c_str());
			return;
		}

		PhysicsWorld* pWorld = m_pWorlds[_name];
		uint i = 0;
		for(i = 0; i < m_vWorlds.size(); ++i)
		{
			if(m_vWorlds[i] == pWorld)
				break;
		}
		
		m_vWorlds.erase(m_vWorlds.begin() + i);
		NGENE_DELETE(m_pWorlds[_name]);
		m_pWorlds.erase(_name);
	}
//----------------------------------------------------------------------
	void Physics::removeWorld(uint _index)
	{
		// World doesn't exist, so exit
		if(_index >= m_vWorlds.size())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"World does not exist");
			return;
		}

		wstring stName = m_vWorlds[_index]->getName();

		m_vWorlds.erase(m_vWorlds.begin() + _index);
		NGENE_DELETE(m_pWorlds[stName]);
		m_pWorlds.erase(stName);
	}
//----------------------------------------------------------------------
	ShapeEnumerator* Physics::getShapeEnumerator() const
	{
		return m_pShapeEnumerator->getSingletonPtr();
	}
//----------------------------------------------------------------------
	ForceFieldShapeEnumerator* Physics::getForceFieldShapeEnumerator() const
	{
		return m_pForceFieldShapeEnumerator->getSingletonPtr();
	}
//----------------------------------------------------------------------
	JointEnumerator* Physics::getJointEnumerator() const
	{
		return m_pJointEnumerator->getSingletonPtr();
	}
//----------------------------------------------------------------------
}