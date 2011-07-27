/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsWorld.h
Version:	0.17
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PHYSICSWORLD_H_
#define __INC_PHYSICSWORLD_H_


#include "BaseClass.h"
#include "HashTable.h"
#include "Prerequisities.h"
#include "PhysicsActor.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Description of the physical world.
	*/
	typedef struct SPhysicsWorldDesc
	{
		/// Gravity field strength
		Vector3 vecGravity;
		
		/// Sleep angular velocity
		Real sleepAngularVelocity;
		/// Sleep linear velocity
		Real sleepLinearVelocity;
		/// Skin width
		Real skinWidth;
		/// Slo-motion rate
		Real slowMotionRate;
		/// Simulation step (1/60th of a second)
		Real simulationTimeStep;

		/// Is collision detection active
		bool collisionDetection;

		SPhysicsWorldDesc():
			vecGravity(0.0f, -9.81f, 0.0f),
			sleepAngularVelocity(0.15f),
			sleepLinearVelocity(0.14f),
			skinWidth(0.01f),
			slowMotionRate(1.0f),
			simulationTimeStep(1.0f / 60.0f),
			collisionDetection(true)
		{
		}
	} WORLD_DESC;


	/** Physical world - a wrapper around PhysX NxScene.
		@remarks
			Physical world is a container for all the bodies,
			joints and materials. It has also some features
			of its own (eg. gravity) specifying the bodies
			behaviour.
	*/
	class nGENEDLL PhysicsWorld: public BaseClass
	{
		EXPOSE_TYPE
	private:
		/// PhysX scene
		NxScene* m_pScene;
		/// Pointer to the PhysX
		NxPhysicsSDK* m_pPhysX;

		/// Contact report
		PhysicsCollisionListener* m_pCollisionListener;

		/// Trigger report
		PhysicsTriggerListener* m_pTriggerListener;

		/// World's description
		SPhysicsWorldDesc m_WorldDesc;

		/// World's name
		wstring m_stName;

		dword m_lPrevTime;

		/// HashTable holding all actors
		HashTable <wstring, PhysicsActor*> m_pBodies;

		/// HashTable holding all vehicles
		HashTable <wstring, Vehicle*> m_pVehicles;

		/// HashTable holding all character controllers
		HashTable <wstring, CharacterController*> m_pCharacterControllers;

		/// HashTable holding all force fields
		HashTable <wstring, ForceField*> m_pForceFields;

		/// HashTable holding all joints.
		HashTable <wstring, Joint*> m_pJoints;

		/// HashTable holding all materials
		HashTable <wstring, PhysicsMaterial*> m_pMaterials;
		vector <PhysicsMaterial*> m_vMaterials;

		/** Vector holding all shapes.
			@remarks
				Reasons why it is being held on the side of the world, not
				actor are a few. First of all the code is of better quality.
				Secondly it will allow some reuse.
		*/
		vector <Shape*> m_pShapes;

		/// Collision listeners.
		vector <AutoPointer <CollisionListener> > m_vCollisionListeners;

		/// Trigger listeners.
		vector <AutoPointer <TriggerListener> > m_vTriggerListeners;

		/// Is world enabled?
		bool m_bEnabled;

	public:
		PhysicsWorld(NxPhysicsSDK* _physX);
		virtual ~PhysicsWorld();

		/// Initializes the world.
		void init();
		void cleanup();

		/// Updates the world.
		void update();

		/// Sets world name.
		void setName(const wstring& _name);
		/// Returns world name.
		wstring& getName();

		/// Returns world description.
		const WORLD_DESC& getDesc() const;

		/// Sets gravity vector.
		void setGravity(const Vector3& _grav);
		/// Sets gravity vector.
		void setGravity(Real _x, Real _y, Real _z);
		/// Returns gravity vector.
		Vector3& getGravity();

		/// Sets slo-motion rate.
		void setSlowMotionRate(Real _value);
		/// Returns slo-motion rate.
		Real getSlowMotionRate() const;

		/** Sets collision detection.
			@param
				_value specifies whether collisions between actors are enabled.
				If set to true, collisions are enabled, and disabled in other
				case.
		*/
		void setCollisionDetection(bool _value);
		/// Checks if collision detection is turned on.
		bool getCollisionDetection() const;

		/// Sets sleep angular velocity for all rigid bodies.
		void setSleepAngularVelocity(float _velocity);
		/// Returns sleep angular velocity for all rigid bodies.
		float getSleepAngularVelocity() const;

		/// Sets sleep linear velocity for all rigid bodies.
		void setSleepLinearVelocity(float _velocity);
		/// Returns sleep linear velocity for all rigid bodies.
		float getSleepLinearVelocity() const;

		/// Sets skin width.
		void setSkinWidth(float _width);
		/// Returns skin width.
		float getSkinWidth() const;

		/// Creates new CharacterController.
		CharacterController* createController(CONTROLLER_DESC& _desc);

		/// Creates new Vehicle.
		Vehicle* createVehicle(PhysicsActor* _body);

		/// Creates new ForceField.
		ForceField* createForceField(FORCE_FIELD_DESC& _desc);
		/// Creates new ForceFieldShape.
		ForceFieldShape* createForceFieldShape(const wstring& _shapeTypeName);
		/// Creates new ForceFieldShapesGroup.
		ForceFieldShapesGroup* createForceFieldShapesGroup(bool _isIncluded);

		/// Adds force field to the world.
		void addForceField(const wstring& _name, ForceField* _field);
		/// Destroys given ForceField.
		void removeForceField(const wstring& _name);
		/// Returns existing force field.
		ForceField* getForceField(const wstring& _name);

		/// Destroys given Joint.
		void removeJoint(const wstring& _name);
		/// Returns existing Joint.
		Joint* getJoint(const wstring& _name);

		/// Adds character controller to the world.
		void addController(const wstring& _name, CharacterController* _controller);
		/// Destroys given CharacterController.
		void removeCharacterController(const wstring& _name);
		/// Returns existing character controller.
		CharacterController* getCharacterController(const wstring& _name);

		/** Adds vehicle to the world.
			@todo finish vehicle physics...
		*/
		void addVehicle(const wstring& _name, Vehicle* _vehicle);

		/** Creates new PhysicsActor.
			@remarks
				Using this method you will need to manually specify all actor's attributes.
		*/
		PhysicsActor* createPhysicsActor();

		/** Creates new PhysicsActor.
			@param
				_shapeTypeName name of the shape type, eg. "Box", "Capsule", "Sphere".
			@param
				_dimensions dimensions of the Actor. Meaning depends on _shapeTypeName.
			@param
				_type type of the Actor. Use PhysicsActor::BT_STATIC for static actors,
				PhysicsActor::BT_KINEMATIC for kinematic and PhysicsActor::BT_DYNAMIC
				for dynamic ones.
			@param
				_trigger this flag specifies if the Actor is a trigger.
			@remarks
				In case _trigger flag is set and actor is either of BT_DYNAMIC or
				BT_KINEMATIC type then it has to have at least two shapes one of which
				have to be dummy.
		*/
		PhysicsActor* createPhysicsActor(const wstring& _shapeTypeName, const Vector3& _dimensions,
										 PhysicsActor::BODY_TYPE _type=PhysicsActor::BT_STATIC,
										 bool _trigger=false,
										 const Vector3& _initialVelocity=Vector3::ZERO_VECTOR);

		/// Creates new PhysicsActor.
		PhysicsActor* createPhysicsActor(Shape* _shape,
										 PhysicsActor::BODY_TYPE _type=PhysicsActor::BT_STATIC,
										 const Vector3& _initialVelocity=Vector3::ZERO_VECTOR);
		/// Creates new PhysicsActor.
		PhysicsActor* createPhysicsActor(const vector <Shape*>& _shapes,
										 PhysicsActor::BODY_TYPE _type=PhysicsActor::BT_STATIC,
										 const Vector3& _initialVelocity=Vector3::ZERO_VECTOR);

		/** Creates new PhysicsActor.
			@param
				_desc description of the PhysicsActor.
		*/
		PhysicsActor* createPhysicsActor(const PHYSICS_ACTOR_DESC& _desc);

		/** Creates new PhysicsActor for a set of vertices.
			@remarks
				As in the majority of cases resulting shape will not be convex,
				it should be most often created with BT_STATIC value for _type
				parameter.
		*/
		PhysicsActor* createPhysicsActorMesh(const Vector3& _dimensions, const VertexBuffer& _vb,
											 PhysicsActor::BODY_TYPE _type=PhysicsActor::BT_STATIC,
											 bool _flipNormals=true,
											 const Vector3& _initialVelocity=Vector3::ZERO_VECTOR);

		/// Creates new Joint object.
		Joint* createJoint(const wstring& _jointTypeName, JOINT_DESC& _desc);

		/** Creates new PhysicsActor from height-field.
			@remarks
				As the purpose of this function is to create terrain,
				one cannot specify BODY_TYPE - it is always being
				set to BT_STATIC.
		*/
		PhysicsActor* createTerrain(const TERRAIN_DESC& _desc);

		/// Returns existing rigid body.
		PhysicsActor* getPhysicsActor(const wstring& _name);

		/// Adds rigid body to the world.
		void addPhysicsActor(const wstring& _name, PhysicsActor* _actor);


		/// Creates new PhysicsMaterial with given name.
		PhysicsMaterial* createPhysicsMaterial(const wstring& _name);
		/// Creates empty PhysicsMaterial.
		PhysicsMaterial* createPhysicsMaterial();
		/// Returns physics material by name.
		PhysicsMaterial* getPhysicsMaterial(const wstring& _name);

		/// Adds existing material to the world.
		void addPhysicsMaterial(const wstring& _name, const PhysicsMaterial& _material);
		/// Removes existing material from the world.
		void removePhysicsMaterial(const wstring& _name);

		/// Destroys given PhysicsActor.
		void removePhysicsActor(const wstring& _name);
		/// Destroys given PhysicsActor.
		void removePhysicsActor(PhysicsActor* _actor);


		/// Creates new shape and adds it to the vector.
		Shape* createShape(const wstring& _shapeType);
		/// Returns shape with the given index.
		Shape* getShape(uint _index) const;
		/// Return number of shapes.
		uint getShapesNum() const;


		/// Registers new collision listener.
		virtual void addCollisionListener(CollisionListener* _listener);
		/// Unregisters existing collision listener.
		virtual void removeCollisionListener(uint _index);
		/// Unregisters all existing collision listeners.
		virtual void removeAllCollisionListeners();

		/// Registers new trigger listener.
		virtual void addTriggerListener(TriggerListener* _listener);
		/// Unregisters existing trigger listener.
		virtual void removeTriggerListener(uint _index);
		/// Unregisters all existing trigger listeners.
		virtual void removeAllTriggerListeners();

		/** Sets actor group pair flags.
			@par
				This method defines how actors belonging to each of the groups
				interact with actors belonging to the second one.
			@remarks
				You can specify both parameters to the same value to define interaction
				between actors of the same group.
			@param
				_group1 id of the first group.
			@param
				_group2 id of the second group.
			@param
				_flags combination of COLLISION_TYPE flags.
		*/
		virtual void setGroupsFlags(ushort _group1, ushort _group2, uint _flags);

		/** Sets actor group collision flags.
			@par
				This method specifies if collisions between members of the specified
				groups are possible. It is useful to disable collisions between
				actors and triggers for instance.
			@param
				_group1 id of the first group.
			@param
				_group2 id of the second group.
			@param
				_collides if true members of _group1 and _group2 will collide with each
				other.
		*/
		virtual void setCollisionFlags(ushort _group1, ushort _group2, bool _collides);


		/// Specifies whether world is enabled i.e. simulated.
		void setEnabled(bool _value);
		/// Toggles enabled parameter of the world.
		void toggleEnabled();
		/// Checks if the world is enabled.
		bool isEnabled() const;

		/// Sets simulation time step.
		void setSimulationTimeStep(Real _timeStep);
		/// Returns simulation time step.
		Real getSimulationTimeStep() const;
	};



	inline const WORLD_DESC& PhysicsWorld::getDesc() const
	{
		return m_WorldDesc;
	}
