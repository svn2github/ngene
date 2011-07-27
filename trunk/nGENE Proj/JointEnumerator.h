/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointEnumerator.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_JOINTENUMERATOR_H_
#define __INC_JOINTENUMERATOR_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "JointCylinder.h"
#include "JointDistance.h"
#include "JointFixed.h"
#include "JointHinge.h"
#include "JointPointInPlane.h"
#include "JointPointOnLine.h"
#include "JointPrism.h"
#include "JointPulley.h"
#include "JointSphere.h"


namespace nGENE
{
	/** Joint Enumerator class.
		@remarks
			Enumerators lets you easily extend nGENE Tech by just adding
			factories of new type to the enumerator. Then by specifying
			the type name you can create objects of any available type
			in the enumerator.
	*/
	class nGENEDLL JointEnumerator: public Singleton <JointEnumerator>
	{
	private:
		// Hash table containing all factories
		HashTable <wstring, JointFactory*> m_pFactories;

		// Default factories
		JointCylinderFactory m_CylinderFactory;
		JointDistanceFactory m_DistanceFactory;
		JointFixedFactory m_FixedFactory;
		JointHingeFactory m_HingeFactory;
		JointPointInPlaneFactory m_PointInPlane;
		JointPointOnLineFactory m_PointOnLine;
		JointPrismFactory m_PrismFactory;
		JointPulleyFactory m_PulleyFactory;
		JointSphereFactory m_SphereFactory;

	public:
		JointEnumerator();
		~JointEnumerator();

		/** Adds new factory to the enumerator.
			@remarks
				Factory has to be added to the enumerator prior to
				using it.
		*/
		void addFactory(JointFactory& _factory, const wstring& _typeName);
		/// Removes factory with a given name.
		void removeFactory(const wstring& _typeName);

		/// Creates new Joint object.
		Joint* create(NxScene* _scene, const wstring& _typeName, JOINT_DESC& _desc);
		/// Destroys Joint object.
		void destroy(Joint* _shape);
	};
}


#endif