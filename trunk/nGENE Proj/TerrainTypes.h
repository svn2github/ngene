/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TerrainTypes.h
Version:	0.04
Info:		This file defines some terrain related types
			which are used by renderer and physics modules.
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TERRAINTYPES_H_
#define __INC_TERRAINTYPES_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Structure describing patch of the terrain.
		@remarks
			Patch is a terrain part you create when creating
			NodeTerrain object. Patches consists of tiles.
	*/
	typedef struct STerrainPatchDescription
	{
		uint columns;						///< Number of vertices per row
		uint rows;							///< Number of vertices per column
		uint tiles_x;						///< Number of tiles in the x direction
		uint tiles_z;						///< Number of tiles in the z direction

		float texScaleX;					///< Texture u-coordinate scaling factor
		float texScaleY;					///< Texture v-coordinate scaling factor
		float heightScale;					///< Height will be scaled by this value
		float step;							///< Distance between two neighbouring vertices
		float skirtSize;					///< Height of the skirt
		
		float* heights;						/**< Heights of all points. It could be of float type
												 but to make integration with current version
												 of PhysX easier, it was decided to use byte instead. */
		vector <pair <uint, float> > LODs;	///< Level of Details

		STerrainPatchDescription():
			columns(0),
			rows(0),
			tiles_x(0),
			tiles_z(0),
			texScaleX(1.0f),
			texScaleY(1.0f),
			heightScale(1.0f),
			step(1.0f),
			skirtSize(0.1f),
			heights(NULL)
		{
		}
	} TERRAIN_DESC;
}


#endif