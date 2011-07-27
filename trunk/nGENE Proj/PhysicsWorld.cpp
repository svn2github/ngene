/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsWorld.cpp
Version:	0.22
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PhysicsWorld.h"
#include "CharacterController.h"
#include "CollisionListener.h"
#include "ForceField.h"
#include "ForceFieldShapeEnumerator.h"
#include "ForceFieldShapesGroup.h"
#include "Joint.h"
#include "JointEnumerator.h"
#include "PhysicsCollisionListener.h"
#include "PhysicsTriggerListener.h"
#include "PhysicsMaterial.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"
#include "ShapeEnumerator.h"
#include "TriggerListener.h"
#include "Vehicle.h"


#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
	#include "NxScene.h"
	#include "NxSceneDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo PhysicsWorld::Type(L"PhysicsWorld", &BaseClass::Type);


	PhysicsWorld::PhysicsWorld(NxPhysicsSDK* _physX):
		m_pScene(NULL),
		m_pPhysX(_physX),
		m_stName(L""),
		m_bEnabled(true),
		m_pCollisionListener(NULL),
		m_pTriggerListener(NULL)
	{
		init();
	}
//----------------------------------------------------------------------
	PhysicsWorld::~PhysicsWorld()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PhysicsWorld::init()
	{
		// Create event listeners
		m_pCollisionListener = new PhysicsCollisionListener();
		m_pTriggerListener = new PhysicsTriggerListener();

		// Create PhysX scene
		NxSceneDesc desc;
		desc.simType = NX_SIMULATION_HW;
		desc.userContactReport = m_pCollisionListener;
		desc.userTriggerReport = m_pTriggerListener;
		desc.gravity = PhysXMapping::vector3ToNxVec3(Vector3(0.0f, -9.8f, 0.0f));

		m_pScene = m_pPhysX->createScene(desc);
		// No hardware support - try to recreate in software mode
		if(!m_pScene)
		{
			desc.simType = NX_SIMULATION_SW;
			m_pScene = m_pPhysX->createScene(desc);
			if(!m_pScene)
			{
				Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Physical world could not be created");
				return;
			}
			else
			{
				Log::log(LET_COMMENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"PhysX running in SOFTWARE mode");
			}
		}
		else
		{
			Log::log(LET_COMMENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"PhysX running in HARDWARE mode");
		}

		// Set fixed time step
		m_pScene->setTiming(m_WorldDesc.simulationTimeStep, 4, NX_TIMESTEP_FIXED);

		// Set the counter
		m_lPrevTime = Engine::getSingleton().getTimer().getMilliseconds();

		// Simulate physics for the 1st time
		float fDelta = static_cast<float>(Engine::getSingleton().getTimer().getMilliseconds() - m_lPrevTime) * 0.001f;
		m_pScene->simulate(fDelta);
		m_pScene->flushStream();
	}
