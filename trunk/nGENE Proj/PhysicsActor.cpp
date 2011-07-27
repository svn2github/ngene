/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsActor.cpp
Version:	0.17
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PhysicsActor.h"
#include "NodeVisible.h"
#include "PhysicsMaterial.h"
#include "PhysXMapping.h"
#include "ShapeEnumerator.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxActor.h"
	#include "NxScene.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo PhysicsActor::Type(L"PhysicsActor", &BaseClass::Type);


	PhysicsActor::PhysicsActor(NxScene* _scene, PhysicsWorld* _world):
		m_pNode(NULL),
		m_pScene(_scene),
		m_pWorld(_world),
		m_pActor(NULL),
		m_CollisionMask(GROUP_NON_COLLIDABLE),
		m_bInfluencedByGravity(true)
	{
	}
//----------------------------------------------------------------------
	PhysicsActor::PhysicsActor(NxScene* _scene, Shape* _shape, PhysicsWorld* _world,
		BODY_TYPE _type, ushort _collidable, bool _trigger, const Vector3& _initialVelocity):
		m_pNode(NULL),
		m_pScene(_scene),
		m_pWorld(_world),
		m_pActor(NULL),
		m_CollisionMask(_collidable),
		m_bInfluencedByGravity(true)
	{
		init();
		createActor(_shape, _type, _trigger, _initialVelocity);
	}
//----------------------------------------------------------------------
	PhysicsActor::PhysicsActor(NxScene* _scene, const vector <Shape*>& _shapes,
		PhysicsWorld* _world, BODY_TYPE _type, ushort _collidable, bool _trigger,
		const Vector3& _initialVelocity):
		m_pNode(NULL),
		m_pScene(_scene),
		m_pWorld(_world),
		m_pActor(NULL),
		m_CollisionMask(_collidable),
		m_bInfluencedByGravity(true)
	{
		init();
		createActor(_shapes, _type, _trigger, _initialVelocity);
	}
//----------------------------------------------------------------------
	PhysicsActor::PhysicsActor(NxScene* _scene, PhysicsWorld* _world,
		const PHYSICS_ACTOR_DESC& _desc):
		m_pNode(NULL),
		m_pScene(_scene),
		m_pWorld(_world),
		m_pActor(NULL),
		m_CollisionMask(_desc.collidable),
		m_bInfluencedByGravity(_desc.influencedByGravity)
	{
		init();
		createActor(_desc.shape, _desc.bodyType, _desc.trigger, _desc.initialVelocity);
	}
//----------------------------------------------------------------------
	PhysicsActor::~PhysicsActor()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PhysicsActor::createActor(Shape* _shape, BODY_TYPE _type, bool _trigger,
		const Vector3& _initialVelocity)
	{
		_shape->setActor(this);

		// Create PhysX actor
		NxActorDesc actorDesc;

		m_pShapes.push_back(_shape);

		NxShapeDesc* shapeDesc = m_pShapes.at(m_pShapes.size() - 1)->getShapeDesc();
		if(_trigger)
			shapeDesc->group = PhysicsActor::GROUP_TRIGGER;
		else
			shapeDesc->group = m_CollisionMask;
		shapeDesc->userData = _shape;
		actorDesc.shapes.pushBack(shapeDesc);

		m_BodyType = _type;
		NxBodyDesc bodyDesc;
		if(m_BodyType == BT_KINEMATIC)
			bodyDesc.flags |= NX_BF_KINEMATIC;
		else if(m_BodyType == BT_DYNAMIC)
			bodyDesc.linearVelocity = PhysXMapping::vector3ToNxVec3(_initialVelocity);

		if(m_BodyType == BT_DYNAMIC || m_BodyType == BT_KINEMATIC)
		{
			if(_trigger)
				bodyDesc.mass = 1.0f;
			actorDesc.body = &bodyDesc;
			actorDesc.density = 10.0f;
		}

		if(actorDesc.isValid())
		{
			m_pActor = m_pScene->createActor(actorDesc);
			m_pActor->userData = this;
			if(!isCollidable())
				m_pActor->raiseActorFlag(NX_AF_DISABLE_COLLISION);


			NxShape* const* shapes = m_pActor->getShapes();
			for(uint i = 0; i < m_pShapes.size(); ++i)
			{
				m_pShapes[i]->setShape(shapes[i]);
			}

			Vector3 vecPos;
			Quaternion quatRot;
			m_pActor->setGlobalPosition(PhysXMapping::vector3ToNxVec3(vecPos));
			m_pActor->setGlobalOrientation(PhysXMapping::quaternionToNxQuat(quatRot));

			// Not ifluenced by gravity - raise proper flag
			if(!m_bInfluencedByGravity)
			{
				m_pActor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
			}
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Invalid rigid body description");
		}
	}
