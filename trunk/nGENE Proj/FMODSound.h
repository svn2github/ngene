/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FMODSound.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FMODSOUND_H_
#define __INC_FMODSOUND_H_


#include "Prerequisities.h"
#include "ISound.h"


namespace FMOD
{
	class Channel;
	class Sound;
	class System;
}


namespace nGENE
{
	/// Sound class for FMOD library.
	class nGENEDLL FMODSound: public ISound
	{
	private:
		FMOD::Sound* m_pSound;			///< Pointer to the sound
		FMOD::System* m_pSystem;		///< Pointer to the sound system

		bool m_bIsLooped;				///< Is sound looped?

	public:
		FMODSound(FMOD::System* _system, const SOUND_DESC& _desc);
		virtual ~FMODSound();

		void init();
		void cleanup();

		void setLooped(bool _value);
		bool isLooped() const;

		/// Returns pointer to the FMOD sound.
		FMOD::Sound* getSound() const;
		/// Returns pointer to the FMOD system.
		FMOD::System* getSystem() const;
	};

	/// Instance of the FMOD sound.
	class nGENEDLL FMODSoundInstance: public ISoundInstance
	{
	private:
		FMOD::Channel* m_pChannel;
		FMODSound* m_pSound;			///< Pointer to the FMODSound object

		bool m_bIsPaused;				///< Is sound paused?

		Vector3 m_vecPosition;
		Vector3 m_vecVelocity;

		Real m_fFrequency;				///< Sound frequency
		Real m_fVolume;					///< Sound volume
		Real m_fPan;					///< Sound pan


	public:
		FMODSoundInstance(FMODSound* _sound);
		~FMODSoundInstance();

		void play();
		void stop();
		bool isPlaying() const;

		void pause(bool _value);
		bool isPaused() const;

		void setFrequency(Real _frequency);
		Real getFrequency() const;

		void setVolume(Real _volume);
		Real getVolume() const;

		void setPan(Real _pan);
		Real getPan() const;

		void setPosition(Real _x, Real _y, Real _z);
		void setVelocity(Real _x, Real _y, Real _z);

		ISound* getSound() const;
	};


	inline bool FMODSound::isLooped() const
	{
		return m_bIsLooped;
	}
//----------------------------------------------------------------------
	inline bool FMODSoundInstance::isPaused() const
	{
		return m_bIsPaused;
	}
//----------------------------------------------------------------------
	inline ISound* FMODSoundInstance::getSound() const
	{
		return m_pSound;
	}
//----------------------------------------------------------------------
}


#endif
