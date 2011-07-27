/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeSky.cpp
Version:	0.12
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "NodeSky.h"

#include "Camera.h"
#include "ITexture.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "NodeLight.h"
#include "PrefabSphere.h"
#include "RenderPass.h"
#include "Shader.h"
#include "ShaderConstant.h"
#include "ShaderInstance.h"
#include "Surface.h"
#include "TextureManager.h"
#include "TimeDate.h"
#include "Timer.h"

#include <process.h>


namespace nGENE
{
namespace Nature
{
	// Initialize static members
	TypeInfo NodeSky::Type(L"NodeSky", &Node::Type);


	NodeSky::NodeSky():
		m_pLight(NULL),
		m_pSkyMat(NULL),
		m_pHazeMat(NULL),
		m_pSky(NULL),
		m_nShadowResolution(512),
		m_fSunSize(0.00001f),
		m_fSunBrightness(4000.0f),
		m_fDayLength(360.0f),
		m_bSimulate(true),
		m_bFixed(false),
		m_bCastShadows(true),
		m_fSkyRadius(500.0f),
		m_SunAngle(0.0f)
	{
		m_Scatter.setInscattering(0.02f);
		m_Scatter.setTurbidity(2.0f);

		m_SunRise.hours = 8;
		m_Sunset.hours = 22;
	}
//----------------------------------------------------------------------
	NodeSky::NodeSky(Real _radius):
		m_pLight(NULL),
		m_pSkyMat(NULL),
		m_pHazeMat(NULL),
		m_pSky(NULL),
		m_nShadowResolution(512),
		m_fSunSize(0.00001f),
		m_fSunBrightness(4000.0f),
		m_fDayLength(360.0f),
		m_bSimulate(true),
		m_bFixed(false),
		m_bCastShadows(true),
		m_fSkyRadius(_radius),
		m_SunAngle(0.0f)
	{
		m_Scatter.setInscattering(0.02f);
		m_Scatter.setTurbidity(2.0f);

		m_SunRise.hours = 8;
		m_Sunset.hours = 22;
	}
//----------------------------------------------------------------------
	NodeSky::~NodeSky()
	{
	}
//----------------------------------------------------------------------
	void NodeSky::init()
	{
		// Add subtask
		m_UpdateTask.setSky(this);
		addSubtask(&m_UpdateTask);

		m_pSkyMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"preetham_sky");
		m_pHazeMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"light_sun_preetham");

		// Create textures and samplers
		m_pS0Mie = TextureManager::getSingleton().createTexture(L"texS0Mie",
																36,
																1,
																TT_1D,
																TU_DYNAMIC,
																TF_A32B32G32R32F);
		m_pS0Ray = TextureManager::getSingleton().createTexture(L"texS0Ray",
																36,
																1,
																TT_1D,
																TU_DYNAMIC,
																TF_A32B32G32R32F);

		TextureSampler sampler;		
		sampler.setMipFilter(FILTER_NONE);		
		sampler.setMagFilter(FILTER_NEAREST);
		sampler.setMinFilter(FILTER_NEAREST);
		sampler.setTexture(m_pS0Mie);
		sampler.setTextureUnit(3);
		sampler.setAddressingMode(TAM_CLAMP);
		
		RenderPass& pass = m_pHazeMat->getActiveRenderTechnique()->getRenderPass(0);
		pass.addTextureSampler(sampler);

		TextureSampler sampler2;
		sampler2.setMipFilter(FILTER_NONE);	
		sampler2.setMagFilter(FILTER_NEAREST);
		sampler2.setMinFilter(FILTER_NEAREST);
		sampler2.setTexture(m_pS0Ray);
		sampler2.setTextureUnit(4);
		sampler2.setAddressingMode(TAM_CLAMP);
		pass.addTextureSampler(sampler2);

		if(m_pCreator)
		{
			// Add sky dome
			m_pSky = m_pCreator->createSphere(40, 40, m_fSkyRadius, Vector2::ONE_VECTOR, Maths::PI_HALF);
			m_pSky->setSerializable(false);
			addChild(L"SkyDome", *m_pSky);
			Surface* surf = m_pSky->getSurface(L"Base");
			surf->setMaterial(m_pSkyMat);
			surf->flipNormals();
			surf->flipWindingOrder();


			// Add sun light
			m_pLight = m_pCreator->createLight(LT_DIRECTIONAL);
			m_pLight->setPosition(0.0f, 0.0f, 0.0f);
			m_pLight->setDirection(Vector3(0.0f, -1.0f, 0.05f));
			m_pLight->setRadius(m_fSkyRadius);
			m_pLight->setMaterial(m_pHazeMat);
			m_pLight->setDrawDebug(false);
			m_pLight->setSerializable(false);
			addChild(L"SunLight", *m_pLight);
			Renderer::getSingleton().addLight(m_pLight);

			m_pLight->setCastShadows(m_bCastShadows);
			m_pLight->setShadowResolution(m_nShadowResolution);
		}

