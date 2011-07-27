/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsActor.h
Version:	0.15
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PHYSICSACTOR_H_
#define __INC_PHYSICSACTOR_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "TypeInfo.h"


namespace nGENE
{
	typedef struct SPhysicsActorDesc PHYSICS_ACTOR_DESC;


	/// Force
	typedef struct SForce
	{
		Vector3 vecDirection;			///< Direction of the force
		Real fValue;					///< Value of the force
	} FORCE_DESC;


	/** Physics actor class.
		@par
			A physics actor is an object which points are
			keeping distance throughout simulation time.
		@par
			If an actor is marked as a trigger and it is either dynamic
			or kinematic it has to have at least one shape which is
			not trigger (call Shape::setTrigger(false) to achieve this).
			This is due to PhysX internal workings.
		@par
			If the actor is dynamic (kinematic) it's type can be switched
			to the kinematic (dynamic) at any time by calling setBodyType().
			Actors marked as static at creation cannot change their type.
	*/
	class nGENEDLL PhysicsActor: public BaseClass
	{
		EXPOSE_TYPE
	public:
		/** Type of the body.
			@remarks
				Bodies behave differently depending on their type.
		*/
		enum BODY_TYPE
		{
			BT_UNKNOWN,
			BT_STATIC,
			BT_KINEMATIC,
			BT_DYNAMIC
		};

		/** Collision groups.
			@remarks
				It is mainly gameplay feature as it is used to determine
				whether character controller (and therefore character itself)
				can collide with game objects and push them around.
		*/
		enum COLLISION_GROUP
		{
			GROUP_NON_COLLIDABLE,
			GROUP_COLLIDABLE_NON_PUSHABLE,
			GROUP_COLLIDABLE_PUSHABLE,
			GROUP_CHARACTER_CONTROLLER,
			GROUP_TRIGGER
		};

	private:
		Node* m_pNode;						///< Pointer to node

		PhysicsWorld* m_pWorld;				///< Pointer to the world object

		NxScene* m_pScene;					///< Pointer to PhysX scene
		NxActor* m_pActor;					///< Pointer to PhysX actor

		/// Geometrical shapes approximating the body's shape
		vector <Shape*> m_pShapes;

		/// Type of the body
		BODY_TYPE m_BodyType;

		ushort m_CollisionMask;				///< Is collidable?
		
		bool m_bInfluencedByGravity;		///< Is influenced by gravity?

	public:
		PhysicsActor(NxScene* _scene, Shape* _shape, PhysicsWorld* _world, BODY_TYPE _type,
				  ushort _collidable=GROUP_COLLIDABLE_NON_PUSHABLE, bool _trigger=false,
				  const Vector3& _initialVelocity=Vector3::ZERO_VECTOR);
		PhysicsActor(NxScene* _scene, const vector <Shape*>& _shapes, PhysicsWorld* _world, BODY_TYPE _type,
				  ushort _collidable=GROUP_COLLIDABLE_NON_PUSHABLE, bool _trigger=false,
				  const Vector3& _initialVelocity=Vector3::ZERO_VECTOR);
		PhysicsActor(NxScene* _scene, PhysicsWorld* _world);
		PhysicsActor(NxScene* _scene, PhysicsWorld* _world, const PHYSICS_ACTOR_DESC& _desc);
		~PhysicsActor();

		void init();
		void cleanup();

		/// Creates actor from a given shape.
		void createActor(Shape* _shape, BODY_TYPE _type, bool _trigger,
			const Vector3& _initialVelocity=Vector3::ZERO_VECTOR);
		/// Creates actor from a given shapes vector.
		void createActor(const vector <Shape*>& _shapes, BODY_TYPE _type, bool _trigger,
			const Vector3& _initialVelocity=Vector3::ZERO_VECTOR);
		/// Returns pointer to the PhysX actor.
		NxActor* getActor() const;

		/** Updates the PhysicsActor.
			@remarks
				Note that only enabled nodes get updated.
		*/
		void update();

		/** Attaches node to the actor.
			@remarks
				Only NodeVisibles could be linked to the PhysicsActor.
		*/
		void attachNode(Node* _node);
		/// Returns node attached to this actor.
		Node* getNode() const;

		/// Adds new shape to the physics actor's body.
		void addShape(Shape* _shape);

		/** Applies the force.
			@remarks
				Force direction would be normalized automatically.
		*/
		void applyForce(const FORCE_DESC& _force);

		/** Applies the torque to the body.
			@remarks
				Torque direction would be normalized automatically.
		*/
		void applyTorque(const FORCE_DESC& _torque);

