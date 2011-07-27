/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointDistance.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTDISTANCE_H_
#define __INC_JOINTDISTANCE_H_


#include "Joint.h"
#include "JointFactory.h"


namespace nGENE
{
	/// JointDistance flags
	enum nGENEDLL JOINT_DISTANCE_FLAGS
	{
		JDF_MAX_DISTANCE_ENABLED = (1 << 0),
		JDF_MIN_DISTANCE_ENABLED = (1 << 1),
		JDF_SPRING_ENABLED		 = (1 << 2)
	};

	/// Spherical Joint descriptor.
	typedef struct nGENEDLL SDistanceJointDesc: public SJointDesc
	{
		Real maxDistance;			///< The maximum rest length of the rope or rod between the two anchor points [minDistance, infinity)
		Real minDistance;			///< The minimum rest length of the rope or rod between the two anchor points [0, maxDistance]
		JOINT_SPRING_DESC spring;
		dword flags;				///< Flags

		SDistanceJointDesc()
		{
			maxDistance = 0.0f;
			minDistance = 0.0f;
			flags = 0;
		}
	} DISTANCE_JOINT_DESC;

	/** Distance joint.
		@par
			Distance joint keeps distance between two actors. It simulates
			rope in the way it can stretch and shorten.
		@par
			Use SDistanceJointDesc when creating this Joint to specify its
			'rope' attributes.
	*/
	class nGENEDLL JointDistance: public Joint
	{
		EXPOSE_TYPE
	private:
		/// PhysX box descriptor
		NxDistanceJointDesc* m_pDistanceDesc;

	public:
		JointDistance(NxScene* _scene, SJointDesc& _desc);
		virtual ~JointDistance();

		void init();
		void cleanup();

		NxJointDesc* getJointDesc();
	};

	/// Factory to be used for creating spherical joints.
	class nGENEDLL JointDistanceFactory: public JointFactory
	{
	public:
		JointDistanceFactory();
		~JointDistanceFactory();

		Joint* createJoint(NxScene* _scene, JOINT_DESC& _desc);
	};
}


#endif