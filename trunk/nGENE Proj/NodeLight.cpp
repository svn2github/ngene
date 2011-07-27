/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeLight.cpp
Version:	0.21
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "AABB.h"
#include "NodeLight.h"
#include "NodePrefab.h"
#include "OBB.h"
#include "ScenePartitioner.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MaterialLibrary.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo NodeLight::Type(L"NodeLight", &Node::Type);


	NodeLight::NodeLight():
		m_LightType(LT_DIRECTIONAL),
		m_pMaterial(NULL),
		m_pPrefab(NULL),
		m_nShadowResolution(512),
		m_fIntensity(1.0f),
		m_fFov(Maths::PI * 0.25f),
		m_fShadowBias(1.0f),
		m_fShadowDensity(0.2f),
		m_fSpotExponent(20.0f),
		m_fConstantAttenuation(0.1f),
		m_fLinearAttenuation(0.05f),
		m_fQuadraticAttenuation(0.025f),
		m_bCastShadows(true),
		m_bDrawDebug(false),
		m_bIndividualShadow(false),
		m_bStaticShadow(false),
		m_bRequestedShadow(true),
		m_vecDirection(Vector3::NEGATIVE_UNIT_Y),
		m_vecWorldDirection(Vector3::NEGATIVE_UNIT_Y),
		m_vecRadius(5.0f, 5.0f, 5.0f)
	{
	}
//----------------------------------------------------------------------
	NodeLight::NodeLight(const LIGHT_TYPE& _lightType):
		m_LightType(_lightType),
		m_pMaterial(NULL),
		m_pPrefab(NULL),
		m_nShadowResolution(512),
		m_fIntensity(1.0f),
		m_fFov(Maths::PI * 0.25f),
		m_fShadowBias(1.0f),
		m_fShadowDensity(0.2f),
		m_fSpotExponent(20.0f),
		m_fConstantAttenuation(0.1f),
		m_fLinearAttenuation(0.05f),
		m_fQuadraticAttenuation(0.025f),
		m_bCastShadows(true),
		m_bDrawDebug(false),
		m_bIndividualShadow(false),
		m_bStaticShadow(false),
		m_bRequestedShadow(true),
		m_vecDirection(Vector3::NEGATIVE_UNIT_Y),
		m_vecWorldDirection(Vector3::NEGATIVE_UNIT_Y),
		m_vecRadius(5.0f, 5.0f, 5.0f)
	{
	}
//----------------------------------------------------------------------
	NodeLight::~NodeLight()
	{
		cleanup();
	}
//---------------------------------------------------------------------
	void NodeLight::cleanup()
	{
		// We don't need to free memory allocated for prefab.
		// It will be released by Node's destructor.
	}
//---------------------------------------------------------------------
	void NodeLight::init()
	{
		setLightType(m_LightType);

		calculateBoundingBox();
	}
//---------------------------------------------------------------------
	void NodeLight::onUpdate()
	{
		Node::onUpdate();

		ICullable::setChanged(Node::m_bHasChanged);

		if(Node::m_bHasChanged)
		{
			m_vecWorldPosition = Vector3(m_matTransform._m41,
										 m_matTransform._m42,
										 m_matTransform._m43);

			m_quatOrientation = m_pParent->getRotation();
			m_vecWorldDirection = m_quatOrientation.multiply(m_vecDirection);
		}
	}
//---------------------------------------------------------------------
	void NodeLight::calculateBoundingBox()
	{
		// Both OBB and AABB are the same for directional and omni light
		m_pOBB->setOBB(m_vecWorldPosition - m_vecRadius,
					   m_vecWorldPosition + m_vecRadius);
		m_pAABB->setAABB(m_vecWorldPosition - m_vecRadius,
						 m_vecWorldPosition + m_vecRadius);
	}
//---------------------------------------------------------------------
	void NodeLight::updateBoundingBox()
	{
		// Both OBB and AABB are the same for directional and omni light
		m_pOBB->setOBB(m_vecWorldPosition - m_vecRadius,
					   m_vecWorldPosition + m_vecRadius);
		m_pAABB->setAABB(m_vecWorldPosition - m_vecRadius,
						 m_vecWorldPosition + m_vecRadius);
	}
//---------------------------------------------------------------------
	void NodeLight::setLightType(const LIGHT_TYPE& _lightType)
	{
		m_LightType = _lightType;

		switch(m_LightType)
		{
		case LT_DIRECTIONAL:
			m_pMaterial = MaterialManager::getSingleton().getLibrary(L"core")->getMaterial(L"light_directional");
			break;
		case LT_POINT:
			m_pMaterial = MaterialManager::getSingleton().getLibrary(L"core")->getMaterial(L"light_point");
			break;
		case LT_SPOT:
			m_pMaterial = MaterialManager::getSingleton().getLibrary(L"core")->getMaterial(L"light_spot");
			break;
		}

		if(!m_pPrefab)
			createLightPrefab();
	}
