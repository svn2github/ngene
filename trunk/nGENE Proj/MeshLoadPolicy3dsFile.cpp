/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MeshLoadPolicy3dsFile.cpp
Version:	0.13
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FileNarrow.h"
#include "MeshLoadPolicy3dsFile.h"
#include "Point.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "VertexBufferManager.h"
#include "AABB.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo MeshLoadPolicy3dsFile::Type(L"MeshLoadPolicy3dsFile", NULL);


	MeshLoadPolicy3dsFile::MeshLoadPolicy3dsFile():
		m_pFile(NULL),
		m_bMoveToCentre(false)
	{
	}
//----------------------------------------------------------------------
	vector <Surface> MeshLoadPolicy3dsFile::loadMesh(const wstring& _fileName, const Matrix4x4& _transform,
		bool _moveToCenter)
	{
		m_matTransform = _transform;
		m_bMoveToCentre = _moveToCenter;

		vector <Surface> vPolicySurfaces;		// Mesh surfaces


		// Open file
		m_pFile = FileManager::getSingleton().openFile(_fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		if(!m_pFile)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Mesh file: %ls does not exist", _fileName.c_str());
			return vPolicySurfaces;
		}

		SChunk current = processChunk();

		// The first chunk has to be root
		if(current.ID != ROOT)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Root chunk not found! Model cannot be processed in file: %ls", _fileName.c_str());
			return vPolicySurfaces;
		}

		while(!m_pFile->eof())
		{
			current = processChunk();

			switch(current.ID)
			{
			case VERSION:
				dword dwVersion;
				m_pFile->read((char*)&dwVersion, sizeof(dword));
				if(dwVersion > 3)
				{
					Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
						L"Version of the file is greater than 3 (%d). File: %ls may not load properly.",
						dwVersion, _fileName.c_str());
				}
				
				break;
			case OBJECTINFO:
				break;
			case OBJECT:
				processObject(vPolicySurfaces);
				break;
			default:
				unknownChunk(current);
				break;
			}
		}

		FileManager::getSingleton().closeFile(m_pFile->getFileName());


		return vPolicySurfaces;
	}
//----------------------------------------------------------------------
	void MeshLoadPolicy3dsFile::unknownChunk(const SChunk& _chunk)
	{
		Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Unknown or unused .3ds chunk: %#.4X. Skipping.", _chunk.ID);
		m_pFile->seek(_chunk.length - 6, SD_CURRENT);
	}
//----------------------------------------------------------------------
	SChunk MeshLoadPolicy3dsFile::processChunk()
	{
		SChunk chunk;
		m_pFile->read((char*)&chunk.ID, sizeof(ushort));
		m_pFile->read((char*)&chunk.length, sizeof(uint));
		
		return chunk;
	}
//----------------------------------------------------------------------
	void MeshLoadPolicy3dsFile::addSurface(vector <Surface>& _surfaces, const wstring& _name)
	{
		Surface surface;
		surface.setName(_name);
		_surfaces.push_back(surface);
	}