		// Simulate at least once
		m_UpdateTask.requestUpdate();

		setSunBrightness(m_fSunBrightness);
		setSunSize(m_fSunSize);

		m_dwLastTime = Engine::getSingleton().getTimer().getMilliseconds();
	}
//----------------------------------------------------------------------
	void NodeSky::updateMTInternal()
	{		
		updateSunAndSky();		
	}
//----------------------------------------------------------------------
	void NodeSky::onUpdate()
	{
		if(m_pSky && !m_bFixed)
		{
			Camera* pCamera = Engine::getSingleton().getActiveCamera();
			if(pCamera)
			{
				Vector3& cameraPos = pCamera->getPositionLocal();
				m_vecPosition.set(cameraPos.x,
								  getPositionLocal().y,
								  cameraPos.z);
				Node::m_bHasChanged = true;
			}

			Node::applyTransform();
		}

		Node::onUpdate();


		// No need to run simulation
		if(!m_bSimulate)
			return;


		float delta = static_cast<float>(Engine::getSingleton().getTimer().getMilliseconds() - m_dwLastTime);

		Timer time = Engine::getSingleton().getTimer();
		m_dwLastTime = time.getMilliseconds();

		m_SunAngle += delta * 0.18f / m_fDayLength;
		Maths::clamp_roll <float> (m_SunAngle, 0.0f, 180.0f);
	

		m_PrevSunAngle = m_SunAngle;		
	}
//----------------------------------------------------------------------
	void NodeSky::requestUpdate()
	{
		bool oldSimulate = m_bSimulate;
		float sunAngle = m_SunAngle;

		m_bSimulate = true;

		m_UpdateTask.requestUpdate();

		m_bSimulate = oldSimulate;
		m_SunAngle = sunAngle;
	}
//----------------------------------------------------------------------
	void NodeSky::setDayTime(const TimeDate& _time)
	{
		m_Time = _time;

		float fPassed = _time.hours * 3600.0f + _time.minutes * 60.0f + _time.seconds;
		float fSunRise = m_SunRise.hours * 3600.0f + m_SunRise.minutes * 60.0f + m_SunRise.seconds;
		float fSunset = m_Sunset.hours * 3600.0f + m_Sunset.minutes * 60.0f + m_Sunset.seconds;
		float fDayLength = fSunset - fSunRise;

		m_SunAngle = (fPassed - fSunRise) / (fDayLength) * 180.0f;

		Maths::clamp <float> (m_SunAngle, 0.0f, 180.0f);

		m_UpdateTask.requestUpdate();
	}
//----------------------------------------------------------------------
	void NodeSky::setSunSize(Real _size)
	{
		m_fSunSize = _size;

		if(m_pSkyMat)
		{
			RenderPass& pass = m_pSkyMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("sunSize");
			if(pConstant)
				pConstant->setDefaultValue(&m_fSunSize);
		}
	}
//----------------------------------------------------------------------
	void NodeSky::setSunBrightness(Real _brightness)
	{
		m_fSunBrightness = _brightness;

		if(m_pSkyMat)
		{
			RenderPass& pass = m_pSkyMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("sunBrightness");
			if(pConstant)
				pConstant->setDefaultValue(&m_fSunBrightness);
		}
	}
//----------------------------------------------------------------------
	void NodeSky::updateSunAndSky()
	{
		float angle = m_SunAngle * Maths::PI / 180.0f;

		// Update scattering terms
		m_Scatter.setSunAngle(angle);

		// Update light properties
		Vector3 sunDir = m_Scatter.getSunLightDirection();
		m_pLight->setDirection(-sunDir);		
	}
