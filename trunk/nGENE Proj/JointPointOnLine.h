/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointPointOnLine.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTPOINTONLINE_H_
#define __INC_JOINTPOINTONLINE_H_


#include "Joint.h"
#include "JointFactory.h"


namespace nGENE
{
	/** Point-on-Line joint.
		@par
			This joint limits a point from one body to move only on a
			line attached to the second body. The starting point is
			an anchor point and the line is further specified using
			joint global axis (direction) vector.
	*/
	class nGENEDLL JointPointOnLine: public Joint
	{
		EXPOSE_TYPE
	private:
		/// PhysX point-in-plane descriptor
		NxPointOnLineJointDesc* m_pPointOnLineDesc;

	public:
		JointPointOnLine(NxScene* _scene, SJointDesc& _desc);
		virtual ~JointPointOnLine();

		void init();
		void cleanup();

		NxJointDesc* getJointDesc();
	};

	/// Factory to be used for creating spherical joints.
	class nGENEDLL JointPointOnLineFactory: public JointFactory
	{
	public:
		JointPointOnLineFactory();
		~JointPointOnLineFactory();

		Joint* createJoint(NxScene* _scene, JOINT_DESC& _desc);
	};
}


#endif