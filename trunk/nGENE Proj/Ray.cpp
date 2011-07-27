/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Ray.cpp
Version:	0.09
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Ray.h"
#include "AABB.h"
#include "Plane.h"
#include "Surface.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	Ray::Ray()
	{
	}
//----------------------------------------------------------------------
	Ray::Ray(Point& _origin, Vector3& _direction):
		m_ptOrigin(_origin),
		m_vecDirection(_direction),
		m_fTMin(1e9)
	{
	}
//----------------------------------------------------------------------
	Ray::~Ray()
	{
	}
//----------------------------------------------------------------------
	bool Ray::intersectAABB(AABB& _aabb)
	{
		float t = 0.0f;
		float tmax = 1e9;

		Vector3& vecMin = _aabb.getMinimum();
		Vector3& vecMax = _aabb.getMaximum();

		for(uint i = 0; i < 3; ++i)
		{
			if(!Maths::floatCompare(m_vecDirection[i], 0.0f))
			{
				if(m_ptOrigin[i] < vecMin[i] || m_ptOrigin[i] > vecMax[i])
					return false;
			}
			else
			{
				float inv = 1.0f / m_vecDirection[i];
				float t1 = (vecMin[i] - m_ptOrigin[i]) * inv;
				float t2 = (vecMax[i] - m_ptOrigin[i]) * inv;

				if(t1 > t2)
					Maths::swap(t1, t2);
				if(t1 > t)
					t = t1;
				if(t2 < tmax)
					tmax = t2;

				if(t > tmax)
					return false;
			}
		}

		m_ptIntersectionPoint = m_ptOrigin + m_vecDirection * t;

		if(t < m_fTMin)
			m_fTMin = t;

		return true;
	}
//----------------------------------------------------------------------
	bool Ray::intersectPlane(const Plane& _plane)
	{
		const Vector3& normal = _plane.getPlaneNormal();
		float dot = m_vecDirection % normal;
		if(dot >= 0.0f)
			return false;
		float t = (-_plane.getPlaneD() - m_ptOrigin % normal) / dot;

		m_ptIntersectionPoint = m_ptOrigin + m_vecDirection * t;

		if(t < m_fTMin)
			m_fTMin = t;

		return true;
	}
//----------------------------------------------------------------------
	bool Ray::intersectTriangle(const Vector3& _vec0,
								const Vector3& _vec1,
								const Vector3& _vec2)
	{
		Vector3 vec1 = _vec1 - _vec0;
		Vector3 vec2 = _vec2 - _vec0;

		Vector3 normal = vec2 * vec1;
		float dot = normal % m_vecDirection;

		if(dot >= 0.0f)
			return false;

		float d = normal % _vec0;
		float t = d - normal % m_ptOrigin;

		if(t > 0.0f || t < dot)
			return false;

		t /= dot;
		Vector3 intersectPt = m_ptOrigin + m_vecDirection * t;

		float u0, u1, u2;
		float v0, v1, v2;
		if(Maths::abs(normal.x) > Maths::abs(normal.y))
		{
			if(Maths::abs(normal.x) > Maths::abs(normal.z))
			{
				u0 = intersectPt.y - _vec0.y;
				u1 = _vec1.y - _vec0.y;
				u2 = _vec2.y - _vec0.y;

				v0 = intersectPt.z - _vec0.z;
				v1 = _vec1.z - _vec0.z;
				v2 = _vec2.z - _vec0.z;
			}
			else
			{
				u0 = intersectPt.x - _vec0.x;
				u1 = _vec1.x - _vec0.x;
				u2 = _vec2.x - _vec0.x;

				v0 = intersectPt.y - _vec0.y;
				v1 = _vec1.y - _vec0.y;
				v2 = _vec2.y - _vec0.y;
			}
		}
		else
		{
			if(Maths::abs(normal.y) > Maths::abs(normal.z))
			{
				u0 = intersectPt.x - _vec0.x;
				u1 = _vec1.x - _vec0.x;
				u2 = _vec2.x - _vec0.x;

				v0 = intersectPt.z - _vec0.z;
				v1 = _vec1.z - _vec0.z;
				v2 = _vec2.z - _vec0.z;
			}
			else
			{
				u0 = intersectPt.x - _vec0.x;
				u1 = _vec1.x - _vec0.x;
				u2 = _vec2.x - _vec0.x;

				v0 = intersectPt.y - _vec0.y;
				v1 = _vec1.y - _vec0.y;
				v2 = _vec2.y - _vec0.y;
			}
		}

		float temp = u1 * v2 - v1 * u2;
		if(!Maths::floatCompare(temp, 0.0f))
			return false;

		temp = 1.0f / temp;

		float alpha = (u0 * v2 - v0 * u2) * temp;
		if(alpha < 0.0f)
			return false;

		float beta = (u1 * v0 - v1 * u0) * temp;
		if(beta < 0.0f)
			return false;

		float gamma = 1.0f - alpha - beta;
		if(gamma < 0.0f)
			return false;

		m_ptIntersectionPoint = m_ptOrigin + m_vecDirection * t;
		
		if(t < m_fTMin)
			m_fTMin = t;

		return true;
	}