//----------------------------------------------------------------------
	void MeshLoadPolicy3dsFile::processObject(vector <Surface>& _surfaces)
	{
		// Read name
		char cName[255];
		uint i = 0;
        do
        {
			m_pFile->read(&cName[i], 1);
        } while(cName[i++] != '\0');

		string stTemp = cName;
		wstring stName(stTemp.begin(), stTemp.end());

		SChunk current = processChunk();
		
		while(!m_pFile->eof())
		{
			switch(current.ID)
			{
			case MESH:
				addSurface(_surfaces, stName);
				processMesh(_surfaces);
				break;
			case OBJECT:
				processObject(_surfaces);
				break;
			default:
				unknownChunk(current);
				break;
			}

			if(!m_pFile->eof())
				current = processChunk();
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicy3dsFile::processMesh(vector <Surface>& _surfaces)
	{
		SChunk current = processChunk();
		SModel model;
		model.pVertices = NULL;
		model.pIndices = NULL;
		model.nVerticesCount = 0;
		model.nIndicesCount = 0;
		uint index = _surfaces.size() - 1;
		
		while(!m_pFile->eof())
		{
			switch(current.ID)
			{
			case VERTICES: processVertices(&model);
				break;
			case FACES: processIndices(&model);
				break;
			case UV: processTexCoords(&model);
				break;
			case OBJECT:
				processObject(_surfaces);
				break;
			default:
				unknownChunk(current);
				break;
			}

			if(!m_pFile->eof())
				current = processChunk();
		}

		computeNormals(&model);
		createMesh(_surfaces, _surfaces[index], &model);
	}
//----------------------------------------------------------------------
	void MeshLoadPolicy3dsFile::processVertices(SModel* _model)
	{
		m_pFile->read((char*)&_model->nVerticesCount, sizeof(ushort));
		_model->pVertices = new SVertex[_model->nVerticesCount];
        for(uint i = 0; i < _model->nVerticesCount; ++i)
		{
			m_pFile->read((char*)&_model->pVertices[i].vecPosition, sizeof(Vector3));
			
			if(!m_matTransform.isIdentity())
			{
				Vector4 vecPos(_model->pVertices[i].vecPosition, 0.0f);
				vecPos.multiply(m_matTransform);
				_model->pVertices[i].vecPosition.set(vecPos.x, vecPos.y, vecPos.z);
			}
		}

		if(m_bMoveToCentre)
		{
			AABB box;
			box.construct(_model->pVertices, _model->nVerticesCount, sizeof(SVertex), 0);
			Vector3 vecCentre = box.getCentre();
			for(uint i = 0; i < _model->nVerticesCount; ++i)
			{
				_model->pVertices[i].vecPosition -= vecCentre;
			}
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicy3dsFile::processIndices(SModel* _model)
	{
		m_pFile->read((char*)&_model->nIndicesCount, sizeof(ushort));
		_model->nIndicesCount *= 3;
		_model->pIndices = new ushort[_model->nIndicesCount];
		ushort sFlags = 0;
        for(uint i = 0; i < _model->nIndicesCount; i += 3)
        {
			m_pFile->read((char*)&_model->pIndices[i + 2], sizeof(ushort));
			m_pFile->read((char*)&_model->pIndices[i + 1], sizeof(ushort));
			m_pFile->read((char*)&_model->pIndices[i + 0], sizeof(ushort));
			m_pFile->read((char*)&sFlags, sizeof(ushort));
        }
	}
//----------------------------------------------------------------------
	void MeshLoadPolicy3dsFile::computeNormals(SModel* _model)
	{
		Point p0, p1, p2;
		Vector3 v1, v2;
		Vector3 vecNormal;
		Vector3* tempNormals = new Vector3[_model->nIndicesCount / 3];
		for(uint i = 0; i < _model->nIndicesCount; i += 3)
		{
			p0 = _model->pVertices[_model->pIndices[i + 0]].vecPosition;
			p1 = _model->pVertices[_model->pIndices[i + 1]].vecPosition;
			p2 = _model->pVertices[_model->pIndices[i + 2]].vecPosition;

			v1 = p0 - p2;
			v2 = p1 - p2;
			vecNormal = v1.crossProduct(v2);
			tempNormals[i / 3] = vecNormal;
		}

		vecNormal = Vector3::ZERO_VECTOR;
		uint shared = 0;
		
		for(uint i = 0; i < _model->nVerticesCount; ++i)
		{
			for (uint j = 0; j < _model->nIndicesCount; ++j)	
			{												
				if(_model->pIndices[j] == i ||
				   _model->pIndices[j + 1] == i ||
				   _model->pIndices[j + 2] == i)
				{
					vecNormal += tempNormals[j / 3];
					++shared;	
				}
			}      
			
			_model->pVertices[i].vecNormal = vecNormal / -Real(shared);
			_model->pVertices[i].vecNormal.normalize();
			
			vecNormal = Vector3::ZERO_VECTOR;	 
			shared = 0;
		}

		NGENE_DELETE_ARRAY(tempNormals);
	}
//----------------------------------------------------------------------
	void MeshLoadPolicy3dsFile::processTexCoords(SModel* _model)
	{
		uint nCount = 0;
		m_pFile->read((char*)&nCount, sizeof(ushort));
        for(uint i = 0; i < nCount; ++i)
		{
			m_pFile->read((char*)&_model->pVertices[i].vecTexCoord, sizeof(Vector2));
			_model->pVertices[i].vecTexCoord.y = 1.0f - _model->pVertices[i].vecTexCoord.y;
		}
	}
//----------------------------------------------------------------------
	void MeshLoadPolicy3dsFile::createMesh(vector <Surface>& _surfaces, Surface& _surface,
		SModel* _model)
	{
		if(!_model->nVerticesCount)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Invalid mesh file. Vertex count is 0");
			return;
		}

		IndexedBuffer* ivb;
		VertexBuffer* vb;

		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = _model->pVertices;
		vbDesc.verticesNum = _model->nVerticesCount;
		vbDesc.primitivesNum = _model->nIndicesCount / 3;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		vb = manager.createVertexBuffer(vbDesc);

		_surface.setVertexBuffer(vb);
		if(_model->nIndicesCount)
		{
			INDEXEDBUFFER_DESC ivbDesc;
			ivbDesc.indices = _model->pIndices;
			ivbDesc.indicesNum = _model->nIndicesCount;
			ivb = manager.createIndexedBuffer(ivbDesc);
			vb->setIndexedBuffer(ivb);

			NGENE_DELETE_ARRAY(_model->pIndices);
		}


		SVertex* pNormals = new SVertex[_model->nVerticesCount * 2];
		for(uint i = 0; i < _model->nVerticesCount * 2; i += 2)
		{
			pNormals[i].vecPosition = _model->pVertices[i / 2].vecPosition;
			pNormals[i + 1].vecPosition = _model->pVertices[i / 2].vecPosition + _model->pVertices[i / 2].vecNormal * 0.25f;
		}

		VERTEXBUFFER_DESC vbDescNormals;
		vbDescNormals.vertices = pNormals;
		vbDescNormals.verticesNum = _model->nVerticesCount * 2;
		vbDescNormals.primitivesNum = _model->nVerticesCount;
		vbDescNormals.primitivesType = PT_LINELIST;
		vbDescNormals.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		VertexBuffer* vbNormals = manager.createVertexBuffer(vbDescNormals);

		Surface temp;
		temp.setVertexBuffer(vbNormals);
		temp.setName(L"Normals_" + _surface.getName());
		temp.setPickable(false);
		_surfaces.push_back(temp);
		NGENE_DELETE_ARRAY(pNormals);


		NGENE_DELETE_ARRAY(_model->pVertices);
	}
//----------------------------------------------------------------------
}
