/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointPrism.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTPRISM_H_
#define __INC_JOINTPRISM_H_


#include "Joint.h"
#include "JointFactory.h"


namespace nGENE
{
	/** Prismatic joint.
		@remarks
			This kind of Joint allows relative movement along an axis
			between two bodies but permits relative rotation.
	*/
	class nGENEDLL JointPrism: public Joint
	{
		EXPOSE_TYPE
	private:
		/// PhysX box descriptor
		NxPrismaticJointDesc* m_pPrismDesc;

	public:
		JointPrism(NxScene* _scene, SJointDesc& _desc);
		virtual ~JointPrism();

		void init();
		void cleanup();

		NxJointDesc* getJointDesc();
	};

	/// Factory to be used for creating spherical joints.
	class nGENEDLL JointPrismFactory: public JointFactory
	{
	public:
		JointPrismFactory();
		~JointPrismFactory();

		Joint* createJoint(NxScene* _scene, JOINT_DESC& _desc);
	};
}


#endif