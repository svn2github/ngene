/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointHinge.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTHINGE_H_
#define __INC_JOINTHINGE_H_


#include "Joint.h"
#include "JointFactory.h"


namespace nGENE
{
	/** Hinge like joint.
	*/
	class nGENEDLL JointHinge: public Joint
	{
		EXPOSE_TYPE
	private:
		/// PhysX revolute joint descriptor
		NxRevoluteJointDesc* m_pHingeDesc;
		/// Pointer to the revolute joint.
		NxRevoluteJoint* m_pHinge;

	public:
		JointHinge(NxScene* _scene, SJointDesc& _desc);
		virtual ~JointHinge();

		void init();
		void cleanup();

		/// Sets Joint motor.
		void setMotor(const MOTOR_DESC& _motor);
		/// Returns Joint motor.
		MOTOR_DESC getMotor();

		/// Sets Joint limits.
		void setLimits(const JOINT_LIMITS_PAIR_DESC& _limits);

		/// Specifies whether motor should be enabled.
		void setMotorEnabled(bool _value);
		/// Checks if motor is enabled.
		bool isMotorEnabled();
		/** Specifies whether spring should be enabled.
			@remarks
				Spring will only take effect when motor is disabled.
		*/
		void setSpringEnabled(bool _value);
		/// Checks if spring is enabled.
		bool isSpringEnabled();
		/// Specifies whether limits should be enabled.
		void setLimitsEnabled(bool _value);
		/// Checks if limits enabled.
		bool isLimitsEnabled();

		NxJointDesc* getJointDesc();
	};

	/// Factory to be used for creating spherical joints.
	class nGENEDLL JointHingeFactory: public JointFactory
	{
	public:
		JointHingeFactory();
		~JointHingeFactory();

		Joint* createJoint(NxScene* _scene, JOINT_DESC& _desc);
	};
}


#endif