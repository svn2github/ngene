/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointCylinder.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTCYLINDER_H_
#define __INC_JOINTCYLINDER_H_


#include "Joint.h"
#include "JointFactory.h"


namespace nGENE
{
	/** Cylindrical joint.
		@remarks
			It is a "sliding" joint.
	*/
	class nGENEDLL JointCylinder: public Joint
	{
		EXPOSE_TYPE
	private:
		/// PhysX box descriptor
		NxCylindricalJointDesc* m_pCylinderDesc;

	public:
		JointCylinder(NxScene* _scene, SJointDesc& _desc);
		virtual ~JointCylinder();

		void init();
		void cleanup();

		NxJointDesc* getJointDesc();
	};

	/// Factory to be used for creating spherical joints.
	class nGENEDLL JointCylinderFactory: public JointFactory
	{
	public:
		JointCylinderFactory();
		~JointCylinderFactory();

		Joint* createJoint(NxScene* _scene, JOINT_DESC& _desc);
	};
}


#endif