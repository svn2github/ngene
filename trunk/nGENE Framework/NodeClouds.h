/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeClouds.h
Version:	0.09
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODECLOUDS_H_
#define __INC_NODECLOUDS_H_


#include "NodeVisible.h"
#include "Prerequisities.h"
#include "TimeDate.h"
#include "TypeInfo.h"


namespace nGENE
{
namespace Nature
{
	/// Description of the clouds node.
	typedef struct SCloudsDescription
	{
		Real domeRadius;						///< Radius of the dome clouds are mapped onto
		Real domeHeight;						/**< It describes how much "sperical" the dome is.
													 The lesser the value, the more planar the dome is.	*/
		uint textureSize;						///< Dimension of the noise texture
		uint verticesCount;						///< Number of vertices per sky dome shape row

		SCloudsDescription():
			domeRadius(490.0f),
			textureSize(128),
			domeHeight(0.0004375f),
			verticesCount(21)
		{}
	} CLOUDS_DESC;

	/** Node for simulating 2-dimensional clouds.
		@remarks
			Be aware that when creating NodeSky, PrefabSphere with the name
			"CloudsDome" is implicitly created.
		@par
			Clouds have to be restored manually after rendering device loss,
			resolution change or switching between windowed/full screen mode.
			To do this, simply call init() method.
		@par
			When HDR is used clouds are rendered to the off-screen buffer using
			clouds_HDR material (instead regular clouds_NON_HDR material) meaning
			user is responsible to mapping this texture onto geometry. For convenience
			if there is NodeSky object present in the scene it makes use of this
			texture.
		@par
			When HDR is used horizon is in range [0; 1] instead of regular [0; infinity).
			This is due to the fact that more operations are done in texture space.
	*/
	class NodeClouds: public NodeVisible
	{
		EXPOSE_TYPE
	private:
		CLOUDS_DESC m_Desc;						///< Description of the clouds

		dword m_dwPassedTime;					///< Passed time
		dword m_dwLastTime;						///< Last time
		dword m_dwUpdateFrequency;				///< Frequency of the textures update

		Material* m_pCloudsMat;					///< Pointer to the clouds material
		Material* m_pCloudsMatHDR;				///< Pointer to the clouds material
		Material* m_pCloudsDensityMat;			///< Pointer to the clouds density material

		float m_SunAngle;						///< Sun angle
		float m_fDayLength;						///< Length of the day in seconds
		float m_fCover;							///< Clouds cover
		float m_fSharpness;						///< Clouds sharpness

		float m_fDensity;						///< Density of the clouds

		float m_fHorizonLevel;					///< Height at which the horizon is
		float m_fHorizonHeight;					///< Height of the horizon

		float m_fTextureScaling;				///< Texture scaling

		Colour m_SunColour;						///< Colour of the Sun

		bool m_bSimulate;						///< Is simulation taking places?
		bool m_bFixed;							///< Should clouds move with the camera?

		TimeDate m_Time;						///< Current time
		TimeDate m_SunRise;						///< Sun rise time
		TimeDate m_Sunset;						///< Sunset time

		SPriorityMaterial m_PostCloudsDensity;	///< Post clouds material
		SPriorityMaterial m_PostClouds;			///< Post clouds material


		void createCloudsPlane();
		void updateClouds();
		void updateColours();

		void initTextures();

		/// Creates cloud noise data.
		nGENE::byte* createCloudNoise();

		/// Updates clouds.
		void onUpdate();
		void updateTextures();

	public:
		NodeClouds();
		explicit NodeClouds(const CLOUDS_DESC& _desc);
		~NodeClouds();

		void init();
		void reset();

		/** Sets day length for simulation.
			@param
				_length day length in seconds.
		*/
		void setDayLength(Real _length);
		/// Returns length of day.
		float getDayLength() const;

		/// Sets time of day.
		void setDayTime(const TimeDate& _time);
		/// Returns time of day.
		TimeDate& getDayTime();

		/// Sets sun rise time.
		void setSunRiseTime(const TimeDate& _time);
		/// Returns sun rise time.
		TimeDate& getSunRiseTime();

