/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TerrainTile.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "TerrainTile.h"
#include "AABB.h"
#include "OBB.h"
#include "Surface.h"
#include "TerrainTypes.h"
#include "UtilitiesTypes.h"
#include "Vertex.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo TerrainTile::Type(L"TerrainTile", &BaseClass::Type);


	TerrainTile::TerrainTile():
		m_pSurface(NULL),
		m_pVB(NULL)
	{
		init();
	}
//----------------------------------------------------------------------
	TerrainTile::~TerrainTile()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void TerrainTile::init(Surface* _surface, const RECT_FLOAT& _rect,
		SVertex* _vertices,	const TERRAIN_DESC& _desc, IndexedBuffer* _ivb)
	{
		m_pSurface = _surface;

		dword dwVertsNum = (_rect.right - _rect.left + 1) * (_rect.bottom - _rect.top + 1) +
			4 * (_rect.right - _rect.left + 1) + 4 * (_rect.bottom - _rect.top + 1);
		SVertex* pVertices = new SVertex[dwVertsNum];
		uint i = 0;
		float xTexCoord = _desc.texScaleX / ((float)_desc.columns / (float)_desc.tiles_x);
		float zTexCoord = _desc.texScaleY / ((float)_desc.rows / (float)_desc.tiles_z);
		for(uint z = _rect.top; z <= _rect.bottom; ++z)
		{
			for(uint x = _rect.left; x <= _rect.right; ++x)
			{
				uint z_new = z;
				uint x_new = x;
				if(x == _desc.columns || z == _desc.rows)
				{
					if(x == _desc.columns)
						--x_new;
					if(z == _desc.rows)
						--z_new;
				}
				pVertices[i] = _vertices[z_new * _desc.columns + x_new];
				pVertices[i].vecTexCoord.x = (float)x * xTexCoord;
				pVertices[i++].vecTexCoord.y = (float)z * zTexCoord;
			}
		}

		// Now time for skirt
		float skirtHeight = _desc.skirtSize;
		for(uint x = 0; x <= (_rect.right - _rect.left); ++x)
		{
			pVertices[i] = pVertices[x];
			pVertices[i + uint(_rect.right - _rect.left + 1)].vecPosition = Vector3(pVertices[i].vecPosition.x,
																				pVertices[i].vecPosition.y - skirtHeight,
																				pVertices[i].vecPosition.z);

			pVertices[uint(i + 2 * (_rect.right - _rect.left + 1))] = pVertices[uint((_rect.bottom - _rect.top) * (_rect.right - _rect.left + 1) + x)];
			pVertices[uint(i + 3 * (_rect.right - _rect.left + 1))].vecPosition = Vector3(pVertices[uint(i + 2 * (_rect.right - _rect.left + 1))].vecPosition.x,
																				pVertices[uint(i + 2 * (_rect.right - _rect.left + 1))].vecPosition.y - skirtHeight,
																				pVertices[uint(i + 2 * (_rect.right - _rect.left + 1))].vecPosition.z);
			++i;
		}

		i += 3 * (_rect.right - _rect.left + 1);

		for(uint z = 0; z <= (_rect.bottom - _rect.top); ++z)
		{
			pVertices[i] = pVertices[uint(z * (_rect.right - _rect.left + 1))];
			pVertices[i + uint(_rect.bottom - _rect.top + 1)].vecPosition = Vector3(pVertices[i].vecPosition.x,
																				pVertices[i].vecPosition.y - skirtHeight,
																				pVertices[i].vecPosition.z);

			pVertices[uint(i + 2 * (_rect.bottom - _rect.top + 1))] = pVertices[uint(z * (_rect.right - _rect.left + 1) + (_rect.right - _rect.left))];
			pVertices[uint(i + 3 * (_rect.bottom - _rect.top + 1))].vecPosition = Vector3(pVertices[uint(i + 2 * (_rect.bottom - _rect.top + 1))].vecPosition.x,
																				pVertices[uint(i + 2 * (_rect.bottom - _rect.top + 1))].vecPosition.y - skirtHeight,
																				pVertices[uint(i + 2 * (_rect.bottom - _rect.top + 1))].vecPosition.z);
			++i;
		}

		dword facesNum = _ivb->getIndicesNum() / 3;
		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = pVertices;
		vbDesc.verticesNum = dwVertsNum;
		vbDesc.primitivesNum = facesNum;
		vbDesc.indices = _ivb;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		m_pVB = manager.createVertexBuffer(vbDesc);

		m_pSurface->setVertexBuffer(m_pVB);


		NGENE_DELETE_ARRAY(pVertices);
	}
//----------------------------------------------------------------------
	void TerrainTile::cleanup()
	{
	}
//----------------------------------------------------------------------
	void TerrainTile::setIndexedBuffer(IndexedBuffer* _ivb)
	{
		m_pVB->setIndexedBuffer(_ivb);
		m_pVB->setPrimitivesNum(_ivb->getIndicesNum() / 3);
	}
//----------------------------------------------------------------------
	const Vector3& TerrainTile::getCentre() const
	{
		return m_pSurface->getBoundingBox()->getCentre();
	}
//----------------------------------------------------------------------
	AABB* TerrainTile::getAABB() const
	{
		return m_pSurface->getBoundingBox();
	}
//----------------------------------------------------------------------
}
