/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabBox.cpp
Version:	0.11
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PrefabBox.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo PrefabBox::Type(L"PrefabBox", &NodePrefab::Type);


	PrefabBox::PrefabBox(AABB& aabb, const Vector2& _texCoordMul):
		NodePrefab(_texCoordMul),
		m_Box(aabb)
	{
	}
//----------------------------------------------------------------------
	PrefabBox::PrefabBox()
	{
	}
//----------------------------------------------------------------------
	PrefabBox::~PrefabBox()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PrefabBox::init()
	{
		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabBox::updateGeometry()
	{
		// Create box's geometry
		SVertex vertices[24];

		// Set position vectors and colours
		setPositions(vertices);

		// Set texture coordinates
		setTexCoords(vertices);

		// Set normal vectors
		setNormals(vertices);

		// Set indices
		short indices[36] = {3, 1, 0, 0, 2, 3,
							 4, 5, 7, 7, 6, 4,
							 9, 11, 15, 15, 13, 9,
							 14, 10, 8, 8, 12, 14,
							 16, 17, 21, 21, 20, 16,
							 23, 19, 18, 18, 22, 23};

		VertexBufferManager& manager = VertexBufferManager::getSingleton();
		VertexBuffer* vb;
		IndexedBuffer* ivb;

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = vertices;
		vbDesc.verticesNum = 24;
		vbDesc.primitivesNum = 12;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		vb = manager.createVertexBuffer(vbDesc);

		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indices = indices;
		ivbDesc.indicesNum = 36;
		ivb = manager.createIndexedBuffer(ivbDesc);
		vb->setIndexedBuffer(ivb);

		
		updateSurface(L"Base", vb);


		calculateBoundingBox();
	}
//----------------------------------------------------------------------
	void PrefabBox::setPositions(SVertex* _vertices)
	{
		uint b = 0;
		for(uint i = 0; i < 3; ++i)
		{
			_vertices[b].vecPosition = m_Box.getMinimum();
			_vertices[b].dwColour = 0xffffffff;

			++b;
			_vertices[b].vecPosition.x = m_Box.getMaximum().x;
			_vertices[b].vecPosition.y = m_Box.getMinimum().y;
			_vertices[b].vecPosition.z = m_Box.getMinimum().z;
			_vertices[b].dwColour = 0xffffffff;

			++b;
			_vertices[b].vecPosition.x = m_Box.getMinimum().x;
			_vertices[b].vecPosition.y = m_Box.getMinimum().y;
			_vertices[b].vecPosition.z = m_Box.getMaximum().z;
			_vertices[b].dwColour = 0xffffffff;
			
			++b;
			_vertices[b].vecPosition.x = m_Box.getMaximum().x;
			_vertices[b].vecPosition.y = m_Box.getMinimum().y;
			_vertices[b].vecPosition.z = m_Box.getMaximum().z;
			_vertices[b].dwColour = 0xffffffff;

			++b;
			_vertices[b].vecPosition.x = m_Box.getMinimum().x;
			_vertices[b].vecPosition.y = m_Box.getMaximum().y;
			_vertices[b].vecPosition.z = m_Box.getMinimum().z;
			_vertices[b].dwColour = 0xffffffff;

			++b;
			_vertices[b].vecPosition.x = m_Box.getMaximum().x;
			_vertices[b].vecPosition.y = m_Box.getMaximum().y;
			_vertices[b].vecPosition.z = m_Box.getMinimum().z;
			_vertices[b].dwColour = 0xffffffff;
			
			++b;
			_vertices[b].vecPosition.x = m_Box.getMinimum().x;
			_vertices[b].vecPosition.y = m_Box.getMaximum().y;
			_vertices[b].vecPosition.z = m_Box.getMaximum().z;
			_vertices[b].dwColour = 0xffffffff;

			++b;
			_vertices[b].vecPosition = m_Box.getMaximum();
			_vertices[b].dwColour = 0xffffffff;

			++b;
		}
	}
//----------------------------------------------------------------------
	void PrefabBox::setTexCoords(SVertex* _vertices)
	{
		_vertices[0].vecTexCoord =
		_vertices[4].vecTexCoord =
		_vertices[9].vecTexCoord =
		_vertices[8].vecTexCoord =
		_vertices[16].vecTexCoord=
		_vertices[18].vecTexCoord= Vector2(0.0f,
										   m_vecTexCoordMul.y * 1.0f);

		_vertices[1].vecTexCoord =
		_vertices[5].vecTexCoord =
		_vertices[11].vecTexCoord=
		_vertices[10].vecTexCoord=
		_vertices[17].vecTexCoord=
		_vertices[19].vecTexCoord= Vector2(m_vecTexCoordMul.x * 1.0f,
										   m_vecTexCoordMul.y * 1.0f);

		_vertices[2].vecTexCoord =
		_vertices[6].vecTexCoord =
		_vertices[13].vecTexCoord=
		_vertices[12].vecTexCoord=
		_vertices[20].vecTexCoord=
		_vertices[22].vecTexCoord= Vector2(0.0f,
										   0.0f);

		_vertices[3].vecTexCoord =
		_vertices[7].vecTexCoord =
		_vertices[15].vecTexCoord=
		_vertices[14].vecTexCoord=
		_vertices[21].vecTexCoord=
		_vertices[23].vecTexCoord= Vector2(m_vecTexCoordMul.x * 1.0f,
										   0.0f);
	}
//----------------------------------------------------------------------
	void PrefabBox::setNormals(SVertex* _vertices)
	{
		_vertices[0].vecNormal =
		_vertices[1].vecNormal =
		_vertices[2].vecNormal =
		_vertices[3].vecNormal = Vector3::NEGATIVE_UNIT_Y;

		_vertices[4].vecNormal =
		_vertices[5].vecNormal =
		_vertices[6].vecNormal =
		_vertices[7].vecNormal = Vector3::UNIT_Y;

		_vertices[9].vecNormal  =
		_vertices[11].vecNormal =
		_vertices[13].vecNormal =
		_vertices[15].vecNormal = Vector3::UNIT_X;

		_vertices[8].vecNormal  =
		_vertices[10].vecNormal =
		_vertices[12].vecNormal =
		_vertices[14].vecNormal = Vector3::NEGATIVE_UNIT_X;

		_vertices[16].vecNormal =
		_vertices[17].vecNormal =
		_vertices[20].vecNormal =
		_vertices[21].vecNormal = Vector3::NEGATIVE_UNIT_Z;

		_vertices[18].vecNormal =
		_vertices[19].vecNormal =
		_vertices[22].vecNormal =
		_vertices[23].vecNormal = Vector3::UNIT_Z;
	}
//----------------------------------------------------------------------
	void PrefabBox::cleanup()
	{
	}
//----------------------------------------------------------------------
	void PrefabBox::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			NodePrefab::serialize(_serializer, false, _serializeChildren);

			// Serialize box data
			Property <Vector3> prMin(m_Box.getMinimum());
			Property <Vector3> prMax(m_Box.getMaximum());

			_serializer->addProperty("Minimum", prMin);
			_serializer->addProperty("Maximum", prMax);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void PrefabBox::deserialize(ISerializer* _serializer)
	{
		// Deserialize box data
		Vector3 min, max;
		Property <Vector3> prMin(min);
		Property <Vector3> prMax(max);

		_serializer->getProperty("Minimum", prMin);
		_serializer->getProperty("Maximum", prMax);

		m_Box.setAABB(min, max);

		NodePrefab::deserialize(_serializer);
	}
//----------------------------------------------------------------------
}