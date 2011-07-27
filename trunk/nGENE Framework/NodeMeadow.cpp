/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeMeadow.cpp
Version:	0.09
---------------------------------------------------------------------------
*/

#include "NodeMeadow.h"

#include "Billboard.h"
#include "ITexture.h"
#include "NodeTerrain.h"
#include "Point.h"
#include "Ray.h"

#include <sstream>


namespace nGENE
{
namespace Nature
{
	NodeMeadow::NodeMeadow()
	{
	}
//----------------------------------------------------------------------
	NodeMeadow::NodeMeadow(NodeTerrain* _terrain):
		m_pTerrain(_terrain)
	{
	}
//----------------------------------------------------------------------
	NodeMeadow::~NodeMeadow()
	{
	}
//----------------------------------------------------------------------
	void NodeMeadow::addPlant(PLANT_DESC& _desc)
	{
		if(!m_pTerrain)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Terrain for meadow is not set");
			return;
		}

		HashTable <uint, NodeBillboardSet*> sets;
		

		TERRAIN_DESC& terrainDesc = m_pTerrain->getTerrainDesc();
		Ray ray;
		Point origin;
		Vector3 direction(0.0f, -2000.0f, 0.0f);
		Matrix4x4 matTerrain = m_pTerrain->getTransform();

		Billboard bill;

		float x, y, z;
		int nx, nz;

		Surface* terrainSurf = NULL;

		float width = float(terrainDesc.columns) * terrainDesc.step * 0.5f;
		float height = float(terrainDesc.rows) * terrainDesc.step * 0.5f;
		int halfCols = terrainDesc.columns >> 1;
		int halfRows = terrainDesc.rows >> 1;

		float xDiff = width + m_pTerrain->getPositionLocal().x;
		float zDiff = height + m_pTerrain->getPositionLocal().z;

		int pointPerTileX = terrainDesc.columns / terrainDesc.tiles_x;
		int pointPerTileZ = terrainDesc.rows / terrainDesc.tiles_z;

		ITexture* pTexture = _desc.distributionMap;
		vector < pair<float, float> > possibleHeights;
		TEXTURE_DESC textureDesc;
		if(pTexture)
		{
			textureDesc = pTexture->getTextureDesc();
			nGENE::byte* pTempHeights = (nGENE::byte*)pTexture->getData();
			nGENE::byte* pHeights = new nGENE::byte[textureDesc.width * textureDesc.height];

			// We obtain only single channel from texture
			uint index = 0;
			for(uint z = 0; z < textureDesc.height; ++z)
			{
				for(uint x = 0; x < textureDesc.width; ++x)
				{
					pHeights[index] = pTempHeights[index++ * pTexture->getBitDepth()]; 
				}
			}

			NGENE_DELETE_ARRAY(pTempHeights);

			// Now get only 'possible' data
			index = 0;
			float texWidth = (float)textureDesc.width;
			float texHeight = (float)textureDesc.height;
			for(uint z = 0; z < textureDesc.height; ++z)
			{
				for(uint x = 0; x < textureDesc.width; ++x)
				{
					if(pHeights[z * textureDesc.width + (textureDesc.width - x - 1)] > 127)
					{
						possibleHeights.push_back(make_pair(((float)x / texWidth) * width * 2.0f - width,
															((float)z / texHeight) * height * 2.0f - height));
					}
				}
			}

			NGENE_DELETE_ARRAY(pHeights);
		}

		uint heightsCount = possibleHeights.size();
		float step = terrainDesc.step;
		float halfStep = step * 0.5f;


		INTERSECTION_DESC intersectDesc;
		for(uint i = 0; i < _desc.plantsCount; ++i)
		{
			uint index = 0;

			do
			{
				if(pTexture)
				{
					// Select valid pair
					uint heightIndex = abs(Maths::perlinNoiseGenerator(i * 3, i + 1, rand() % 1000)) * (float)heightsCount;

					x = possibleHeights[heightIndex].first + halfStep + Maths::perlinNoiseGenerator(i * 3, i + 1, rand() % 1000) * halfStep;
					z = possibleHeights[heightIndex].second + halfStep + Maths::perlinNoiseGenerator(i + 100, i * 2, rand() % 1000) * halfStep;
				}
				else
				{	
					x = Maths::perlinNoiseGenerator(i * 3, i + 1, rand() % 1000) * width;
					z = Maths::perlinNoiseGenerator(i + 100, i * 2, rand() % 1000) * height;
				}

				nx = (int)floor(x / step) + halfCols;
				nz = (int)floor(z / step) + halfRows;

				x += xDiff;
				z += zDiff;


				// Prepare ray
				origin.set(x, 1000.0f, z);
				ray.set(origin, direction);
				ray.moveToObjectSpace(matTerrain);

				index = (nz / pointPerTileZ) * terrainDesc.tiles_x + (nx / pointPerTileX);
				terrainSurf = m_pTerrain->getSurface(index);
			} while(!ray.intersectSurface(*terrainSurf, intersectDesc));

			bill.m_fWidth = _desc.width + Maths::perlinNoiseGenerator(i * 3, i + 1, rand() % 1000) * _desc.widthSpread;
			bill.m_fHeight = _desc.height + Maths::perlinNoiseGenerator(i * 3, i + 1, rand() % 1000) * _desc.heightSpread;

			y = ray.getIntersectionPoint().y - _desc.undergroundLevel * bill.m_fHeight + matTerrain._m42;
			
			bill.m_vecPosition = Vector3(x, y, z);
			bill.m_vecNormal = intersectDesc.normal;


			// Create new billboard set if it doesn't exist
			if(sets.find(index) == sets.end())
			{
				if(m_pCreator)
					sets[index] = m_pCreator->createBillboardSet();
				else
				{
					Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
						__LINE__, L"The meadow has not been created. Call setCreator() function");
					return;
				}
			}
			sets[index]->addBillboard(bill);
		}
		HashTable <uint, NodeBillboardSet*>::iterator iter;
		NodeBillboardSet* pSet = NULL;
		for(iter = sets.begin(); iter != sets.end(); ++iter)
		{
			pSet = iter->second;
			pSet->validate();

			Surface* surf = pSet->getSurface(L"Base");
			surf->setMaterial(_desc.material);
			wostringstream buffer;
			buffer << L"Plant_" << m_vPlants.size() << "_" << pSet;
			addChild(buffer.str(), pSet);
		}


