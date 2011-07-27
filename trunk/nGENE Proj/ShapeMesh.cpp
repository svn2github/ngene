/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShapeMesh.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "IndexedBuffer.h"
#include "PhysXMapping.h"
#include "PhysXStream.h"
#include "Shape.h"
#include "ShapeMesh.h"
#include "ShapeEnumerator.h"
#include "Vertex.h"
#include "VertexBuffer.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxCooking.h"
	#include "NxTriangleMeshDesc.h"
	#include "NxTriangleMeshShapeDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo ShapeMesh::Type(L"ShapeMesh", &Shape::Type);


	ShapeMesh::ShapeMesh():
		m_pMeshGeometry(new NxTriangleMeshDesc()),
		m_pMeshDesc(new NxTriangleMeshShapeDesc())
	{
		init();
	}
//----------------------------------------------------------------------
	ShapeMesh::~ShapeMesh()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ShapeMesh::init()
	{
	}
//----------------------------------------------------------------------
	void ShapeMesh::cleanup()
	{
		NGENE_DELETE(m_pMeshDesc);
		NGENE_DELETE(m_pMeshGeometry);
	}
//----------------------------------------------------------------------
	void ShapeMesh::setDimensions(const Vector3& _dims)
	{
		Shape::setDimensions(_dims);
	}
//----------------------------------------------------------------------
	NxShapeDesc* ShapeMesh::getShapeDesc()
	{
		return static_cast<NxShapeDesc*>(m_pMeshDesc);
	}
//----------------------------------------------------------------------
	void ShapeMesh::setTrigger(bool _value)
	{
		Shape::setTrigger(_value);
		if(_value)
			m_pMeshDesc->shapeFlags |= NX_TRIGGER_ENABLE;
		else
			m_pMeshDesc->shapeFlags &= ~NX_TRIGGER_ENABLE;
	}
//----------------------------------------------------------------------
	void ShapeMesh::setLocalPos(const Vector3& _pos)
	{
		Shape::setLocalPos(_pos);
		m_pMeshDesc->localPose.t.x = _pos.x;
		m_pMeshDesc->localPose.t.y = _pos.y;
		m_pMeshDesc->localPose.t.z = _pos.z;
	}
//----------------------------------------------------------------------
	void ShapeMesh::setGeometryData(const VertexBuffer& _vb, NxPhysicsSDK* _physx, bool _flipNormals)
	{
		// There's no valid geometrical data.
		IndexedBuffer* pIVB = _vb.getIndexedBuffer();
		if(!pIVB)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"To create ShapeMesh index data is necessary");
			return;
		}

		SVertex* pVertices = new SVertex[_vb.getVerticesNum()];
		Vector3* pPositions = new Vector3[_vb.getVerticesNum()];
		_vb.getVertexData(pVertices, 0, _vb.getVerticesNum());
		for(uint i = 0; i < _vb.getVerticesNum(); ++i)
		{
			pPositions[i] = pVertices[i].vecPosition * 2.0f;
			pPositions[i].x *= m_vecDimensions.x;
			pPositions[i].y *= m_vecDimensions.y;
			pPositions[i].z *= m_vecDimensions.z;
		}

		void* pIndices = NULL;
		if(pIVB->getIndicesNum() < 65356 && pIVB->is32Bit())
			pIndices = new uint[pIVB->getIndicesNum()];
		else
			pIndices = new ushort[pIVB->getIndicesNum()];

		pIVB->getIndexData(pIndices, 0, pIVB->getIndicesNum());

		m_pMeshGeometry->numVertices		= _vb.getVerticesNum();
		m_pMeshGeometry->numTriangles		= _vb.getIndexedBuffer()->getIndicesNum() / 3;
		m_pMeshGeometry->pointStrideBytes	= sizeof(Vector3);
		m_pMeshGeometry->triangleStrideBytes= 3 * _vb.getIndexedBuffer()->getIndexSize();
		m_pMeshGeometry->points				= pPositions;
		m_pMeshGeometry->triangles			= pIndices;
		if(_flipNormals)
			m_pMeshGeometry->flags			|= NX_MF_FLIPNORMALS;
		if(_vb.getIndexedBuffer()->getIndexSize() == sizeof(ushort))
			m_pMeshGeometry->flags			|= NX_MF_16_BIT_INDICES;

		if(!m_pMeshGeometry->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Invalid mesh geometry data");
			return;
		}

		// Init cooking SDK
		if(!NxInitCooking())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cooking initialization failed");
			return;
		}

		MemoryWriteBuffer buffer;
		if(NxCookTriangleMesh(*m_pMeshGeometry, buffer))
		{
			m_pMeshDesc->meshData = _physx->createTriangleMesh(MemoryReadBuffer(buffer.getData()));
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Mesh could not be properly cooked");
			return;
		}

		NGENE_DELETE_ARRAY(pIndices);
		NGENE_DELETE_ARRAY(pPositions);
		NGENE_DELETE_ARRAY(pVertices);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	MeshShapeFactory::MeshShapeFactory()
	{
	}
//----------------------------------------------------------------------
	MeshShapeFactory::~MeshShapeFactory()
	{
	}
//----------------------------------------------------------------------
	Shape* MeshShapeFactory::createShape()
	{
		return new ShapeMesh();
	}
//----------------------------------------------------------------------
}