//----------------------------------------------------------------------
	void PhysicsActor::createActor(const vector <Shape*>& _shapes, BODY_TYPE _type,
		bool _trigger, const Vector3& _initialVelocity)
	{
		// Create PhysX actor
		NxActorDesc actorDesc;

		for(uint i = 0; i < _shapes.size(); ++i)
		{
			Shape* pShape = _shapes[i];
			pShape->setActor(this);

			m_pShapes.push_back(pShape);

			NxShapeDesc* shapeDesc = m_pShapes.at(m_pShapes.size() - 1)->getShapeDesc();
			if(_trigger)
				shapeDesc->group = PhysicsActor::GROUP_TRIGGER;
			else
				shapeDesc->group = m_CollisionMask;
			shapeDesc->userData = pShape;
			actorDesc.shapes.pushBack(shapeDesc);
		}

		m_BodyType = _type;
		NxBodyDesc bodyDesc;
		if(m_BodyType == BT_KINEMATIC)
			bodyDesc.flags |= NX_BF_KINEMATIC;
		else if(m_BodyType == BT_DYNAMIC)
			bodyDesc.linearVelocity = PhysXMapping::vector3ToNxVec3(_initialVelocity);

		if(m_BodyType == BT_DYNAMIC || m_BodyType == BT_KINEMATIC)
		{
			if(_trigger)
				bodyDesc.mass = 1.0f;
			actorDesc.body = &bodyDesc;
			actorDesc.density = 10.0f;
		}

		if(actorDesc.isValid())
		{
			m_pActor = m_pScene->createActor(actorDesc);
			m_pActor->userData = this;
			if(!isCollidable())
				m_pActor->raiseActorFlag(NX_AF_DISABLE_COLLISION);


			NxShape* const* shapes = m_pActor->getShapes();
			for(uint i = 0; i < m_pShapes.size(); ++i)
			{
				m_pShapes[i]->setShape(shapes[i]);
			}

			Vector3 vecPos;
			Quaternion quatRot;
			m_pActor->setGlobalPosition(PhysXMapping::vector3ToNxVec3(vecPos));
			m_pActor->setGlobalOrientation(PhysXMapping::quaternionToNxQuat(quatRot));

			// Not ifluenced by gravity - raise proper flag
			if(!m_bInfluencedByGravity)
			{
				m_pActor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
			}
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Invalid rigid body description");
		}
	}
//----------------------------------------------------------------------
	void PhysicsActor::init()
	{
	}
//----------------------------------------------------------------------
	void PhysicsActor::addShape(Shape* _shape)
	{
		if(!m_pActor)
		{
			createActor(_shape, m_BodyType, false);
		}
		else
		{
			_shape->setActor(this);

			m_pShapes.push_back(_shape);

			NxShapeDesc* shapeDesc = m_pShapes.at(m_pShapes.size() - 1)->getShapeDesc();
			shapeDesc->group = m_CollisionMask;
			shapeDesc->userData = _shape;

			NxShape* pShape = m_pActor->createShape(*shapeDesc);
			m_pActor->updateMassFromShapes(1.0f, m_pActor->getMass());

			_shape->setShape(pShape);
		}
	}