		/// Sets linear velocity of this actor.
		void setVelocity(const Vector3& _velocity);
		/// Returns linear velocity of this actor.
		Vector3 getVelocity();

		/// Sets angular velocity of this actor.
		void setAngularVelocity(const Vector3& _velocity);
		/// Returns angular velocity of this actor.
		Vector3 getAngularVelocity();

		/// Sets maximum angular velocity.
		void setMaxAngularVelocity(Real _value);
		/// Returns maximum angular velocity.
		Real getMaxAngularVelocity();

		/// Sets the PhysicsActor's mass.
		void setMass(Real _mass);
		/// Returns mass of this PhysicsActor.
		Real getMass() const;

		/// Sets mass space inertia vector.
		void setMassSpaceInertia(const Vector3& _inertia);
		/// Returns mass space inertia vector.
		Vector3 getMassSpaceInertia();

		/// Sets whether the actor is influenced by gravity force or not.
		void setInfluencedByGravity(bool _value);
		/// Checks whether the actor is influenced by gravity force.
		bool isInfluencedByGravity() const;

		/** Sets body type.
			@par
				This method may be used at actor's creation to specify whether
				it's dynamic, kinematic or static. It can be also used to switch
				between dynamic and kinematic type later on.
		*/
		void setBodyType(BODY_TYPE _type);
		/// Returns body type of this actor.
		BODY_TYPE getBodyType() const {return m_BodyType;}

		/// Returns desired shape.
		Shape* getShape(uint _index) const {return m_pShapes[_index];}

		/// Sets material to the shape.
		void setShapeMaterial(uint _index, PhysicsMaterial& _material);
		/// Sets material to the shape.
		void setShapeMaterial(uint _index, PhysicsMaterial* _material);
		/// Returns material for the given shape.
		PhysicsMaterial* getShapeMaterial(uint _index);

		/// Lets you specify if the actor collides with other actors.
		void setCollidable(ushort _mask);
		/// Lets you specify if the specified Shape collides with other actors.
		void setCollidable(uint _index, ushort _mask);
		/// Checks if this actor can collide with others.
		bool isCollidable() const;

		/// Sets sleep angular velocity for this actor.
		void setSleepAngularVelocity(float _velocity);
		/// Returns sleep angular velocity for this actor.
		float getSleepAngularVelocity() const;

		/// Sets sleep linear velocity for this actor.
		void setSleepLinearVelocity(float _velocity);
		/// Returns sleep linear velocity for this actor
		float getSleepLinearVelocity() const;

		/// Sets angular damping of this actor.
		void setAngularDamping(float _damping);
		/// Returns angular damping factor for this actor.
		float getAngularDamping() const;

		/// Sets position.
		void setPosition(const Vector3& _position);
		/// Returns actor position in world space.
		Vector3 getPosition();

		/** Attaches this actor to the specified group.
			@par
				All actors sharing group id belong to the 
				same group.
		*/
		void setGroup(ushort _id);
		/// Returns id of this actors body group.
		ushort getGroup() const;

		/// Sets world space position of the actor.
		void setPositionWorld(const Vector3& _position);
		/// Sets world space rotation of the actor.
		void setRotationWorld(const Quaternion& _quat);

		/// Returns PhysicsWorld this actor belongs to.
		PhysicsWorld* getPhysicsWorld() const;
	};



	inline bool PhysicsActor::isCollidable() const
	{
		return (m_CollisionMask == GROUP_NON_COLLIDABLE ? false : true);
	}
//----------------------------------------------------------------------
	inline Node* PhysicsActor::getNode() const
	{
		return m_pNode;
	}
//----------------------------------------------------------------------
	inline PhysicsWorld* PhysicsActor::getPhysicsWorld() const
	{
		return m_pWorld;
	}
//----------------------------------------------------------------------
	inline bool PhysicsActor::isInfluencedByGravity() const
	{
		return m_bInfluencedByGravity;
	}
//----------------------------------------------------------------------



	/// Descriptor of the Physics Actor
	typedef struct SPhysicsActorDesc
	{
		Shape* shape;					///< Shape of the actor
		PhysicsActor::BODY_TYPE bodyType;///< Body type
		ushort collidable;				///< Does collide with other actors?
		bool trigger;					///< Is trigger?
		bool influencedByGravity;		///< Is influenced by gravity?
		Vector3 initialVelocity;		///< Initial velocity

		SPhysicsActorDesc();
	} PHYSICS_ACTOR_DESC;
}


#endif