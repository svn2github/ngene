/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeTerrain.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "NodeTerrain.h"
#include "AABB.h"
#include "Camera.h"
#include "FuncSplit.h"
#include "Surface.h"
#include "TerrainTile.h"
#include "VertexBufferManager.h"

#include <sstream>


namespace nGENE
{
	// Initialize static members
	TypeInfo NodeTerrain::Type(L"NodeTerrain", &NodeVisible::Type);


	NodeTerrain::NodeTerrain()
	{
	}
//----------------------------------------------------------------------
	NodeTerrain::NodeTerrain(const TERRAIN_DESC& _desc):
		m_TerrainDesc(_desc)
	{
	}
//----------------------------------------------------------------------
	NodeTerrain::~NodeTerrain()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void NodeTerrain::init()
	{
		float* pHeights = m_TerrainDesc.heights;

		const dword vertsNum = m_TerrainDesc.columns * m_TerrainDesc.rows;

		SVertex* pVertices = new SVertex[vertsNum];

		uint counter = 0;
		float fHeight = 0.0f;
		for(uint z = 0; z < m_TerrainDesc.rows; ++z)
		{
			for(uint x = 0; x < m_TerrainDesc.columns; ++x)
			{
				fHeight = pHeights[counter++];
				pVertices[z * m_TerrainDesc.columns + x].dwColour = 0xffffffff;
				pVertices[z * m_TerrainDesc.columns + x].vecPosition =
					Vector3((float)x * m_TerrainDesc.step,
							fHeight * m_TerrainDesc.heightScale,
							(float)z * m_TerrainDesc.step);
				pVertices[z * m_TerrainDesc.columns + x].vecTexCoord = Vector2((float)x / m_TerrainDesc.columns, (float)z / m_TerrainDesc.rows);

				m_TerrainDesc.heights[z * m_TerrainDesc.columns + x] = fHeight;
			}
		}

		// Compute normals
		computeNormals(pVertices);

		// Create indices buffers
		if(!m_TerrainDesc.LODs.size())
			m_TerrainDesc.LODs.push_back(make_pair(1, (float)1e9));
		for(uint i = 0; i < m_TerrainDesc.LODs.size(); ++i)
			createIndicesBuffer(m_TerrainDesc.LODs[i].first);


		// Create tiles
		createTiles(pVertices);


		SVertex* pNormals = new SVertex[vertsNum * 2];
		for(uint i = 0; i < vertsNum * 2; i += 2)
		{
			pNormals[i].vecPosition = pVertices[i / 2].vecPosition;
			pNormals[i + 1].vecPosition = pVertices[i / 2].vecPosition + pVertices[i / 2].vecNormal * 0.25f;
		}

		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.vertices = pNormals;
		vbDesc.verticesNum = vertsNum * 2;
		vbDesc.primitivesNum = vertsNum;
		vbDesc.primitivesType = PT_LINELIST;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		VertexBuffer* vbNormals = manager.createVertexBuffer(vbDesc);

		Surface temp;
		temp.setVertexBuffer(vbNormals);
		addSurface(L"Normals", temp);

		
		NGENE_DELETE_ARRAY(pVertices);
		NGENE_DELETE_ARRAY(pNormals);
	}
//----------------------------------------------------------------------
	void NodeTerrain::cleanup()
	{
		for(uint i = 0; i < m_pIVBs.size(); ++i)
			m_pIVBs[i]->setReferenceCount(m_pIVBs[i]->getReferenceCount() - 1);

		NGENE_DELETE_ARRAY(m_TerrainDesc.heights);
		
		m_pIVBs.clear();
		m_vTiles.clear();
	}
//----------------------------------------------------------------------
	void NodeTerrain::computeNormals(SVertex* _vertices)
	{
		float h1, h2, h3, h4;
		for(uint z = 0; z < m_TerrainDesc.rows; ++z)
		{
			for(uint x = 0; x < m_TerrainDesc.columns; ++x)
			{
				h1 = h2 = h3 = h4 = 0.0f;
				uint i = z * m_TerrainDesc.columns + x;

				if(x > 0)
					h1 = _vertices[i - 1].vecPosition.y;

				if(x < m_TerrainDesc.columns - 1)
					h2 = _vertices[i + 1].vecPosition.y;

				if(z > 0)
					h3 = _vertices[i - m_TerrainDesc.columns].vecPosition.y;

				if(z < m_TerrainDesc.rows - 1)
					h4 = _vertices[i + m_TerrainDesc.columns].vecPosition.y;

				_vertices[i].vecNormal = Vector3((h1 - h2),
												 2.0f,
												 (h3 - h4));
				_vertices[i].vecNormal.normalize();
			}
		}
	}
//----------------------------------------------------------------------
	void NodeTerrain::createTiles(SVertex* _vertices)
	{
		uint axis_z = m_TerrainDesc.rows;
		uint axis_x = m_TerrainDesc.columns;
		uint tiles_x = m_TerrainDesc.tiles_x;
		uint tiles_z = m_TerrainDesc.tiles_z;
		uint i = 0;
		for(uint z = 0; z < tiles_z; ++z)
		{
			for(uint x = 0; x < tiles_x; ++x)
			{
				TerrainTile tile;
				RECT_FLOAT rect(axis_x / tiles_x * x,
								axis_z / tiles_z * z,
								axis_x / tiles_x * (x + 1),
								axis_z / tiles_z * (z + 1));

				Surface surface;

				wostringstream buffer;
				buffer << L"Base" << i++;
				addSurface(buffer.str(), surface);

				tile.init(&m_Surfaces[buffer.str()], rect, _vertices, m_TerrainDesc, m_pIVBs[0]);

				m_vTiles.push_back(tile);
			}
		}
	}
//----------------------------------------------------------------------
	void NodeTerrain::onUpdate()
	{
		NodeVisible::onUpdate();

		vector<TerrainTile>::iterator iter;
		Vector3 eyePos;
		if(Engine::getSingleton().getActiveCamera())
			 eyePos = Engine::getSingleton().getActiveCamera()->getPositionLocal();


		for(iter = m_vTiles.begin(); iter != m_vTiles.end(); ++iter)
		{
			float distance = iter->getAABB()->distanceToVector(eyePos);

			// Now choose appropriate LOD
			for(uint i = 0; i < m_TerrainDesc.LODs.size(); ++i)
			{
				if(distance < m_TerrainDesc.LODs[i].second)
				{
					iter->setIndexedBuffer(m_pIVBs[i]);
					break;
				}
			}
		}
	}
//----------------------------------------------------------------------
	void NodeTerrain::createIndicesBuffer(uint _step)
	{
		uint axis_z = m_TerrainDesc.rows;
		uint axis_x = m_TerrainDesc.columns;
		uint tiles_x = m_TerrainDesc.tiles_x;
		uint tiles_z = m_TerrainDesc.tiles_z;
		dword indices_step = _step;
		dword faces = ((axis_z / tiles_z) * (axis_x / tiles_x) * 2) / (indices_step * indices_step) + 4 * (axis_x / tiles_x) / indices_step
			+ 4 * (axis_z / tiles_z) / indices_step;
		ushort* inds = new ushort[faces * 3];
		uint begin = 0;

		for(uint i = 0; i < faces * 3 - (3 * 4 * (axis_x / tiles_x)) / indices_step - (3 * 4 * (axis_z / tiles_z)) / indices_step; i += 6)
		{
			inds[i] = begin;
			inds[i + 1] = inds[i] + indices_step;
			inds[i + 2] = inds[i] + indices_step * (axis_x / tiles_x + 1) + indices_step;
			inds[i + 3] = inds[i];
			inds[i + 4] = inds[i + 2];
			inds[i + 5] = inds[i] + indices_step * (axis_x / tiles_x + 1);

			begin += indices_step;

			if((begin + 1) % (axis_x / tiles_x + 1) == 0)
				begin += (indices_step - 1) * (axis_x / tiles_x) + indices_step;
		}

		// Create skirt
		begin = (axis_z / tiles_z + 1) * (axis_x / tiles_x + 1);
		bool back = false;
		for(uint i = faces * 3 - (3 * 4 * (axis_x / tiles_x)) / indices_step - (3 * 4 * (axis_z / tiles_z)) / indices_step; i < 3 * faces - (3 * 4 * (axis_z / tiles_z)) / indices_step; i += 6)
		{
			if(back)
			{
				inds[i] = begin;
				inds[i + 1] = inds[i] + indices_step;
				inds[i + 2] = inds[i] + (axis_x / tiles_x + 1) + indices_step;
				inds[i + 3] = inds[i];
				inds[i + 4] = inds[i + 2];
				inds[i + 5] = inds[i] + (axis_x / tiles_x + 1);
			}
			else
			{
				inds[i + 2] = begin;
				inds[i + 1] = inds[i + 2] + indices_step;
				inds[i + 0] = inds[i + 2] + (axis_x / tiles_x + 1) + indices_step;
				inds[i + 5] = inds[i + 2];
				inds[i + 4] = inds[i];
				inds[i + 3] = inds[i + 2] + (axis_x / tiles_x + 1);
			}

			begin += indices_step;

			if((begin + 1) % (axis_x / tiles_x + 1) == 0)
			{
				begin = (axis_z / tiles_z + 1) * (axis_x / tiles_x + 1) + 2 * (axis_x / tiles_x + 1);
				back = true;
			}
		}


		begin = (axis_z / tiles_z + 1) * (axis_x / tiles_x + 1) + 4 * (axis_x / tiles_x + 1);
		back = false;
		for(uint i = faces * 3 - (3 * 4 * (axis_z / tiles_z)) / indices_step; i < 3 * faces; i += 6)
		{
			if(!back)
			{
				inds[i] = begin;
				inds[i + 1] = inds[i] + indices_step;
				inds[i + 2] = inds[i] + (axis_z / tiles_z + 1) + indices_step;
				inds[i + 3] = inds[i];
				inds[i + 4] = inds[i + 2];
				inds[i + 5] = inds[i] + (axis_z / tiles_z + 1);
			}
			else
			{
				inds[i + 2] = begin;
				inds[i + 1] = inds[i + 2] + indices_step;
				inds[i + 0] = inds[i + 2] + (axis_z / tiles_z + 1) + indices_step;
				inds[i + 5] = inds[i + 2];
				inds[i + 4] = inds[i];
				inds[i + 3] = inds[i + 2] + (axis_z / tiles_z + 1);
			}

			begin += indices_step;

			if((begin + 1) % (axis_z / tiles_z + 1) == 0)
			{
				begin = (axis_z / tiles_z + 1) * (axis_x / tiles_x + 1) + 4 * (axis_x / tiles_x + 1) + 2 * (axis_z / tiles_z + 1);
				back = true;
			}
		}


		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indices = inds;
		ivbDesc.indicesNum = faces * 3;
		IndexedBuffer* pivb = VertexBufferManager::getSingleton().createIndexedBuffer(ivbDesc);
		m_pIVBs.push_back(pivb);

		pivb->setReferenceCount(1);


		NGENE_DELETE_ARRAY(inds);
	}
//----------------------------------------------------------------------
	void NodeTerrain::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(_serializeType) _serializer->addObject(this->Type);
			NodeVisible::serialize(_serializer, false, true);