//----------------------------------------------------------------------
	bool Ray::intersectSurface(const Surface& _surface)
	{
		VertexBuffer* pVB = _surface.getVertexBuffer();
		IndexedBuffer* pIVB = pVB->getIndexedBuffer();

		// No index buffers, so definitely false
		if(!pIVB)
			return false;

		VERTEXBUFFER_DESC& desc = pVB->getDesc();

		SVertex* pVertices;
		pVB->lock(0, desc.verticesNum, (void**)&pVertices);

		ushort* pIndices = new ushort[pIVB->getDesc().indicesNum];
		pIVB->getIndexData(pIndices, 0, pIVB->getDesc().indicesNum);

		bool result = false;

		for(uint i = 0; i < desc.primitivesNum; ++i)
		{
			if(intersectTriangle(pVertices[pIndices[i * 3 + 0]].vecPosition,
								 pVertices[pIndices[i * 3 + 1]].vecPosition,
								 pVertices[pIndices[i * 3 + 2]].vecPosition))
			{
				result = true;
				break;
			}
		}

		NGENE_DELETE_ARRAY(pIndices);
		pVB->unlock();

		return result;
	}
//----------------------------------------------------------------------
	bool Ray::intersectSurface(const Surface* _surface)
	{
		VertexBuffer* pVB = _surface->getVertexBuffer();
		IndexedBuffer* pIVB = pVB->getIndexedBuffer();

		// No index buffers, so definitely false
		if(!pIVB)
			return false;

		VERTEXBUFFER_DESC& desc = pVB->getDesc();

		SVertex* pVertices;
		pVB->lock(0, desc.verticesNum, (void**)&pVertices);

		ushort* pIndices = new ushort[pIVB->getDesc().indicesNum];
		pIVB->getIndexData(pIndices, 0, pIVB->getDesc().indicesNum);

		bool result = false;

		for(uint i = 0; i < desc.primitivesNum; ++i)
		{
			if(intersectTriangle(pVertices[pIndices[i * 3 + 0]].vecPosition,
								 pVertices[pIndices[i * 3 + 1]].vecPosition,
								 pVertices[pIndices[i * 3 + 2]].vecPosition))
			{
				result = true;
				break;
			}
		}

		NGENE_DELETE_ARRAY(pIndices);
		pVB->unlock();

		return result;
	}
//----------------------------------------------------------------------
	bool Ray::intersectSurface(const Surface& _surface, INTERSECTION_DESC& _desc)
	{
		VertexBuffer* pVB = _surface.getVertexBuffer();
		IndexedBuffer* pIVB = pVB->getIndexedBuffer();

		// No index buffers, so definitely false
		if(!pIVB)
			return false;

		VERTEXBUFFER_DESC& desc = pVB->getDesc();

		SVertex* pVertices;
		pVB->lock(0, desc.verticesNum, (void**)&pVertices);

		ushort* pIndices = new ushort[pIVB->getDesc().indicesNum];
		pIVB->getIndexData(pIndices, 0, pIVB->getDesc().indicesNum);

		bool result = false;

		for(uint i = 0; i < desc.primitivesNum; ++i)
		{
			if(intersectTriangle(pVertices[pIndices[i * 3 + 0]].vecPosition,
								 pVertices[pIndices[i * 3 + 1]].vecPosition,
								 pVertices[pIndices[i * 3 + 2]].vecPosition))
			{
				result = true;

				// Fill the structure
				_desc.intersectionPoint = m_ptIntersectionPoint;
				Vector3 normal = (pVertices[pIndices[i * 3 + 2]].vecPosition - pVertices[pIndices[i * 3 + 0]].vecPosition) *
								 (pVertices[pIndices[i * 3 + 1]].vecPosition - pVertices[pIndices[i * 3 + 0]].vecPosition);
				normal.normalize();
				_desc.normal = normal;

				break;
			}
		}

		NGENE_DELETE_ARRAY(pIndices);
		pVB->unlock();

		return result;
	}
//----------------------------------------------------------------------
	void Ray::moveToObjectSpace(const Matrix4x4& _mat)
	{
		Matrix4x4 m = _mat;

		m.invert();

		Vector4 temp;
		temp.set(m_ptOrigin.x, m_ptOrigin.y, m_ptOrigin.z, 1.0f);
		temp = temp * m;
		m_ptOrigin = Point(temp.x, temp.y, temp.z);

		temp.set(m_vecDirection.x, m_vecDirection.y, m_vecDirection.z, 0.0f);
		temp = temp * m;
		m_vecDirection = Vector3(temp.x, temp.y, temp.z);
	}
//----------------------------------------------------------------------
}