/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FMODSoundManager.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FMODSOUNDMANAGER_H_
#define __INC_FMODSOUNDMANAGER_H_


#include "HashTable.h"
#include "ISoundManager.h"
#include "Prerequisities.h"


namespace FMOD
{
	class System;
}


namespace nGENE
{
	/** Manages all the sounds in the nGENE Tech.
		@par
			This class uses FMOD library internally.
	*/
	class nGENEDLL FMODSoundManager: public ISoundManager
	{
	private:
		/// Pointer to the sound system
		FMOD::System* m_pSoundSystem;

		/// HashTable with all sounds
		HashTable <wstring, ISound*> m_Sounds;

		/// Previous position of the listener
		Vector3 m_vecLastPos;

		dword m_dwLast;						///< Previous frame time

		bool m_bEnabled;					///< Is sound manager enabled?

	public:
		FMODSoundManager();
		virtual ~FMODSoundManager();

		/// Initializes sound system.
		void init();
		/// Frees allocated memory.
		void cleanup();

		/// Updates the sound system.
		void update();

		/// Creates new sound from file.
		ISoundInstance* createSound(const wstring& _name, const SOUND_DESC& _desc);
		/// Returns existing sound or NULL if it wasn't found.
		ISound* getSound(const wstring& _name);

		/// Checks for existence of given sound in the system.
		bool hasSound(const wstring& _name) const;

		/// Removes given sound from the system.
		void removeSound(const wstring& _name);

		/// Specifies whether manager is enabled.
		void setEnabled(bool _value);
		/// Checks if manager is enabled.
		bool isEnabled() const;
	};



	inline void FMODSoundManager::setEnabled(bool _value)
	{
		m_bEnabled = _value;
	}
//----------------------------------------------------------------------
	inline bool FMODSoundManager::isEnabled() const
	{
		return m_bEnabled;
	}
//----------------------------------------------------------------------



	/// Sound manager factory for the FMOD library.
	class nGENEDLL FMODSoundManagerFactory: public SoundManagerFactory
	{
	public:
		FMODSoundManagerFactory();

		ISoundManager* createSoundManager();
	};
}


#endif