//----------------------------------------------------------------------
	void PhysicsActor::cleanup()
	{
		m_pScene->releaseActor(*m_pActor);

		if(!m_pShapes.empty())
			m_pShapes.clear();

		m_pActor = NULL;
		m_pScene = NULL;
		m_pNode = NULL;
	}
//----------------------------------------------------------------------
	void PhysicsActor::update()
	{
		// No node attached, so return
		if(!m_pNode)
			return;

		// Re-enable collisions if necessary
		if(m_pNode->isEnabled() && m_pActor->readActorFlag(NX_AF_DISABLE_COLLISION) && isCollidable())
			m_pActor->clearActorFlag(NX_AF_DISABLE_COLLISION);


		if(!m_pNode->isEnabled())
		{
			// Disable collisions if node is disabled
			if(!m_pNode->isEnabled() && isCollidable())
				m_pActor->raiseActorFlag(NX_AF_DISABLE_COLLISION);
			return;
		}

		// We cannot update other parameters, so we are done
		if(m_BodyType != BT_KINEMATIC && m_BodyType != BT_DYNAMIC)
			return;

		if(m_BodyType != BT_KINEMATIC)
		{
			// Node changed position/rotation between frames
			if(m_pNode->hasChanged())
			{
				Vector3 vecPos = m_pNode->getPositionWorld();
				m_pActor->setGlobalPosition(PhysXMapping::vector3ToNxVec3(vecPos));
				m_pActor->setGlobalOrientation(PhysXMapping::quaternionToNxQuat(m_pNode->getRotation()));
			}

			Vector3 vecPos = PhysXMapping::nxVec3ToVector3(m_pActor->getGlobalPosition());
			m_pNode->setPosition(vecPos);
			m_pNode->setRotation(PhysXMapping::nxQuatToQuaternion(m_pActor->getGlobalOrientationQuat()));
		}
		else
		{
			Vector3 vecPos = m_pNode->getPositionWorld();
			Quaternion quatRot = m_pNode->getRotationWorld();
			m_pActor->setGlobalPosition(PhysXMapping::vector3ToNxVec3(vecPos));
			m_pActor->setGlobalOrientation(PhysXMapping::quaternionToNxQuat(quatRot));
		}
	}
//----------------------------------------------------------------------
	void PhysicsActor::attachNode(Node* _node)
	{
		m_pNode = _node;

		if(m_pNode)
		{
			Vector3 vecPos = m_pNode->getPositionWorld();
			m_pActor->setGlobalPosition(PhysXMapping::vector3ToNxVec3(vecPos));
			m_pActor->setGlobalOrientation(PhysXMapping::quaternionToNxQuat(m_pNode->getRotation()));
		}
	}
//----------------------------------------------------------------------
	void PhysicsActor::applyForce(const FORCE_DESC& _force)
	{
		Vector3 force = Maths::normalize(_force.vecDirection) * _force.fValue;
		m_pActor->addForce(PhysXMapping::vector3ToNxVec3(force));
	}
//----------------------------------------------------------------------
	void PhysicsActor::applyTorque(const FORCE_DESC& _torque)
	{
		Vector3 torque = Maths::normalize(_torque.vecDirection)*_torque.fValue;
		m_pActor->addTorque(PhysXMapping::vector3ToNxVec3(torque));
	}
//----------------------------------------------------------------------
	void PhysicsActor::setMass(Real _mass)
	{
		m_pActor->setMass(_mass);
	}
//----------------------------------------------------------------------
	Real PhysicsActor::getMass() const
	{
		return m_pActor->getMass();
	}
//----------------------------------------------------------------------
	void PhysicsActor::setMassSpaceInertia(const Vector3& _inertia)
	{
		m_pActor->setMassSpaceInertiaTensor(PhysXMapping::vector3ToNxVec3(_inertia));
	}
