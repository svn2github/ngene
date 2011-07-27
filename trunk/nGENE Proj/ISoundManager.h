/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ISoundManager.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ISOUNDMANAGER_H_
#define __INC_ISOUNDMANAGER_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "Singleton.h"
#include "Task.h"


namespace nGENE
{
	/** Manages all the sounds in the nGENE Tech.
		@par
			It is abstract interface for managing sounds.
	*/
	class nGENEDLL ISoundManager: public Singleton <ISoundManager>,
		public Task
	{
	public:
		/// Initializes sound system.
		virtual void init()=0;
		/// Frees allocated memory.
		virtual void cleanup()=0;

		/// Updates the system.
		virtual void update()=0;

		/// Creates new sound instance from file.
		virtual ISoundInstance* createSound(const wstring& _name, const SOUND_DESC& _desc)=0;
		/// Returns existing sound or NULL if it wasn't found.
		virtual ISound* getSound(const wstring& _name)=0;

		/// Checks for existence of given sound in the system.
		virtual bool hasSound(const wstring& _name) const=0;

		/// Removes given sound from the system.
		virtual void removeSound(const wstring& _name)=0;

		/// Specifies whether manager is enabled.
		virtual void setEnabled(bool _value)=0;
		/// Checks if manager is enabled.
		virtual bool isEnabled() const=0;
	};


	/// Abstract interface for creating ISoundManager objects.
	class nGENEDLL SoundManagerFactory
	{
	public:
		virtual ISoundManager* createSoundManager()=0;
	};
}


#endif