//----------------------------------------------------------------------
	inline float PhysicsWorld::getSkinWidth() const
	{
		return m_WorldDesc.skinWidth;
	}
//----------------------------------------------------------------------
	inline Shape* PhysicsWorld::getShape(uint _index) const
	{
		return m_pShapes[_index];
	}
//----------------------------------------------------------------------
	inline uint PhysicsWorld::getShapesNum() const
	{
		return m_pShapes.size();
	}
//----------------------------------------------------------------------
	inline void PhysicsWorld::setEnabled(bool _value)
	{
		m_bEnabled = _value;
	}
//----------------------------------------------------------------------
	inline void PhysicsWorld::toggleEnabled()
	{
		m_bEnabled = m_bEnabled ? false : true;
	}
//----------------------------------------------------------------------
	inline bool PhysicsWorld::isEnabled() const
	{
		return m_bEnabled;
	}
//----------------------------------------------------------------------
	inline void PhysicsWorld::setName(const wstring& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------	
	inline wstring& PhysicsWorld::getName()
	{
		return m_stName;
	}
//----------------------------------------------------------------------
	inline void PhysicsWorld::setSlowMotionRate(Real _value)
	{
		m_WorldDesc.slowMotionRate = _value;
	}
//----------------------------------------------------------------------
	inline Real PhysicsWorld::getSlowMotionRate() const
	{
		return m_WorldDesc.slowMotionRate;
	}
//----------------------------------------------------------------------
	inline Vector3& PhysicsWorld::getGravity()
	{
		return m_WorldDesc.vecGravity;
	}
//----------------------------------------------------------------------
	inline bool PhysicsWorld::getCollisionDetection() const
	{
		return m_WorldDesc.collisionDetection;
	}
//----------------------------------------------------------------------
	inline void PhysicsWorld::setSimulationTimeStep(Real _timeStep)
	{
		m_WorldDesc.simulationTimeStep = _timeStep;
	}
//----------------------------------------------------------------------
	inline Real PhysicsWorld::getSimulationTimeStep() const
	{
		return m_WorldDesc.simulationTimeStep;
	}
//----------------------------------------------------------------------
}


#endif