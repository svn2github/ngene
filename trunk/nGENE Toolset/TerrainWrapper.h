/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TerrainWrapper.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TERRAINWRAPPER_H_
#define __INC_TERRAINWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	
	/// Wrapper for a PrefabBox class.
	ref class TerrainWrapper: public NodeVisibleWrapper
	{
	protected:
		NodeTerrain* m_pTerrain;

		static int s_nTerrainCount;

		System::Drawing::Bitmap^ heightmapFile;
		bool useLod;

	public:
		TerrainWrapper(ITexture* pTexture)
		{
			TEXTURE_DESC textureDesc = pTexture->getTextureDesc();
			TERRAIN_DESC terrainDesc;

			terrainDesc.columns = textureDesc.width;
			terrainDesc.rows = textureDesc.height;
			terrainDesc.heightScale = 1.0f;
			terrainDesc.step = 1.0f;
			useLod = true;
			if(!(terrainDesc.columns % 32))
				terrainDesc.tiles_x = terrainDesc.columns / 32;
			else
			{
				terrainDesc.tiles_x = 1;
				useLod = false;
			}
			if(!(terrainDesc.rows % 32))
				terrainDesc.tiles_z = terrainDesc.rows / 32;
			else
			{
				terrainDesc.tiles_z = 1;
				useLod = false;
			}
			terrainDesc.texScaleX = 3.0f;
			terrainDesc.texScaleY = 3.0f;
			terrainDesc.skirtSize = 3.5f;
			FuncFilterBox <nGENE::byte, float> boxFilter;
			nGENE::byte* pTempHeights = (nGENE::byte*)pTexture->getData();
			nGENE::byte* pHeights = new nGENE::byte[textureDesc.width * textureDesc.height];

			// We obtain only single channel from texture
			uint index = 0;
			for(uint z = 0; z < textureDesc.height; ++z)
			{
				for(uint x = 0; x < textureDesc.width; ++x)
				{
					pHeights[z * textureDesc.width + (textureDesc.width - x - 1)] = pTempHeights[index++ * pTexture->getBitDepth()]; 
				}
			}
			NGENE_DELETE_ARRAY(pTempHeights);
			terrainDesc.heights = boxFilter(pHeights,
											textureDesc.width,
											textureDesc.height);

			// Now let's add LOD
			if(useLod)
			{
				terrainDesc.LODs.push_back(make_pair(1, 820.0f));
				terrainDesc.LODs.push_back(make_pair(2, 1000.0f));
				terrainDesc.LODs.push_back(make_pair(4, 1400.0f));
				terrainDesc.LODs.push_back(make_pair(8, 2000.0f));
				terrainDesc.LODs.push_back(make_pair(16, 1e9));
			}

			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pTerrain = sm->createTerrain(terrainDesc);
			m_pTerrain->setPosition(-float(terrainDesc.columns) * 0.5f, 0.0f, -float(terrainDesc.rows) * 0.5f);
			Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"terrain");
			Surface* pSurface = NULL;
			for(uint i = 0; i < m_pTerrain->getSurfacesNum() - 1; ++i)
			{
				wostringstream buffer;
				pSurface = m_pTerrain->getSurface(i);
				pSurface->setMaterial(pMaterial);
			}

			wostringstream buffer;
			buffer << "Landscape";
			buffer << s_nTerrainCount;
			wstring text = buffer.str();
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pTerrain);
			NGENE_DELETE_ARRAY(pHeights);

			m_pNode = m_pTerrain;

			m_Position = gcnew Vector3(&m_pTerrain->getPositionLocal());
			m_Rotation = gcnew Quaternion(&m_pTerrain->getRotation());
			m_Dimensions = gcnew Vector3(&m_pTerrain->getScale());

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			this->Name = "Landscape" + (s_nTerrainCount);
			this->Key = "Landscape" + (s_nTerrainCount++);
			frmSceneGraph::graph->addNode(this);

			/*frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pTerrain->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pTerrain->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pTerrain->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();*/
		}

		TerrainWrapper(NodeTerrain* _terrain, String^ _name):
			NodeVisibleWrapper(_terrain, _name, 0)
		{
			m_pTerrain = _terrain;
			s_nTerrainCount++;


			/*frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pTerrain->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pTerrain->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pTerrain->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();*/
		}

		virtual ~TerrainWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Terrain"),
		DescriptionAttribute("Height map to be used for landscape creation")]
		property System::Drawing::Bitmap^ Heightmap
		{
			System::Drawing::Bitmap^ get()
			{
				return heightmapFile;
			}

			void set(System::Drawing::Bitmap^ value)
			{
				heightmapFile = value;
				// Recreate terrain
				if(value != nullptr)
				{
					if(!frmRenderTarget::engine->tryLock())
						return;

						TERRAIN_DESC terrainDesc = m_pTerrain->getTerrainDesc();
						ScenePartitioner* partitioner = m_pTerrain->getCreator()->getScenePartitioner();
						if(partitioner)
							m_pTerrain->removeFromPartitioner(partitioner);
						m_pTerrain->cleanup();

						uint height = value->Height;
						uint width = value->Width;

						FuncFilterBox <nGENE::byte, float> boxFilter;
						nGENE::byte* pHeights = new nGENE::byte[width * height];
						
						// We obtain only single channel from texture
						for(uint z = 0; z < height; ++z)
						{
							for(uint x = 0; x < width; ++x)
							{
								pHeights[z * width + (width - x - 1)] = value->GetPixel(x, z).R; 
							}
						}
						terrainDesc.rows = width;
						terrainDesc.columns = height;
						useLod = true;
						if(!(terrainDesc.columns % 32))
							terrainDesc.tiles_x = terrainDesc.columns / 32;
						else
						{
							terrainDesc.tiles_x = 1;
							useLod = false;
						}
						if(!(terrainDesc.rows % 32))
							terrainDesc.tiles_z = terrainDesc.rows / 32;
						else
						{
							terrainDesc.tiles_z = 1;
							useLod = false;
						}
						terrainDesc.heights = boxFilter(pHeights,
														width,
														height);

						// Now let's add LOD
						terrainDesc.LODs.clear();
						if(useLod)
						{
							terrainDesc.LODs.push_back(make_pair(1, 820.0f));
							terrainDesc.LODs.push_back(make_pair(2, 1000.0f));
							terrainDesc.LODs.push_back(make_pair(4, 1400.0f));
							terrainDesc.LODs.push_back(make_pair(8, 2000.0f));
							terrainDesc.LODs.push_back(make_pair(16, 1e9));
						}

						m_pTerrain->setTerrainDesc(terrainDesc);
						m_pTerrain->init();
						Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"terrain");
						Surface* pSurface = NULL;
						for(uint i = 0; i < m_pTerrain->getSurfacesNum() - 1; ++i)
						{
							wostringstream buffer;
							pSurface = m_pTerrain->getSurface(i);
							pSurface->setMaterial(pMaterial);
						}

						if(partitioner)
							m_pTerrain->addToPartitioner(partitioner);

					frmRenderTarget::engine->releaseLock();
				}
			}
		}

		[Browsable(true), CategoryAttribute("Terrain"),
		DescriptionAttribute("Modifies height read from height-map by multiplying it")]
		property Real HeightScale
		{
			Real get()
			{
				return m_pTerrain->getTerrainDesc().heightScale;
			}

			void set(Real value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

					TERRAIN_DESC terrainDesc = m_pTerrain->getTerrainDesc();
					ScenePartitioner* partitioner = m_pTerrain->getCreator()->getScenePartitioner();
					if(partitioner)
						m_pTerrain->removeFromPartitioner(partitioner);

					terrainDesc.heightScale = value;

					// Now let's add LOD
					terrainDesc.LODs.clear();
					if(useLod)
					{
						terrainDesc.LODs.push_back(make_pair(1, 820.0f));
						terrainDesc.LODs.push_back(make_pair(2, 1000.0f));
						terrainDesc.LODs.push_back(make_pair(4, 1400.0f));
						terrainDesc.LODs.push_back(make_pair(8, 2000.0f));
						terrainDesc.LODs.push_back(make_pair(16, 1e9));
					}

					uint count = terrainDesc.columns * terrainDesc.rows;
					float* pHeights = new float[count];
					memcpy(pHeights, terrainDesc.heights, sizeof(float) * count);

					m_pTerrain->cleanup();
					terrainDesc.heights = pHeights;

					m_pTerrain->setTerrainDesc(terrainDesc);
					m_pTerrain->init();
					Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"terrain");
					Surface* pSurface = NULL;
					for(uint i = 0; i < m_pTerrain->getSurfacesNum() - 1; ++i)
					{
						wostringstream buffer;
						pSurface = m_pTerrain->getSurface(i);
						pSurface->setMaterial(pMaterial);
					}

					if(partitioner)
						m_pTerrain->addToPartitioner(partitioner);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Terrain"),
		DescriptionAttribute("Specifies size of skirt")]
		property Real SkirtHeight
		{
			Real get()
			{
				return m_pTerrain->getTerrainDesc().skirtSize;
			}

			void set(Real value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

					TERRAIN_DESC terrainDesc = m_pTerrain->getTerrainDesc();
					ScenePartitioner* partitioner = m_pTerrain->getCreator()->getScenePartitioner();
					if(partitioner)
						m_pTerrain->removeFromPartitioner(partitioner);

					terrainDesc.skirtSize = value;

					// Now let's add LOD
					terrainDesc.LODs.clear();
					if(useLod)
					{
						terrainDesc.LODs.push_back(make_pair(1, 820.0f));
						terrainDesc.LODs.push_back(make_pair(2, 1000.0f));
						terrainDesc.LODs.push_back(make_pair(4, 1400.0f));
						terrainDesc.LODs.push_back(make_pair(8, 2000.0f));
						terrainDesc.LODs.push_back(make_pair(16, 1e9));
					}

					uint count = terrainDesc.columns * terrainDesc.rows;
					float* pHeights = new float[count];
					memcpy(pHeights, terrainDesc.heights, sizeof(float) * count);

					m_pTerrain->cleanup();
					terrainDesc.heights = pHeights;

					m_pTerrain->setTerrainDesc(terrainDesc);
					m_pTerrain->init();
					Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"terrain");
					Surface* pSurface = NULL;
					for(uint i = 0; i < m_pTerrain->getSurfacesNum() - 1; ++i)
					{
						wostringstream buffer;
						pSurface = m_pTerrain->getSurface(i);
						pSurface->setMaterial(pMaterial);
					}

					if(partitioner)
						m_pTerrain->addToPartitioner(partitioner);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Terrain"),
		DescriptionAttribute("Specifies step")]
		property Real Step
		{
			Real get()
			{
				return m_pTerrain->getTerrainDesc().step;
			}

			void set(Real value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

					TERRAIN_DESC terrainDesc = m_pTerrain->getTerrainDesc();
					ScenePartitioner* partitioner = m_pTerrain->getCreator()->getScenePartitioner();
					if(partitioner)
						m_pTerrain->removeFromPartitioner(partitioner);

					terrainDesc.step = value;

					// Now let's add LOD
					terrainDesc.LODs.clear();
					if(useLod)
					{
						terrainDesc.LODs.push_back(make_pair(1, 820.0f));
						terrainDesc.LODs.push_back(make_pair(2, 1000.0f));
						terrainDesc.LODs.push_back(make_pair(4, 1400.0f));
						terrainDesc.LODs.push_back(make_pair(8, 2000.0f));
						terrainDesc.LODs.push_back(make_pair(16, 1e9));
					}

					uint count = terrainDesc.columns * terrainDesc.rows;
					float* pHeights = new float[count];
					memcpy(pHeights, terrainDesc.heights, sizeof(float) * count);

					m_pTerrain->cleanup();
					terrainDesc.heights = pHeights;

					m_pTerrain->setTerrainDesc(terrainDesc);
					m_pTerrain->init();
					Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"terrain");
					Surface* pSurface = NULL;
					for(uint i = 0; i < m_pTerrain->getSurfacesNum() - 1; ++i)
					{
						wostringstream buffer;
						pSurface = m_pTerrain->getSurface(i);
						pSurface->setMaterial(pMaterial);
					}

					if(partitioner)
						m_pTerrain->addToPartitioner(partitioner);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Terrain"),
		DescriptionAttribute("Specifies texture multiplier for x axis")]
		property Real TexScaleX
		{
			Real get()
			{
				return m_pTerrain->getTerrainDesc().texScaleX;
			}

			void set(Real value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

					TERRAIN_DESC terrainDesc = m_pTerrain->getTerrainDesc();
					ScenePartitioner* partitioner = m_pTerrain->getCreator()->getScenePartitioner();
					if(partitioner)
						m_pTerrain->removeFromPartitioner(partitioner);

					terrainDesc.texScaleX = value;

					uint count = terrainDesc.columns * terrainDesc.rows;
					float* pHeights = new float[count];
					memcpy(pHeights, terrainDesc.heights, sizeof(float) * count);

					m_pTerrain->cleanup();
					terrainDesc.heights = pHeights;

					m_pTerrain->setTerrainDesc(terrainDesc);
					m_pTerrain->init();
					Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"terrain");
					Surface* pSurface = NULL;
					for(uint i = 0; i < m_pTerrain->getSurfacesNum() - 1; ++i)
					{
						wostringstream buffer;
						pSurface = m_pTerrain->getSurface(i);
						pSurface->setMaterial(pMaterial);
					}

					if(partitioner)
						m_pTerrain->addToPartitioner(partitioner);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Terrain"),
		DescriptionAttribute("Specifies texture multiplier for z axis")]
		property Real TexScaleZ
		{
			Real get()
			{
				return m_pTerrain->getTerrainDesc().texScaleY;
			}

			void set(Real value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

					TERRAIN_DESC terrainDesc = m_pTerrain->getTerrainDesc();
					ScenePartitioner* partitioner = m_pTerrain->getCreator()->getScenePartitioner();
					if(partitioner)
						m_pTerrain->removeFromPartitioner(partitioner);

					terrainDesc.texScaleY = value;

					uint count = terrainDesc.columns * terrainDesc.rows;
					float* pHeights = new float[count];
					memcpy(pHeights, terrainDesc.heights, sizeof(float) * count);

					m_pTerrain->cleanup();
					terrainDesc.heights = pHeights;

					m_pTerrain->setTerrainDesc(terrainDesc);
					m_pTerrain->init();
					Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"terrain");
					Surface* pSurface = NULL;
					for(uint i = 0; i < m_pTerrain->getSurfacesNum() - 1; ++i)
					{
						wostringstream buffer;
						pSurface = m_pTerrain->getSurface(i);
						pSurface->setMaterial(pMaterial);
					}

					if(partitioner)
						m_pTerrain->addToPartitioner(partitioner);

				frmRenderTarget::engine->releaseLock();
			}
		}
	};
}


#endif