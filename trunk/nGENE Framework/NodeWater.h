/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeWater.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEWATER_H_
#define __INC_NODEWATER_H_


#include "Node.h"
#include "Prerequisities.h"
#include "TimeDate.h"
#include "TypeInfo.h"


namespace nGENE
{
namespace Nature
{
	class ReflectionStage;

	/** Node representing sea water.
		@par
			The technique used in nGENE for water rendering is
			quite innovative in a sense it is being done in
			a post-processing stage yet it allows you to have
			some nice waves.
		@par
			If you want correct reflections make sure to pass
			valid ReflectionStage pointer to constructor or by
			calling setReflectionStage() method.
	*/
	class NodeWater: public Node
	{
		EXPOSE_TYPE
	private:
		long m_dwLastTime;						///< Last time

		Material* m_pWaterMat;					///< Pointer to the water material

		float m_SunAngle;						///< Sun angle
		float m_fDayLength;						///< Length of the day in seconds
		float m_fAmplitude;						///< Waves amplitude

		Vector2 m_vecWind;						///< Wind strength
		Vector2 m_vecWavesScale;				///< Waves scale
		Vector3 m_vecFoam;						///< Existence of foam
		Vector3 m_vecExtinction;				///< Colours extinction
		Real m_fMuddiness;						///< How muddy water is
		Real m_fRefractionStrength;				///< Set to higher to emphasize refraction
		Real m_fRefractionIndex;				/**< Index of refraction, for water it should
													 be 1.(3) according to tables. */
		Real m_fRefractionScale;				///< Refraction animation strength
		Real m_fWaterLevel;						///< Level of the water surface
		Real m_fShoreHardness;					///< Hardness of water-shore connection
		Real m_fShininess;						///< Water shininess
		Real m_fDisplace;						///< Level of reflection displacement

		Real m_fNormalScale;					///< Normal vectors modifier

		Real m_fHorizontalVisibility;			///< Transparency of water along view vector

		Colour m_SunColour;						///< Colour of the Sun
		Colour m_SurfaceColour;					///< Colour of the surface
		Colour m_DepthColour;					///< Colour of the depth

		bool m_bSimulate;						///< Is simulation taking places?

		TimeDate m_Time;						///< Current time
		TimeDate m_SunRise;						///< Sun rise time
		TimeDate m_Sunset;						///< Sunset time

		SPriorityMaterial m_PostWater;			///< Post water material

		ReflectionStage* m_pReflectionStage;	///< Pointer to reflection stage


		void updateWater();
		void updateColours();

		/// Updates water object.
		void onUpdate();

	public:
		NodeWater(ReflectionStage* _stage=NULL);
		~NodeWater();

		void init();

		void setEnabled(bool _value);

		/** Sets day length for simulation.
			@param
				_length day length in seconds.
		*/
		void setDayLength(Real _length);
		/// Returns day length for simulation.
		float getDayLength() const;

		/// Specifies if sunlight is simulated.
		void setSimulated(bool _value);
		/// Checks if sunlight is simulated.
		bool isSimulated() const;

		/// Sets current time of day.
		void setDayTime(const TimeDate& _time);
		/// Returns current time of day.
		TimeDate& getDayTime();

		/// Sets sun rise time.
		void setSunRiseTime(const TimeDate& _time);
		/// Returns sun rise time.
		TimeDate& getSunRiseTime();

		/// Sets sunset time.
		void setSunsetTime(const TimeDate& _time);
		/// Returns sunset time.
		TimeDate& getSunsetTime();

		/// Sets amplitude of sea waves.
		void setWavesAmplitude(Real _amplitude);
		/// Returns waves amplitude.
		Real getWavesAmplitude() const;

		/// Sets refraction index.
		void setRefractionIndex(Real _index);
		/// Returns refraction index.
		Real getRefractionIndex() const;

		/// Sets water surface colour.
		void setSurfaceColour(const Colour& _colour);
		Colour& getSurfaceColour();

		/// Sets water depth colour.
		void setDepthColour(const Colour& _colour);
		/// Returns water depth colour.
		Colour& getDepthColour();

		/// Sets horizontal visibility.
		void setHorizontalVisibility(Real _visibility);
		/// Returns horizontal visibility.
		Real getHorizontalVisibility() const;

		/// Sets level of the water surface (y position).
		void setWaterLevel(Real _level);
		/// Returns water level.
		Real getWaterLevel() const;

		/// Sets reflection stage.
		void setReflectionStage(ReflectionStage* _stage);
		ReflectionStage* getReflectionStage() const;
		void setMaterial(Material* _material);

		/// Same as calling setWaterLevel().
		void setPosition(const Vector3& _pos);
		/// Same as calling setWaterLevel().
		void setPosition(Real _x, Real _y, Real _z);