//---------------------------------------------------------------------
	void NodeLight::createLightPrefab()
	{
		static uint l_LightPrefabCounter;


		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		m_pPrefab = (NodePrefab*)sm->createSphere(7, 7, 0.125f);
		m_pPrefab->setEnabled(true);

		m_pPrefab->setSerializable(false);

		wstringstream buffer;
		buffer << L"LightPrefab" << l_LightPrefabCounter++;
		this->addChild(buffer.str(), m_pPrefab);
	}
//---------------------------------------------------------------------
	LIGHT_TYPE NodeLight::getLightType() const
	{
		return m_LightType;
	}
//---------------------------------------------------------------------
	void NodeLight::setColour(const Colour& _colour)
	{
		m_Colour = _colour;
	}
//---------------------------------------------------------------------
	void NodeLight::setColour(float _r, float _g, float _b)
	{
		m_Colour.setRGBA(_r, _g, _b);
	}
//---------------------------------------------------------------------
	void NodeLight::setColour(dword _colour)
	{
		m_Colour.setRGBA(_colour);
	}
//---------------------------------------------------------------------
	Colour& NodeLight::getColour()
	{
		return m_Colour;
	}
//---------------------------------------------------------------------
	void NodeLight::setIntensity(Real _intensity)
	{
		m_fIntensity = _intensity;
	}
//---------------------------------------------------------------------
	Real NodeLight::getIntensity() const
	{
		return m_fIntensity;
	}
//---------------------------------------------------------------------
	void NodeLight::setDirection(const Vector3& _direction)
	{
		m_vecDirection = _direction;
		
		Node::m_bHasChanged = true;
	}
//---------------------------------------------------------------------
	void NodeLight::setDirection(Real _x, Real _y, Real _z)
	{
		m_vecDirection = Vector3(_x, _y, _z);
		
		Node::m_bHasChanged = true;
	}
//---------------------------------------------------------------------
	Vector3& NodeLight::getDirection()
	{
		return m_vecWorldDirection;
	}
//---------------------------------------------------------------------
	void NodeLight::setRadius(Real _radius)
	{
		m_vecRadius.set(_radius, _radius, _radius);

		calculateBoundingBox();

		Node::m_bHasChanged = true;
	}
//---------------------------------------------------------------------
	void NodeLight::setRadius(Real _radiusx, Real _radiusy, Real _radiusz)
	{
		m_vecRadius.set(_radiusx, _radiusy, _radiusz);

		calculateBoundingBox();

		Node::m_bHasChanged = true;
	}
//---------------------------------------------------------------------
	void NodeLight::setRadius(const Vector3& _radius)
	{
		m_vecRadius = _radius;

		calculateBoundingBox();

		Node::m_bHasChanged = true;
	}
//---------------------------------------------------------------------
	Vector3& NodeLight::getRadius()
	{
		return m_vecRadius;
	}
//---------------------------------------------------------------------
	void NodeLight::setDrawDebug(bool _value, Material* _mat)
	{
		m_bDrawDebug = _value;
		if(m_pPrefab)
		{
			m_pPrefab->setEnabled(_value);
			if(_value)
			{
				uint i = 0;
				m_pPrefab->getSurface(i)->setMaterial(_mat);
			}
		}
	}
//---------------------------------------------------------------------
	bool NodeLight::isDrawDebug() const
	{
		return m_bDrawDebug;
	}
//---------------------------------------------------------------------
	void NodeLight::addToPartitioner(ScenePartitioner* _partitioner)
	{
		_partitioner->addNode(this);
	}
//---------------------------------------------------------------------
	void NodeLight::removeFromPartitioner(ScenePartitioner* _partitioner)
	{
		_partitioner->removeNode(this);
	}
//---------------------------------------------------------------------
	NodeVisible* NodeLight::getLightDebugPrefab() const
	{
		return m_pPrefab;
	}
//---------------------------------------------------------------------
	void NodeLight::setShadowResolution(uint _resolution)
	{
		m_nShadowResolution = _resolution;
		Renderer::getSingleton().sortLights();
	}
