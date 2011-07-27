/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeWater.cpp
Version:	0.07
---------------------------------------------------------------------------
*/

#include "NodeWater.h"
#include "ISerializer.h"
#include "ReflectionStage.h"

#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "PostStage.h"
#include "Shader.h"
#include "ShaderConstant.h"
#include "ShaderInstance.h"
#include "TimeDate.h"
#include "Timer.h"


namespace nGENE
{
namespace Nature
{
	// Initialize static members
	TypeInfo NodeWater::Type(L"NodeWater", &Node::Type);


	NodeWater::NodeWater(ReflectionStage* _stage):
		m_pWaterMat(NULL),
		m_fDayLength(360.0f),
		m_fAmplitude(1.0f),
		m_fRefractionIndex(1.33333f),
		m_fWaterLevel(0.0f),
		m_fHorizontalVisibility(4.0f),
		m_bSimulate(true),
		m_pReflectionStage(_stage),
		m_vecWind(-0.3f, 0.7f),
		m_vecWavesScale(0.005f, 0.005f),
		m_vecFoam(0.65f, 1.35f, 0.5f),
		m_vecExtinction(7.0f, 30.0f, 40.0f),
		m_fMuddiness(0.15f),
		m_fRefractionStrength(0.0f),
		m_fRefractionScale(0.005f),
		m_fShoreHardness(1.0f),
		m_fShininess(0.7f),
		m_fNormalScale(1.0f),
		m_fDisplace(1.7f),
		m_SunAngle(0.0f)
	{
		m_SunRise.hours = 8;
		m_Sunset.hours = 22;
	}
//----------------------------------------------------------------------
	NodeWater::~NodeWater()
	{
		// Release reflection
		ReflectionStage::releaseReflection();
	}
//----------------------------------------------------------------------
	void NodeWater::init()
	{
		m_pWaterMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"water");