		m_vPlants.push_back(_desc);
	}
//----------------------------------------------------------------------
	void NodeMeadow::addDecorator(DECORATOR_DESC& _desc)
	{
		// No terrain, so exit
		if(!m_pTerrain)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Terrain for meadow is not set");
			return;
		}

		TERRAIN_DESC& terrainDesc = m_pTerrain->getTerrainDesc();
		Ray ray;
		Point origin;
		Vector3 direction(0.0f, -2000.0f, 0.0f);
		Matrix4x4 matTerrain = m_pTerrain->getTransform();

		float x, y, z;
		int nx, nz;

		Surface* terrainSurf = NULL;

		float width = float(terrainDesc.columns) * terrainDesc.step * 0.5f;
		float height = float(terrainDesc.rows) * terrainDesc.step * 0.5f;
		int halfCols = terrainDesc.columns >> 1;
		int halfRows = terrainDesc.rows >> 1;

		float xDiff = width + m_pTerrain->getPositionLocal().x;
		float zDiff = height + m_pTerrain->getPositionLocal().z;

		int pointPerTileX = terrainDesc.columns / terrainDesc.tiles_x;
		int pointPerTileZ = terrainDesc.rows / terrainDesc.tiles_z;

		Vector3 currentScale;

		ITexture* pTexture = _desc.distributionMap;
		nGENE::byte* pHeights = NULL;
		TEXTURE_DESC textureDesc;
		if(pTexture)
		{
			textureDesc = pTexture->getTextureDesc();
			nGENE::byte* pTempHeights = (nGENE::byte*)pTexture->getData();
			pHeights = new nGENE::byte[textureDesc.width * textureDesc.height];

			// We obtain only single channel from texture
			uint index = 0;
			for(uint z = 0; z < textureDesc.height; ++z)
			{
				for(uint x = 0; x < textureDesc.width; ++x)
				{
					pHeights[index] = pTempHeights[index++ * pTexture->getBitDepth()]; 
				}
			}
			NGENE_DELETE_ARRAY(pTempHeights);
		}


		for(uint i = 0; i < _desc.objectsCount; ++i)
		{
			bool result = false;

			NodeVisible* pNewNode = new NodeVisible(*_desc.node);
			pNewNode->init();
			pNewNode->setCreator(this->getCreator());

			currentScale = pNewNode->getScale();
			currentScale.add(Maths::perlinNoiseGenerator(i * 5, i + 12, rand() % 1000) * _desc.scaleSpread);
			pNewNode->setScale(currentScale);
			while(!result)
			{
				do
				{
					x = Maths::perlinNoiseGenerator(i * 3, i + 1, rand() % 1000) * width;
					z = Maths::perlinNoiseGenerator(i + 100, i * 2, rand() % 1000) * height;
					nx = (int)floor(x / terrainDesc.step) + halfCols;
					nz = (int)floor(z / terrainDesc.step) + halfRows;

					x += xDiff;
					z += zDiff;

					// Prepare ray
					origin.set(x, 1000.0f, z);
					ray.set(origin, direction);
					ray.moveToObjectSpace(matTerrain);

					wostringstream buffer;
					buffer << L"Base" << (nz / pointPerTileZ) * terrainDesc.tiles_x + (nx / pointPerTileX);
					terrainSurf = m_pTerrain->getSurface(buffer.str());
				} while(!ray.intersectSurface(*terrainSurf));

				y = ray.getIntersectionPoint().y - _desc.undergroundLevel + matTerrain._m42;
				
				pNewNode->setPosition(x, y, z);
				for(uint i = 0; i < pNewNode->getSurfacesNum(); ++i)
				{
					pNewNode->getSurface(i)->setNode(pNewNode);
				}
				if(pTexture)
				{
					int texX = float(textureDesc.width) * ((float)nx / float(halfCols * 2));
					int texY = float(textureDesc.height) * ((float)nz / float(halfRows * 2));

					if(pHeights[texY * textureDesc.width + (textureDesc.width - texX - 1)] > 127)
						result = true;
				}
				else
					result = true;
			}
			wostringstream buffer;
			buffer << L"Object_" << _desc.node << L"_" << i;
			wstring name = buffer.str();

			this->addChild(name, pNewNode);
		}

		NGENE_DELETE_ARRAY(pHeights);
	}
//----------------------------------------------------------------------
	void NodeMeadow::onUpdate()
	{
		Node::onUpdate();
	}
//----------------------------------------------------------------------
}
}