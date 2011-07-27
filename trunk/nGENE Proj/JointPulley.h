/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointPulley.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTPULLEY_H_
#define __INC_JOINTPULLEY_H_


#include "Joint.h"
#include "JointFactory.h"


namespace nGENE
{
	/// JointPulley flags
	enum nGENEDLL JOINT_PULLEY_FLAGS
	{
		JPF_IS_RIGID = (1 << 0),
		JPF_MOTOR_ENABLED = (1 << 1),
	};

	/// Spherical Joint descriptor.
	typedef struct nGENEDLL SPulleyJointDesc: public SJointDesc
	{
		Vector3 pulley1;			///< 1st pulley position in world space
		Vector3 pulley2;			///< 2nd pulley position in world space

		Real distance;				///< The rest length of the rope between objects [0; infinity)
		Real stiffness;				///< [0;1]
		Real transmission;			///< Transmission ratio [0;1]

		dword flags;

		SPulleyJointDesc():
			distance(0.0f),
			stiffness(1.0f),
			transmission(1.0f),
			flags(0)
		{}
	} PULLEY_JOINT_DESC;

	/** Pulley joint.
		@par
			This kind of Joint simulates a rope between two bodies
			passing over 2 pulleys.
		@par
			Use object of SPulleyJointDesc when creating this Joint
			to describe its features.
	*/
	class nGENEDLL JointPulley: public Joint
	{
		EXPOSE_TYPE
	private:
		/// PhysX pulley descriptor
		NxPulleyJointDesc* m_pPulleyDesc;
		/// Pointer to the pulley joint.
		NxPulleyJoint* m_pPulley;

	public:
		JointPulley(NxScene* _scene, SJointDesc& _desc);
		virtual ~JointPulley();

		void init();
		void cleanup();

		/// Sets Joint motor.
		void setMotor(const MOTOR_DESC& _motor);
		/// Returns Joint motor.
		MOTOR_DESC getMotor();

		/// Specifies whether motor should be enabled.
		void setMotorEnabled(bool _value);
		/// Checks if motor is enabled.
		bool isMotorEnabled();

		NxJointDesc* getJointDesc();
	};

	/// Factory to be used for creating spherical joints.
	class nGENEDLL JointPulleyFactory: public JointFactory
	{
	public:
		JointPulleyFactory();
		~JointPulleyFactory();

		Joint* createJoint(NxScene* _scene, JOINT_DESC& _desc);
	};
}


#endif