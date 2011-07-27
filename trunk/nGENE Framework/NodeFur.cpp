/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeFur.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "NodeFur.h"

#include "ITexture.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "RenderPass.h"
#include "Shader.h"
#include "ShaderConstant.h"
#include "ShaderInstance.h"
#include "TextureManager.h"

#include <sstream>


namespace nGENE
{
namespace Nature
{
	const float INV_RAND_MAX = 1.0f / (RAND_MAX + 1);

	inline float rnd(float max=1.0)
	{
		return max * INV_RAND_MAX * rand();
	}

	inline float rnd(float min, float max)
	{
		return min + (max - min) * INV_RAND_MAX * rand();
	}



	// Initialize static members
	uint FurTexture::s_TexturesCount = 0;


	FurTexture::FurTexture():
		m_nSize(0),
		m_nNumLayers(0)
	{
	}
//----------------------------------------------------------------------
	FurTexture::~FurTexture()
	{
		m_vTextures.clear();
	}
//----------------------------------------------------------------------
	void FurTexture::cleanup()
	{
		m_nSize = 0;
		m_nNumLayers = 0;

		TextureManager& mgr = TextureManager::getSingleton();
		for(uint i = 0; i < m_vTextures.size(); ++i)
			mgr.removeTexture(m_vTextures[i]);
		m_vTextures.clear();
	}
//----------------------------------------------------------------------
	void FurTexture::init(uint _seed, uint _size, uint _num, uint _density)
	{
		m_nSize      = _size;
		m_nNumLayers = _num;

		Colour* data = new Colour[m_nNumLayers * m_nSize * m_nSize];
		#define DATA(layer, x, y) data[m_nSize * m_nSize * (layer) + m_nSize * (y) + (x)]

		// Set all the pixels to the same colour, transparent black!
		{
			for(uint layer = 0; layer < m_nNumLayers; ++layer)
			{
				for(uint x = 0; x < m_nSize; ++x)
				{
					for(uint y = 0; y < m_nSize; ++y)
						DATA(layer, x, y) = Colour(0, 0, 0, 0);
				}
			}
		}


		float fLayers = static_cast<float>(m_nNumLayers);

		// This is slightly different than above, we now do it so that
		// as we move to the outer layers, we have less and less strands of hair
		{
			for(uint layer = 0; layer < m_nNumLayers; ++layer)
			{	
				float length = float(layer) / fLayers; // 0 to 1
				length = 1.0f - length; // 1 to 0
				int density = int(_density * length * 6.0f);
				for(int i = 0; i < density; ++i)
				{
					int xrand = (int)rnd(0, (float)m_nSize);
					int yrand = (int)rnd(0, (float)m_nSize);
					
					DATA(layer, xrand, yrand) = Colour(255, 0, 0, 255);
				}
			}
		}

		// Loop across all the pixels, and make the top layer of pixels more
		// transparent (top hairs)
		{
			for(uint x = 0; x < m_nSize; x++)
			{
				for(uint y = 0; y < m_nSize; y++)
				{
					for(uint layer = 0; layer < m_nNumLayers; ++layer)
					{
						float length = (float)layer / fLayers; // 0 to 1
					
						// tip of the hair is semi-transparent
						float alpha = DATA(layer, x, y).getAlpha();
						if(alpha > 0.0f)
							DATA(layer, x, y).setAlpha((1.0f - length) * 255.0f);
					}
				}
			}
		}

		// Well now, hairs that are closer to the centre are darker as they get less light
		{
			for(uint x = 0; x < m_nSize; ++x)
			{
				for(uint y = 0; y < m_nSize; ++y)
				{
					for(uint layer = 0; layer < m_nNumLayers; ++layer)
					{
						float length = (float)layer / fLayers; // 0 to 1
					
						// tip of the hair is semi-transparent
						float scale = 1.0f - length;
						scale = max(scale, 0.9f);
						float alpha = DATA(layer, x, y).getAlpha();
						if(alpha > 0.0f)
						{
							Colour temp = DATA(layer, x, y);
							temp.setRed(temp.getRed() * scale);
							temp.setGreen(temp.getGreen() * scale);
							temp.setBlue(temp.getBlue() * scale);
						}
					}
				}
			}
		}


		// Create all the textures and fill them with proper data
		for(uint layer = 0; layer < m_nNumLayers; ++layer)
		{
			dword* pixels = new dword[m_nSize * m_nSize];
			dword* p = pixels;
			Colour* pColor = &DATA(layer, 0, 0);
			for(uint i = 0; i < m_nSize * m_nSize; ++i)
				*p++ = (pColor++->getDwordARGB());

			wostringstream buffer;
			buffer << L"fur_texture_" << s_TexturesCount << "_" << layer;

			ITexture* pTexture = TextureManager::getSingleton().
				createTexture(buffer.str(),
							  m_nSize,
							  m_nSize,
							  TT_2D,
							  TU_NORMAL,
							  TF_A8R8G8B8);
			
			p = pixels;

			pTexture->setData(p);
			m_vTextures.push_back(pTexture);

			NGENE_DELETE_ARRAY(pixels);
		}

		++s_TexturesCount;

		NGENE_DELETE_ARRAY(data);
}
//----------------------------------------------------------------------
	ITexture* FurTexture::getTexture(uint _index)
	{
		if(_index >= m_vTextures.size())
			return NULL;

		return m_vTextures[_index];
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	NodeFur::NodeFur(uint _layersNum, uint _textureSize, uint _density, uint _seed):
		m_nLayersNum(_layersNum),
		m_nTextureSize(_textureSize),
		m_nDensity(_density),
		m_nSeed(_seed),
		m_fFurLength(0.25f),
		m_pFurMat(NULL)
	{
	}
//----------------------------------------------------------------------
	NodeFur::~NodeFur()
	{
	}
//----------------------------------------------------------------------
	void NodeFur::init()
	{
		m_Texture.init(m_nSeed, m_nTextureSize, m_nLayersNum, m_nDensity);

		m_pFurMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"fur");
		if(m_pFurMat)
		{
			TextureSampler sampler;
			sampler.setTexture(m_Texture.getTexture(0));
			sampler.setTextureUnit(0);
			sampler.setAddressingMode(TAM_MIRROR);

			float fLayers = static_cast<float>(m_nLayersNum);

			float layer = 0.0f;
			float length = 0.0f;
			ShaderConstant constant;
			constant.setValuesCount(1);
			constant.setName("layer");
			constant.setDefaultValue(&layer);

			ShaderConstant constant2;
			constant2.setValuesCount(1);
			constant2.setName("furLength");
			constant2.setDefaultValue(&length);

			RenderTechnique* pTechnique = m_pFurMat->getActiveRenderTechnique();
			RenderPass& pass = pTechnique->getRenderPass(0);
			pass.getVertexShader()->addShaderConstant(constant);
			pass.getVertexShader()->addShaderConstant(constant2);

			pass.setRenderCount(1);
			pass.addTextureSampler(sampler);

			for(uint i = 1; i < m_nLayersNum; ++i)
			{
				RenderPass newPass = pTechnique->getRenderPass(0);
				pTechnique->addRenderPass(newPass);

				RenderPass& pass = pTechnique->getRenderPass(i);

				TextureSampler& sampler = pass.getTextureSampler(pass.getTextureSamplersNum() - 1);
				sampler.setTexture(m_Texture.getTexture(i));
				sampler.setTextureUnit(0);
				sampler.setAddressingMode(TAM_MIRROR);


				layer = float(i) / (fLayers - 1.0f);
				length = m_fFurLength * layer;

				ShaderConstant* pConstant = pass.getVertexShader()->getConstant("layer");
				if(pConstant)
					pConstant->setDefaultValue(&layer);

				pConstant = pass.getVertexShader()->getConstant("furLength");
				if(pConstant)
					pConstant->setDefaultValue(&length);
			}
		}
	}
//----------------------------------------------------------------------
	void NodeFur::setFurLength(Real _length)
	{
		m_fFurLength = _length;

		float fLayers = static_cast<float>(m_nLayersNum);
		float layer = 0.0f;
		float length = 0.0f;

		RenderTechnique* pTechnique = m_pFurMat->getActiveRenderTechnique();
		for(uint i = 0; i < m_nLayersNum; ++i)
		{
			RenderPass& pass = pTechnique->getRenderPass(i);

			layer = float(i) / (fLayers - 1.0f);
			length = m_fFurLength * layer;

			ShaderConstant* pConstant = pass.getVertexShader()->getConstant("furLength");
			if(pConstant)
				pConstant->setDefaultValue(&length);
		}
	}
//----------------------------------------------------------------------
	void NodeFur::setLayersNum(uint _layers)
	{
		m_Texture.cleanup();

		m_nLayersNum = _layers;
		init();
	}
//----------------------------------------------------------------------
	void NodeFur::setFurDensity(uint _density)
	{
		m_Texture.cleanup();

		m_nDensity = _density;
		init();
	}
//----------------------------------------------------------------------
	void NodeFur::setTextureSize(uint _size)
	{
		m_Texture.cleanup();

		m_nTextureSize = _size;
		init();
	}
//----------------------------------------------------------------------
	void NodeFur::addSurface(const wstring& _name, Surface& _surf)
	{
		NodeVisible::addSurface(_name, _surf);

		m_vSurfaces.back()->setMaterial(m_pFurMat);

		calculateBoundingBox();
	}
//----------------------------------------------------------------------
	void NodeFur::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(_serializeType) _serializer->addObject(this->Type);
			NodeVisible::serialize(_serializer, false, _serializeChildren);

			Property <uint> prLayersNum(m_nLayersNum);
			_serializer->addProperty("LayersCount", prLayersNum);
			Property <uint> prTexSize(m_nTextureSize);
			_serializer->addProperty("TextureSize", prTexSize);
			Property <uint> prDensity(m_nDensity);
			_serializer->addProperty("Density", prDensity);
			Property <uint> prSeed(m_nSeed);
			_serializer->addProperty("Seed", prSeed);

			Property <float> prLength(m_fFurLength);
			_serializer->addProperty("Length", prLength);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeFur::deserialize(ISerializer* _serializer)
	{
		Property <uint> prLayersNum(m_nLayersNum);
		_serializer->getProperty("LayersCount", prLayersNum);
		Property <uint> prTexSize(m_nTextureSize);
		_serializer->getProperty("TextureSize", prTexSize);
		Property <uint> prDensity(m_nDensity);
		_serializer->getProperty("Density", prDensity);
		Property <uint> prSeed(m_nSeed);
		_serializer->getProperty("Seed", prSeed);

		Property <float> prLength(m_fFurLength);
		_serializer->getProperty("Length", prLength);

		NodeVisible::deserialize(_serializer);
	}
//----------------------------------------------------------------------
}
}