//----------------------------------------------------------------------
	void NodeSky::ProcessData()
	{		
		float angle = m_SunAngle * Maths::PI / 180.0f;
		Vector3 sunDir = m_Scatter.getSunLightDirection();
		
		Vector3* pSrc = m_Scatter.getS0Mie();		
		Vector4 temp[36];
	
		uint i = 0;
		for(i = 0; i < 36; ++i)
		{
			temp[i].x = pSrc[i].x;
			temp[i].y = pSrc[i].y;
			temp[i].z = pSrc[i].z;				
			temp[i].w = 1.0f;			
		}	

		// Set the texture		
		m_pS0Mie->setDataFloat(&temp[0], sizeof(Vector4) * 36);	
		Vector3* pSrcRay = m_Scatter.getS0Rayleigh();
		for(i = 0; i < 36; ++i)
		{
			temp[i].x = pSrcRay[i].x;
			temp[i].y = pSrcRay[i].y;
			temp[i].z = pSrcRay[i].z;
			temp[i].w = 1.0f;
		}

		// Set the texture		
		m_pS0Ray->setDataFloat(&temp[0], sizeof(Vector4) * 36);	
		
		if(m_pSkyMat)
		{
			RenderPass& pass = m_pSkyMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("a_exposure");
			float exposure = m_Scatter.getExposure();
			if(pConstant)
				pConstant->setDefaultValue(&exposure);

			pConstant = pShader->getConstant("vecSunDirection");
			if(pConstant)
				pConstant->setDefaultValue(sunDir.getData());

			float theta[2] = {angle, cos(angle) * cos(angle)};
			pConstant = pShader->getConstant("vecThetaSun");
			if(pConstant)
				pConstant->setDefaultValue(theta);

			pConstant = pShader->getConstant("vecZenithColor");
			if(pConstant)
				pConstant->setDefaultValue(m_Scatter.getZenithColor().getData());
		}
		
		if(m_pHazeMat)
		{
			RenderPass& pass = m_pHazeMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("a_betaMie");
			if(pConstant)
				pConstant->setDefaultValue(m_Scatter.getMieFactor().getData());

			pConstant = pShader->getConstant("a_betaRay");
			
			if(pConstant)
				pConstant->setDefaultValue(m_Scatter.getRayleighFactor().getData());

			Camera* cam = Engine::getSingleton().getActiveCamera();
			float H1 = static_cast<float>(exp(-m_Scatter.getAlphaFactors().x * cam->getPositionLocal().y));
			float H2 = static_cast<float>(exp(-m_Scatter.getAlphaFactors().y * cam->getPositionLocal().y));

			Vector3& vecMie = m_Scatter.getMieFactor();
			Vector3& vecRay = m_Scatter.getRayleighFactor();
			float cplus[3] = {vecMie.x * H1 + vecRay.x * H2, 
							  vecMie.y * H1 + vecRay.y * H2, 
							  vecMie.z * H1 + vecRay.z * H2};
			pConstant = pShader->getConstant("a_C1plusC2");
			if(pConstant)
				pConstant->setDefaultValue(cplus);
			
			float constants[3] = {H1, H2, m_Scatter.getInscattering()};
			pConstant = pShader->getConstant("a_constants");
			if(pConstant)
				pConstant->setDefaultValue(constants);
			
			pShader = pass.getVertexShader()->getShader();
			pConstant = pShader->getConstant("lightDir");
			if(pConstant)
			{
				sunDir.negate();
				pConstant->setDefaultValue(sunDir.getData());
			}
		}

		// Change shadow density
		float density = 0.3f;
		if(m_SunAngle < 80.0f)
			density = Maths::lerp(1.0f, 0.3f, m_SunAngle / 80.0f);
		else if(m_SunAngle > 100.0f)
			density = Maths::lerp(0.3f, 1.0f, (m_SunAngle - 100.0f) / 80.f);
		m_pLight->setShadowDensity(density);
	}
//----------------------------------------------------------------------
	void NodeSky::setShadowResolution(uint _value)
	{
		m_pLight->setShadowResolution(_value);
		m_nShadowResolution = _value;
	}
//----------------------------------------------------------------------
	uint NodeSky::getShadowResolution() const
	{
		return m_nShadowResolution;
	}
//----------------------------------------------------------------------
	void NodeSky::setCastShadows(bool _value)
	{
		m_pLight->setCastShadows(_value);
		m_bCastShadows = _value;
	}
//----------------------------------------------------------------------
	bool NodeSky::isCastingShadows() const
	{
		return m_bCastShadows;
	}
