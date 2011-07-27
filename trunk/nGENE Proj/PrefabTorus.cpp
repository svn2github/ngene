/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabTorus.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PrefabTorus.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo PrefabTorus::Type(L"PrefabTorus", &NodePrefab::Type);


	PrefabTorus::PrefabTorus(uint _radialSlices, uint _circleSlices, float _innerRadius,
		float _outerRadius, const Vector2& _texCoordMul):
		NodePrefab(_texCoordMul),
		m_fInnerRadius(_innerRadius),
		m_fOuterRadius(_outerRadius),
		m_nRadialSlices(_radialSlices),
		m_nCircleSlices(_circleSlices)
	{
	}
//----------------------------------------------------------------------
	PrefabTorus::PrefabTorus()
	{
	}
//----------------------------------------------------------------------
	PrefabTorus::~PrefabTorus()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PrefabTorus::init()
	{
		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabTorus::cleanup()
	{
	}
//----------------------------------------------------------------------
	void PrefabTorus::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			NodePrefab::serialize(_serializer, false, _serializeChildren);

			// Serialize torus
			Property <float> prInnerRadius(m_fInnerRadius);
			Property <float> prOuterRadius(m_fOuterRadius);

			_serializer->addProperty("InnerRadius", prInnerRadius);
			_serializer->addProperty("OuterRadius", prOuterRadius);

			Property <uint> prRadialSlices(m_nRadialSlices);
			_serializer->addProperty("RadialSlices", prRadialSlices);
			Property <uint> prCircleSlices(m_nCircleSlices);
			_serializer->addProperty("CircleSlices", prCircleSlices);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void PrefabTorus::deserialize(ISerializer* _serializer)
	{
		// Deserialize torus
		Property <float> prInnerRadius(m_fInnerRadius);
		Property <float> prOuterRadius(m_fOuterRadius);

		_serializer->getProperty("InnerRadius", prInnerRadius);
		_serializer->getProperty("OuterRadius", prOuterRadius);

		Property <uint> prRadialSlices(m_nRadialSlices);
		_serializer->getProperty("RadialSlices", prRadialSlices);
		Property <uint> prCircleSlices(m_nCircleSlices);
		_serializer->getProperty("CircleSlices", prCircleSlices);

		NodePrefab::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	void PrefabTorus::updateGeometry()
	{
		// Create torus' geometry
		uint index = 0;

		// Set vertices
		dword dwNumVertices = (m_nCircleSlices + 1) * (m_nRadialSlices + 1);
		SVertex* pVertices = new SVertex[dwNumVertices];
		memset(pVertices, 0, dwNumVertices * sizeof(SVertex));

		float invCircleSlices = 1.0f / m_nCircleSlices;
		float invRadialSlices = 1.0f / m_nRadialSlices;


		Vector3 radialAxis;
		Vector3 torusMiddle;
		Vector3 tmpNormal;
		index = 0;
		for(uint nCircle = 0; nCircle < m_nCircleSlices; ++nCircle)
		{
			float circleFrac = nCircle * invCircleSlices;
			float theta = Maths::PI_DOUBLE * circleFrac;
			float cosTheta = Maths::fcos(theta);
			float sinTheta = Maths::fsin(theta);
			radialAxis.set(cosTheta, 0.0f, sinTheta);
			torusMiddle = radialAxis * m_fOuterRadius;

			uint prevIndex = index;
			for(uint nRadial = 0; nRadial < m_nRadialSlices; ++nRadial)
			{
				float radialFrac = nRadial * invRadialSlices;
				float phi = Maths::PI_DOUBLE * radialFrac;
				float cosPhi = Maths::fcos(phi);
				float sinPhi = Maths::fsin(phi);
				tmpNormal = radialAxis;
				tmpNormal.multiplyByNumber(cosPhi);
				tmpNormal.y += sinPhi;

				pVertices[index].vecNormal = tmpNormal;
				pVertices[index].dwColour = 0xffffffff;

				tmpNormal *= m_fInnerRadius;
				tmpNormal += torusMiddle;

				pVertices[index].vecPosition = tmpNormal;
				pVertices[index].vecTexCoord.x = (1.0f - radialFrac) * m_vecTexCoordMul.x;
				pVertices[index].vecTexCoord.y = circleFrac * m_vecTexCoordMul.y;

				++index;
			}

			pVertices[index].vecNormal = pVertices[prevIndex].vecNormal;
			pVertices[index].dwColour = 0xffffffff;
			pVertices[index].vecPosition = pVertices[prevIndex].vecPosition;
			pVertices[index].vecTexCoord.x = 0.0f;
			pVertices[index].vecTexCoord.y = circleFrac * m_vecTexCoordMul.y;

			++index;
		}

		for(uint nRadial = 0; nRadial <= m_nRadialSlices; ++nRadial, ++index)
		{
			pVertices[index].vecNormal = pVertices[nRadial].vecNormal;
			pVertices[index].dwColour = 0xffffffff;
			pVertices[index].vecPosition = pVertices[nRadial].vecPosition;
			pVertices[index].vecTexCoord.x = pVertices[nRadial].vecTexCoord.x * m_vecTexCoordMul.x;
			pVertices[index].vecTexCoord.y = 1.0f * m_vecTexCoordMul.y;
		}


		// Set indices
		dword dwNumIndices = 6 * m_nCircleSlices * m_nRadialSlices;
		short* pIndices = new short[dwNumIndices];

		index = 0;
		uint baseIndex = 0;
		for(uint nCircle = 0; nCircle < m_nCircleSlices; ++nCircle)
		{
			uint index0 = baseIndex;
			uint index1 = index0 + 1;
			baseIndex += m_nRadialSlices + 1;
			uint index2 = baseIndex;
			uint index3 = index2 + 1;

			for(uint nRadial = 0; nRadial < m_nRadialSlices; ++nRadial)
			{
				pIndices[index++] = index2;
				pIndices[index++] = index1;
				pIndices[index++] = index0++;
				pIndices[index++] = index2++;
				pIndices[index++] = index3++;
				pIndices[index++] = index1++;
			}
		}


		VertexBuffer* vb;
		IndexedBuffer* ivb;
		VertexBufferManager& manager = VertexBufferManager::getSingleton();


		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indices = pIndices;
		ivbDesc.indicesNum = dwNumIndices;
		ivb = manager.createIndexedBuffer(ivbDesc);

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = pVertices;
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
			pNormals[i].vecPosition = pVertices[i / 2].vecPosition;
			pNormals[i + 1].vecPosition = pVertices[i / 2].vecPosition + pVertices[i / 2].vecNormal * 0.25f;
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
		NGENE_DELETE_ARRAY(pIndices);
		NGENE_DELETE_ARRAY(pVertices);


		calculateBoundingBox();
	}
//----------------------------------------------------------------------
	void PrefabTorus::setInnerRadius(Real _radius)
	{
		m_fInnerRadius = _radius;

		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabTorus::setOuterRadius(Real _radius)
	{
		m_fOuterRadius = _radius;

		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabTorus::setRadialSlices(uint _slices)
	{
		m_nRadialSlices = _slices;

		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabTorus::setCircleSlices(uint _slices)
	{
		m_nCircleSlices = _slices;

		updateGeometry();
	}
//----------------------------------------------------------------------
}