//---------------------------------------------------------------------
	void NodeLight::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			Node::serialize(_serializer, false, true);

			Property <Colour> prColour(m_Colour);
			_serializer->addProperty("Colour", prColour);

			Property <float> prIntensity(m_fIntensity);
			_serializer->addProperty("Intensity", prIntensity);
			Property <float> prFOV(m_fFov);
			_serializer->addProperty("FOV", prFOV);
			Property <float> prSpotExponent(m_fSpotExponent);
			_serializer->addProperty("SpotExponent", prSpotExponent);
			Property <float> prShadowBias(m_fShadowBias);
			_serializer->addProperty("ShadowBias", prShadowBias);
			Property <float> prShadowDensity(m_fShadowDensity);
			_serializer->addProperty("ShadowDensity", prShadowDensity);

			Property <Vector3> prDirection(m_vecDirection);
			_serializer->addProperty("Direction", prDirection);
			Property <Vector3> prRadius(m_vecRadius);
			_serializer->addProperty("Radius", prRadius);

			Property <bool> prShadows(m_bCastShadows);
			_serializer->addProperty("CastsShadows", prShadows);
			Property <bool> prTransluent(m_bTransluent);
			_serializer->addProperty("Translucent", prTransluent);
			Property <bool> prDebug(m_bDrawDebug);
			_serializer->addProperty("DrawDebug", prDebug);
			Property <bool> prIndividual(m_bIndividualShadow);
			_serializer->addProperty("IndividualShadow", prIndividual);
			Property <bool> prStatic(m_bStaticShadow);
			_serializer->addProperty("StaticShadow", prStatic);

			uint nTmp = static_cast<uint>(m_LightType);
			Property <uint> prType(nTmp);
			_serializer->addProperty("LightType", prType);
			Property <uint> prShadowRes(m_nShadowResolution);
			_serializer->addProperty("ShadowResolution", prShadowRes);

			// Serialize material data
			if(m_pMaterial)
			{
				Property <wstring> prMat(m_pMaterial->getName());
				_serializer->addProperty("Material", prMat);
				Property <wstring> prLib(m_pMaterial->getMaterialLibrary()->getName());
				_serializer->addProperty("Library", prLib);
			}
		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeLight::deserialize(ISerializer* _serializer)
	{
		Property <Colour> prColour(m_Colour);
		_serializer->getProperty("Colour", prColour);

		Property <float> prIntensity(m_fIntensity);
		_serializer->getProperty("Intensity", prIntensity);
		Property <float> prFOV(m_fFov);
		_serializer->getProperty("FOV", prFOV);
		Property <float> prSpotExponent(m_fSpotExponent);
		_serializer->getProperty("SpotExponent", prSpotExponent);
		Property <float> prShadowBias(m_fShadowBias);
		_serializer->getProperty("ShadowBias", prShadowBias);
		Property <float> prShadowDensity(m_fShadowDensity);
		_serializer->getProperty("ShadowDensity", prShadowDensity);

		Property <Vector3> prDirection(m_vecDirection);
		_serializer->getProperty("Direction", prDirection);
		Property <Vector3> prRadius(m_vecRadius);
		_serializer->getProperty("Radius", prRadius);

		Property <bool> prShadows(m_bCastShadows);
		_serializer->getProperty("CastsShadows", prShadows);
		Property <bool> prTransluent(m_bTransluent);
		_serializer->getProperty("Translucent", prTransluent);
		Property <bool> prDebug(m_bDrawDebug);
		_serializer->getProperty("DrawDebug", prDebug);
		Property <bool> prIndividual(m_bIndividualShadow);
		_serializer->getProperty("IndividualShadow", prIndividual);
		Property <bool> prStatic(m_bStaticShadow);
		_serializer->getProperty("StaticShadow", prStatic);

		uint nTmp;
		Property <uint> prType(nTmp);
		_serializer->getProperty("LightType", prType);
		LIGHT_TYPE lightType = static_cast<LIGHT_TYPE>(nTmp);
		setLightType(lightType);

		Property <uint> prShadowRes(m_nShadowResolution);
		_serializer->getProperty("ShadowResolution", prShadowRes);

		Node::deserialize(_serializer);

		wstring stLib;
		wstring stMat;
		Property <wstring> prLib(stLib);
		Property <wstring> prMat(stMat);
		_serializer->getProperty("Library", prLib);
		_serializer->getProperty("Material", prMat);
		if(stLib != L"" && stMat != L"")
		{
			m_pMaterial = MaterialManager::getSingleton().getLibrary(stLib)->getMaterial(stMat);
		}

		Node::m_bHasChanged = true;
		update();

		Renderer::getSingleton().addLight(this);
	}
//----------------------------------------------------------------------
	bool NodeLight::tryToPick(const Ray& _ray)
	{
		return true;
	}
//----------------------------------------------------------------------
}