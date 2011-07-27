/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabCylinder.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PrefabCylinder.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo PrefabCylinder::Type(L"PrefabCylinder", &NodePrefab::Type);


	PrefabCylinder::PrefabCylinder(uint _slices, uint _stacks, float _radius,
		float _height, bool _closed, const Vector2& _texCoordMul):
		NodePrefab(_texCoordMul),
		m_fRadius(_radius),
		m_fHeight(_height),
		m_nSlices(_slices),
		m_nStacks(_stacks),
		m_bClosed(_closed)
	{
	}
//----------------------------------------------------------------------
	PrefabCylinder::PrefabCylinder()
	{
	}
//----------------------------------------------------------------------
	PrefabCylinder::~PrefabCylinder()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PrefabCylinder::init()
	{
		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabCylinder::cleanup()
	{
	}
//----------------------------------------------------------------------
	void PrefabCylinder::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			NodePrefab::serialize(_serializer, false, _serializeChildren);

			// Serialize cylinder
			Property <float> prRadius(m_fRadius);
			Property <float> prHeight(m_fHeight);

			_serializer->addProperty("Radius", prRadius);
			_serializer->addProperty("Height", prHeight);

			Property <uint> prSlices(m_nSlices);
			_serializer->addProperty("Slices", prSlices);
			Property <uint> prStacks(m_nStacks);
			_serializer->addProperty("Stacks", prStacks);

			Property <bool> prClosed(m_bClosed);
			_serializer->addProperty("IsClosed", prClosed);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void PrefabCylinder::deserialize(ISerializer* _serializer)
	{
		// Deserialize cylinder
		Property <float> prRadius(m_fRadius);
		Property <float> prHeight(m_fHeight);

		_serializer->getProperty("Radius", prRadius);
		_serializer->getProperty("Height", prHeight);

		Property <uint> prSlices(m_nSlices);
		_serializer->getProperty("Slices", prSlices);
		Property <uint> prStacks(m_nStacks);
		_serializer->getProperty("Stacks", prStacks);

		Property <bool> prClosed(m_bClosed);
		_serializer->getProperty("IsClosed", prClosed);

		NodePrefab::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	void PrefabCylinder::updateGeometry()
	{
		// Create cylinders's geometry
		uint index = 0;

		// Set vertices
		dword dwNumVertices = m_nStacks * (m_nSlices + 1) + (m_bClosed ? 2 : 0);
		SVertex* pVertices = new SVertex[dwNumVertices];
		memset(pVertices, 0, dwNumVertices * sizeof(SVertex));

		float inverseRadial = 1.0f / m_nSlices;
		float inverseAxisLess = 1.0f / (m_bClosed ? m_nStacks - 3 : m_nStacks - 1);
		float inverseAxisLessTexture = 1.0f / (m_nStacks - 1);
		float halfHeight = 0.5f * m_fHeight;

		float* sin = new float[m_nSlices + 1];
		float* cos = new float[m_nSlices + 1];

		for (uint i = 0; i < m_nSlices; ++i) {
			float angle = Maths::PI_DOUBLE * inverseRadial * i;
			cos[i] = Maths::fcos(angle);
			sin[i] = Maths::fsin(angle);
		}
		sin[m_nSlices] = sin[0];
		cos[m_nSlices] = cos[0];


		Vector3 tempNormal;
		for(uint stack = 0; stack < m_nStacks; ++stack)
		{
			float stackFrac;
			float stackFracTex;
			int topBottom = 0;
			if(!m_bClosed)
			{
				stackFrac = stack * inverseAxisLess;
				stackFracTex = stackFrac;
			}
			else
			{
				if(!stack)
				{
					topBottom = -1;
					stackFrac = 0;
					stackFracTex = inverseAxisLessTexture;
				}
				else if(stack == m_nStacks - 1)
				{
					topBottom = 1;
					stackFrac = 1;
					stackFracTex = 1 - inverseAxisLessTexture;
				}
				else
				{
					stackFrac = (stack - 1) * inverseAxisLess;
					stackFracTex = stack * inverseAxisLessTexture;
				}
			}
			float y = -halfHeight + m_fHeight * stackFrac;

			Vector3 centre(0.0f, y, 0.0f);

			int prevIndex = index;
			for(uint slice = 0; slice < m_nSlices; ++slice)
			{
				float radialFrac = slice * inverseRadial;
				tempNormal.set(cos[slice], 0.0f, sin[slice]);

				if(topBottom)
					pVertices[index].vecNormal = Vector3::UNIT_Y * topBottom;
				else
					pVertices[index].vecNormal = tempNormal;

				tempNormal *= m_fRadius;
				tempNormal += centre;

				pVertices[index].vecPosition = tempNormal;
				pVertices[index].vecTexCoord.x = radialFrac * m_vecTexCoordMul.x;
				pVertices[index].vecTexCoord.y = (1.0f - stackFracTex) * m_vecTexCoordMul.y;
				pVertices[index].dwColour = 0xffffffff;

				++index;
			}

			pVertices[index].vecNormal = pVertices[prevIndex].vecNormal;
			pVertices[index].dwColour = 0xffffffff;
			pVertices[index].vecPosition = pVertices[prevIndex].vecPosition;
			pVertices[index].vecTexCoord.x = 1.0f * m_vecTexCoordMul.x;
			pVertices[index].vecTexCoord.y = (1.0f - stackFracTex) * m_vecTexCoordMul.y;

			++index;
		}

		if(m_bClosed)
		{
			pVertices[index].vecPosition = Vector3(0.0f, -halfHeight, 0.0f);
			pVertices[index].vecNormal = Vector3::UNIT_Y;
			pVertices[index].vecTexCoord = Vector2(0.5f, 1.0f);

			++index;

			pVertices[index].vecPosition = Vector3(0.0f, halfHeight, 0.0f);
			pVertices[index].vecNormal = Vector3::UNIT_Y;
			pVertices[index].vecTexCoord = Vector2(0.5f, 0.0f);

			++index;
		}


		// Set indices
		dword dwNumIndices = 3 * (((m_bClosed ? 2 : 0) + 2 * (m_nStacks - 1)) * m_nSlices);
		short* pIndices = new short[dwNumIndices];

		index = 0;
		uint baseIndex = 0;
		for(uint stack = 0; stack < m_nStacks - 1; ++stack)
		{
			uint index0 = baseIndex;
			uint index1 = index0 + 1;
			baseIndex += m_nSlices + 1;
			uint index2 = baseIndex;
			uint index3 = index2 + 1;

			for(uint i = 0; i < m_nSlices; ++i)
			{
				if(m_bClosed && !stack)
				{
					pIndices[index++] = index0++;
					pIndices[index++] = dwNumVertices - 2;
					pIndices[index++] = index1++;
				}
				else if(m_bClosed && stack == m_nStacks - 2)
				{
					pIndices[index++] = index2++;
					pIndices[index++] = index3++;
					pIndices[index++] = dwNumVertices - 1;
				}
				else
				{
					pIndices[index++] = index0++;
					pIndices[index++] = index1;
					pIndices[index++] = index2;
					pIndices[index++] = index1++;
					pIndices[index++] = index3++;
					pIndices[index++] = index2++;
				}
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


		NGENE_DELETE_ARRAY(sin);
		NGENE_DELETE_ARRAY(cos);


		NGENE_DELETE_ARRAY(pNormals);
		NGENE_DELETE_ARRAY(pIndices);
		NGENE_DELETE_ARRAY(pVertices);


		calculateBoundingBox();
	}
//----------------------------------------------------------------------
	void PrefabCylinder::setRadius(Real _radius)
	{
		m_fRadius = _radius;

		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabCylinder::setHeight(Real _radius)
	{
		m_fHeight = _radius;

		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabCylinder::setSlices(uint _slices)
	{
		m_nSlices = _slices;

		updateGeometry();
	}
//----------------------------------------------------------------------
	void PrefabCylinder::setStacks(uint _slices)
	{
		m_nStacks = _slices;

		updateGeometry();
	}
//----------------------------------------------------------------------
}