//----------------------------------------------------------------------
	void PhysicsWorld::cleanup()
	{
		// Update before exiting
		Timer& timer = Engine::getSingleton().getTimer();

		float fDelta = static_cast<float>(timer.getMilliseconds() - m_lPrevTime) * 0.001f;
		m_pScene->simulate(fDelta);
		m_pScene->flushStream();
		while(!m_pScene->fetchResults(NX_RIGID_BODY_FINISHED, false));

		if(!m_pShapes.empty())
		{
			vector <Shape*>::iterator iter;
			for(iter = m_pShapes.begin(); iter != m_pShapes.end(); ++iter)
				NGENE_DELETE(*iter);

			m_pShapes.clear();
		}

		if(!m_pBodies.empty())
		{
			HashTable <wstring, PhysicsActor*>::iterator iter;
			for(iter = m_pBodies.begin(); iter != m_pBodies.end(); ++iter)
				NGENE_DELETE(iter->second);
			m_pBodies.clear();
		}

		if(!m_pCharacterControllers.empty())
		{
			HashTable <wstring, CharacterController*>::iterator iter;
			for(iter = m_pCharacterControllers.begin(); iter != m_pCharacterControllers.end(); ++iter)
				NGENE_DELETE(iter->second);
			m_pCharacterControllers.clear();
		}

		if(!m_pForceFields.empty())
		{
			HashTable <wstring, ForceField*>::iterator iter;
			for(iter = m_pForceFields.begin(); iter != m_pForceFields.end(); ++iter)
				NGENE_DELETE(iter->second);
			m_pForceFields.clear();
		}

		if(!m_pJoints.empty())
		{
			HashTable <wstring, Joint*>::iterator iter;
			for(iter = m_pJoints.begin(); iter != m_pJoints.end(); ++iter)
				NGENE_DELETE(iter->second);
			m_pJoints.clear();
		}

		if(!m_pMaterials.empty())
		{
			HashTable <wstring, PhysicsMaterial*>::iterator iter;
			for(iter = m_pMaterials.begin(); iter != m_pMaterials.end(); ++iter)
				NGENE_DELETE(iter->second);
			m_pMaterials.clear();
			m_vMaterials.clear();
		}

		m_pPhysX->releaseScene(*m_pScene);
		m_pScene = NULL;
		m_pPhysX = NULL;

		// Remove all event listeners
		removeAllCollisionListeners();
		removeAllTriggerListeners();

		NGENE_DELETE(m_pCollisionListener);
		NGENE_DELETE(m_pTriggerListener);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::update()
	{
		// Check if there are no broken joints
		HashTable <wstring, Joint*>::iterator iter5;
		for(iter5 = m_pJoints.begin(); iter5 != m_pJoints.end(); ++iter5)
		{
			if(iter5->second->isBroken())
			{
				NGENE_DELETE((iter5->second));
				m_pJoints.erase(iter5);
			}
		}

		// Update the scene.
		Timer& timer = Engine::getSingleton().getTimer();

		float fDelta = static_cast<float>(timer.getMilliseconds() - m_lPrevTime) * 
			0.001f * m_WorldDesc.slowMotionRate;

		m_pScene->simulate(fDelta);
		m_pScene->flushStream();
		while(!m_pScene->fetchResults(NX_RIGID_BODY_FINISHED, false));

		// Update all actors
		HashTable <wstring, PhysicsActor*>::iterator iter1;
		for(iter1 = m_pBodies.begin(); iter1 != m_pBodies.end(); ++iter1)
			iter1->second->update();

		// Update all character controllers
		HashTable <wstring, CharacterController*>::iterator iter2;
		for(iter2 = m_pCharacterControllers.begin(); iter2 != m_pCharacterControllers.end(); ++iter2)
			iter2->second->update(fDelta);

		// Update all vehicles
		HashTable <wstring, Vehicle*>::iterator iter3;
		for(iter3 = m_pVehicles.begin(); iter3 != m_pVehicles.end(); ++iter3)
			iter3->second->update(fDelta);

		// Update all force fields
		HashTable <wstring, ForceField*>::iterator iter4;
		for(iter4 = m_pForceFields.begin(); iter4 != m_pForceFields.end(); ++iter4)
			iter4->second->update();


		m_lPrevTime = timer.getMilliseconds();
	}
//----------------------------------------------------------------------
	void PhysicsWorld::setGravity(const Vector3& _grav)
	{
		m_WorldDesc.vecGravity = _grav;
		m_pScene->setGravity(PhysXMapping::vector3ToNxVec3(_grav));
	}
//----------------------------------------------------------------------
	void PhysicsWorld::setGravity(Real _x, Real _y, Real _z)
	{
		m_WorldDesc.vecGravity = Vector3(_x, _y, _z);
		m_pScene->setGravity(PhysXMapping::vector3ToNxVec3(m_WorldDesc.vecGravity));
	}
//----------------------------------------------------------------------
	PhysicsActor* PhysicsWorld::createPhysicsActor()
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new physics actor");

		PhysicsActor* pBody = new PhysicsActor(m_pScene, this);
		return pBody;
	}
