/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabSphere.cpp
Version:	0.12
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PrefabSphere.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo PrefabSphere::Type(L"PrefabSphere", &NodePrefab::Type);


	PrefabSphere::PrefabSphere(uint _slices, uint _stacks, Real _radius,
		const Vector2& _texCoordMul, Real _maxPhi):
		NodePrefab(_texCoordMul),
		m_nSlices(_slices),
		m_nStacks(_stacks),
		m_fRadius(_radius),
		m_fPhi(_maxPhi)
	{
	}
//----------------------------------------------------------------------
	PrefabSphere::~PrefabSphere()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PrefabSphere::init()
	{
		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabSphere::updateGeometry()
	{
		// Create sphere's geometry
		uint index = 0;

		// Set vertices
		dword dwNumVertices = (m_nStacks + 1) * m_nSlices;
		SVertex* pVertices = new SVertex[dwNumVertices];
		memset(pVertices, 0, dwNumVertices * sizeof(SVertex));

		for(uint stackNumber = 0; stackNumber <= m_nStacks; ++stackNumber)
		{
			float theta = stackNumber * m_fPhi / m_nStacks;
			float sinTheta = Maths::fsin(theta);
			float cosTheta = Maths::fcos(theta);

			for(uint sliceNumber = 0; sliceNumber < m_nSlices; ++sliceNumber)
			{
				index = stackNumber * m_nSlices + sliceNumber;
				float phi = sliceNumber * Maths::PI_DOUBLE / (m_nSlices - 1);
				float sinPhi = Maths::fsin(phi);
				float cosPhi = Maths::fcos(phi);

				if(sliceNumber == (m_nSlices - 1))
					pVertices[index].vecPosition = pVertices[stackNumber * m_nSlices].vecPosition;
				else
				{
					pVertices[index].vecPosition = Vector3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
					pVertices[index].vecPosition *= m_fRadius;
				}

				pVertices[index].vecNormal = pVertices[index].vecPosition;
				pVertices[index].vecNormal.normalize();
				pVertices[index].dwColour = 0xffffffff;
				pVertices[index].vecTexCoord = Vector2(m_vecTexCoordMul.x * ((float)sliceNumber) / (float)(m_nSlices - 1),
					m_vecTexCoordMul.y * ((float)stackNumber) / ((float)m_nStacks));
			}
		}

		// Set indices
		vector <short> indices;
		uint begin = 0;
		for(uint stackNumber = 0; stackNumber < m_nStacks; ++stackNumber)
		{
			begin = stackNumber * m_nSlices;
			for(uint sliceNumber = 0; sliceNumber < m_nSlices - 1; ++sliceNumber)
			{
				indices.push_back(begin + sliceNumber + m_nSlices);
				indices.push_back(begin + sliceNumber + 1);
				indices.push_back(begin + sliceNumber);				

				indices.push_back(begin + sliceNumber + m_nSlices);
				indices.push_back(begin + sliceNumber + m_nSlices + 1);
				indices.push_back(begin + sliceNumber + 1);
			}
		}

		/// Create normals surface
		SVertex* pNormals = new SVertex[dwNumVertices * 2];
		for(uint i = 0; i < dwNumVertices * 2; i += 2)
		{
			pNormals[i].vecPosition = pVertices[i / 2].vecPosition;
			pNormals[i + 1].vecPosition = pVertices[i / 2].vecPosition + pVertices[i / 2].vecNormal * 0.25f;
			pNormals[i].dwColour = pNormals[i + 1].dwColour = 0xffffffff;
			pNormals[i].vecTexCoord = pNormals[i + 1].vecTexCoord = Vector2::ZERO_VECTOR;
			pNormals[i].vecNormal = pNormals[i + 1].vecNormal = Vector3::UNIT_Y;
		}

		VertexBuffer* vb;
		IndexedBuffer* ivb;
		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		short* pIndices = new short[indices.size()];
		for(uint i = 0; i < indices.size(); ++i)
			pIndices[i] = indices[i];
		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indices = pIndices;
		ivbDesc.indicesNum = indices.size();
		ivb = manager.createIndexedBuffer(ivbDesc);

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = pVertices;
		vbDesc.verticesNum = dwNumVertices;
		vbDesc.primitivesNum = indices.size() / 3;
		vbDesc.primitivesType = PT_TRIANGLELIST;
		vbDesc.indices = ivb;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		vb = manager.createVertexBuffer(vbDesc);


		updateSurface(L"Base", vb);


		// Create surface for normals
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
	void PrefabSphere::cleanup()
	{
	}
//----------------------------------------------------------------------
	void PrefabSphere::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			NodePrefab::serialize(_serializer, false, _serializeChildren);
			
			Property <uint> prSlices(m_nSlices);
			_serializer->addProperty("Slices", prSlices);
			Property <uint> prStacks(m_nStacks);
			_serializer->addProperty("Stacks", prStacks);

			Property <float> prRadius(m_fRadius);
			_serializer->addProperty("Radius", prRadius);
			Property <float> prPhi(m_fPhi);
			_serializer->addProperty("Phi", prPhi);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void PrefabSphere::deserialize(ISerializer* _serializer)
	{
		Property <uint> prSlices(m_nSlices);
		_serializer->getProperty("Slices", prSlices);
		Property <uint> prStacks(m_nStacks);
		_serializer->getProperty("Stacks", prStacks);

		Property <float> prRadius(m_fRadius);
		_serializer->getProperty("Radius", prRadius);
		Property <float> prPhi(m_fPhi);
		_serializer->getProperty("Phi", prPhi);

		NodePrefab::deserialize(_serializer);
	}
//----------------------------------------------------------------------
}