//----------------------------------------------------------------------
	void NodeSky::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(_serializeType) _serializer->addObject(this->Type);

			Node::serialize(_serializer, false, true);
			
			Property <float> prSunAngle(m_SunAngle);
			_serializer->addProperty("SunAngle", prSunAngle);
			Property <float> prSunSize(m_fSunSize);
			_serializer->addProperty("SunSize", prSunSize);
			Property <float> prSunBrightness(m_fSunBrightness);
			_serializer->addProperty("SunBrightness", prSunBrightness);
			Property <float> prDayLength(m_fDayLength);
			_serializer->addProperty("DayLength", prDayLength);
			Property <float> prSkyRadius(m_fSkyRadius);
			_serializer->addProperty("SkyRadius", prSkyRadius);

			Property <bool> prSimulate(m_bSimulate);
			_serializer->addProperty("Simulated", prSimulate);
			Property <bool> prFixed(m_bFixed);
			_serializer->addProperty("Fixed", prFixed);
				
			Property <TimeDate> prTime(m_Time);
			_serializer->addProperty("Time", prTime);
			Property <TimeDate> prSunRise(m_SunRise);
			_serializer->addProperty("SunRise", prSunRise);
			Property <TimeDate> prSunset(m_Sunset);
			_serializer->addProperty("Sunset", prSunset);

			// Serialize scattering related properties
			float fTmp = m_Scatter.getExposure();
			Property <float> prExposure(fTmp);
			_serializer->addProperty("Exposure", prExposure);

			fTmp = m_Scatter.getInscattering();
			Property <float> prInscattering(fTmp);
			_serializer->addProperty("Inscatter", prInscattering);

			double dTmp = m_Scatter.getTurbidity();
			Property <double> prTurbidity(dTmp);
			_serializer->addProperty("Turbidity", prTurbidity);

			// Serialize shadow related properties
			bool bCastShadows = m_bCastShadows;
			Property <bool> prCastShadows(bCastShadows);
			_serializer->addProperty("CastsShadows", prCastShadows);

			uint nShadowRes = m_nShadowResolution;
			Property <uint> prShadowRes(nShadowRes);
			_serializer->addProperty("ShadowResolution", prShadowRes);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeSky::deserialize(ISerializer* _serializer)
	{
		Property <float> prSunAngle(m_SunAngle);
		_serializer->getProperty("SunAngle", prSunAngle);
		Property <float> prSunSize(m_fSunSize);
		_serializer->getProperty("SunSize", prSunSize);
		Property <float> prSunBrightness(m_fSunBrightness);
		_serializer->getProperty("SunBrightness", prSunBrightness);
		Property <float> prDayLength(m_fDayLength);
		_serializer->getProperty("DayLength", prDayLength);
		Property <float> prSkyRadius(m_fSkyRadius);
		_serializer->getProperty("SkyRadius", prSkyRadius);

		Property <bool> prSimulate(m_bSimulate);
		_serializer->getProperty("Simulated", prSimulate);
		Property <bool> prFixed(m_bFixed);
		_serializer->getProperty("Fixed", prFixed);
			
		Property <TimeDate> prTime(m_Time);
		_serializer->getProperty("Time", prTime);
		Property <TimeDate> prSunRise(m_SunRise);
		_serializer->getProperty("SunRise", prSunRise);
		Property <TimeDate> prSunset(m_Sunset);
		_serializer->getProperty("Sunset", prSunset);

		// Deserialize scattering data
		float fTmp;
		Property <float> prExposure(fTmp);
		_serializer->getProperty("Exposure", prExposure);
		m_Scatter.setExposure(fTmp);

		Property <float> prInscattering(fTmp);
		_serializer->getProperty("Inscatter", prInscattering);
		m_Scatter.setInscattering(fTmp);

		double dTmp;
		Property <double> prTurbidity(dTmp);
		_serializer->getProperty("Turbidity", prTurbidity);
		m_Scatter.setTurbidity(dTmp);

		// Deserialize shadow related properties
		Property <bool> prCastShadows(m_bCastShadows);
		_serializer->getProperty("CastsShadows", prCastShadows);

		Property <uint> prShadowRes(m_nShadowResolution);
		_serializer->getProperty("ShadowResolution", prShadowRes);


		Node::deserialize(_serializer);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	NodeSky::NodeSkyUpdateDataTask::NodeSkyUpdateDataTask():
		m_pSky(NULL),
		m_bUpdate(false)
	{
	}
//----------------------------------------------------------------------
	void NodeSky::NodeSkyUpdateDataTask::setSky(NodeSky* _sky)
	{
		m_pSky = _sky;
	}
//----------------------------------------------------------------------
	void NodeSky::NodeSkyUpdateDataTask::update()
	{
		if(!m_pSky->isSimulated() && !m_bUpdate)
			return;

		m_pSky->updateMTInternal();
		m_pSky->ProcessData();

		m_bUpdate = false;
	}
//----------------------------------------------------------------------
	void NodeSky::NodeSkyUpdateDataTask::requestUpdate()
	{
		m_bUpdate = true;
	}
//----------------------------------------------------------------------
}
}
