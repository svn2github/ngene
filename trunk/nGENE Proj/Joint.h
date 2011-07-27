/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Joint.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINT_H_
#define __INC_JOINT_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Base Joint descriptor.
		@remarks
			The most essential fields are actor1 and actor2. These are
			the pointers to PhysicsActor objects to which the joint will be
			bound. Maximum one of them can be null (then the joint will
			be bound to the actor1/actor2 and the scene itself). None of the
			actors can be BT_STATIC and at least one of them must be of BT_DYNAMIC
			type.
	*/
	typedef struct nGENEDLL SJointDesc
	{
		PhysicsActor* actor1;
		PhysicsActor* actor2;

		Vector3 localAnchor1;			///< Anchor point in actor1 space
		Vector3 localAnchor2;			///< Anchor point in actor2 space
		Vector3 localDirection1;		///< Axis in actor1 space
		Vector3 localDirection2;		///< Axis in actor2 space

		Vector3 globalAnchor;			///< Anchor in world space coordinates
		Vector3 globalDirection;		///< Axis in world space coordinates

		SJointDesc():
			localDirection1(Vector3::UNIT_Z),
			localDirection2(Vector3::UNIT_Z)
		{
			actor1 = NULL;
			actor2 = NULL;
		}
	} JOINT_DESC;


	/// Descriptor of the motor.
	typedef struct SMotorDesc
	{
		Real targetVelocity;			///< Motor target velocity
		Real maxForce;					///< Maximum force value motor can exert
		bool freeSpin;					/**< When true motor will not break under
										maxForce > targetVelocity condition */
	} MOTOR_DESC;

	/// Descriptor of the joint (hinge, twist) limits.
	typedef struct SJointLimitsPairDesc
	{
		Real lowValue;					///< Lower boundary angle value (-PI, PI)
		Real lowRestitution;			///< Ranges 0.0 to 1.0
		Real highValue;					///< Higher boundary angle value (-PI, PI)
		Real highRestitution;			///< Ranges 0.0 to 1.0

		SJointLimitsPairDesc()
		{
			lowValue = 0.0f;
			highValue = 0.0f;
			lowRestitution = 0.0f;
			highRestitution = 0.0f;
		}
	} JOINT_LIMITS_PAIR_DESC;

	/// Descriptor of the joint (hinge, twist) limits.
	typedef struct SJointLimitsDesc
	{
		Real value;					///< Value (-PI, PI)
		Real restitution;			///< Ranges 0.0 to 1.0

		SJointLimitsDesc()
		{
			value = 0.0f;
			restitution = 0.0f;
		}
	} JOINT_LIMITS_DESC;

	/// Descriptor of the joint spring.
	typedef struct SJointSpringDesc
	{
		Real springCoeff;			///< Spring coefficient [0; infinity)
		Real damperCoeff;			///< Damper coefficient [0; infinity)

		SJointSpringDesc()
		{
			springCoeff = 0.0f;
			damperCoeff = 0.0f;
		}
	} JOINT_SPRING_DESC;


	/** PhysX base joint class.
	*/
	class nGENEDLL Joint: public BaseClass
	{
		EXPOSE_TYPE
	private:
		// Prevent copying and assignment
		Joint(const Joint& _src);
		Joint& operator=(const Joint& _rhs);

	protected:
		/// PhysX joint object.
		NxJoint* m_pJoint;
		/// Pointer to PhysX scene
		NxScene* m_pScene;
		/// Descriptor of this Joint.
		SJointDesc& m_JointDesc;

	public:
		Joint(NxScene* _scene, SJointDesc& _desc);
		virtual ~Joint();

		virtual void init();
		virtual void cleanup();

		/// Returns PhysX shape description.
		virtual NxJointDesc* getJointDesc()=0;

		/// Sets Joint world position.
		void setPosition(const Vector3& _vec);

		/// Sets breakable constraints for this joint.
		void setBreakable(Real _force, Real _torque);
		/** This function returns maximum linear and angular forces
			under which joint remains unbroken.
		*/
		void getBreakable(Real& _force, Real& _torque);
		/** Checks if the joint was broken.
			@remarks
				Broken joint is no longer simulated so it can be released.
		*/
		bool isBroken();

		/** Sets Joint global axis (direction).
			@param
				_direction unit length vector specifying direction.
		*/
		void setDirection(const Vector3& _direction);
		/// Returns Joint global axis (direction).
		Vector3 getDirection();

		/// Returns PhysX joint.
		virtual NxJoint* getJoint() const;
	};
}


#endif