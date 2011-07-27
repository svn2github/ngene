/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabPlane.cpp
Version:	0.12
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PrefabPlane.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo PrefabPlane::Type(L"PrefabPlane", &NodePrefab::Type);


	PrefabPlane::PrefabPlane(Plane& _plane, Real _sideLength,
		const Vector2& _texCoordMul):
		NodePrefab(_texCoordMul),
		m_Plane(_plane),
		m_fSideLength(_sideLength)
	{
	}
//----------------------------------------------------------------------
	PrefabPlane::PrefabPlane()
	{
	}
//----------------------------------------------------------------------
	PrefabPlane::~PrefabPlane()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PrefabPlane::init()
	{
		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabPlane::cleanup()
	{
	}
//----------------------------------------------------------------------
	void PrefabPlane::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			NodePrefab::serialize(_serializer, false, _serializeChildren);
			
			Property <Plane> prPlane(m_Plane);
			_serializer->addProperty("Plane", prPlane);

			Property <float> prSideLength(m_fSideLength);
			_serializer->addProperty("SideLength", prSideLength);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void PrefabPlane::deserialize(ISerializer* _serializer)
	{
		Property <Plane> prPlane(m_Plane);
		_serializer->getProperty("Plane", prPlane);

		Property <float> prSideLength(m_fSideLength);
		_serializer->getProperty("SideLength", prSideLength);

		NodePrefab::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	void PrefabPlane::updateGeometry()
	{
		// Create plane's geometry
		SVertex vertices[4];

		Vector3 p0 = m_Plane.getPlanePoint();
		Vector3 normal = m_Plane.getPlaneNormal();

		vertices[0].vecPosition.x = p0.x - (m_fSideLength * 0.5f);
		vertices[0].vecPosition.y = p0.y;
		vertices[0].vecPosition.z = p0.z - (m_fSideLength * 0.5f);
		vertices[0].dwColour = 0xffffffff;
		vertices[0].vecNormal = normal;
		vertices[0].vecTexCoord = Vector2(0.0f, m_vecTexCoordMul.y * 1.0f);

		vertices[1].vecPosition.x = p0.x + (m_fSideLength * 0.5f);
		vertices[1].vecPosition.y = p0.y;
		vertices[1].vecPosition.z = p0.z - (m_fSideLength * 0.5f);
		vertices[1].dwColour = 0xffffffff;
		vertices[1].vecNormal = normal;
		vertices[1].vecTexCoord = Vector2(m_vecTexCoordMul.x * 1.0f,
			m_vecTexCoordMul.y * 1.0f);

		vertices[2].vecPosition.x = p0.x - (m_fSideLength * 0.5f);
		vertices[2].vecPosition.y = p0.y;
		vertices[2].vecPosition.z = p0.z + (m_fSideLength * 0.5f);
		vertices[2].dwColour = 0xffffffff;
		vertices[2].vecNormal = normal;
		vertices[2].vecTexCoord = Vector2(0.0f, 0.0f);

		vertices[3].vecPosition.x = p0.x + (m_fSideLength * 0.5f);
		vertices[3].vecPosition.y = p0.y;
		vertices[3].vecPosition.z = p0.z + (m_fSideLength * 0.5f);
		vertices[3].dwColour = 0xffffffff;
		vertices[3].vecNormal = normal;
		vertices[3].vecTexCoord = Vector2(m_vecTexCoordMul.x * 1.0f,
			0.0f);

		short indices[6] = {0, 1, 2, 3, 2, 1};

		VertexBufferManager& manager = VertexBufferManager::getSingleton();
		VertexBuffer* vb;
		IndexedBuffer* ivb;

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = vertices;
		vbDesc.verticesNum = 4;
		vbDesc.primitivesNum = 2;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		vb = manager.createVertexBuffer(vbDesc);

		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indices = indices;
		ivbDesc.indicesNum = 6;
		ivb = manager.createIndexedBuffer(ivbDesc);

		vb->setIndexedBuffer(ivb);


		updateSurface(L"Base", vb);


		calculateBoundingBox();
	}
//----------------------------------------------------------------------
}