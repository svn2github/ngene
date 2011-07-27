/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		OBB.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "OBB.h"
#include "Vector4.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo OBB::Type(L"OBB", &BoundingVolume::Type);


	OBB::OBB()
	{
	}
//----------------------------------------------------------------------
	OBB::OBB(const Vector3& _min, const Vector3& _max):
		m_vecMinimum(_min),
		m_vecMaximum(_max)
	{
		computeCentre();
	}
//----------------------------------------------------------------------
	OBB::OBB(VertexBuffer& _vb)
	{
		construct(_vb);
	}
//----------------------------------------------------------------------
	OBB::~OBB()
	{
	}
//----------------------------------------------------------------------
	void OBB::computeCentre()
	{
		m_vecCentre = 0.5f * (m_vecMinimum + m_vecMaximum);
	}
//----------------------------------------------------------------------
	void OBB::setOBB(const Vector3& _min, const Vector3& _max)
	{
		m_vecMinimum = _min;
		m_vecMaximum = _max;
		computeCentre();
	}
//----------------------------------------------------------------------
	void OBB::construct(VertexBuffer& _vb)
	{
		uint nCount = _vb.getVerticesNum();

		// VertexBuffer contains no vertices, so exit
		if(!nCount)
			return;

		VERTEX_ELEMENT* pElement = _vb.getDesc().vertexDeclaration->getVertexElement(VEU_POSITION);
		// VertexBuffer contains no position data, so exit
		if(!pElement)
			return;
		dword offset = pElement->offset;

		byte* pVertices;
		_vb.lock(0, nCount, (void**)&pVertices);
		construct(pVertices,
			nCount,
			_vb.getDesc().vertexSize,
			offset);
		_vb.unlock();
	}
//----------------------------------------------------------------------
	void OBB::construct(VertexBuffer* _vb)
	{
		uint nCount = _vb->getVerticesNum();

		// VertexBuffer contains no vertices, so exit
		if(!nCount)
			return;

		VERTEX_ELEMENT* pElement = _vb->getDesc().vertexDeclaration->getVertexElement(VEU_POSITION);
		// VertexBuffer contains no position data, so exit
		if(!pElement)
			return;
		dword offset = pElement->offset;

		byte* pVertices;
		_vb->lock(0, nCount, (void**)&pVertices);
			construct(pVertices,
					  nCount,
					  _vb->getDesc().vertexSize,
					  offset);
		_vb->unlock();
	}
//----------------------------------------------------------------------
	void OBB::construct(void* _vertices, uint _count, uint _size, uint _offset)
	{
		Vector3 vMin(1e9, 1e9, 1e9);
		Vector3 vMax(-1e9, -1e9, -1e9);
		Vector3 vCurrent;

		// Find the bounding points
		byte* pVertices = (byte*)_vertices;
		for(uint i = 0; i < _count; ++i)
		{
			memcpy(&vCurrent, pVertices + i * _size + _offset, sizeof(Vector3));


			if(vCurrent.x < vMin.x)
				vMin.x = vCurrent.x;
			else if(vCurrent.x > vMax.x)
				vMax.x = vCurrent.x;

			if(vCurrent.y < vMin.y)
				vMin.y = vCurrent.y;
			else if(vCurrent.y > vMax.y)
				vMax.y = vCurrent.y;

			if(vCurrent.z < vMin.z)
				vMin.z = vCurrent.z;
			else if(vCurrent.z > vMax.z)
				vMax.z = vCurrent.z;
		}


		// Set the class members
		m_vecMinimum = vMin;
		m_vecMaximum = vMax;

		computeCentre();
	}
//----------------------------------------------------------------------
	void OBB::setTransform(const Matrix4x4& _mat)
	{
		m_matTransform = _mat;
	}
//----------------------------------------------------------------------
	Matrix4x4& OBB::getTransform()
	{
		return m_matTransform;
	}
//----------------------------------------------------------------------
}