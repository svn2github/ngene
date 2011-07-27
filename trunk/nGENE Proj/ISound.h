/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ISound.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ISOUND_H_
#define __INC_ISOUND_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "Node.h"
#include "Singleton.h"
#include "TypeInfo.h"


namespace nGENE
{
	/// Descriptor for the sound object.
	typedef struct nGENEDLL SSoundDesc
	{
		bool is3D;							/**< Specifies if the sound can be
												 positioned in the 3D space */
		bool isUsingHardware;				/**< Specifies if the sound is using
												 hardware acceleration */
		bool isCompressed;					///< Useful for MP3 files

		wstring fileName;					///< Name of the sound's file name

		SSoundDesc():
			fileName(L"")
		{
			is3D = true;
			isUsingHardware = true;
			isCompressed = false;
		}
	} SOUND_DESC;


	/// Abstract sound interface.
	class nGENEDLL ISound: public BaseClass
	{
	protected:
		SOUND_DESC m_Desc;
		wstring m_stName;

	public:
		ISound(const SOUND_DESC& _desc);
		virtual ~ISound() {}

		/// Initializes the sound.
		virtual void init()=0;
		virtual void cleanup()=0;

		/// Specifies if the sound is looped or is played only once.
		virtual void setLooped(bool _value)=0;
		/// Checks if the sound is looped
		virtual bool isLooped() const=0;

		/// Returns sound description.
		virtual SOUND_DESC& getDesc();
		
		/// Sets sound name.
		virtual void setName(const wstring& _name);
		/// Returns sound name.
		virtual wstring& getName();
	};

	/** Instance of the sound object.
		@par
			This class is necessary as many libraries separate sound
			data from buffers/sources.
	*/
	class nGENEDLL ISoundInstance
	{
	public:
		virtual ~ISoundInstance() {}

		/// Plays the sound.
		virtual void play()=0;
		/// Stops playing the sound.
		virtual void stop()=0;
		/// Checks if the sound is playing.
		virtual bool isPlaying() const=0;

		/// Pauses/resumes the sound.
		virtual void pause(bool _value)=0;
		/// Checks if the sound is in the paused state.
		virtual bool isPaused() const=0;

		/// Sets sound frequency.
		virtual void setFrequency(Real _frequency)=0;
		/// Returns sound frequency.
		virtual Real getFrequency() const=0;

		/// Sets sound volume.
		virtual void setVolume(Real _volume)=0;
		/// Returns sound volume.
		virtual Real getVolume() const=0;

		/** Sets pan.
			@param
				_pan if < 0 the sound is panned left. If > 0 - right. Ranges [-1;1].
			@remarks
				This method should be used only on sound instances of 2D sounds.
		*/
		virtual void setPan(Real _pan)=0;
		/// Returns sound pan.
		virtual Real getPan() const=0;

		/// Sets sound position in the world space.
		virtual void setPosition(Real _x, Real _y, Real _z)=0;
		/// Sets sound velocity.
		virtual void setVelocity(Real _x, Real _y, Real _z)=0;

		/// Returns internal sound object.
		virtual ISound* getSound() const=0;
	};



	inline SOUND_DESC& ISound::getDesc()
	{
		return m_Desc;
	}
//----------------------------------------------------------------------
	inline void ISound::setName(const wstring& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
	inline wstring& ISound::getName()
	{
		return m_stName;
	}
//----------------------------------------------------------------------
}


#endif