		// Add water material
		m_PostWater.priority = 0;
		m_PostWater.material = m_pWaterMat;
		((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"))->addToRender(&m_PostWater);

		// Bind default reflection stage if not set already
		if(!m_pReflectionStage)
			m_pReflectionStage = (ReflectionStage*)Renderer::getSingleton().getRenderStage(L"Reflection");

		// Set default properties
		setWavesAmplitude(m_fAmplitude);
		setDepthColour(m_DepthColour);
		setDisplacement(m_fDisplace);
		setExtinction(m_vecExtinction);
		setFoam(m_vecFoam);
		setHorizontalVisibility(m_fHorizontalVisibility);
		setMuddiness(m_fMuddiness);
		setNormalScale(m_fNormalScale);
		setRefractionIndex(m_fRefractionIndex);
		setRefractionScale(m_fRefractionScale);
		setRefractionStrength(m_fRefractionStrength);
		setShininess(m_fShininess);
		setShoreHardness(m_fShoreHardness);
		setSurfaceColour(m_SurfaceColour);
		setWaterLevel(m_fWaterLevel);
		setWavesScale(m_vecWavesScale);
		setWind(m_vecWind);
		if(!m_bSimulate)
			setDayTime(m_Time);

		m_dwLastTime = Engine::getSingleton().getTimer().getMilliseconds();

		// Request reflections
		ReflectionStage::requestReflection();

		// Simulate at least once
		update();
	}
//----------------------------------------------------------------------
	void NodeWater::setDayTime(const TimeDate& _time)
	{
		m_Time = _time;

		float fPassed = _time.hours * 3600.0f + _time.minutes * 60.0f + _time.seconds;
		float fSunRise = m_SunRise.hours * 3600.0f + m_SunRise.minutes * 60.0f + m_SunRise.seconds;
		float fSunset = m_Sunset.hours * 3600.0f + m_Sunset.minutes * 60.0f + m_Sunset.seconds;
		float fDayLength = fSunset - fSunRise;

		m_SunAngle = (fPassed - fSunRise) / (fDayLength) * 180.0f;

		Maths::clamp <float> (m_SunAngle, 0.0f, 180.0f);

		updateColours();
		updateWater();
	}
//----------------------------------------------------------------------
	void NodeWater::onUpdate()
	{
		// No need to run simulation
		if(!m_bSimulate)
			return;


		float delta = static_cast<float>(Engine::getSingleton().getTimer().getMilliseconds() - m_dwLastTime);

		Timer time = Engine::getSingleton().getTimer();
		m_dwLastTime = time.getMilliseconds();

		m_SunAngle += delta * 0.18f / m_fDayLength;
		Maths::clamp_roll <float> (m_SunAngle, 0.0f, 180.0f);

		updateWater();
		updateColours();
	}
//----------------------------------------------------------------------
	void NodeWater::updateColours()
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
			m_SunColour.setGreen(Maths::lerp <int>(255, 145, fPassed));
			m_SunColour.setBlue(Maths::lerp <int>(255, 20, fPassed));
		}
	}
//----------------------------------------------------------------------
	void NodeWater::updateWater()
	{
		float angle = m_SunAngle * Maths::PI / 180.0f;


		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("lightDir");
			if(pConstant)
			{
				Vector3 dir = Vector3(-cos(angle), -sin(angle), 0.0f);
				dir.normalize();

				pConstant->setDefaultValue(dir.getData());
			}

			pConstant = pShader->getConstant("sunColor");
			if(pConstant)
			{
				float colour[3] = {m_SunColour.getRedF(),
								   m_SunColour.getGreenF(),
								   m_SunColour.getBlueF()};
				pConstant->setDefaultValue(colour);
			}
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setWavesAmplitude(Real _amplitude)
	{
		m_fAmplitude = _amplitude;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("maxAmplitude");
			if(pConstant)
				pConstant->setDefaultValue(&m_fAmplitude);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setRefractionIndex(Real _index)
	{
		m_fRefractionIndex = _index;
		float R0 = powf(1.0f - _index, 2.0f) / powf(1.0f + _index, 2.0f);

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("R0");
			if(pConstant)
				pConstant->setDefaultValue(&R0);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setWaterLevel(Real _level)
	{
		m_fWaterLevel = _level;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("waterLevel");
			if(pConstant)
				pConstant->setDefaultValue(&m_fWaterLevel);
		}

		if(m_pReflectionStage)
			m_pReflectionStage->setReflectionPlane(0.0f, 1.0f, 0.0f, -m_fWaterLevel);
	}
//----------------------------------------------------------------------
	void NodeWater::setSurfaceColour(const Colour& _colour)
	{
		m_SurfaceColour = _colour;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			float colour[3] = {m_SurfaceColour.getRedF(),
							   m_SurfaceColour.getGreenF(),
							   m_SurfaceColour.getBlueF()};
			pConstant = pShader->getConstant("depthColour");
			if(pConstant)
				pConstant->setDefaultValue(colour);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setDepthColour(const Colour& _colour)
	{
		m_DepthColour = _colour;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			float colour[3] = {m_DepthColour.getRedF(),
							   m_DepthColour.getGreenF(),
							   m_DepthColour.getBlueF()};
			pConstant = pShader->getConstant("bigDepthColour");
			if(pConstant)
				pConstant->setDefaultValue(colour);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setHorizontalVisibility(Real _visibility)
	{
		m_fHorizontalVisibility = _visibility;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("visibility");
			if(pConstant)
				pConstant->setDefaultValue(&m_fHorizontalVisibility);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setEnabled(bool _value)
	{
		if(_value && !m_bEnabled)
			ReflectionStage::requestReflection();
		else if(!_value && m_bEnabled)
			ReflectionStage::releaseReflection();

		m_pWaterMat->setEnabled(_value);

		Node::setEnabled(_value);
	}
//----------------------------------------------------------------------
	void NodeWater::setReflectionStage(ReflectionStage* _stage)
	{
		m_pReflectionStage = _stage;
	}
//----------------------------------------------------------------------
	ReflectionStage* NodeWater::getReflectionStage() const
	{
		return m_pReflectionStage;
	}
//----------------------------------------------------------------------
	void NodeWater::setMaterial(Material* _material)
	{
		// Remove previous material
		PostStage* pStage = ((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"));
		if(m_pWaterMat)
			pStage->removeFromRender(&m_PostWater);

		// Add water material
		m_pWaterMat = _material;
		m_PostWater.priority = 0;
		m_PostWater.material = m_pWaterMat;
		pStage->addToRender(&m_PostWater);

		// Set default properties
		setWavesAmplitude(m_fAmplitude);
		setDepthColour(m_DepthColour);
		setDisplacement(m_fDisplace);
		setExtinction(m_vecExtinction);
		setFoam(m_vecFoam);
		setHorizontalVisibility(m_fHorizontalVisibility);
		setMuddiness(m_fMuddiness);
		setNormalScale(m_fNormalScale);
		setRefractionIndex(m_fRefractionIndex);
		setRefractionScale(m_fRefractionScale);
		setRefractionStrength(m_fRefractionStrength);
		setShininess(m_fShininess);
		setShoreHardness(m_fShoreHardness);
		setSurfaceColour(m_SurfaceColour);
		setWaterLevel(m_fWaterLevel);
		setWavesScale(m_vecWavesScale);
		setWind(m_vecWind);
		if(!m_bSimulate)
			setDayTime(m_Time);

		m_dwLastTime = Engine::getSingleton().getTimer().getMilliseconds();

		// Simulate at least once
		update();
	}
//----------------------------------------------------------------------
	void NodeWater::setPosition(const Vector3& _pos)
	{
		Node::setPosition(_pos);
		setWaterLevel(_pos.y);
	}
//----------------------------------------------------------------------
	void NodeWater::setPosition(Real _x, Real _y, Real _z)
	{
		Node::setPosition(_x, _y, _z);
		setWaterLevel(_y);
	}
//----------------------------------------------------------------------
	void NodeWater::setFoam(const Vector3& _foam)
	{
		m_vecFoam = _foam;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("foamExistence");
			if(pConstant)
				pConstant->setDefaultValue(m_vecFoam.getData());
		}
	}
//----------------------------------------------------------------------
	Vector3& NodeWater::getFoam()
	{
		return m_vecFoam;
	}
//----------------------------------------------------------------------
	void NodeWater::setMuddiness(Real _value)
	{
		m_fMuddiness = _value;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("fadeSpeed");
			if(pConstant)
				pConstant->setDefaultValue(&m_fMuddiness);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setRefractionStrength(Real _value)
	{
		m_fRefractionStrength = _value;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("refractionStrength");
			if(pConstant)
				pConstant->setDefaultValue(&m_fRefractionStrength);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setShoreHardness(Real _value)
	{
		m_fShoreHardness = _value;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("shoreHardness");
			if(pConstant)
				pConstant->setDefaultValue(&m_fShoreHardness);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setShininess(Real _shininess)
	{
		m_fShininess = _shininess;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("shininess");
			if(pConstant)
				pConstant->setDefaultValue(&m_fShininess);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setDisplacement(Real _displacement)
	{
		m_fDisplace = _displacement;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("displace");
			if(pConstant)
				pConstant->setDefaultValue(&m_fDisplace);
		}
	}
//----------------------------------------------------------------------
	Vector3& NodeWater::getExtinction()
	{
		return m_vecExtinction;
	}
//----------------------------------------------------------------------
	void NodeWater::setExtinction(const Vector3& _extinction)
	{
		m_vecExtinction = _extinction;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("extinction");
			if(pConstant)
				pConstant->setDefaultValue(m_vecExtinction.getData());
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setWind(const Vector2& _wind)
	{
		m_vecWind = _wind;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("wind");
			if(pConstant)
				pConstant->setDefaultValue(m_vecWind.getData());
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setWavesScale(const Vector2& _scale)
	{
		m_vecWavesScale = _scale;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("scale");
			if(pConstant)
				pConstant->setDefaultValue(m_vecWavesScale.getData());
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setRefractionScale(Real _scale)
	{
		m_fRefractionScale = _scale;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("refractionScale");
			if(pConstant)
				pConstant->setDefaultValue(&m_fRefractionScale);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::setNormalScale(Real _scale)
	{
		m_fNormalScale = _scale;

		if(m_pWaterMat)
		{
			RenderPass& pass = m_pWaterMat->getActiveRenderTechnique()->getRenderPass(0);
			Shader* pShader = pass.getPixelShader()->getShader();
			ShaderConstant* pConstant = NULL;

			pConstant = pShader->getConstant("normalScale");
			if(pConstant)
				pConstant->setDefaultValue(&m_fNormalScale);
		}
	}
//----------------------------------------------------------------------
	void NodeWater::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(_serializeType) _serializer->addObject(this->Type);
			Node::serialize(_serializer, false, _serializeChildren);
			
			Property <float> prSunAngle(m_SunAngle);
			_serializer->addProperty("SunAngle", prSunAngle);
			Property <float> prDayLength(m_fDayLength);
			_serializer->addProperty("DayLength", prDayLength);
			Property <float> prAmplitude(m_fAmplitude);
			_serializer->addProperty("Amplitude", prAmplitude);
			Property <float> prMuddiness(m_fMuddiness);
			_serializer->addProperty("Muddiness", prMuddiness);
			Property <float> prRefractionStrength(m_fRefractionStrength);
			_serializer->addProperty("RefractionStrength", prRefractionStrength);
			Property <float> prRefractionIndex(m_fRefractionIndex);
			_serializer->addProperty("RefractionIndex", prRefractionIndex);
			Property <float> prRefractionScale(m_fRefractionScale);
			_serializer->addProperty("RefractionScale", prRefractionScale);
			Property <float> prWaterLevel(m_fWaterLevel);
			_serializer->addProperty("WaterLevel", prWaterLevel);
			Property <float> prShoreHardness(m_fShoreHardness);
			_serializer->addProperty("ShoreHardness", prShoreHardness);
			Property <float> prShininess(m_fShininess);
			_serializer->addProperty("Shininess", prShininess);
			Property <float> prDisplace(m_fDisplace);
			_serializer->addProperty("Displace", prDisplace);
			Property <float> prNormalScale(m_fNormalScale);
			_serializer->addProperty("NormalScale", prNormalScale);
			Property <float> prHorizontalVisibility(m_fHorizontalVisibility);
			_serializer->addProperty("HorizontalVisibility", prHorizontalVisibility);

			Property <Vector2> prWind(m_vecWind);
			_serializer->addProperty("Wind", prWind);
			Property <Vector2> prWavesScale(m_vecWavesScale);
			_serializer->addProperty("WavesScale", prWavesScale);

			Property <Vector3> prFoam(m_vecFoam);
			_serializer->addProperty("Foam", prFoam);
			Property <Vector3> prExtinction(m_vecExtinction);
			_serializer->addProperty("Extinction", prExtinction);

			Property <Colour> prSurfaceColour(m_SurfaceColour);
			_serializer->addProperty("SurfaceColour", prSurfaceColour);
			Property <Colour> prDepthColour(m_DepthColour);
			_serializer->addProperty("DepthColour", prDepthColour);

			Property <bool> prSimulate(m_bSimulate);
			_serializer->addProperty("Simulated", prSimulate);
				
			Property <TimeDate> prTime(m_Time);
			_serializer->addProperty("Time", prTime);
			Property <TimeDate> prSunRise(m_SunRise);
			_serializer->addProperty("SunRise", prSunRise);
			Property <TimeDate> prSunset(m_Sunset);
			_serializer->addProperty("Sunset", prSunset);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeWater::deserialize(ISerializer* _serializer)
	{
		Property <float> prSunAngle(m_SunAngle);
		_serializer->getProperty("SunAngle", prSunAngle);
		Property <float> prDayLength(m_fDayLength);
		_serializer->getProperty("DayLength", prDayLength);
		Property <float> prAmplitude(m_fAmplitude);
		_serializer->getProperty("Amplitude", prAmplitude);
		Property <float> prMuddiness(m_fMuddiness);
		_serializer->getProperty("Muddiness", prMuddiness);
		Property <float> prRefractionStrength(m_fRefractionStrength);
		_serializer->getProperty("RefractionStrength", prRefractionStrength);
		Property <float> prRefractionIndex(m_fRefractionIndex);
		_serializer->getProperty("RefractionIndex", prRefractionIndex);
		Property <float> prRefractionScale(m_fRefractionScale);
		_serializer->getProperty("RefractionScale", prRefractionScale);
		Property <float> prWaterLevel(m_fWaterLevel);
		_serializer->getProperty("WaterLevel", prWaterLevel);
		Property <float> prShoreHardness(m_fShoreHardness);
		_serializer->getProperty("ShoreHardness", prShoreHardness);
		Property <float> prShininess(m_fShininess);
		_serializer->getProperty("Shininess", prShininess);
		Property <float> prDisplace(m_fDisplace);
		_serializer->getProperty("Displace", prDisplace);
		Property <float> prNormalScale(m_fNormalScale);
		_serializer->getProperty("NormalScale", prNormalScale);
		Property <float> prHorizontalVisibility(m_fHorizontalVisibility);
		_serializer->getProperty("HorizontalVisibility", prHorizontalVisibility);

		Property <Vector2> prWind(m_vecWind);
		_serializer->getProperty("Wind", prWind);
		Property <Vector2> prWavesScale(m_vecWavesScale);
		_serializer->getProperty("WavesScale", prWavesScale);

		Property <Vector3> prFoam(m_vecFoam);
		_serializer->getProperty("Foam", prFoam);
		Property <Vector3> prExtinction(m_vecExtinction);
		_serializer->getProperty("Extinction", prExtinction);

		Property <Colour> prSurfaceColour(m_SurfaceColour);
		_serializer->getProperty("SurfaceColour", prSurfaceColour);
		Property <Colour> prDepthColour(m_DepthColour);
		_serializer->getProperty("DepthColour", prDepthColour);

		Property <bool> prSimulate(m_bSimulate);
		_serializer->getProperty("Simulated", prSimulate);
			
		Property <TimeDate> prTime(m_Time);
		_serializer->getProperty("Time", prTime);
		Property <TimeDate> prSunRise(m_SunRise);
		_serializer->getProperty("SunRise", prSunRise);
		Property <TimeDate> prSunset(m_Sunset);
		_serializer->getProperty("Sunset", prSunset);

		Node::deserialize(_serializer);
	}
//----------------------------------------------------------------------
}
}