/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointFixed.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTFIXED_H_
#define __INC_JOINTFIXED_H_


#include "Joint.h"
#include "JointFactory.h"


namespace nGENE
{
	/** Fixed joint.
		@remarks
			Two bodies are glued together, no relational movement
			is possible between them.
	*/
	class nGENEDLL JointFixed: public Joint
	{
		EXPOSE_TYPE
	private:
		/// PhysX box descriptor
		NxFixedJointDesc* m_pFixedDesc;

	public:
		JointFixed(NxScene* _scene, SJointDesc& _desc);
		virtual ~JointFixed();

		void init();
		void cleanup();

		NxJointDesc* getJointDesc();
	};

	/// Factory to be used for creating spherical joints.
	class nGENEDLL JointFixedFactory: public JointFactory
	{
	public:
		JointFixedFactory();
		~JointFixedFactory();

		Joint* createJoint(NxScene* _scene, JOINT_DESC& _desc);
	};
}


#endif