		/// Sets sunset time.
		void setSunsetTime(const TimeDate& _time);
		/// Returns sunset time.
		TimeDate& getSunsetTime();

		void setSimulated(bool _value);
		bool isSimulated() const;

		/// Sets clouds cover.
		void setCloudCover(float _value);
		Real getCloudCover() const;

		/// Sets clouds sharpness.
		void setCloudSharpness(float _value);
		/// Returns clouds sharpness.
		Real getCloudSharpness() const;

		/// Sets clouds density.
		void setCloudDensity(float _value);
		/// Returns clouds density.
		Real getCloudDensity() const;

		/// Sets horizon level.
		void setHorizonLevel(float _value);
		Real getHorizonLevel() const;

		/// Sets horizon height.
		void setHorizonHeight(float _value);
		Real getHorizonHeight() const;

		/// Sets texture scaling.
		void setTextureScaling(float _value);
		Real getTextureScaling() const;

		/** Sets whether sky should be fixed or not (ie. be static or move
			with the camera).
		*/
		void setFixed(bool _value);
		bool isFixed() const;

		/** Sets how often new density map will be generated.
			@param
				_frequency frequency of updates in milliseconds. Pass 0 to disable
				updates.
		*/
		void setUpdateFrequency(dword _frequency);
		/// Returns update frequency.
		dword getUpdateFrequency() const;


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline void NodeClouds::setDayLength(Real _length)
	{
		m_fDayLength = _length;
	}
//----------------------------------------------------------------------
	inline float NodeClouds::getDayLength() const
	{
		return m_fDayLength;
	}
//----------------------------------------------------------------------
	inline void NodeClouds::setSimulated(bool _value)
	{
		m_bSimulate = _value;
	}
//----------------------------------------------------------------------
	inline bool NodeClouds::isSimulated() const
	{
		return m_bSimulate;
	}
//----------------------------------------------------------------------
	inline Real NodeClouds::getCloudCover() const
	{
		return m_fCover;
	}
//----------------------------------------------------------------------
	inline Real NodeClouds::getCloudSharpness() const
	{
		return m_fSharpness;
	}
//----------------------------------------------------------------------
	inline Real NodeClouds::getCloudDensity() const
	{
		return m_fDensity;
	}
//----------------------------------------------------------------------
	inline Real NodeClouds::getHorizonLevel() const
	{
		return m_fHorizonLevel;
	}
//----------------------------------------------------------------------
	inline Real NodeClouds::getHorizonHeight() const
	{
		return m_fHorizonHeight;
	}
//----------------------------------------------------------------------
	inline Real NodeClouds::getTextureScaling() const
	{
		return m_fTextureScaling;
	}
//----------------------------------------------------------------------
	inline void NodeClouds::setFixed(bool _value)
	{
		m_bFixed = _value;
	}
//----------------------------------------------------------------------
	inline bool NodeClouds::isFixed() const
	{
		return m_bFixed;
	}
//----------------------------------------------------------------------
	inline TimeDate& NodeClouds::getDayTime()
	{
		return m_Time;
	}
//----------------------------------------------------------------------
	inline void NodeClouds::setSunRiseTime(const TimeDate& _time)
	{
		m_SunRise = _time;
	}
//----------------------------------------------------------------------
	inline TimeDate& NodeClouds::getSunRiseTime()
	{
		return m_SunRise;
	}
//----------------------------------------------------------------------
	inline void NodeClouds::setSunsetTime(const TimeDate& _time)
	{
		m_Sunset = _time;
	}
//----------------------------------------------------------------------
	inline TimeDate& NodeClouds::getSunsetTime()
	{
		return m_Sunset;
	}
//----------------------------------------------------------------------
	inline void NodeClouds::setUpdateFrequency(dword _frequency)
	{
		m_dwUpdateFrequency = _frequency;
	}
//----------------------------------------------------------------------
	inline dword NodeClouds::getUpdateFrequency() const
	{
		return m_dwUpdateFrequency;
	}
//----------------------------------------------------------------------
}
}


#endif