			Property <uint> prColumns(m_TerrainDesc.columns);
			_serializer->addProperty("Columns", prColumns);
			Property <uint> prRows(m_TerrainDesc.rows);
			_serializer->addProperty("Rows", prRows);
			Property <uint> prTilesX(m_TerrainDesc.tiles_x);
			_serializer->addProperty("Tiles_X", prTilesX);
			Property <uint> prTilesZ(m_TerrainDesc.tiles_z);
			_serializer->addProperty("Tiles_Z", prTilesZ);

			Property <float> prTexScaleX(m_TerrainDesc.texScaleX);
			_serializer->addProperty("TexScaleX", prTexScaleX);
			Property <float> prTexScaleY(m_TerrainDesc.texScaleY);
			_serializer->addProperty("TexScaleY", prTexScaleY);
			Property <float> prHeightScale(m_TerrainDesc.heightScale);
			_serializer->addProperty("HeightScale", prHeightScale);
			Property <float> prStep(m_TerrainDesc.step);
			_serializer->addProperty("Step", prStep);
			Property <float> prSkirtSize(m_TerrainDesc.skirtSize);
			_serializer->addProperty("SkirtSize", prSkirtSize);
			
			// Save height data
			stringstream buffer;
			for(uint z = 0; z < m_TerrainDesc.rows; ++z)
			{
				for(uint x = 0; x < m_TerrainDesc.columns; ++x)
					buffer << m_TerrainDesc.heights[z * m_TerrainDesc.columns + x] << ";";
			}