//----------------------------------------------------------------------
	PhysicsActor* PhysicsWorld::createPhysicsActor(const wstring& _shapeTypeName,
		const Vector3& _dimensions,	PhysicsActor::BODY_TYPE _type, bool _trigger,
		const Vector3& _initialVelocity)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new physics actor of shape: %ls", _shapeTypeName.c_str());

		// Create new actor
		Shape* pShape = createShape(_shapeTypeName);
		pShape->setDimensions(_dimensions);
		pShape->setTrigger(_trigger);

		PhysicsActor::COLLISION_GROUP collision = (m_WorldDesc.collisionDetection ?
													PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE : 
													PhysicsActor::GROUP_NON_COLLIDABLE);

		PhysicsActor* pBody = new PhysicsActor(m_pScene,
											   pShape,
											   this,
											   _type,
											   collision,
											   _trigger,
											   _initialVelocity);
		pBody->setCollidable(collision);

		m_pShapes.push_back(pShape);


		return pBody;
	}
//----------------------------------------------------------------------
	PhysicsActor* PhysicsWorld::createPhysicsActor(Shape* _shape,
		PhysicsActor::BODY_TYPE _type, const Vector3& _initialVelocity)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new physics actor");

		// Create new actor
		PhysicsActor* pBody = new PhysicsActor(m_pScene,
											   _shape,
											   this,
											   _type,
											   PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE,
											   false,
											   _initialVelocity);

		PhysicsActor::COLLISION_GROUP collision = (m_WorldDesc.collisionDetection ?
													PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE : 
													PhysicsActor::GROUP_NON_COLLIDABLE);
		pBody->setCollidable(collision);

		m_pShapes.push_back(_shape);


		return pBody;
	}
//----------------------------------------------------------------------
	PhysicsActor* PhysicsWorld::createPhysicsActor(const vector <Shape*>& _shapes,
		PhysicsActor::BODY_TYPE _type, const Vector3& _initialVelocity)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new physics actor");

		// Create new actor
		PhysicsActor* pBody = new PhysicsActor(m_pScene,
											   _shapes,
											   this,
											   _type,
											   PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE,
											   false,
											   _initialVelocity);

		PhysicsActor::COLLISION_GROUP collision = (m_WorldDesc.collisionDetection ?
			PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE : 
		PhysicsActor::GROUP_NON_COLLIDABLE);
		pBody->setCollidable(collision);

		for(uint i = 0; i < _shapes.size(); ++i)
			m_pShapes.push_back(_shapes[i]);


		return pBody;
	}
//----------------------------------------------------------------------
	PhysicsActor* PhysicsWorld::createPhysicsActor(const PHYSICS_ACTOR_DESC& _desc)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new physics actor");

		// Create new actor
		PhysicsActor* pBody = new PhysicsActor(m_pScene, this, _desc);

		PhysicsActor::COLLISION_GROUP collision = (m_WorldDesc.collisionDetection ?
			PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE : 
		PhysicsActor::GROUP_NON_COLLIDABLE);
		pBody->setCollidable(collision);

		m_pShapes.push_back(_desc.shape);


		return pBody;
	}
//----------------------------------------------------------------------
	PhysicsActor* PhysicsWorld::createPhysicsActorMesh(const Vector3& _dimensions,
			const VertexBuffer& _vb, PhysicsActor::BODY_TYPE _type,
			bool _flipNormals, const Vector3& _initialVelocity)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new physics actor from 3D mesh");

		// Create new actor
		ShapeMesh* pShape = static_cast<ShapeMesh*>(createShape(L"Mesh"));
		pShape->setDimensions(_dimensions);
		pShape->setGeometryData(_vb, m_pPhysX, _flipNormals);
		PhysicsActor* pBody = new PhysicsActor(m_pScene,
											   pShape,
											   this,
											   _type,
											   PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE,
											   false,
											   _initialVelocity);
		
		PhysicsActor::COLLISION_GROUP collision = (m_WorldDesc.collisionDetection ?
													PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE : 
													PhysicsActor::GROUP_NON_COLLIDABLE);
		pBody->setCollidable(collision);

		m_pShapes.push_back(pShape);


		return pBody;
	}
