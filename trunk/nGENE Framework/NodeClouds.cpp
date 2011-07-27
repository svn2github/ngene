/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeClouds.cpp
Version:	0.13
---------------------------------------------------------------------------
*/

#define NOMINMAX
#include "NodeClouds.h"

#include "Camera.h"
#include "ITexture.h"
#include "LightStage.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "PerlinNoise.h"
#include "PostStage.h"
#include "Shader.h"
#include "ShaderConstant.h"
#include "ShaderInstance.h"
#include "TextureManager.h"
#include "TextureSampler.h"
#include "TimeDate.h"
#include "Timer.h"
#include "VertexBufferManager.h"


namespace nGENE
{
namespace Nature
{
	// Initialize static members
	TypeInfo NodeClouds::Type(L"NodeClouds", &NodeVisible::Type);


	NodeClouds::NodeClouds():
		m_pCloudsMat(NULL),
		m_pCloudsMatHDR(NULL),
		m_pCloudsDensityMat(NULL),
		m_fDayLength(360.0f),
		m_fCover(0.5f),
		m_fSharpness(0.94f),
		m_fDensity(0.02f),
		m_fHorizonLevel(0.0f),
		m_fHorizonHeight(10.0f),
		m_fTextureScaling(1.0f),
		m_bSimulate(true),
		m_bFixed(false),
		m_SunAngle(0.0f),
		m_dwPassedTime(0),
		m_dwUpdateFrequency(1000)
	{
		m_SunRise.hours = 8;
		m_Sunset.hours = 22;
	}
//----------------------------------------------------------------------
	NodeClouds::NodeClouds(const CLOUDS_DESC& _desc):
		m_Desc(_desc),
		m_pCloudsMat(NULL),
		m_pCloudsMatHDR(NULL),
		m_pCloudsDensityMat(NULL),
		m_fDayLength(360.0f),
		m_fCover(0.45f),
		m_fDensity(0.02f),
		m_fSharpness(0.94f),
		m_fHorizonLevel(0.0f),
		m_fHorizonHeight(10.0f),
		m_fTextureScaling(1.0f),
		m_bSimulate(true),
		m_bFixed(false),
		m_SunAngle(0.0f),
		m_dwPassedTime(0),
		m_dwUpdateFrequency(10000)
	{
		m_SunRise.hours = 8;
		m_Sunset.hours = 22;
	}
//----------------------------------------------------------------------
	NodeClouds::~NodeClouds()
	{
	}
//----------------------------------------------------------------------
	void NodeClouds::init()
	{
		createCloudsPlane();

		initTextures();

		// Add clouds material
		m_PostCloudsDensity.priority = 0;
		m_PostCloudsDensity.material = m_pCloudsDensityMat;
		((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"))->addToRender(&m_PostCloudsDensity);

		m_PostClouds.priority = 1;
		m_PostClouds.material = m_pCloudsMatHDR;
		((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"))->addToRender(&m_PostClouds);


		Surface* surf = getSurface(L"Base");
		surf->setMaterial(m_pCloudsMat);
		surf->flipWindingOrder();
		surf->setPickable(false);

		// Set default parameters for shaders
		setCloudCover(m_fCover);
		setCloudSharpness(m_fSharpness);
		setCloudDensity(m_fDensity);
		setTextureScaling(m_fTextureScaling);
		setHorizonLevel(m_fHorizonLevel);
		setHorizonHeight(m_fHorizonHeight);

		// Simulate at least once
		updateColours();
		updateClouds();

		m_dwLastTime = Engine::getSingleton().getTimer().getMilliseconds();
	}
//----------------------------------------------------------------------
	void NodeClouds::initTextures()
	{
		// Create textures
		ITexture* pCloudsSrc = TextureManager::getSingleton().createTexture(L"cloud_cover_source",
																			m_Desc.textureSize,
																			m_Desc.textureSize,
																			TT_2D,
																			TU_DYNAMIC,
																			TF_A8);

		ITexture* pCloudsDest = TextureManager::getSingleton().createTexture(L"cloud_cover_destination",
																			 m_Desc.textureSize,
																			 m_Desc.textureSize,
																			 TT_2D,
																			 TU_DYNAMIC,
																			 TF_A8);

		// Generate Perlin noise
		nGENE::byte* pCloudMap = createCloudNoise();
		pCloudsSrc->setData(pCloudMap);
		NGENE_DELETE_ARRAY(pCloudMap);

		pCloudMap = createCloudNoise();
		pCloudsDest->setData(pCloudMap);
		NGENE_DELETE_ARRAY(pCloudMap);
		

		// Set non HDR version
		m_pCloudsMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"clouds_NON_HDR");
		if(m_pCloudsMat)
		{
			TextureSampler sampler;
			sampler.setTexture(pCloudsSrc);
			sampler.setTextureUnit(1);
			sampler.setAddressingMode(TAM_MIRROR);
			RenderPass& pass = m_pCloudsMat->getActiveRenderTechnique()->getRenderPass(0);
			pass.addTextureSampler(sampler);

			sampler.setTexture(pCloudsDest);
			sampler.setTextureUnit(2);
			sampler.setAddressingMode(TAM_MIRROR);
			pass.addTextureSampler(sampler);
		}

		// Same goes for HDR version
		m_pCloudsMatHDR = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"clouds_HDR");
		if(m_pCloudsMatHDR)
		{
			TextureSampler sampler;
			sampler.setTexture(pCloudsSrc);
			sampler.setTextureUnit(1);
			sampler.setAddressingMode(TAM_WRAP);
			RenderPass& pass = m_pCloudsMatHDR->getActiveRenderTechnique()->getRenderPass(0);
			pass.addTextureSampler(sampler);

			sampler.setTexture(pCloudsDest);
			sampler.setTextureUnit(2);
			sampler.setAddressingMode(TAM_WRAP);
			pass.addTextureSampler(sampler);
		}

		// And cloud map generator
		m_pCloudsDensityMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"clouds_density");
		if(m_pCloudsDensityMat)
		{
			TextureSampler sampler;
			sampler.setTexture(pCloudsSrc);
			sampler.setTextureUnit(1);
			sampler.setAddressingMode(TAM_WRAP);
			RenderPass& pass = m_pCloudsDensityMat->getActiveRenderTechnique()->getRenderPass(0);
			pass.addTextureSampler(sampler);

			sampler.setTexture(pCloudsDest);
			sampler.setTextureUnit(2);
			sampler.setAddressingMode(TAM_WRAP);
			pass.addTextureSampler(sampler);
		}
	}
//----------------------------------------------------------------------
	void NodeClouds::updateTextures()
	{
		dword delta = static_cast<float>(Engine::getSingleton().getTimer().getMilliseconds() - m_dwLastTime);

		Timer time = Engine::getSingleton().getTimer();
		m_dwLastTime = time.getMilliseconds();

		bool bHDR = ((LightStage*)Renderer::getSingleton().getRenderStage(L"Light"))->isUsingHDR();
		Material* pMat = bHDR ? m_pCloudsMatHDR : m_pCloudsMat;

		if(bHDR)
		{
			getSurface(L"Base")->setEnabled(false);
			m_pCloudsMatHDR->setEnabled(true);
		}
		else
		{
			bool prev = getSurface(L"Base")->isEnabled();
			if(!prev)
			{
				ITexture* pTexture = TextureManager::getSingleton().getTexture(L"cloud_cover_map");
				pTexture->clearRenderTarget();

				getSurface(L"Base")->setEnabled(true);
				m_pCloudsMatHDR->setEnabled(false);
			}
		}


		m_dwPassedTime += delta;
		if(m_dwPassedTime > m_dwUpdateFrequency && m_dwUpdateFrequency)
		{
			m_dwPassedTime -= m_dwUpdateFrequency;

			// Create textures
			ITexture* pCloudsSrc = TextureManager::getSingleton().getTexture(L"cloud_cover_source");
			ITexture* pCloudsDest = TextureManager::getSingleton().getTexture(L"cloud_cover_destination");

			// Generate Perlin noise
			void* pCloudMap = pCloudsDest->getData();
			pCloudsSrc->setData(pCloudMap);
			NGENE_DELETE_ARRAY(pCloudMap);

			pCloudMap = createCloudNoise();
			pCloudsDest->setData(pCloudMap);
			NGENE_DELETE_ARRAY(pCloudMap);
		}


		if(pMat)
		{
			RenderPass& pass = pMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("timer");
			float t = 0.0f;
			if(m_dwUpdateFrequency)
				t = (float)m_dwPassedTime / (float)m_dwUpdateFrequency;
			if(pConstant)
				pConstant->setDefaultValue(&t);
		}

		if(m_pCloudsDensityMat)
		{
			RenderPass& pass = m_pCloudsDensityMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("timer");
			float t = 0.0f;
			if(m_dwUpdateFrequency)
				t = (float)m_dwPassedTime / (float)m_dwUpdateFrequency;
			if(pConstant)
				pConstant->setDefaultValue(&t);
		}
	}
//----------------------------------------------------------------------
	void NodeClouds::reset()
	{
		initTextures();
	}
//----------------------------------------------------------------------
	nGENE::byte* NodeClouds::createCloudNoise()
	{
		uint nSize = m_Desc.textureSize;

		// 2-dimensional Perlin noise
		PerlinNoise <2> clouds(nSize);
		clouds.setup();

		// Generate Perlin noise
		nGENE::byte* pCloudMap = new nGENE::byte[nSize * nSize];
		memset(pCloudMap, 0, sizeof(nGENE::byte) * nSize * nSize);

		for(uint i = 0; i < nSize; ++i)
		{
			for(uint j = 0; j < nSize; ++j)
			{
				vector <uint> inds;
				inds.push_back(i);
				inds[0] %= nSize;
				Maths::clamp <uint> (inds[0], 1, nSize - 2);

				inds.push_back(j);
				inds[1] %= nSize;
				Maths::clamp <uint> (inds[1], 1, nSize - 2);

				pCloudMap[i * nSize + j] += (nSize + clouds.smooth(inds) * static_cast<float>((int)nSize - 1));
			}
		}


		return pCloudMap;
	}
//----------------------------------------------------------------------
	void NodeClouds::createCloudsPlane()
	{
		// Set vertices
		uint verticesRow = m_Desc.verticesCount;
		dword dwNumVertices = verticesRow * verticesRow;
		SVertex* pVertices = new SVertex[dwNumVertices];
		memset(pVertices, 0, dwNumVertices * sizeof(SVertex));

		float b = -m_Desc.domeHeight;

		float fRadius = m_Desc.domeRadius;
		float fRadiusDouble = fRadius * 2.0f;
		float fRadiusPerIter = fRadiusDouble / static_cast<float>(verticesRow - 1);

		uint index = 0;
		for(uint z = 0; z < verticesRow; ++z)
		{
			for(uint x = 0; x < verticesRow; ++x)
			{
				float X = x * fRadiusPerIter - fRadius;
				float Z = z * fRadiusPerIter - fRadius;
				pVertices[index].vecPosition.x = X;
				pVertices[index].vecPosition.z = Z;
				pVertices[index].vecPosition.y = b * (X * X + Z * Z);
				pVertices[index].vecTexCoord = Vector2((X + fRadius) / fRadiusDouble,
													   (Z + fRadius) / fRadiusDouble);
				++index;
			}
		}

		// Set indices
		dword dwNumIndices = (verticesRow - 1) * (verticesRow - 1) * 6;
		short* pIndices = new short[dwNumIndices];
		uint begin = 0;

		for(uint i = 0; i < dwNumIndices; i += 6)
		{
			pIndices[i + 0] = begin;
			pIndices[i + 1] = begin + 1;
			pIndices[i + 2] = begin + verticesRow + 1;

			pIndices[i + 3] = begin;
			pIndices[i + 4] = begin + verticesRow + 1;
			pIndices[i + 5] = begin + verticesRow;

			if((++begin + 1) % verticesRow == 0)
				++begin;
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

		Surface temp;
		temp.setVertexBuffer(vb);
		addSurface(L"Base", temp);

		NGENE_DELETE_ARRAY(pIndices);
		NGENE_DELETE_ARRAY(pVertices);
	}
//----------------------------------------------------------------------
	void NodeClouds::setDayTime(const TimeDate& _time)
	{
		m_Time = _time;

		float fPassed = _time.hours * 3600.0f + _time.minutes * 60.0f + _time.seconds;
		float fSunRise = m_SunRise.hours * 3600.0f + m_SunRise.minutes * 60.0f + m_SunRise.seconds;
		float fSunset = m_Sunset.hours * 3600.0f + m_Sunset.minutes * 60.0f + m_Sunset.seconds;
		float fDayLength = fSunset - fSunRise;

		m_SunAngle = (fPassed - fSunRise) / (fDayLength) * 180.0f;

		Maths::clamp <float> (m_SunAngle, 0.0f, 180.0f);

		updateColours();
		updateClouds();
	}
//----------------------------------------------------------------------
	void NodeClouds::onUpdate()
	{
		if(!m_bFixed)
		{
			Camera* pCamera = Engine::getSingleton().getActiveCamera();
			if(pCamera)
			{
				Vector3& cameraPos = pCamera->getPositionLocal();
				setPosition(cameraPos.x,
							getPositionLocal().y,
							cameraPos.z);

				Node::applyTransform();
			}
		}

		NodeVisible::onUpdate();

		// Update textures
		updateTextures();


		// No need to run simulation
		if(!m_bSimulate)
			return;

		dword delta = static_cast<float>(Engine::getSingleton().getTimer().getMilliseconds() - m_dwLastTime);

		Timer time = Engine::getSingleton().getTimer();
		m_dwLastTime = time.getMilliseconds();

		m_SunAngle += (float)delta * 0.18f / m_fDayLength;
		Maths::clamp_roll <float> (m_SunAngle, 0.0f, 180.0f);


		updateColours();
		updateClouds();
	}
//----------------------------------------------------------------------
	void NodeClouds::updateColours()
	{
		if(m_SunAngle <= 105.0f)
		{
			float fPassed = m_SunAngle / 75.0f;
			Maths::clamp(fPassed, 0.0f, 1.0f);
			m_SunColour.setRed(Maths::lerp <int>(205, 255, fPassed));
			m_SunColour.setGreen(Maths::lerp <int>(92, 255, fPassed));
			m_SunColour.setBlue(Maths::lerp <int>(92, 255, fPassed));
		}
		else if(m_SunAngle > 105.0f)
		{
			float fPassed = (m_SunAngle - 105.0f) / 75.0f;
			m_SunColour.setRed(Maths::lerp <int>(255, 255, fPassed));
			m_SunColour.setGreen(Maths::lerp <int>(255, 85, fPassed));
			m_SunColour.setBlue(Maths::lerp <int>(255, 20, fPassed));
		}
	}
//----------------------------------------------------------------------
	void NodeClouds::updateClouds()
	{
		float angle = m_SunAngle * Maths::PI / 180.0;

		bool bHDR = ((LightStage*)Renderer::getSingleton().getRenderStage(L"Light"))->isUsingHDR();
		Material* pMat = bHDR ? m_pCloudsMatHDR : m_pCloudsMat;

		if(bHDR)
		{
			getSurface(L"Base")->setEnabled(false);
			m_pCloudsMatHDR->setEnabled(true);
		}
		else
		{
			bool prev = getSurface(L"Base")->isEnabled();
			if(!prev)
			{
				ITexture* pTexture = TextureManager::getSingleton().getTexture(L"cloud_cover_map");
				pTexture->clearRenderTarget();

				getSurface(L"Base")->setEnabled(true);
				m_pCloudsMatHDR->setEnabled(false);
			}
		}


		if(pMat)
		{
			RenderPass& pass = pMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;


			Vector3 dir = Vector3(-cos(angle), -sin(angle), 0.0f);
			pConstant = pShader->getConstant("lightDir");
			if(pConstant)
				pConstant->setDefaultValue(dir.getData());

			float colour[3] = {(float)m_SunColour.getRed() / 255.0f,
							   (float)m_SunColour.getGreen() / 255.0f,
							   (float)m_SunColour.getBlue() / 255.0f};
			pConstant = pShader->getConstant("SunColor");
			if(pConstant)
				pConstant->setDefaultValue(colour);
		}
	}
//----------------------------------------------------------------------
	void NodeClouds::setCloudCover(float _value)
	{
		m_fCover = _value;

		if(m_pCloudsMat)
		{
			RenderPass& pass = m_pCloudsMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			float cover = _value;
			pConstant = pShader->getConstant("cloudCover");
			if(pConstant)
				pConstant->setDefaultValue(&cover);
		}

		if(m_pCloudsMatHDR)
		{
			RenderPass& pass = m_pCloudsMatHDR->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			float cover = _value;
			pConstant = pShader->getConstant("cloudCover");
			if(pConstant)
				pConstant->setDefaultValue(&cover);
		}

		if(m_pCloudsDensityMat)
		{
			RenderPass& pass = m_pCloudsDensityMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			float cover = _value;
			pConstant = pShader->getConstant("cloudCover");
			if(pConstant)
				pConstant->setDefaultValue(&cover);
		}
	}
//----------------------------------------------------------------------
	void NodeClouds::setCloudSharpness(float _value)
	{
		m_fSharpness = _value;

		if(m_pCloudsMat)
		{
			RenderPass& pass = m_pCloudsMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			float sharpness = _value;
			pConstant = pShader->getConstant("cloudSharpness");
			if(pConstant)
				pConstant->setDefaultValue(&sharpness);
		}

		if(m_pCloudsMatHDR)
		{
			RenderPass& pass = m_pCloudsMatHDR->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			float sharpness = _value;
			pConstant = pShader->getConstant("cloudSharpness");
			if(pConstant)
				pConstant->setDefaultValue(&sharpness);
		}

		if(m_pCloudsDensityMat)
		{
			RenderPass& pass = m_pCloudsDensityMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			float sharpness = _value;
			pConstant = pShader->getConstant("cloudSharpness");
			if(pConstant)
				pConstant->setDefaultValue(&sharpness);
		}
	}
//----------------------------------------------------------------------
	void NodeClouds::setTextureScaling(float _value)
	{
		m_fTextureScaling = _value;

		if(m_pCloudsMat)
		{
			RenderPass& pass = m_pCloudsMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("textureScaling");
			if(pConstant)
				pConstant->setDefaultValue(&m_fTextureScaling);
		}
		
		if(m_pCloudsMatHDR)
		{
			RenderPass& pass = m_pCloudsMatHDR->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("textureScaling");
			if(pConstant)
				pConstant->setDefaultValue(&m_fTextureScaling);
		}

		if(m_pCloudsDensityMat)
		{
			RenderPass& pass = m_pCloudsDensityMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("textureScaling");
			if(pConstant)
				pConstant->setDefaultValue(&m_fTextureScaling);
		}
	}
//----------------------------------------------------------------------
	void NodeClouds::setCloudDensity(float _value)
	{
		m_fDensity = _value;

		if(m_pCloudsMat)
		{
			RenderPass& pass = m_pCloudsMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("cloudDensity");
			if(pConstant)
				pConstant->setDefaultValue(&m_fDensity);
		}

		if(m_pCloudsMatHDR)
		{
			RenderPass& pass = m_pCloudsMatHDR->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("cloudDensity");
			if(pConstant)
				pConstant->setDefaultValue(&m_fDensity);
		}
	}
//----------------------------------------------------------------------
	void NodeClouds::setHorizonLevel(float _value)
	{
		m_fHorizonLevel = _value;

		if(m_pCloudsMat)
		{
			RenderPass& pass = m_pCloudsMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("horizonLevel");
			if(pConstant)
				pConstant->setDefaultValue(&m_fHorizonLevel);
		}

		if(m_pCloudsMatHDR)
		{
			RenderPass& pass = m_pCloudsMatHDR->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("horizonLevel");
			if(pConstant)
				pConstant->setDefaultValue(&m_fHorizonLevel);
		}
	}
//----------------------------------------------------------------------
	void NodeClouds::setHorizonHeight(float _value)
	{
		m_fHorizonHeight = _value;

		if(m_pCloudsMat)
		{
			RenderPass& pass = m_pCloudsMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("horizonHeight");
			if(pConstant)
				pConstant->setDefaultValue(&m_fHorizonHeight);
		}

		if(m_pCloudsMatHDR)
		{
			RenderPass& pass = m_pCloudsMatHDR->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("horizonHeight");
			if(pConstant)
				pConstant->setDefaultValue(&m_fHorizonHeight);
		}
	}
//----------------------------------------------------------------------
	void NodeClouds::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(_serializeType) _serializer->addObject(this->Type);
			Node::serialize(_serializer, false, true);
			
			Property <float> prDomeRadius(m_Desc.domeRadius);
			_serializer->addProperty("DomeRadius", prDomeRadius);
			Property <float> prDomeHeight(m_Desc.domeHeight);
			_serializer->addProperty("DomeHeight", prDomeHeight);
			Property <float> prDayLength(m_fDayLength);
			_serializer->addProperty("DayLength", prDayLength);
			Property <float> prCover(m_fCover);
			_serializer->addProperty("Cover", prCover);
			Property <float> prSharpness(m_fSharpness);
			_serializer->addProperty("Sharpness", prSharpness);
			Property <float> prDensity(m_fDensity);
			_serializer->addProperty("Density", prDensity);
			Property <float> prHorizonLevel(m_fHorizonLevel);
			_serializer->addProperty("HorizonLevel", prHorizonLevel);
			Property <float> prHorizonHeight(m_fHorizonHeight);
			_serializer->addProperty("HorizonHeight", prHorizonHeight);
			Property <float> prTexScale(m_fTextureScaling);
			_serializer->addProperty("TextureScaling", prTexScale);
			
			Property <bool> prSimulate(m_bSimulate);
			_serializer->addProperty("Simulated", prSimulate);
			Property <bool> prFixed(m_bFixed);
			_serializer->addProperty("Fixed", prFixed);

			Property <uint> prTexSize(m_Desc.textureSize);
			_serializer->addProperty("TextureSize", prTexSize);
			Property <uint> prVertsCount(m_Desc.verticesCount);
			_serializer->addProperty("VerticesCount", prVertsCount);

			Property <dword> prFrequency(m_dwUpdateFrequency);
			_serializer->addProperty("UpdateFrequency", prFrequency);

			Property <TimeDate> prTime(m_Time);
			_serializer->addProperty("Time", prTime);
			Property <TimeDate> prSunRise(m_SunRise);
			_serializer->addProperty("SunRise", prSunRise);
			Property <TimeDate> prSunset(m_Sunset);
			_serializer->addProperty("Sunset", prSunset);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeClouds::deserialize(ISerializer* _serializer)
	{
		Property <float> prDomeRadius(m_Desc.domeRadius);
		_serializer->getProperty("DomeRadius", prDomeRadius);
		Property <float> prDomeHeight(m_Desc.domeHeight);
		_serializer->getProperty("DomeHeight", prDomeHeight);
		Property <float> prDayLength(m_fDayLength);
		_serializer->getProperty("DayLength", prDayLength);
		Property <float> prCover(m_fCover);
		_serializer->getProperty("Cover", prCover);
		Property <float> prSharpness(m_fSharpness);
		_serializer->getProperty("Sharpness", prSharpness);
		Property <float> prDensity(m_fDensity);
		_serializer->getProperty("Density", prDensity);
		Property <float> prHorizonLevel(m_fHorizonLevel);
		_serializer->getProperty("HorizonLevel", prHorizonLevel);
		Property <float> prHorizonHeight(m_fHorizonHeight);
		_serializer->getProperty("HorizonHeight", prHorizonHeight);
		Property <float> prTexScale(m_fTextureScaling);
		_serializer->getProperty("TextureScaling", prTexScale);
		
		Property <bool> prSimulate(m_bSimulate);
		_serializer->getProperty("Simulated", prSimulate);
		Property <bool> prFixed(m_bFixed);
		_serializer->getProperty("Fixed", prFixed);

		Property <uint> prTexSize(m_Desc.textureSize);
		_serializer->getProperty("TextureSize", prTexSize);
		Property <uint> prVertsCount(m_Desc.verticesCount);
		_serializer->getProperty("VerticesCount", prVertsCount);

		Property <dword> prFrequency(m_dwUpdateFrequency);
		_serializer->getProperty("UpdateFrequency", prFrequency);

		Property <TimeDate> prTime(m_Time);
		_serializer->getProperty("Time", prTime);
		Property <TimeDate> prSunRise(m_SunRise);
		_serializer->getProperty("SunRise", prSunRise);
		Property <TimeDate> prSunset(m_Sunset);
		_serializer->getProperty("Sunset", prSunset);

		Property <bool> prLightable(m_bLightable);
		_serializer->getProperty("Lightable", prLightable);

		Node::deserialize(_serializer);
	}
//----------------------------------------------------------------------
}
}