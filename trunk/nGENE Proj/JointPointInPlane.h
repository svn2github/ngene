/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointPointInPlane.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTPOINTINPLANE_H_
#define __INC_JOINTPOINTINPLANE_H_


#include "Joint.h"
#include "JointFactory.h"


namespace nGENE
{
	/** Point-in-Plane joint.
		@par
			This joint limits a point from one body to move only on a
			plane attached to the second body. The starting point is
			an anchor point and the plane is further specified using
			joint global axis (direction) vector.
	*/
	class nGENEDLL JointPointInPlane: public Joint
	{
		EXPOSE_TYPE
	private:
		/// PhysX point-in-plane descriptor
		NxPointInPlaneJointDesc* m_pPointInPlaneDesc;

	public:
		JointPointInPlane(NxScene* _scene, SJointDesc& _desc);
		virtual ~JointPointInPlane();

		void init();
		void cleanup();

		NxJointDesc* getJointDesc();
	};

	/// Factory to be used for creating spherical joints.
	class nGENEDLL JointPointInPlaneFactory: public JointFactory
	{
	public:
		JointPointInPlaneFactory();
		~JointPointInPlaneFactory();

		Joint* createJoint(NxScene* _scene, JOINT_DESC& _desc);
	};
}


#endif