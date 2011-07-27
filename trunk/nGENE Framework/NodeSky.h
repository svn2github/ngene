/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeSky.h
Version:	0.10
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODESKY_H_
#define __INC_NODESKY_H_


#include "Node.h"
#include "PreethamScatter.h"
#include "Prerequisities.h"
#include "TimeDate.h"
#include "TypeInfo.h"


namespace nGENE
{
namespace Nature
{
	/** Node for simulating sky using Preetham scattering.
		@remarks
			Be aware that when creating NodeSky, PrefabSphere with the name
			"SkyDome" is implicitly created.
	*/
	class NodeSky: public Node
	{
		EXPOSE_TYPE
	private:
		class NodeSkyUpdateDataTask: public Task
		{
		private:
			NodeSky* m_pSky;
			bool m_bUpdate;

		public:
			NodeSkyUpdateDataTask();

			void setSky(NodeSky* _sky);
			void requestUpdate();
			void update();
		} m_UpdateTask;

	private:
		long m_dwLastTime;						///< Last time

		uint m_nShadowResolution;				/// Resolution of the shadows

		NodeLight* m_pLight;					///< Pointer to the light node

		Material* m_pSkyMat;					///< Pointer to the sky material
		Material* m_pHazeMat;					///< Pointer to the haze material

		PrefabSphere* m_pSky;					///< Pointer to the sky dome

		ITexture* m_pS0Mie;
		ITexture* m_pS0Ray;

		float m_SunAngle;						///< Sun angle
		float m_PrevSunAngle;						///< Sun angle
		float m_fSunSize;						///< Size of the sun
		float m_fSunBrightness;					///< Brightness of the sun
		float m_fDayLength;						///< Length of the day in seconds
		
		float m_fSkyRadius;						///< Sky dome radius

		PreethamScatter m_Scatter;				///< Preetham atmospheric scattering model

		bool m_bSimulate;						///< Is simulation taking places?
		bool m_bFixed;							///< Should sky move with the camera?
		bool m_bCastShadows;					///< Is casting shadows?

		TimeDate m_Time;						///< Current time
		TimeDate m_SunRise;						///< Sun rise time
		TimeDate m_Sunset;						///< Sunset time

		
		void updateSunAndSky();
		void onUpdate();

	public:
		NodeSky();
		explicit NodeSky(Real _radius);
		~NodeSky();

		void init();

		void ProcessData();

		void updateMTInternal();
		/// Updates unconditionally.
		void requestUpdate();

		/** Sets day length for simulation.
			@param
				_length day length in seconds.
		*/
		void setDayLength(Real _length);
		/// Returns day length.
		float getDayLength() const;

		/// Sets size of the sun.
		void setSunSize(Real _size);
		/// Returns size of the sun.
		float getSunSize() const;

		/// Sets brightness of the sun.
		void setSunBrightness(Real _brightness);
		/// Returns sun brightness.
		float getSunBrightness() const;

		void setSimulated(bool _value);
		bool isSimulated() const;

		/// Sets time of day.
		void setDayTime(const TimeDate& _time);
		/// Returns time of day.
		TimeDate& getDayTime();

		/// Sets sun rise time.
		void setSunRiseTime(const TimeDate& _time);
		/// Returns time of sun rise.
		TimeDate& getSunRiseTime();

		/// Sets sunset time.
		void setSunsetTime(const TimeDate& _time);
		/// Returns sunset time.
		TimeDate& getSunsetTime();

		/// Returns light associated with the sky.
		NodeLight* getLight() const;
		
		/// Sets sun shadow resolution.
		void setShadowResolution(uint _value);
		/// Returns sun shadow resolution.
		uint getShadowResolution() const;

		/// Specifies if is casting shadows.
		void setCastShadows(bool _value);
		/// Returns if sun is casting shadows.
		bool isCastingShadows() const;

		/// Returns Preetham scattering.
		PreethamScatter* getScattering();

		/** Sets whether sky should be fixed or not (i.e. be static or move
			with the camera).
		*/
		void setFixed(bool _value);
		bool isFixed() const;


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline void NodeSky::setDayLength(Real _length)
	{
		m_fDayLength = _length;
	}
//----------------------------------------------------------------------
	inline float NodeSky::getDayLength() const
	{
		return m_fDayLength;
	}
//----------------------------------------------------------------------
	inline void NodeSky::setSimulated(bool _value)
	{
		m_bSimulate = _value;
	}
//----------------------------------------------------------------------
	inline bool NodeSky::isSimulated() const
	{
		return m_bSimulate;
	}
//----------------------------------------------------------------------
	inline NodeLight* NodeSky::getLight() const
	{
		return m_pLight;
	}
//----------------------------------------------------------------------
	inline PreethamScatter* NodeSky::getScattering()
	{
		return &m_Scatter;
	}
//----------------------------------------------------------------------
	inline void NodeSky::setFixed(bool _value)
	{
		m_bFixed = _value;
	}
//----------------------------------------------------------------------
	inline bool NodeSky::isFixed() const
	{
		return m_bFixed;
	}
//----------------------------------------------------------------------
	inline TimeDate& NodeSky::getDayTime()
	{
		return m_Time;
	}
//----------------------------------------------------------------------
	inline void NodeSky::setSunRiseTime(const TimeDate& _time)
	{
		m_SunRise = _time;
	}
//----------------------------------------------------------------------
	inline TimeDate& NodeSky::getSunRiseTime()
	{
		return m_SunRise;
	}
//----------------------------------------------------------------------
	inline void NodeSky::setSunsetTime(const TimeDate& _time)
	{
		m_Sunset = _time;
	}
//----------------------------------------------------------------------
	inline TimeDate& NodeSky::getSunsetTime()
	{
		return m_Sunset;
	}
//----------------------------------------------------------------------
	inline Real NodeSky::getSunSize() const
	{
		return m_fSunSize;
	}
//----------------------------------------------------------------------
	inline Real NodeSky::getSunBrightness() const
	{
		return m_fSunBrightness;
	}
//----------------------------------------------------------------------
}
}


#endif