		/** Sets foam existence, that is at what depths/heights it appears.
			@param
				_foam
					* x component - depth at which foam starts to disappear,
					* y component - depth at which foam is no longer visible.
					* z component - height at which foam starts to appear on waves.
		*/
		void setFoam(const Vector3& _foam);
		/// Returns foam vector.
		Vector3& getFoam();

		/// Sets 'muddiness' of water.
		void setMuddiness(Real _value);
		Real getMuddiness() const;

		/** Sets refraction strength.
			@param
				_value strength of refraction. Set to bigger values to
				emphasize refraction.
		*/
		void setRefractionStrength(Real _value);
		Real getRefractionStrength() const;

		/** Sets water-shore connection hardness.
			@param
				_value the smaller this value is, the more soft the transition
				between shore and water. If you want hard edges use very big value.
		*/
		void setShoreHardness(Real _value);
		/// Returns shore hardness.
		Real getShoreHardness() const;

		/// Sets water shininess.
		void setShininess(Real _shininess);
		/// Returns water shininess.
		Real getShininess() const;

		/// Sets colour extinction.
		void setExtinction(const Vector3& _extinction);
		/// Returns colour extinction.
		Vector3& getExtinction();

		/// Sets water reflection displacement.
		void setDisplacement(Real _displacement);
		/// Returns water reflection displacement.
		Real getDisplacement() const;

		/// Sets wind strength.
		void setWind(const Vector2& _wind);
		/// Returns wind strength.
		Vector2& getWind();

		/// Sets waves scale.
		void setWavesScale(const Vector2& _scale);
		/// Returns waves scale.
		Vector2& getWavesScale();

		/// Sets refraction scale.
		void setRefractionScale(Real _scale);
		/// Returns refraction scale.
		Real getRefractionScale() const;

		/** Sets normal vectors scale.
			@remarks
				Higher waves, i.e. bigger waves amplitude require setting
				higher value for this parameter.
		*/
		void setNormalScale(Real _scale);
		/// Returns normal vectors scale.
		Real getNormalScale() const;


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline void NodeWater::setDayLength(Real _length)
	{
		m_fDayLength = _length;
	}
//----------------------------------------------------------------------
	inline float NodeWater::getDayLength() const
	{
		return m_fDayLength;
	}
//----------------------------------------------------------------------
	inline void NodeWater::setSimulated(bool _value)
	{
		m_bSimulate = _value;
	}
//----------------------------------------------------------------------
	inline bool NodeWater::isSimulated() const
	{
		return m_bSimulate;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getWavesAmplitude() const
	{
		return m_fAmplitude;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getRefractionIndex() const
	{
		return m_fRefractionIndex;
	}
//----------------------------------------------------------------------
	inline Colour& NodeWater::getSurfaceColour()
	{
		return m_SurfaceColour;
	}
//----------------------------------------------------------------------
	inline Colour& NodeWater::getDepthColour()
	{
		return m_DepthColour;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getHorizontalVisibility() const
	{
		return m_fHorizontalVisibility;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getWaterLevel() const
	{
		return m_fWaterLevel;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getMuddiness() const
	{
		return m_fMuddiness;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getRefractionStrength() const
	{
		return m_fRefractionStrength;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getShoreHardness() const
	{
		return m_fShoreHardness;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getShininess() const
	{
		return m_fShininess;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getDisplacement() const
	{
		return m_fDisplace;
	}
//----------------------------------------------------------------------
	inline Vector2& NodeWater::getWind()
	{
		return m_vecWind;
	}
//----------------------------------------------------------------------
	inline Vector2& NodeWater::getWavesScale()
	{
		return m_vecWavesScale;
	}
//----------------------------------------------------------------------
	inline void NodeWater::setSunRiseTime(const TimeDate& _time)
	{
		m_SunRise = _time;
	}
//----------------------------------------------------------------------
	inline TimeDate& NodeWater::getSunRiseTime()
	{
		return m_SunRise;
	}
//----------------------------------------------------------------------
	inline void NodeWater::setSunsetTime(const TimeDate& _time)
	{
		m_Sunset = _time;
	}
//----------------------------------------------------------------------
	inline TimeDate& NodeWater::getDayTime()
	{
		return m_Time;
	}
//----------------------------------------------------------------------
	inline TimeDate& NodeWater::getSunsetTime()
	{
		return m_Sunset;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getRefractionScale() const
	{
		return m_fRefractionScale;
	}
//----------------------------------------------------------------------
	inline Real NodeWater::getNormalScale() const
	{
		return m_fNormalScale;
	}
//----------------------------------------------------------------------
}
}


#endif