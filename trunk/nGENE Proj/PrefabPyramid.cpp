/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabPyramid.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PrefabPyramid.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo PrefabPyramid::Type(L"PrefabPyramid", &NodePrefab::Type);


	PrefabPyramid::PrefabPyramid(float _height, float _side, const Vector2& _texCoordMul):
		NodePrefab(_texCoordMul),
		m_fHeight(_height),
		m_fSideLength(_side)
	{
	}
//----------------------------------------------------------------------
	PrefabPyramid::PrefabPyramid()
	{
	}
//----------------------------------------------------------------------
	PrefabPyramid::~PrefabPyramid()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PrefabPyramid::init()
	{
		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabPyramid::cleanup()
	{
	}
//----------------------------------------------------------------------
	void PrefabPyramid::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			NodePrefab::serialize(_serializer, false, _serializeChildren);

			// Serialize pyramid
			Property <float> prHeight(m_fHeight);
			Property <float> prSide(m_fSideLength);

			_serializer->addProperty("Height", prHeight);
			_serializer->addProperty("SideLength", prSide);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void PrefabPyramid::deserialize(ISerializer* _serializer)
	{
		// Deserialize pyramid
		Property <float> prHeight(m_fHeight);
		Property <float> prSide(m_fSideLength);

		_serializer->getProperty("Height", prHeight);
		_serializer->getProperty("SideLength", prSide);

		NodePrefab::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	void PrefabPyramid::updateGeometry()
	{
		// Create Pyramid's geometry
		float pkx = 0.0f, pky = m_fHeight * 0.5f, pkz = 0.0f;
		float vx0 = -m_fSideLength * 0.5f, vy0 = -m_fHeight * 0.5f, vz0 = -m_fSideLength * 0.5f;
		float vx1 =  m_fSideLength * 0.5f, vy1 = -m_fHeight * 0.5f, vz1 = -m_fSideLength * 0.5f;
		float vx2 =  m_fSideLength * 0.5f, vy2 = -m_fHeight * 0.5f, vz2 =  m_fSideLength * 0.5f;
		float vx3 = -m_fSideLength * 0.5f, vy3 = -m_fHeight * 0.5f, vz3 =  m_fSideLength * 0.5f;
		float aPositions[] =
		{
			vx3, vy3, vz3, vx2, vy2, vz2, vx1, vy1, vz1, vx0, vy0, vz0,
			vx0, vy0, vz0, vx1, vy1, vz1, pkx, pky, pkz,
			vx1, vy1, vz1, vx2, vy2, vz2, pkx, pky, pkz,
			vx2, vy2, vz2, vx3, vy3, vz3, pkx, pky, pkz,
			vx3, vy3, vz3, vx0, vy0, vz0, pkx, pky, pkz
		};

		float pn = 0.70710677f, nn = -0.70710677f;
		float aNormals[] =
		{
			0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			0.0f, pn, nn, 0.0f, pn, nn, 0.0f, pn, nn,
			pn, pn, 0.0f, pn, pn, 0.0f, pn, pn, 0.0f,
			0.0f, pn, pn, 0.0f, pn, pn, 0.0f, pn, pn,
			nn, pn, 0.0f, nn, pn, 0.0f, nn, pn, 0.0f
		};

		float aTexCoords[] =
		{
			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.75f, 0.0f, 0.5f, 1.0f, 0.75f, 0.0f,
			0.5f, 0.0f, 0.5f, 1.0f, 0.5f, 0.0f, 0.25f, 0.0f,
			0.5f, 1.0f, 0.25f, 0.0f, 0.0f, 0.0f, 0.5f, 1.0f
		};

		// Set vertices
		dword dwNumVertices = 16;
		SVertex vertices[16];

		for(uint i = 0; i < 16; ++i)
		{
			vertices[i].dwColour = 0xffffffff;
			vertices[i].vecPosition = Vector3(aPositions[i * 3],
											  aPositions[i * 3 + 1],
											  aPositions[i * 3 + 2]);
			vertices[i].vecNormal = Vector3(aNormals[i * 3],
											aNormals[i * 3 + 1],
											aNormals[i * 3 + 2]);
			vertices[i].vecTexCoord = Vector2(aTexCoords[i * 2] * m_vecTexCoordMul.x,
											  (1.0f - aTexCoords[i * 2 + 1]) * m_vecTexCoordMul.y);
		}

		// Set indices
		dword dwNumIndices = 18;
		short indices[18] =
		{
			1, 2, 3, 0, 1, 3, 4, 5, 6, 7,
			8, 9, 10, 11, 12, 13, 14, 15
		};


		VertexBuffer* vb;
		IndexedBuffer* ivb;
		VertexBufferManager& manager = VertexBufferManager::getSingleton();


		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indices = indices;
		ivbDesc.indicesNum = dwNumIndices;
		ivb = manager.createIndexedBuffer(ivbDesc);

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = vertices;
		vbDesc.verticesNum = dwNumVertices;
		vbDesc.primitivesNum = dwNumIndices / 3;
		vbDesc.primitivesType = PT_TRIANGLELIST;
		vbDesc.indices = ivb;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		vb = manager.createVertexBuffer(vbDesc);


		updateSurface(L"Base", vb);


		// Create normals surface
		SVertex* pNormals = new SVertex[dwNumVertices * 2];
		for(uint i = 0; i < dwNumVertices * 2; i += 2)
		{
			pNormals[i].vecPosition = vertices[i / 2].vecPosition;
			pNormals[i + 1].vecPosition = vertices[i / 2].vecPosition + vertices[i / 2].vecNormal * 0.25f;
			pNormals[i].dwColour = pNormals[i + 1].dwColour = 0xffffffff;
			pNormals[i].vecTexCoord = pNormals[i + 1].vecTexCoord = Vector2::ZERO_VECTOR;
			pNormals[i].vecNormal = pNormals[i + 1].vecNormal = Vector3::UNIT_Y;
		}

		VERTEXBUFFER_DESC vbDescNormals;
		vbDescNormals.vertices = pNormals;
		vbDescNormals.verticesNum = dwNumVertices * 2;
		vbDescNormals.primitivesNum = dwNumVertices;
		vbDescNormals.primitivesType = PT_LINELIST;
		vbDescNormals.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		VertexBuffer* vbNormals = manager.createVertexBuffer(vbDescNormals);


		Surface* pSurface = updateSurface(L"Normals", vbNormals);
		pSurface->setPickable(false);


		NGENE_DELETE_ARRAY(pNormals);


		calculateBoundingBox();
	}
//----------------------------------------------------------------------
	void PrefabPyramid::setHeight(Real _height)
	{
		m_fHeight = _height;

		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabPyramid::setSideLength(Real _side)
	{
		m_fSideLength = _side;

		updateGeometry();
	}
//----------------------------------------------------------------------
}