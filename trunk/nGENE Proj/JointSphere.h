/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointSphere.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTSPHERE_H_
#define __INC_JOINTSPHERE_H_


#include "Joint.h"
#include "JointFactory.h"


namespace nGENE
{
	/// JointSphere flags
	enum nGENEDLL JOINT_SPHERE_FLAGS
	{
		JSF_TWIST_LIMIT_ENABLED				= (1 << 0),
		JSF_SWING_LIMIT_ENABLED				= (1 << 1),
		JSF_TWIST_SPRING_ENABLED			= (1 << 2),
		JSF_SWING_SPRING_ENABLED			= (1 << 3),
		JSF_JOINT_SPRING_ENABLED			= (1 << 4),
		JSF_PERPENDICULAR_DIR_CONSTRAINTS	= (1 << 5),
	};

	/// Spherical Joint descriptor.
	typedef struct nGENEDLL SSphereJointDesc: public SJointDesc
	{
		Vector3 swingAxis;						///< Swing axis
		dword flags;

		JOINT_LIMITS_PAIR_DESC twistLimit;		///< Limits for twisting
		JOINT_LIMITS_DESC swingLimit;			///< Limits for swinging

		JOINT_SPRING_DESC twistSpring;
		JOINT_SPRING_DESC swingSpring;

		Real projectionDistance;

		SSphereJointDesc():
			swingAxis(Vector3::UNIT_Z),
			flags(0),
			projectionDistance(1.0f)
		{}
	} SPHERE_JOINT_DESC;

	/** Spherical joint.
	*/
	class nGENEDLL JointSphere: public Joint
	{
		EXPOSE_TYPE
	private:
		/// PhysX sphere descriptor
		NxSphericalJointDesc* m_pSphereDesc;

	public:
		JointSphere(NxScene* _scene, SJointDesc& _desc);
		virtual ~JointSphere();

		void init();
		void cleanup();

		NxJointDesc* getJointDesc();
	};

	/// Factory to be used for creating spherical joints.
	class nGENEDLL JointSphereFactory: public JointFactory
	{
	public:
		JointSphereFactory();
		~JointSphereFactory();

		Joint* createJoint(NxScene* _scene, JOINT_DESC& _desc);
	};
}


#endif