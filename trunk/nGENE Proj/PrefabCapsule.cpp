/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabCapsule.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PrefabCapsule.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo PrefabCapsule::Type(L"PrefabCapsule", &NodePrefab::Type);


	PrefabCapsule::PrefabCapsule(uint _slices, uint _stacks, float _height, float _radius,
		const Vector2& _texCoordMul):
		NodePrefab(_texCoordMul),
		m_fHeight(_height),
		m_fRadius(_radius),
		m_nSlices(_slices),
		m_nStacks(_stacks)
	{
	}
//----------------------------------------------------------------------
	PrefabCapsule::PrefabCapsule()
	{
	}
//----------------------------------------------------------------------
	PrefabCapsule::~PrefabCapsule()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PrefabCapsule::init()
	{
		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabCapsule::updateGeometry()
	{
		// Create capsule's geometry
		uint index = 0;

		// Set vertices
		dword dwNumVertices = 2 * m_nSlices + (m_nStacks + 1) * m_nSlices * 2;
		SVertex* pVertices = new SVertex[dwNumVertices];
		memset(pVertices, 0, dwNumVertices * sizeof(SVertex));

		float y = m_fRadius;
		for(uint i = 0; i < 2; ++i)
		{
			for(uint sliceNumber = 0; sliceNumber < m_nSlices; ++sliceNumber)
			{
				index = i * m_nSlices + sliceNumber;
				float phi = sliceNumber * Maths::PI_DOUBLE / (m_nSlices - 1);
				float sinPhi = Maths::fsin(phi);
				float cosPhi = Maths::fcos(phi);

				if(sliceNumber == (m_nSlices - 1))
					pVertices[index].vecPosition = pVertices[i * m_nSlices].vecPosition;
				else
				{
					pVertices[index].vecPosition = Vector3(cosPhi * m_fRadius, y, sinPhi * m_fRadius);
				}

				pVertices[index].vecNormal = pVertices[index].vecPosition;
				pVertices[index].vecNormal.y = 0.0f;
				pVertices[index].vecNormal.normalize();
				pVertices[index].dwColour = 0xffffffff;
				pVertices[index].vecTexCoord = Vector2(m_vecTexCoordMul.x * ((float)sliceNumber) / (float)(m_nSlices - 1),
					m_vecTexCoordMul.y * (1.0f - ((float)i) * 0.5f));
			}

			y += m_fHeight;
		}

		// Set indices
		vector <short> indices;
		uint begin = 0;
		for(uint i = 0; i < 2; ++i)
		{
			begin = i * m_nSlices;
			for(uint sliceNumber = 0; sliceNumber < m_nSlices - 1; ++sliceNumber)
			{
				indices.push_back(begin + sliceNumber);
				indices.push_back(begin + sliceNumber + 1);
				indices.push_back(begin + sliceNumber + m_nSlices);				

				indices.push_back(begin + sliceNumber + 1);
				indices.push_back(begin + sliceNumber + m_nSlices + 1);
				indices.push_back(begin + sliceNumber + m_nSlices);
			}
		}


		// Set vertices
		uint baseIndex = 2 * m_nSlices;
		float m_fPhi = Maths::PI_HALF;

		for(uint stackNumber = 0; stackNumber <= m_nStacks; ++stackNumber)
		{
			float theta = stackNumber * m_fPhi / m_nStacks;
			float sinTheta = Maths::fsin(theta);
			float cosTheta = Maths::fcos(theta);

			for(uint sliceNumber = 0; sliceNumber < m_nSlices; ++sliceNumber)
			{
				index = stackNumber * m_nSlices + sliceNumber + baseIndex;
				float phi = sliceNumber * Maths::PI_DOUBLE / (m_nSlices - 1);
				float sinPhi = Maths::fsin(phi);
				float cosPhi = Maths::fcos(phi);

				if(sliceNumber == (m_nSlices - 1))
					pVertices[index].vecPosition = pVertices[stackNumber * m_nSlices + baseIndex].vecPosition;
				else
				{
					pVertices[index].vecPosition = Vector3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
					pVertices[index].vecPosition *= m_fRadius;
					pVertices[index].vecPosition.y += m_fHeight + m_fRadius;
				}

				pVertices[index].vecNormal = pVertices[index].vecPosition - Vector3(0.0f, m_fHeight, 0.0f);
				pVertices[index].vecNormal.normalize();
				pVertices[index].dwColour = 0xffffffff;
				pVertices[index].vecTexCoord = Vector2(m_vecTexCoordMul.x * ((float)sliceNumber) / (float)(m_nSlices - 1),
					m_vecTexCoordMul.y * (0.5f * stackNumber) / ((float)m_nStacks));
			}
		}

		// Set indices
		for(uint stackNumber = 0; stackNumber < m_nStacks; ++stackNumber)
		{
			begin = stackNumber * m_nSlices + baseIndex;
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


		// Set vertices
		baseIndex = 2 * m_nSlices + (m_nStacks + 1) * m_nSlices;
		for(uint stackNumber = 0; stackNumber <= m_nStacks; ++stackNumber)
		{
			float theta = stackNumber * m_fPhi / m_nStacks + Maths::PI_HALF;
			float sinTheta = Maths::fsin(theta);
			float cosTheta = Maths::fcos(theta);

			for(uint sliceNumber = 0; sliceNumber < m_nSlices; ++sliceNumber)
			{
				index = stackNumber * m_nSlices + sliceNumber + baseIndex;
				float phi = sliceNumber * Maths::PI_DOUBLE / (m_nSlices - 1);
				float sinPhi = Maths::fsin(phi);
				float cosPhi = Maths::fcos(phi);

				if(sliceNumber == (m_nSlices - 1))
					pVertices[index].vecPosition = pVertices[stackNumber * m_nSlices + baseIndex].vecPosition;
				else
				{
					pVertices[index].vecPosition = Vector3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
					pVertices[index].vecPosition *= m_fRadius;
					pVertices[index].vecPosition.y += m_fRadius;
				}

				pVertices[index].vecNormal = pVertices[index].vecPosition - Vector3(0.0f, m_fRadius, 0.0f);
				pVertices[index].vecNormal.normalize();
				pVertices[index].dwColour = 0xffffffff;
				pVertices[index].vecTexCoord = Vector2(m_vecTexCoordMul.x * ((float)sliceNumber) / (float)(m_nSlices - 1),
					m_vecTexCoordMul.y * (0.5f * stackNumber) / ((float)m_nStacks));
			}
		}

		// Set indices
		for(uint stackNumber = 0; stackNumber < m_nStacks; ++stackNumber)
		{
			begin = stackNumber * m_nSlices + baseIndex;
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
	void PrefabCapsule::cleanup()
	{
	}
//----------------------------------------------------------------------
	void PrefabCapsule::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			NodePrefab::serialize(_serializer, false, _serializeChildren);

			// Serialize capsule
			Property <float> prHeight(m_fHeight);
			Property <float> prRadius(m_fRadius);

			_serializer->addProperty("Height", prHeight);
			_serializer->addProperty("Radius", prRadius);

			Property <uint> prSlices(m_nSlices);
			_serializer->addProperty("Slices", prSlices);
			Property <uint> prStacks(m_nStacks);
			_serializer->addProperty("Stacks", prStacks);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void PrefabCapsule::deserialize(ISerializer* _serializer)
	{
		// Deserialize capsule
		Property <float> prHeight(m_fHeight);
		Property <float> prRadius(m_fRadius);

		_serializer->getProperty("Height", prHeight);
		_serializer->getProperty("Radius", prRadius);

		Property <uint> prSlices(m_nSlices);
		_serializer->getProperty("Slices", prSlices);
		Property <uint> prStacks(m_nStacks);
		_serializer->getProperty("Stacks", prStacks);

		NodePrefab::deserialize(_serializer);
	}
//----------------------------------------------------------------------
}