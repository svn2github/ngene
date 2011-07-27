/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TerrainTile.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TERRAINTILE_H_
#define __INC_TERRAINTILE_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "TypeInfo.h"
#include "UtilitiesTypes.h"


namespace nGENE
{
	/** TerrainTile builds up NodeTerrain patch.
	*/
	class nGENEDLL TerrainTile:	public BaseClass
	{
		EXPOSE_TYPE
	private:
		/// Pointer to the surface object.
		Surface* m_pSurface;

		/// Vertex buffer
		VertexBuffer* m_pVB;

	public:
		TerrainTile();
		~TerrainTile();

		void init() {}
		void init(Surface* _surface, const RECT_FLOAT& _rect, SVertex* _vertices,
			const TERRAIN_DESC& _desc, IndexedBuffer* _ivb);
		void cleanup();

		/// Returns AABB of this tile.
		AABB* getAABB() const;

		/// Returns centre of the TerrainTile.
		const Vector3& getCentre() const;
		/// Sets currently active IndexedBuffer for this tile.
		void setIndexedBuffer(IndexedBuffer* _ivb);
	};
}


#endif
