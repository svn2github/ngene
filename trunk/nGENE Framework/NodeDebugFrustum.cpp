/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeDebugFrustum.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "NodeDebugFrustum.h"
#include "Frustum.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "VertexBufferManager.h"


namespace nGENE
{
namespace Debug
{
	// Initialize static members
	TypeInfo NodeDebugFrustum::Type(L"NodeDebugFrustum", &NodeVisible::Type);


	NodeDebugFrustum::NodeDebugFrustum(Frustum* _frustum):
		m_pFrustum(_frustum),
		m_pVB(NULL)
	{
	}
//----------------------------------------------------------------------
	NodeDebugFrustum::~NodeDebugFrustum()
	{
	}
//----------------------------------------------------------------------
	void NodeDebugFrustum::init()
	{
		setSerializable(false);

		// Retrieve points data
		Vector4 points[8];
		m_pFrustum->getFrustumPointsLocal(points);

		// Create curve
		dword dwNumVertices = 8;
		SVertexNonLightable pVertices[8];
		memset(pVertices, 0, dwNumVertices * sizeof(SVertexNonLightable));

		for(uint i = 0; i < dwNumVertices; ++i)
		{
			pVertices[i].dwColour = 0xffff0000;
			pVertices[i].vecPosition = Vector3(points[i].x,
											   points[i].y,
											   points[i].z);
		}

		short indices[24] = {0, 1, 1, 2, 2, 3,
							 0, 3, 4, 5, 5, 6,
							 6, 7, 4, 7, 0, 4,
							 1, 5, 2, 6, 3, 7};

		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = pVertices;
		vbDesc.verticesNum = dwNumVertices;
		vbDesc.primitivesNum = 12;
		vbDesc.primitivesType = PT_LINELIST;
		vbDesc.type = VB_DYNAMIC;
		vbDesc.vertexSize = sizeof(SVertexNonLightable);
		vbDesc.discardData = false;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"NonLightable");
		
		m_pVB = manager.createVertexBuffer(vbDesc);

		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indices = indices;
		ivbDesc.indicesNum = 24;
		IndexedBuffer* ivb = manager.createIndexedBuffer(ivbDesc);

		m_pVB->setIndexedBuffer(ivb);

		// Create surface
		Surface temp;
		temp.setVertexBuffer(m_pVB);
		temp.setMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"debug_point"));
		addSurface(L"Base", temp);

		calculateBoundingBox();
	}
//----------------------------------------------------------------------
	void NodeDebugFrustum::onUpdate()
	{
		NodeVisible::onUpdate();

		updateFrustumGeometry();
	}
//----------------------------------------------------------------------
	void NodeDebugFrustum::updateFrustumGeometry()
	{
		// Retrieve points data
		Vector4 points[8];
		m_pFrustum->getFrustumPointsLocal(points);

		Matrix4x4 matView;
		matView.lookAt(Vector3::ZERO_VECTOR,
					   m_pFrustum->getForward(),
					   m_pFrustum->getUp());

		matView.invert();
		matView.transpose();

		SVertexNonLightable* pVertices;

		m_pVB->lock(0, 8, (void**)&pVertices);

			for(uint i = 0; i < 8; ++i)
			{
				Vector4 temp = matView.multiply(points[i]);
				pVertices[i].vecPosition = Vector3(temp.x,
												   temp.y,
												   temp.z);
			}

		m_pVB->unlock();

		calculateBoundingBox();
	}
//----------------------------------------------------------------------
}
}