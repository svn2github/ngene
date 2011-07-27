/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeTerrain.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODETERRAIN_H_
#define __INC_NODETERRAIN_H_


#include "NodeVisible.h"
#include "Prerequisities.h"
#include "TerrainTile.h"
#include "TerrainTypes.h"


namespace nGENE
{
	/** Node representing 3D landscapes.
	*/
	class nGENEDLL NodeTerrain: public NodeVisible
	{
		EXPOSE_TYPE
	private:
		/// All buffers storing indices for different detail levels
		vector <IndexedBuffer*> m_pIVBs;

		/// All tiles making the terrain patch.
		vector <TerrainTile> m_vTiles;

		TERRAIN_DESC m_TerrainDesc;


		/** Creates buffer for a Level of Detail.
			@param
				_step stride of the vertices.
		*/
		void createIndicesBuffer(uint _step);
		void createTiles(SVertex* _vertices);
		
		/** Computes normals of the terrain.
			@remarks
				For this we use a simple trick, taking into account
				only four neighbours vertices.
		*/
		void computeNormals(SVertex* _vertices);

		/** Updates terrain data.
		@remarks
			This function holds responsibility for setting appropriate
			index buffers for all tiles with respect to distance
			they are away from the camera.
		*/
		void onUpdate();

	public:
		NodeTerrain();
		NodeTerrain(const TERRAIN_DESC& _desc);
		~NodeTerrain();

		void init();
		void cleanup();

		void setTerrainDesc(TERRAIN_DESC& _desc);
		/// Returns description of the terrain.
		TERRAIN_DESC& getTerrainDesc();


		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};



	inline void NodeTerrain::setTerrainDesc(TERRAIN_DESC& _desc)
	{
		m_TerrainDesc = _desc;
	}
//----------------------------------------------------------------------
	inline TERRAIN_DESC& NodeTerrain::getTerrainDesc()
	{
		return m_TerrainDesc;
	}
//----------------------------------------------------------------------
}


#endif