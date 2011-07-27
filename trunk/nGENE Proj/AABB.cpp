/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AABB.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "AABB.h"
#include "OBB.h"
#include "Point.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo AABB::Type(L"AABB", &BoundingVolume::Type);


	AABB::AABB()
	{
	}
//----------------------------------------------------------------------
	AABB::AABB(const Vector3& _min, const Vector3& _max):
		m_vecMinimum(_min),
		m_vecMaximum(_max)
	{
		computeCentre();
	}
//----------------------------------------------------------------------
	AABB::AABB(VertexBuffer& _vb)
	{
		construct(_vb);
	}
//----------------------------------------------------------------------
	AABB::~AABB()
	{
	}
//----------------------------------------------------------------------
	void AABB::computeCentre()
	{
		m_vecCentre = 0.5f * (m_vecMinimum + m_vecMaximum);
	}
//----------------------------------------------------------------------
	void AABB::setAABB(const Vector3& _min, const Vector3& _max)
	{
		m_vecMinimum = _min;
		m_vecMaximum = _max;
		computeCentre();
	}
//----------------------------------------------------------------------
	void AABB::setMinimum(const Vector3& _min)
	{
		m_vecMinimum = _min;
		computeCentre();
	}
//----------------------------------------------------------------------
	void AABB::setMaximum(const Vector3& _max)
	{
		m_vecMaximum = _max;
		computeCentre();
	}
//----------------------------------------------------------------------
	void AABB::construct(VertexBuffer& _vb)
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
	void AABB::construct(VertexBuffer* _vb)
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
	void AABB::construct(void* _vertices, uint _count, uint _size, uint _offset)
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
	void AABB::construct(OBB& _box)
	{
		/// @todo Matrix3x3 * Vector3 multiplication would be faster
		Vector4 temp(_box.getCentre());
		Matrix4x4& matBox = _box.getTransform();
		temp = matBox * temp;
		float* fCoords = matBox[3];
		Vector3 vecCentre(fCoords[0], fCoords[1], fCoords[2]);
		m_vecMinimum =
		m_vecMaximum = vecCentre;

		Vector3 min = _box.getMinimum();
		Vector3 max = _box.getMaximum();


		// Find out new AABB size
		if(matBox._m11 > 0.0f)
		{
			m_vecMinimum.x += matBox._m11 * min.x;
			m_vecMaximum.x += matBox._m11 * max.x;
		}
		else
		{
			m_vecMinimum.x += matBox._m11 * max.x;
			m_vecMaximum.x += matBox._m11 * min.x;
		}

		if(matBox._m12 > 0.0f)
		{
			m_vecMinimum.y += matBox._m12 * min.x;
			m_vecMaximum.y += matBox._m12 * max.x;
		}
		else
		{
			m_vecMinimum.y += matBox._m12 * max.x;
			m_vecMaximum.y += matBox._m12 * min.x;
		}

		if(matBox._m13 > 0.0f)
		{
			m_vecMinimum.z += matBox._m13 * min.x;
			m_vecMaximum.z += matBox._m13 * max.x;
		}
		else
		{
			m_vecMinimum.z += matBox._m13 * max.x;
			m_vecMaximum.z += matBox._m13 * min.x;
		}

		if(matBox._m21 > 0.0f)
		{
			m_vecMinimum.x += matBox._m21 * min.y;
			m_vecMaximum.x += matBox._m21 * max.y;
		}
		else
		{
			m_vecMinimum.x += matBox._m21 * max.y;
			m_vecMaximum.x += matBox._m21 * min.y;
		}

		if(matBox._m22 > 0.0f)
		{
			m_vecMinimum.y += matBox._m22 * min.y;
			m_vecMaximum.y += matBox._m22 * max.y;
		}
		else
		{
			m_vecMinimum.y += matBox._m22 * max.y;
			m_vecMaximum.y += matBox._m22 * min.y;
		}

		if(matBox._m23 > 0.0f)
		{
			m_vecMinimum.z += matBox._m23 * min.y;
			m_vecMaximum.z += matBox._m23 * max.y;
		}
		else
		{
			m_vecMinimum.z += matBox._m23 * max.y;
			m_vecMaximum.z += matBox._m23 * min.y;
		}

		if(matBox._m31 > 0.0f)
		{
			m_vecMinimum.x += matBox._m31 * min.z;
			m_vecMaximum.x += matBox._m31 * max.z;
		}
		else
		{
			m_vecMinimum.x += matBox._m31 * max.z;
			m_vecMaximum.x += matBox._m31 * min.z;
		}

		if(matBox._m32 > 0.0f)
		{
			m_vecMinimum.y += matBox._m32 * min.z;
			m_vecMaximum.y += matBox._m32 * max.z;
		}
		else
		{
			m_vecMinimum.y += matBox._m32 * max.z;
			m_vecMaximum.y += matBox._m32 * min.z;
		}

		if(matBox._m33 > 0.0f)
		{
			m_vecMinimum.z += matBox._m33 * min.z;
			m_vecMaximum.z += matBox._m33 * max.z;
		}
		else
		{
			m_vecMinimum.z += matBox._m33 * max.z;
			m_vecMaximum.z += matBox._m33 * min.z;
		}

		// Recompute centre point
		computeCentre();
	}
//----------------------------------------------------------------------
	bool AABB::fits(AABB& _aabb)
	{
		Vector3& min = _aabb.getMinimum();
		Vector3& max = _aabb.getMaximum();

		if(m_vecMinimum.x < min.x && m_vecMinimum.y < min.y && m_vecMinimum.z < min.z &&
		   m_vecMaximum.x > max.x && m_vecMaximum.y > max.y && m_vecMaximum.z > max.z)
		{
			return true;
		}

		return false;
	}
//----------------------------------------------------------------------
	Real AABB::distanceToVector(const Vector3& _vec)
	{
		float dist = 0.0f;

		for(uint i = 0; i < 3; ++i)
		{
			float v = _vec[i];
			float min = m_vecMinimum[i];
			float max = m_vecMaximum[i];

			if(v < min)
				dist += (min - v) * (min - v);
			if(v > max)
				dist += (max - v) * (max - v);
		}

		return Maths::fsqrt(dist);
	}
//----------------------------------------------------------------------
	Real AABB::distanceToPoint(const Point& _pt)
	{
		float dist = 0.0f;

		for(uint i = 0; i < 3; ++i)
		{
			float p = _pt[i];
			float min = m_vecMinimum[i];
			float max = m_vecMaximum[i];

			if(p < min)
				dist += (min - p) * (min - p);
			if(p > max)
				dist += (max - p) * (max - p);
		}

		return Maths::fsqrt(dist);
	}
//----------------------------------------------------------------------
}