//----------------------------------------------------------------------
	Vector3 PhysicsActor::getMassSpaceInertia()
	{
		return PhysXMapping::nxVec3ToVector3(m_pActor->getMassSpaceInertiaTensor());
	}
//----------------------------------------------------------------------
	void PhysicsActor::setShapeMaterial(uint _index, PhysicsMaterial& _material)
	{
		m_pActor->getShapes()[_index]->setMaterial(_material.getMaterialIndex());
		m_pShapes[_index]->setMaterial(&_material);
	}
//----------------------------------------------------------------------
	void PhysicsActor::setShapeMaterial(uint _index, PhysicsMaterial* _material)
	{
		m_pActor->getShapes()[_index]->setMaterial(_material->getMaterialIndex());
		m_pShapes[_index]->setMaterial(_material);
	}
//----------------------------------------------------------------------
	PhysicsMaterial* PhysicsActor::getShapeMaterial(uint _index)
	{
		return m_pShapes[_index]->getMaterial();
	}
//----------------------------------------------------------------------
	void PhysicsActor::setSleepAngularVelocity(float _velocity)
	{
		m_pActor->setSleepAngularVelocity(_velocity * _velocity);
	}
//----------------------------------------------------------------------
	float PhysicsActor::getSleepAngularVelocity() const
	{
		return Maths::fsqrt(m_pActor->getSleepAngularVelocity());
	}
//----------------------------------------------------------------------
	void PhysicsActor::setSleepLinearVelocity(float _velocity)
	{
		m_pActor->setSleepLinearVelocity(_velocity * _velocity);
	}
//----------------------------------------------------------------------
	float PhysicsActor::getSleepLinearVelocity() const
	{
		return Maths::fsqrt(m_pActor->getSleepLinearVelocity());
	}
//----------------------------------------------------------------------
	void PhysicsActor::setCollidable(ushort _mask)
	{
		m_CollisionMask = _mask;

		if(!m_pActor)
			return;

		if(m_CollisionMask == GROUP_NON_COLLIDABLE)
		{
			m_pActor->raiseActorFlag(NX_AF_DISABLE_COLLISION);
		}

		NxShape* const* shapes = m_pActor->getShapes();
		uint i = m_pActor->getNbShapes();
		while(i--)
		{
			shapes[i]->setGroup(m_CollisionMask);
		}
	}
//----------------------------------------------------------------------
	void PhysicsActor::setCollidable(uint _index, ushort _mask)
	{
		uint num = m_pActor->getNbShapes();
		Assert(_index < num, "Index out of bound");
		
		NxShape* const* shapes = m_pActor->getShapes();
		shapes[_index]->setGroup(_mask);
	}
//----------------------------------------------------------------------
	void PhysicsActor::setAngularDamping(float _damping)
	{
		m_pActor->setAngularDamping(_damping);
	}
//----------------------------------------------------------------------
	float PhysicsActor::getAngularDamping() const
	{
		return m_pActor->getAngularDamping();
	}
//----------------------------------------------------------------------
	NxActor* PhysicsActor::getActor() const
	{
		return m_pActor;
	}
//----------------------------------------------------------------------
	void PhysicsActor::setPosition(const Vector3& _position)
	{
		m_pActor->setGlobalPosition(PhysXMapping::vector3ToNxVec3(_position));
	}
//----------------------------------------------------------------------
	Vector3 PhysicsActor::getPosition()
	{
		return Vector3(PhysXMapping::nxVec3ToVector3(m_pActor->getGlobalPosition()));
	}
//----------------------------------------------------------------------
	void PhysicsActor::setGroup(ushort _id)
	{
		m_pActor->setGroup(_id);
	}
//----------------------------------------------------------------------
	ushort PhysicsActor::getGroup() const
	{
		return m_pActor->getGroup();
	}