			string stTmp = buffer.str();
			Property <string> proper(stTmp);
			_serializer->addProperty("Heights", proper);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeTerrain::deserialize(ISerializer* _serializer)
	{
		Property <uint> prColumns(m_TerrainDesc.columns);
		_serializer->getProperty("Columns", prColumns);
		Property <uint> prRows(m_TerrainDesc.rows);
		_serializer->getProperty("Rows", prRows);
		Property <uint> prTilesX(m_TerrainDesc.tiles_x);
		_serializer->getProperty("Tiles_X", prTilesX);
		Property <uint> prTilesZ(m_TerrainDesc.tiles_z);
		_serializer->getProperty("Tiles_Z", prTilesZ);

		Property <float> prTexScaleX(m_TerrainDesc.texScaleX);
		_serializer->getProperty("TexScaleX", prTexScaleX);
		Property <float> prTexScaleY(m_TerrainDesc.texScaleY);
		_serializer->getProperty("TexScaleY", prTexScaleY);
		Property <float> prHeightScale(m_TerrainDesc.heightScale);
		_serializer->getProperty("HeightScale", prHeightScale);
		Property <float> prStep(m_TerrainDesc.step);
		_serializer->getProperty("Step", prStep);
		Property <float> prSkirtSize(m_TerrainDesc.skirtSize);
		_serializer->getProperty("SkirtSize", prSkirtSize);

		// Parse heights
		string stTmp;
		Property <string> proper(stTmp);
		_serializer->getProperty("Heights", proper);

		vector <string> vecValues;
		FuncSplit <string> tokenizer;
		tokenizer(stTmp, vecValues, ";");

		if(vecValues.size())
		{
			uint count = m_TerrainDesc.columns * m_TerrainDesc.rows;
			m_TerrainDesc.heights = new float[count];

			vector <string>::iterator iter = vecValues.begin();
			for(uint i = 0; i < count; ++i)
			{
				m_TerrainDesc.heights[i] = static_cast<float>(atof((iter++)->c_str()));
			}
		}

		NodeVisible::deserialize(_serializer);
	}
//----------------------------------------------------------------------
}
