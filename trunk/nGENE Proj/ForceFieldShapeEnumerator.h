/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldShapeEnumerator.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FORCEFIELDSHAPEENUMERATOR_H_
#define __INC_FORCEFIELDSHAPEENUMERATOR_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "ForceFieldShape.h"
#include "ForceFieldShapeBox.h"
#include "ForceFieldShapeCapsule.h"
#include "ForceFieldShapeSphere.h"


namespace nGENE
{
	/** Force Field Shape Enumerator class.
		@remarks
			Enumerators lets you easily extend nGENE Tech by just adding
			factories of new type to the enumerator. Then by specifying
			the type name you can create objects of any available type
			in the enumerator.
	*/
	class nGENEDLL ForceFieldShapeEnumerator: public Singleton <ForceFieldShapeEnumerator>
	{
	private:
		// Hash table containing all factories
		HashTable <wstring, ForceFieldShapeFactory*> m_pFactories;

		// Default factories
		ForceFieldBoxShapeFactory m_BoxFactory;
		ForceFieldSphereShapeFactory m_SphereFactory;
		ForceFieldCapsuleShapeFactory m_CapsuleFactory;

	public:
		ForceFieldShapeEnumerator();
		~ForceFieldShapeEnumerator();

		/** Adds new factory to the enumerator.
			@remarks
				Factory has to be added to the enumerator prior to
				using it.
		*/
		void addFactory(ForceFieldShapeFactory& _factory, const wstring& _typeName);
		void removeFactory(const wstring& _typeName);

		/// Creates new shape object.
		ForceFieldShape* create(const wstring& _typeName);
		/// Destroys shape object.
		void destroy(ForceFieldShape* _shape);
	};
}


#endif