//----------------------------------------------------------------------
	void PhysicsActor::setPositionWorld(const Vector3& _position)
	{
		m_pActor->setGlobalPosition(PhysXMapping::vector3ToNxVec3(_position));
	}
//----------------------------------------------------------------------
	void PhysicsActor::setRotationWorld(const Quaternion& _quat)
	{
		m_pActor->setGlobalOrientation(PhysXMapping::quaternionToNxQuat(_quat));
	}
//----------------------------------------------------------------------
	void PhysicsActor::setVelocity(const Vector3& _velocity)
	{
		if(m_BodyType == PhysicsActor::BT_DYNAMIC)
		{
			m_pActor->setLinearVelocity(PhysXMapping::vector3ToNxVec3(_velocity));
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cannot set linear velocity of a non-dynamic actor!");
		}
	}
//----------------------------------------------------------------------
	Vector3 PhysicsActor::getVelocity()
	{
		if(m_BodyType == PhysicsActor::BT_DYNAMIC)
		{
			return PhysXMapping::nxVec3ToVector3(m_pActor->getLinearVelocity());
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cannot get linear velocity of a non-dynamic actor!");

			return Vector3::ZERO_VECTOR;
		}
	}
//----------------------------------------------------------------------
	void PhysicsActor::setAngularVelocity(const Vector3& _velocity)
	{
		if(m_BodyType == PhysicsActor::BT_DYNAMIC)
		{
			m_pActor->setAngularVelocity(PhysXMapping::vector3ToNxVec3(_velocity));
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cannot set angular velocity of a non-dynamic actor!");
		}
	}
//----------------------------------------------------------------------
	Vector3 PhysicsActor::getAngularVelocity()
	{
		if(m_BodyType == PhysicsActor::BT_DYNAMIC)
		{
			return PhysXMapping::nxVec3ToVector3(m_pActor->getAngularVelocity());
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cannot get angular velocity of a non-dynamic actor!");

			return Vector3::ZERO_VECTOR;
		}
	}
//----------------------------------------------------------------------
	void PhysicsActor::setMaxAngularVelocity(Real _value)
	{
		if(m_BodyType == PhysicsActor::BT_DYNAMIC)
		{
			m_pActor->setMaxAngularVelocity(_value);
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cannot set maximum angular velocity of a non-dynamic actor!");
		}
	}
//----------------------------------------------------------------------
	Real PhysicsActor::getMaxAngularVelocity()
	{
		if(m_BodyType == PhysicsActor::BT_DYNAMIC)
		{
			return getMaxAngularVelocity();
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cannot get angular velocity of a non-dynamic actor!");

			return 0.0f;
		}
	}
//----------------------------------------------------------------------
	void PhysicsActor::setInfluencedByGravity(bool _value)
	{
		// No change, so skip the rest
		if(_value == m_bInfluencedByGravity)
			return;

		m_bInfluencedByGravity = _value;
		if(_value)
			m_pActor->clearBodyFlag(NX_BF_DISABLE_GRAVITY);
		else
			m_pActor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
	}
//----------------------------------------------------------------------
	void PhysicsActor::setBodyType(BODY_TYPE _type)
	{
		if(m_pActor)
		{
			if(m_BodyType == PhysicsActor::BT_DYNAMIC && _type == PhysicsActor::BT_KINEMATIC)
				m_pActor->raiseBodyFlag(NX_BF_KINEMATIC);
			else if(m_BodyType == PhysicsActor::BT_KINEMATIC && _type == PhysicsActor::BT_DYNAMIC)
				m_pActor->clearBodyFlag(NX_BF_KINEMATIC);
		}

		m_BodyType = _type;
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	SPhysicsActorDesc::SPhysicsActorDesc():
		shape(NULL),
		bodyType(PhysicsActor::BT_KINEMATIC),
		collidable(PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE),
		trigger(false),
		influencedByGravity(true)
	{
	}
//----------------------------------------------------------------------
}