/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeTerrain.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ShapeEnumerator.h"
#include "ShapeTerrain.h"
#include "TerrainTypes.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxHeightFieldDesc.h"
	#include "NxHeightFieldSample.h"
	#include "NxHeightFieldShapeDesc.h"
	#include "NxScene.h"
#pragma warning(pop) 


namespace nGENE
{
	// Initialize static members
	TypeInfo ShapeTerrain::Type(L"ShapeTerrain", &Shape::Type);


	ShapeTerrain::ShapeTerrain():
		m_pHeightField(NULL),
		m_pTerrainShapeDesc(NULL),
		m_pTerrainDesc(NULL)
	{
	}
//----------------------------------------------------------------------
	ShapeTerrain::~ShapeTerrain()
	{
		NGENE_DELETE(m_pTerrainDesc);
		NGENE_DELETE(m_pTerrainShapeDesc);
	}
//----------------------------------------------------------------------
	void ShapeTerrain::init(NxScene* _scene, const TERRAIN_DESC& _desc)
	{
		m_pTerrainDesc = new NxHeightFieldDesc();

		// Create description of height-field
		m_pTerrainDesc->nbColumns = _desc.rows;
		m_pTerrainDesc->nbRows = _desc.columns;
		m_pTerrainDesc->convexEdgeThreshold = 0.0f;
		m_pTerrainDesc->thickness = -1000.0f;
		m_pTerrainDesc->flags = NX_HF_NO_BOUNDARY_EDGES;
		m_pTerrainDesc->samples = new uint[m_pTerrainDesc->nbColumns * m_pTerrainDesc->nbRows];
		m_pTerrainDesc->sampleStride = sizeof(uint);

		// Invalid description
		if(!m_pTerrainDesc->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Description of the terrain is invalid");
			return;
		}

		// Process height samples
		byte* currentByte = (byte*)m_pTerrainDesc->samples;
		
		for(uint z = 0; z < m_pTerrainDesc->nbRows; ++z)
		{
			for(uint x = 0; x < m_pTerrainDesc->nbColumns; ++x)
			{
				NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte;
				// Desired height value. Signed 16 bit integer (unfortunately)
				currentSample->height = FastFloat::floatToInt(_desc.heights[x * m_pTerrainDesc->nbRows + z]) & 0xffff;
				currentSample->materialIndex0 = 0;
				currentSample->materialIndex1 = 0;
				currentSample->tessFlag = 0;
				currentByte += m_pTerrainDesc->sampleStride;
			}
		}

		// Create terrain shape at last
		m_pHeightField = _scene->getPhysicsSDK().createHeightField(*m_pTerrainDesc);

		m_pTerrainShapeDesc = new NxHeightFieldShapeDesc();
		m_pTerrainShapeDesc->heightField= m_pHeightField;
		m_pTerrainShapeDesc->shapeFlags	= NX_SF_FEATURE_INDICES;
		m_pTerrainShapeDesc->heightScale= _desc.heightScale;
		m_pTerrainShapeDesc->rowScale	= _desc.step;
		m_pTerrainShapeDesc->columnScale= _desc.step;
		m_pTerrainShapeDesc->meshFlags	= NX_MESH_SMOOTH_SPHERE_COLLISIONS;
		m_pTerrainShapeDesc->materialIndexHighBits = 0;
		m_pTerrainShapeDesc->holeMaterial = 2;
	}
//----------------------------------------------------------------------
	NxShapeDesc* ShapeTerrain::getShapeDesc()
	{
		return static_cast<NxShapeDesc*>(m_pTerrainShapeDesc);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	TerrainShapeFactory::TerrainShapeFactory()
	{
	}
//----------------------------------------------------------------------
	TerrainShapeFactory::~TerrainShapeFactory()
	{
	}
//----------------------------------------------------------------------
	Shape* TerrainShapeFactory::createShape()
	{
		return new ShapeTerrain();
	}
//----------------------------------------------------------------------
}