//----------------------------------------------------------------------
	Joint* PhysicsWorld::createJoint(const wstring& _jointTypeName, JOINT_DESC& _desc)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new joint of type: %ls", _jointTypeName.c_str());

		// Create new Joint
		Joint* pJoint = JointEnumerator::getSingleton().create(m_pScene, _jointTypeName, _desc);
		pJoint->init();

		m_pJoints[_jointTypeName] = pJoint;

		return pJoint;
	}
//----------------------------------------------------------------------
	PhysicsActor* PhysicsWorld::getPhysicsActor(const wstring& _name)
	{
		// Body does not exist, so exit
		if(m_pBodies.find(_name) == m_pBodies.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Body: %ls does not exist", _name.c_str());
			return NULL;
		}

		return m_pBodies[_name];
	}
//----------------------------------------------------------------------
	ForceField* PhysicsWorld::getForceField(const wstring& _name)
	{
		// Force field does not exist, so exit
		if(m_pForceFields.find(_name) == m_pForceFields.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Force field: %ls does not exist", _name.c_str());
			return NULL;
		}

		return m_pForceFields[_name];
	}
//----------------------------------------------------------------------
	Joint* PhysicsWorld::getJoint(const wstring& _name)
	{
		// Joint does not exist, so exit
		if(m_pJoints.find(_name) == m_pJoints.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Joint: %ls does not exist", _name.c_str());
			return NULL;
		}

		return m_pJoints[_name];
	}
//----------------------------------------------------------------------
	CharacterController* PhysicsWorld::getCharacterController(const wstring& _name)
	{
		// Force field does not exist, so exit
		if(m_pCharacterControllers.find(_name) == m_pCharacterControllers.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Character controller: %ls does not exist", _name.c_str());
			return NULL;
		}

		return m_pCharacterControllers[_name];
	}
//----------------------------------------------------------------------
	void PhysicsWorld::addPhysicsActor(const wstring& _name, PhysicsActor* _actor)
	{
		// The actor already exists, so return
		if(m_pBodies.find(_name) != m_pBodies.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Rigid Body: %ls already exists", _name.c_str());
			return;
		}
		else
		{
			Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Adding physics actor: %ls", _name.c_str());
		}

		m_pBodies[_name] = _actor;
	}
//----------------------------------------------------------------------
	CharacterController* PhysicsWorld::createController(CONTROLLER_DESC& _desc)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new character controller");

		CharacterController* pController = new CharacterController(m_pScene, _desc);
		pController->init();

		return pController;
	}
//----------------------------------------------------------------------
	ForceField* PhysicsWorld::createForceField(FORCE_FIELD_DESC& _desc)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new force field");

		ForceField* pForceField = new ForceField(m_pScene, _desc);
		pForceField->init();

		return pForceField;
	}
//----------------------------------------------------------------------
	ForceFieldShape* PhysicsWorld::createForceFieldShape(const wstring& _shapeTypeName)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new force field shape: %ls", _shapeTypeName.c_str());

		return ForceFieldShapeEnumerator::getSingleton().create(_shapeTypeName);
	}
//----------------------------------------------------------------------
	ForceFieldShapesGroup* PhysicsWorld::createForceFieldShapesGroup(bool _isIncluded)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating new force field shape group");

		ForceFieldShapesGroup* pGroup = new ForceFieldShapesGroup(m_pScene, _isIncluded);
		return pGroup;
	}
