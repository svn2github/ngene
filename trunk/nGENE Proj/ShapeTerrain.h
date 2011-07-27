/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeTerrain.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHAPETERRAIN_H_
#define __INC_SHAPETERRAIN_H_


#include "Prerequisities.h"
#include "ShapeFactory.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Terrain (height-field shape.
		@remarks
			Terrain shape provides a way to simulate collisions
			between terrain and other rigid bodies.
		@par
			The biggest difference from other shapes is that
			in order to create valid ShapeTerrain you have to
			provide height values and size of the height field.
	*/
	class ShapeTerrain: public Shape
	{
		EXPOSE_TYPE
	private:
		NxHeightField* m_pHeightField;		///< Pointer to PhysX height field

		/// Shape of the terrain
		NxHeightFieldShapeDesc* m_pTerrainShapeDesc;

		/// Terrain description
		NxHeightFieldDesc* m_pTerrainDesc;

	public:
		ShapeTerrain();
		~ShapeTerrain();

		/// Inits terrain height field.
		void init(NxScene* _scene, const TERRAIN_DESC& _desc);

		NxShapeDesc* getShapeDesc();
	};

	/// Factory to be used for creating mesh shapes.
	class nGENEDLL TerrainShapeFactory: public ShapeFactory
	{
	public:
		TerrainShapeFactory();
		~TerrainShapeFactory();

		Shape* createShape();
	};
}


#endif