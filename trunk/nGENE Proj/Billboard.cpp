/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Billboard.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Billboard.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Billboard::Type(L"Billboard", NULL);
	TypeInfo NodeBillboardSet::Type(L"NodeBillboardSet", &NodeVisible::Type);


	Billboard::Billboard()
	{
	}
//----------------------------------------------------------------------
	Billboard::~Billboard()
	{
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	NodeBillboardSet::NodeBillboardSet()
	{
	}
//----------------------------------------------------------------------
	NodeBillboardSet::~NodeBillboardSet()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void NodeBillboardSet::init()
	{
	}
//----------------------------------------------------------------------
	void NodeBillboardSet::cleanup()
	{
	}
//----------------------------------------------------------------------
	void NodeBillboardSet::addBillboard(Billboard _billboard)
	{
		m_vBillboards.push_back(_billboard);
	}
//----------------------------------------------------------------------
	void NodeBillboardSet::validate()
	{
		uint verticesNum = m_vBillboards.size() * 4;
		SVertex* pVertices = new SVertex[verticesNum];
		for(uint i = 0; i < verticesNum; i += 4)
		{
			Billboard& billboard = m_vBillboards[i / 4];
			float fWidth = billboard.m_fWidth * 0.5f;
			float fHeight = billboard.m_fHeight * 0.5f;

			Vector3 vecPosition = billboard.m_vecPosition;
			Vector3 vecNormal = billboard.m_vecNormal;

			pVertices[i + 0].vecPosition = vecPosition;
			pVertices[i + 0].vecNormal = vecNormal;
			pVertices[i + 0].vecTexCoord = Vector2(-fWidth, -fHeight);
			pVertices[i + 0].dwColour = 0xffffffff;
			
			pVertices[i + 1].vecPosition = vecPosition;
			pVertices[i + 1].vecNormal = vecNormal;
			pVertices[i + 1].vecTexCoord = Vector2(fWidth, -fHeight);
			pVertices[i + 1].dwColour = 0xffffffff;
			
			pVertices[i + 2].vecPosition = vecPosition;
			pVertices[i + 2].vecNormal = vecNormal;
			pVertices[i + 2].vecTexCoord = Vector2(-fWidth, fHeight);
			pVertices[i + 2].dwColour = 0xffffffff;

			pVertices[i + 3].vecPosition = vecPosition;
			pVertices[i + 3].vecNormal = vecNormal;
			pVertices[i + 3].vecTexCoord = Vector2(fWidth, fHeight);
			pVertices[i + 3].dwColour = 0xffffffff;
		}


		VertexBufferManager& manager = VertexBufferManager::getSingleton();
		VertexBuffer* vb;
		IndexedBuffer* ivb;

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = pVertices;
		vbDesc.verticesNum = verticesNum;
		vbDesc.primitivesNum = verticesNum / 2;
		vbDesc.primitivesType = PT_TRIANGLELIST;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		vb = manager.createVertexBuffer(vbDesc);

		uint indicesNum = m_vBillboards.size() * 6;
		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indicesNum = indicesNum;
		if(verticesNum > 65536)
		{
			uint* pIndices = new uint[indicesNum];
			uint begin = 0;
			for(uint i = 0; i < indicesNum; i += 6)
			{
				pIndices[i + 0] = begin;
				pIndices[i + 1] = begin + 1;
				pIndices[i + 2] = begin + 3;
				pIndices[i + 3] = begin;
				pIndices[i + 4] = begin + 3;
				pIndices[i + 5] = begin + 2;

				begin += 4;
			}

			ivbDesc.is32bit = true;
			ivbDesc.indices = pIndices;
			ivbDesc.indexSize = 4;

			ivb = manager.createIndexedBuffer(ivbDesc);
			vb->setIndexedBuffer(ivb);

			if(findSurface(L"Base"))
			{
				Surface* temp = getSurface(L"Base");
				temp->setVertexBuffer(vb);
			}
			else
			{
				Surface temp;
				temp.setVertexBuffer(vb);
				addSurface(L"Base", temp);
			}

			calculateBoundingBox();

			NGENE_DELETE_ARRAY(pVertices);
			NGENE_DELETE_ARRAY(pIndices);
		}
		else
		{
			ushort* pIndices = new ushort[indicesNum];
			ushort begin = 0;
			for(uint i = 0; i < indicesNum; i += 6)
			{
				pIndices[i + 0] = begin;
				pIndices[i + 1] = begin + 1;
				pIndices[i + 2] = begin + 3;
				pIndices[i + 3] = begin;
				pIndices[i + 4] = begin + 3;
				pIndices[i + 5] = begin + 2;

				begin += 4;
			}

			ivbDesc.is32bit = false;
			ivbDesc.indices = pIndices;
			ivbDesc.indexSize = 2;

			ivb = manager.createIndexedBuffer(ivbDesc);
			vb->setIndexedBuffer(ivb);

			if(findSurface(L"Base"))
			{
				Surface* temp = getSurface(L"Base");
				temp->setVertexBuffer(vb);
			}
			else
			{
				Surface temp;
				temp.setVertexBuffer(vb);
				addSurface(L"Base", temp);
			}

			calculateBoundingBox();

			NGENE_DELETE_ARRAY(pVertices);
			NGENE_DELETE_ARRAY(pIndices);
		}
	}
//----------------------------------------------------------------------
}