//----------------------------------------------------------------------
	void PhysicsWorld::addForceField(const wstring& _name, ForceField* _field)
	{
		// The force field already exists, so return
		if(m_pForceFields.find(_name) != m_pForceFields.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Force field: %ls already exists", _name.c_str());
			return;
		}
		else
		{
			Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Adding force field: %ls", _name.c_str());
		}

		m_pForceFields[_name] = _field;
	}
//----------------------------------------------------------------------
	void PhysicsWorld::addController(const wstring& _name, CharacterController* _controller)
	{
		// The controller already exists, so return
		if(m_pCharacterControllers.find(_name) != m_pCharacterControllers.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Character controller: %ls already exists", _name.c_str());
			return;
		}
		else
		{
			Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Adding character controller: %ls", _name.c_str());
		}

		m_pCharacterControllers[_name] = _controller;
	}
//----------------------------------------------------------------------
	Vehicle* PhysicsWorld::createVehicle(PhysicsActor* _body)
	{
		Vehicle* pVehicle = new Vehicle(this, _body);
		pVehicle->init();

		return pVehicle;
	}
//----------------------------------------------------------------------
	void PhysicsWorld::addVehicle(const wstring& _name, Vehicle* _vehicle)
	{
		// The vehicle already exists, so return
		if(m_pVehicles.find(_name) != m_pVehicles.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Vehicle: %ls already exists", _name.c_str());
			return;
		}
		else
		{
			Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Adding vehicle: %ls", _name.c_str());
		}

		m_pVehicles[_name] = _vehicle;
	}
//----------------------------------------------------------------------
	PhysicsActor* PhysicsWorld::createTerrain(const TERRAIN_DESC& _desc)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating terrain physics representation");

		// Create new actor
		ShapeTerrain* pShape = static_cast<ShapeTerrain*>(createShape(L"Terrain"));
		pShape->init(m_pScene, _desc);
		PhysicsActor* pBody = new PhysicsActor(m_pScene, pShape, this, PhysicsActor::BT_STATIC);
		
		PhysicsActor::COLLISION_GROUP collision = (m_WorldDesc.collisionDetection ?
													PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE : 
													PhysicsActor::GROUP_NON_COLLIDABLE);
		pBody->setCollidable(collision);

		m_pShapes.push_back(pShape);


		return pBody;
	}
