/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MeshLoadPolicyXFile.cpp
Version:	0.10
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "MeshLoadPolicyXFile.h"
#include "DeviceRender.h"
#include "DX9Mapping.h"
#include "Surface.h"
#include "Vertex.h"
#include "VertexBufferManager.h"
#include "VertexDeclaration.h"
#include "AABB.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo MeshLoadPolicyXFile::Type(L"MeshLoadPolicyXFile", NULL);


	MeshLoadPolicyXFile::MeshLoadPolicyXFile()
	{
	}
//----------------------------------------------------------------------
	vector <Surface> MeshLoadPolicyXFile::loadMesh(const wstring& _fileName, const Matrix4x4& _transform,
		bool _moveToCentre)
	{
		m_matTransform = _transform;
		m_bMoveToCentre = _moveToCentre;

		vector <Surface> vPolicySurfaces;		// Mesh surfaces

		ID3DXMesh* pStartMesh = NULL;
		ID3DXMesh* pMesh = NULL;

		dword dwMaterialNum = 0;
		HRESULT hr = 0;
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		// Open file
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(_fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint size = pFile->getSize();
		void* buffer = new char[size];
		pFile->read((char*)buffer, size);
		mgr.closeFile(_fileName);

		// Load ID3DXMesh
		if(FAILED(hr = D3DXLoadMeshFromXInMemory(buffer,
												 size,
												 D3DXMESH_MANAGED,
												 pDevice,
												 NULL,
												 NULL,
												 NULL,
												 &dwMaterialNum,
												 &pStartMesh)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"%ls .x file loading failed. Reason: %ls", _fileName.c_str(),
				DX9Mapping::getDXErrorDescription(hr).c_str());
			NGENE_DELETE_ARRAY(buffer);

			return vPolicySurfaces;
		}

		NGENE_DELETE_ARRAY(buffer);


		// Clone mesh if necessary
		pMesh = cloneMesh(pStartMesh, pDevice);

		// Optimize mesh
		optimizeMesh(pMesh);

		// Get attributes table
		dword dwAttributesNum = 0;
		D3DXATTRIBUTERANGE* pAttrTable = NULL;
		getAttributesTable(pMesh, dwAttributesNum, &pAttrTable);

		// Process attributes table and mesh itself
		processMesh(pMesh, dwAttributesNum, pAttrTable, vPolicySurfaces);	
		

		NGENE_RELEASE(pMesh);


		return vPolicySurfaces;
	}
//----------------------------------------------------------------------
	ID3DXMesh* MeshLoadPolicyXFile::cloneMesh(ID3DXMesh* _mesh, IDirect3DDevice9* _device)
	{
		VertexDeclaration* pDecl = VertexBufferManager::getSingleton().getVertexDeclaration(L"Default");
		D3DVERTEXELEMENT9 elements[MAX_FVF_DECL_SIZE];
		_mesh->GetDeclaration(elements);
		uint count = 0;

		while(elements[count++].Stream != 255);

		bool equal = true;
		if(--count == pDecl->getCount())
		{
			for(uint i = 0; i < pDecl->getCount(); ++i)
			{
				D3DVERTEXELEMENT9 element = DX9Mapping::svertexElementToD3DVERTEXELEMENT9(*pDecl->getVertexElement(i));
				if((elements[i].Method != element.Method) ||
				   (elements[i].Offset != element.Offset) ||
				   (elements[i].Stream != element.Stream) ||
				   (elements[i].Type != element.Type) ||
				   (elements[i].Usage != element.Usage) ||
				   (elements[i].UsageIndex != (elements[i].UsageIndex)))
				{
					equal = false;
					break;
				}
			}
		}
		else
			equal = false;

		ID3DXMesh* pMesh = NULL;
		if(!equal)
		{
			D3DVERTEXELEMENT9* pElements = new D3DVERTEXELEMENT9[pDecl->getCount() + 1];
			for(uint i = 0; i < pDecl->getCount(); ++i)
				pElements[i] = DX9Mapping::svertexElementToD3DVERTEXELEMENT9(*pDecl->getVertexElement(i));
			pElements[pDecl->getCount()].Method = 0;
			pElements[pDecl->getCount()].Offset = 0;
			pElements[pDecl->getCount()].Stream = 0xFF;
			pElements[pDecl->getCount()].Type   = D3DDECLTYPE_UNUSED;
			pElements[pDecl->getCount()].Usage  = 0;
			pElements[pDecl->getCount()].UsageIndex = 0;

			dword flags = D3DXMESH_MANAGED;
			if(_mesh->GetOptions() & D3DXMESH_32BIT)
				flags |= D3DXMESH_32BIT;
			HRESULT hr = _mesh->CloneMesh(flags, pElements, _device, &pMesh);
			if(hr != S_OK)
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Mesh format doesn't match. Reason: %ls",
					DX9Mapping::getDXErrorDescription(hr).c_str());
				return NULL;
			}
			NGENE_RELEASE(_mesh);
		}
		else
			pMesh = _mesh;

		return pMesh;
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyXFile::optimizeMesh(ID3DXMesh* _mesh)
	{
		ID3DXMesh* pMesh = _mesh;
		dword* pAdj = new dword[3 * pMesh->GetNumFaces() * sizeof(dword)];
		HRESULT hr = 0;

		// Generate adjacency information
		if(FAILED(hr = pMesh->GenerateAdjacency(0.00001f, pAdj)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Adjacency information could not be generated. Reason: %ls", DX9Mapping::getDXErrorDescription(hr));
			return;
		}

		// Optimize in-place
		if(FAILED(hr = pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
											  pAdj,
											  NULL,
											  NULL,
											  NULL)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Mesh could not be optimized. Reason: %ls", DX9Mapping::getDXErrorDescription(hr));
			return;
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyXFile::getAttributesTable(ID3DXMesh* _mesh, dword& _numAttrs, D3DXATTRIBUTERANGE** _table)
	{
		ID3DXMesh* pMesh = _mesh;
		HRESULT hr = 0;

		if(FAILED(hr = pMesh->GetAttributeTable(NULL, &_numAttrs)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Attribute table data could not be obtained. Reason: %ls",
				DX9Mapping::getDXErrorDescription(hr));
			return;
		}

		(*_table) = new D3DXATTRIBUTERANGE[_numAttrs * sizeof(D3DXATTRIBUTERANGE)];
		if(FAILED(hr = pMesh->GetAttributeTable((*_table), NULL)))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Attribute table data could not be obtained. Reason: %ls",
				DX9Mapping::getDXErrorDescription(hr));
			return;
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicyXFile::processMesh(ID3DXMesh* _mesh, dword _numAttributes, D3DXATTRIBUTERANGE* _table,
		vector <Surface>& _surfaces)
	{
		ID3DXMesh* pMesh = _mesh;

		IDirect3DVertexBuffer9* pVB = NULL;
		IDirect3DIndexBuffer9* pIVB = NULL;

		SVertex* pVertices = NULL;
		ushort* pIndices = NULL;


		pMesh->GetVertexBuffer(&pVB);
		pMesh->GetIndexBuffer(&pIVB);


		// Lock the buffers
		dword dwIndexSize = sizeof(ushort);
		if(pMesh->GetOptions() & D3DXMESH_32BIT)
			dwIndexSize = sizeof(dword);

		pVB->Lock(0, pMesh->GetNumVertices() * sizeof(SVertex), reinterpret_cast<void**>(&pVertices), 0);
		pIVB->Lock(0, pMesh->GetNumFaces() * 3 * dwIndexSize, reinterpret_cast<void**>(&pIndices), 0);


		// Process attributes
		for(uint i = 0; i < _numAttributes; ++i)
		{
			SVertex* pVertices2 = new SVertex[_table[i].VertexCount];
			memcpy(pVertices2, pVertices + _table[i].VertexStart, _table[i].VertexCount * sizeof(SVertex));

			void* pTemp = NULL;
			if(pMesh->GetOptions() & D3DXMESH_32BIT)
			{
				dword* pIndices2 = new dword[3 * _table[i].FaceCount];
				pTemp = new dword[3 * _table[i].FaceCount];
				memcpy(pIndices2, pIndices + 3 * _table[i].FaceStart, 3 * _table[i].FaceCount * dwIndexSize);

				for(dword j = 0; j < _table[i].FaceCount; ++j)
					pIndices2[j] -= _table[i].VertexStart;
				memcpy(pTemp, pIndices2, 3 * _table[i].FaceCount * dwIndexSize);

				NGENE_DELETE_ARRAY(pIndices2);
			}
			else
			{
				ushort* pIndices2 = new ushort[3 * _table[i].FaceCount];
				pTemp = new dword[3 * _table[i].FaceCount];
				memcpy(pIndices2, pIndices + 3 * _table[i].FaceStart, 3 * _table[i].FaceCount * dwIndexSize);

				for(dword j = 0; j < 3 * _table[i].FaceCount; ++j)
					pIndices2[j] -= _table[i].VertexStart;
				memcpy(pTemp, pIndices2, 3 * _table[i].FaceCount * dwIndexSize);

				NGENE_DELETE_ARRAY(pIndices2);
			}


			if(!m_matTransform.isIdentity())
			{
				for(uint j = 0; j < _table[i].VertexCount; ++j)
				{	
					Vector4 vecPos(pVertices2[j].vecPosition, 0.0f);
					vecPos.multiply(m_matTransform);
					pVertices2[j].vecPosition.set(vecPos.x, vecPos.y, vecPos.z);
				}
			}

			if(m_bMoveToCentre)
			{
				AABB box;
				box.construct(pVertices2, _table[i].VertexCount, sizeof(SVertex), 0);
				Vector3 vecCentre = box.getCentre();
				for(uint j = 0; j < _table[i].VertexCount; ++j)
				{
					pVertices2[j].vecPosition -= vecCentre;
				}
			}

			VertexBufferManager& manager = VertexBufferManager::getSingleton();
			VertexBuffer* vb;
			IndexedBuffer* ivb;

			VERTEXBUFFER_DESC vbDesc;
			vbDesc.vertices = pVertices2;
			vbDesc.verticesNum = _table[i].VertexCount;
			vbDesc.primitivesNum = _table[i].FaceCount;
			vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
			vb = manager.createVertexBuffer(vbDesc);

			INDEXEDBUFFER_DESC ivbDesc;
			ivbDesc.indices = pTemp;
			ivbDesc.indicesNum = 3 * _table[i].FaceCount;
			ivb = manager.createIndexedBuffer(ivbDesc);

			vb->setIndexedBuffer(ivb);

			wostringstream buffer;
			buffer << L"surface_" << i;

			Surface surface(buffer.str());
			surface.setVertexBuffer(vb);
			_surfaces.push_back(surface);


			NGENE_DELETE_ARRAY(pVertices2);
			NGENE_DELETE_ARRAY(pTemp);
		}


		pVB->Unlock();
		pIVB->Unlock();


		NGENE_RELEASE(pIVB);
		NGENE_RELEASE(pVB);
	}
//----------------------------------------------------------------------
}
