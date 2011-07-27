/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeEnumerator.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHAPEENUMERATOR_H_
#define __INC_SHAPEENUMERATOR_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "Shape.h"
#include "ShapeBox.h"
#include "ShapeCapsule.h"
#include "ShapeMesh.h"
#include "ShapePlane.h"
#include "ShapeSphere.h"
#include "ShapeTerrain.h"
#include "Singleton.h"


namespace nGENE
{
	/** Shape Enumerator class.
		@remarks
			Enumerators lets you easily extend nGENE Tech by just adding
			factories of new type to the enumerator. Then by specifying
			the type name you can create objects of any available type
			in the enumerator.
	*/
	class ShapeEnumerator: public Singleton <ShapeEnumerator>
	{
	private:
		// Hash table containing all factories
		HashTable <wstring, ShapeFactory*> m_pFactories;

		// Default factories
		BoxShapeFactory m_BoxFactory;
		CapsuleShapeFactory m_CapsuleFactory;
		PlaneShapeFactory m_PlaneFactory;
		SphereShapeFactory m_SphereFactory;
		TerrainShapeFactory m_TerrainFactory;
		MeshShapeFactory m_MeshFactory;

	public:
		ShapeEnumerator();
		~ShapeEnumerator();

		/** Adds new factory to the enumerator.
			@remarks
				Factory has to be added to the enumerator prior to
				using it.
		*/
		void addFactory(ShapeFactory& _factory, const wstring& _typeName);
		void removeFactory(const wstring& _typeName);

		/// Creates new shape object.
		Shape* create(const wstring& _typeName);
		/// Destroys shape object.
		void destroy(Shape* _shape);
	};
}


#endif