//----------------------------------------------------------------------
	Shape* PhysicsWorld::createShape(const wstring& _shapeType)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating shape of type: %ls", _shapeType.c_str());

		return ShapeEnumerator::getSingleton().create(_shapeType);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::removePhysicsActor(const wstring& _name)
	{
		// Physics actor does not exist, so exit
		if(m_pBodies.find(_name) == m_pBodies.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"PhysicsActor: %ls does not exist", _name.c_str());
			return;
		}

		NGENE_DELETE(m_pBodies[_name]);
		m_pBodies.erase(_name);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::removePhysicsActor(PhysicsActor* _actor)
	{
		HashTable <wstring, PhysicsActor*>::iterator iter;
		for(iter = m_pBodies.begin(); iter != m_pBodies.end(); ++iter)
		{
			if(iter->second == _actor)
				break;
		}

		if(iter == m_pBodies.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"PhysicsActor does not exist");
			return;
		}

		NGENE_DELETE(iter->second);
		m_pBodies.erase(iter);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::removeCharacterController(const wstring& _name)
	{
		// Controller does not exist, so exit
		if(m_pCharacterControllers.find(_name) == m_pCharacterControllers.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Character controller: %ls does not exist", _name.c_str());
			return;
		}

		NGENE_DELETE(m_pCharacterControllers[_name]);
		m_pCharacterControllers.erase(_name);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::removeForceField(const wstring& _name)
	{
		// Force field does not exist, so exit
		if(m_pForceFields.find(_name) == m_pForceFields.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Force field: %ls does not exist", _name.c_str());
			return;
		}

		NGENE_DELETE(m_pForceFields[_name]);
		m_pForceFields.erase(_name);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::removeJoint(const wstring& _name)
	{
		// Joint does not exist, so exit
		if(m_pJoints.find(_name) == m_pJoints.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Joint: %ls does not exist", _name.c_str());
			return;
		}

		NGENE_DELETE(m_pJoints[_name]);
		m_pJoints.erase(_name);
	}
//----------------------------------------------------------------------
	PhysicsMaterial* PhysicsWorld::createPhysicsMaterial(const wstring& _name)
	{
		// The material already exists, so return it
		if(m_pMaterials.find(_name) != m_pMaterials.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"The material with the name: %ls already exists", _name.c_str());
			return m_pMaterials[_name];
		}

		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating physics material: %ls", _name.c_str());

		PhysicsMaterial* pMaterial = new PhysicsMaterial(m_pScene);
		pMaterial->setName(_name);
		m_pMaterials[_name] = pMaterial;

		m_vMaterials.push_back(pMaterial);


		return pMaterial;
	}
//----------------------------------------------------------------------
	PhysicsMaterial* PhysicsWorld::createPhysicsMaterial()
	{
		return (new PhysicsMaterial(m_pScene));
	}
//----------------------------------------------------------------------
	void PhysicsWorld::addPhysicsMaterial(const wstring& _name, const PhysicsMaterial& _material)
	{
		// The material already exists, so return it
		if(m_pMaterials.find(_name) != m_pMaterials.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material: %ls already exists", _name.c_str());
			return;
		}
		else
		{
			Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Adding physics material: %ls", _name.c_str());
		}

		PhysicsMaterial* pMaterial = new PhysicsMaterial(_material);
		pMaterial->setName(_name);

		m_vMaterials.push_back(pMaterial);

		m_pMaterials[_name] = pMaterial;
	}
//----------------------------------------------------------------------
	PhysicsMaterial* PhysicsWorld::getPhysicsMaterial(const wstring& _name)
	{
		// Material does not exist, so exit
		if(m_pMaterials.find(_name) == m_pMaterials.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material: %ls does not exist", _name.c_str());
			return NULL;
		}

		return m_pMaterials[_name];
	}
//----------------------------------------------------------------------
	void PhysicsWorld::removePhysicsMaterial(const wstring& _name)
	{
		// Material does not exist, so exit
		if(m_pMaterials.find(_name) == m_pMaterials.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Material: %ls does not exist", _name.c_str());
			return;
		}

		PhysicsMaterial* pMaterial = m_pMaterials[_name];

		uint i = 0;
		for(i = 0; i < m_vMaterials.size(); ++i)
		{
			if(m_vMaterials[i] == pMaterial)
				break;
		}

		m_vMaterials.erase(m_vMaterials.begin() + i);

		NGENE_DELETE(pMaterial);
		m_pMaterials.erase(_name);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::setSleepAngularVelocity(float _velocity)
	{
		m_WorldDesc.sleepAngularVelocity = _velocity;
		m_pPhysX->setParameter(NX_DEFAULT_SLEEP_ANG_VEL_SQUARED, _velocity * _velocity);
	}
//----------------------------------------------------------------------
	float PhysicsWorld::getSleepAngularVelocity() const
	{
		return m_WorldDesc.sleepAngularVelocity;
	}
//----------------------------------------------------------------------
	void PhysicsWorld::setSleepLinearVelocity(float _velocity)
	{
		m_WorldDesc.sleepLinearVelocity = _velocity;
		m_pPhysX->setParameter(NX_DEFAULT_SLEEP_LIN_VEL_SQUARED, _velocity * _velocity);
	}
//----------------------------------------------------------------------
	float PhysicsWorld::getSleepLinearVelocity() const
	{
		return m_WorldDesc.sleepLinearVelocity;
	}
//----------------------------------------------------------------------
	void PhysicsWorld::setCollisionDetection(bool _value)
	{
		m_WorldDesc.collisionDetection = _value;

		HashTable <wstring, PhysicsActor*>::iterator iter;
		for(iter = m_pBodies.begin(); iter != m_pBodies.end(); ++iter)
		{
			PhysicsActor::COLLISION_GROUP collision = (m_WorldDesc.collisionDetection ?
													PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE : 
													PhysicsActor::GROUP_NON_COLLIDABLE);
			iter->second->setCollidable(collision);
		}
	}
//----------------------------------------------------------------------
	void PhysicsWorld::setSkinWidth(float _width)
	{
		m_WorldDesc.skinWidth = _width;
		m_pPhysX->setParameter(NX_SKIN_WIDTH, _width);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::addCollisionListener(CollisionListener* _listener)
	{
		// Add listener to the registry so it
		// could handle upcoming messages.
		AutoPointer <CollisionListener> Listener(_listener);
		ListenerRegistry <CollisionEvent>::addListener(Listener.getPointer());
		ListenerRegistry <ControllerCollisionEvent>::addListener(Listener.getPointer());
		ListenerRegistry <ControllerControllerCollisionEvent>::addListener(Listener.getPointer());

		m_vCollisionListeners.push_back(Listener);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::removeCollisionListener(uint _index)
	{
		if(_index >= m_vCollisionListeners.size())
			return;

		CollisionListener* pListener = m_vCollisionListeners[_index];
		ListenerRegistry <CollisionEvent>::removeListener(pListener);
		ListenerRegistry <ControllerCollisionEvent>::removeListener(pListener);
		ListenerRegistry <ControllerControllerCollisionEvent>::removeListener(pListener);

		m_vCollisionListeners.erase(m_vCollisionListeners.begin() + _index);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::removeAllCollisionListeners()
	{
		for(uint i = 0; i < m_vCollisionListeners.size(); ++i)
		{
			CollisionListener* pListener = m_vCollisionListeners[i];
			ListenerRegistry <CollisionEvent>::removeListener(pListener);
			ListenerRegistry <ControllerCollisionEvent>::removeListener(pListener);
			ListenerRegistry <ControllerControllerCollisionEvent>::removeListener(pListener);
		}

		m_vCollisionListeners.clear();
	}
//----------------------------------------------------------------------
	void PhysicsWorld::addTriggerListener(TriggerListener* _listener)
	{
		// Add listener to the registry so it
		// could handle upcoming messages.
		AutoPointer <TriggerListener> Listener(_listener);
		ListenerRegistry <TriggerEvent>::addListener(Listener.getPointer());

		m_vTriggerListeners.push_back(Listener);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::removeTriggerListener(uint _index)
	{
		if(_index >= m_vTriggerListeners.size())
			return;

		TriggerListener* pListener = m_vTriggerListeners[_index];
		ListenerRegistry <TriggerEvent>::removeListener(pListener);

		m_vTriggerListeners.erase(m_vTriggerListeners.begin() + _index);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::removeAllTriggerListeners()
	{
		for(uint i = 0; i < m_vTriggerListeners.size(); ++i)
		{
			TriggerListener* pListener = m_vTriggerListeners[i];
			ListenerRegistry <TriggerEvent>::removeListener(pListener);
		}

		m_vTriggerListeners.clear();
	}
//----------------------------------------------------------------------
	void PhysicsWorld::setGroupsFlags(ushort _group1, ushort _group2, uint _flags)
	{
		uint flags = 0;
		if(_flags & CT_START)
			flags |= NX_NOTIFY_ON_START_TOUCH;
		if(_flags & CT_ON)
			flags |= NX_NOTIFY_ON_TOUCH;
		if(_flags & CT_END)
			flags |= NX_NOTIFY_ON_END_TOUCH;

		m_pScene->setActorGroupPairFlags(_group1, _group2, flags);
	}
//----------------------------------------------------------------------
	void PhysicsWorld::setCollisionFlags(ushort _group1, ushort _group2, bool _collides)
	{
		m_pScene->setGroupCollisionFlag(_group1, _group2, _collides);
	}
//----------------------------------------------------------------------
}