/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeLight.h
Version:	0.18
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODELIGHT_H_
#define __INC_NODELIGHT_H_


#include "Colour.h"
#include "ICullable.h"
#include "Node.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/// Type of the light
	enum nGENEDLL LIGHT_TYPE
	{
		LT_POINT,
		LT_DIRECTIONAL,
		LT_SPOT
	};

	/** Node representing light.
		@par
			Lights are very important factor of the 3D scene,
			as they let you achieve specific mood. Besides
			they can cause shadows, which further enhance
			visual quality of the scene.
		@par
			To draw helper geometry (sphere/cone) which shows
			the position and/or direction of the light call
			setDrawDebug() method and pass true as its argument.
		@par
			Many (if not most) parameters of the light and their
			influence on the final image depends on the light's
			type.
		@par
			If you don't want to cast dynamic shadows you can set
			static flag using setStaticShadow() method.
	*/
	class nGENEDLL NodeLight: public ICullable, public Node
	{
		EXPOSE_TYPE
	private:
		LIGHT_TYPE m_LightType;			///< Type of the light
		Material* m_pMaterial;			///< Lighting model

		NodePrefab* m_pPrefab;			///< Helper geometry

		uint m_nShadowResolution;		///< Resolution of shadow texture associated with this light.

		Colour m_Colour;				///< Light colour
		Real m_fIntensity;				///< Light intensity
		Real m_fFov;					///< Cone angle (only spot)
		Real m_fSpotExponent;			///< Spot exponent (only spot)
		Real m_fConstantAttenuation;	///< Constant attenuation coefficient (only spot)
		Real m_fLinearAttenuation;		///< Linear attenuation coefficient (only spot)
		Real m_fQuadraticAttenuation;	///< Quadratic attenuation coefficient (only spot)
		Real m_fShadowBias;				///< Helper value for shadowing
		Real m_fShadowDensity;			///< Shadow density (0.0 - 1.0)

		Vector3 m_vecDirection;			///< Light direction (only spot/directional)
		Vector3 m_vecRadius;			///< Light radius (only point)

		Vector3 m_vecWorldPosition;		///< Position in regard to parent node

		Vector3 m_vecWorldDirection;	///< Direction in regard to parent node

		bool m_bCastShadows;			///< Does light cause shadow-cast?
		bool m_bTransluent;				///< Does light cause translucency?
		bool m_bDrawDebug;				///< Is helper sphere/cone rendered?
		bool m_bIndividualShadow;
		bool m_bStaticShadow;			///< Are light's shadows static?
		bool m_bRequestedShadow;		///< Should shadow be updated (for static ones)?


		/// Creates light prefab depending on light type.
		void createLightPrefab();

		/// Updates the node.
		virtual void onUpdate();

	public:
		NodeLight();
		NodeLight(const LIGHT_TYPE& _lightType);
		virtual ~NodeLight();

		void init();
		void cleanup();

		void calculateBoundingBox();
		void updateBoundingBox();

		/** Sets type of the light.
			@par
				There are 3 default light types: point, directional and spot.
				However, by setting another material one can achieve completely
				different effect.
		*/
		virtual void setLightType(const LIGHT_TYPE& _lightType);
		/// Returns type of light.
		LIGHT_TYPE getLightType() const;

		/// Sets light colour.
		void setColour(const Colour& _colour);
		/// Sets light colour.
		void setColour(float _r, float _g, float _b);
		/// Sets light colour.
		void setColour(dword _colour);
		/// Returns light colour.
		Colour& getColour();

		/// Sets light intensity.
		void setIntensity(Real _intensity);
		/// Returns light intensity.
		Real getIntensity() const;

		/// Sets light field of view (FOV).
		void setFOV(Real _fov);
		/// Returns spot light field of view (FOV).
		Real getFOV() const;

		/// Sets shadow bias.
		void setShadowBias(Real _bias);
		Real getShadowBias() const;

		/** Sets shadow density.
			@param
				_density density of the shadow. Values should range
				0.0 to 1.0.
		*/
		void setShadowDensity(Real _density);
		/// Returns shadow density.
		Real getShadowDensity() const;

		/// Sets spot exponent.
		void setSpotExponent(Real _exponent);
		/// Returns spot light spot exponent coefficient.
		Real getSpotExponent() const;

		/// Sets constant attenuation.
		void setConstantAttenuation(Real _atten);
		/// Returns constant attenuation coefficient.
		Real getConstantAttenuation() const;

		/// Sets linear attenuation.
		void setLinearAttenuation(Real _atten);
		/// Returns linear attenuation coefficient.
		Real getLinearAttenuation() const;

		/// Sets quadratic attenuation.
		void setQuadraticAttenuation(Real _atten);
		/// Returns quadratic attenuation coefficient.
		Real getQuadraticAttenuation() const;

		/** This function lets you display light helper geometry.
			@remarks
				If you want make this geometry visible you have to
				pass material which will be assigned to it.
		*/
		void setDrawDebug(bool _value, Material* _material=NULL);
		bool isDrawDebug() const;

		void setIndividualShadow(bool _value);
		bool hasIndividualShadow() const;

		/// Specifies whether shadows are static for this light.
		void setStaticShadow(bool _value);
		/// Checks if shadows are static for this light.
		bool hasStaticShadow() const;

		/// Requests shadow update (for static shadows).
		void requestShadowUpdate(bool _value);
		/// Checks if this light should update its shadows.
		bool hasRequestedShadowUpdate() const;

		/** Sets direction of the light.
			@remarks
				Input vector should be normalized.
		*/
		void setDirection(const Vector3& _direction);
		void setDirection(Real _x, Real _y, Real _z);
		/// Returns direction of this light.
		Vector3& getDirection();

		/** Sets radius of the light.
			@remarks
				Even though radius is something characterizing point lights,
				nGENE Tech uses it as a helper value for other types of lights
				as well (eg. to cull them).
		*/
		void setRadius(Real _radius);
		void setRadius(Real _radiusx, Real _radiusy, Real _radiusz);
		void setRadius(const Vector3& _radius);
		/// Returns radius of the light.
		Vector3& getRadius();

		/// Sets node position.
		virtual Vector3& getPositionLocal() {return m_vecWorldPosition;}

		/// Lets you change default material to another one.
		void setMaterial(Material* _material);
		/// Returns material used by this light.
		Material* getMaterial() const;

		/** Sets whether light should cast shadows or not.
			@param
				_value if true, light will be casting shadows.
		*/
		void setCastShadows(bool _value);
		/// Checks if this light is casting shadows.
		bool isCastingShadows() const;


		/// Returns debug prefab.
		NodeVisible* getLightDebugPrefab() const;

		/// Sets shadow texture resolution for this light.
		void setShadowResolution(uint _resolution);
		/// Returns shadow texture resolution for this light.
		uint getShadowResolution() const;


		/// Adds light to the partitioner.
		void addToPartitioner(ScenePartitioner* _partitioner);
		void removeFromPartitioner(ScenePartitioner* _partitioner);

		bool tryToPick(const Ray& _ray);

		bool isEnabled() const;


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline void NodeLight::setFOV(Real _fov)
	{
		m_fFov = _fov;
	}
//---------------------------------------------------------------------
	inline Real NodeLight::getFOV() const
	{
		return m_fFov;
	}
//---------------------------------------------------------------------
	inline void NodeLight::setShadowBias(Real _bias)
	{
		m_fShadowBias = _bias;
	}
//---------------------------------------------------------------------
	inline Real NodeLight::getShadowBias() const
	{
		return m_fShadowBias;
	}
//---------------------------------------------------------------------
	inline void NodeLight::setShadowDensity(Real _density)
	{
		m_fShadowDensity = _density;
	}
//---------------------------------------------------------------------
	inline Real NodeLight::getShadowDensity() const
	{
		return m_fShadowDensity;
	}
//---------------------------------------------------------------------
	inline void NodeLight::setSpotExponent(Real _exponent)
	{
		m_fSpotExponent = _exponent;
	}
//---------------------------------------------------------------------
	inline Real NodeLight::getSpotExponent() const
	{
		return m_fSpotExponent;
	}
//---------------------------------------------------------------------
	inline void NodeLight::setMaterial(Material* _material)
	{
		m_pMaterial = _material;
	}
//---------------------------------------------------------------------
	inline Material* NodeLight::getMaterial() const
	{
		return m_pMaterial;
	}
//---------------------------------------------------------------------
	inline void NodeLight::setCastShadows(bool _value)
	{
		m_bCastShadows = _value;
	}
//---------------------------------------------------------------------
	inline bool NodeLight::isCastingShadows() const
	{
		return m_bCastShadows;
	}
//---------------------------------------------------------------------
	inline void NodeLight::setConstantAttenuation(Real _atten)
	{
		m_fConstantAttenuation = _atten;
	}
//---------------------------------------------------------------------
	inline Real NodeLight::getConstantAttenuation() const
	{
		return m_fConstantAttenuation;
	}
//---------------------------------------------------------------------
	inline void NodeLight::setLinearAttenuation(Real _atten)
	{
		m_fLinearAttenuation = _atten;
	}
	//---------------------------------------------------------------------
	inline Real NodeLight::getLinearAttenuation() const
	{
		return m_fLinearAttenuation;
	}
//---------------------------------------------------------------------
	inline void NodeLight::setQuadraticAttenuation(Real _atten)
	{
		m_fQuadraticAttenuation = _atten;
	}
//---------------------------------------------------------------------
	inline Real NodeLight::getQuadraticAttenuation() const
	{
		return m_fQuadraticAttenuation;
	}
//---------------------------------------------------------------------
	inline uint NodeLight::getShadowResolution() const
	{
		return m_nShadowResolution;
	}
//---------------------------------------------------------------------
	inline void NodeLight::setIndividualShadow(bool _value)
	{
		m_bIndividualShadow = _value;
	}
//---------------------------------------------------------------------
	inline bool NodeLight::hasIndividualShadow() const
	{
		return m_bIndividualShadow;
	}
//---------------------------------------------------------------------
	inline void NodeLight::setStaticShadow(bool _value)
	{
		m_bStaticShadow = _value;
		if(m_bStaticShadow)
		{
			m_bRequestedShadow = true;
			m_bIndividualShadow = true;
		}
	}
//---------------------------------------------------------------------
	inline bool NodeLight::hasStaticShadow() const
	{
		return m_bStaticShadow;
	}
//---------------------------------------------------------------------
	inline void NodeLight::requestShadowUpdate(bool _value)
	{
		m_bRequestedShadow = _value;
	}
//---------------------------------------------------------------------
	inline bool NodeLight::hasRequestedShadowUpdate() const
	{
		return m_bRequestedShadow;
	}
//---------------------------------------------------------------------
	inline bool NodeLight::isEnabled() const
	{
		return Node::isEnabled();
	}
//---------------------------------------------------------------------
}


#endif