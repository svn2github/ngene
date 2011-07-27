/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FMODSound.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FMODSound.h"

#include "FMOD.hpp"
#include "FMOD_Errors.h"


namespace nGENE
{
	FMODSound::FMODSound(FMOD::System* _system, const SOUND_DESC& _desc):
		ISound(_desc),
		m_pSound(NULL),
		m_pSystem(_system),
		m_bIsLooped(false)
	{
	}
//----------------------------------------------------------------------
	FMODSound::~FMODSound()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void FMODSound::init()
	{
		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(m_Desc.fileName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
		uint nSize = pFile->getSize();
		char* pBuffer = new char[nSize];
		pFile->read((char*)pBuffer, nSize);
		mgr.closeFile(m_Desc.fileName);

		FMOD_CREATESOUNDEXINFO desc;
		memset(&desc, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		desc.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		desc.length = nSize;

		uint flags = FMOD_OPENMEMORY;
		flags |= m_Desc.is3D ? FMOD_3D : FMOD_2D;
		flags |= m_Desc.isUsingHardware ? 0 : FMOD_SOFTWARE;
		flags |= m_Desc.isCompressed ? FMOD_CREATECOMPRESSEDSAMPLE : FMOD_CREATESAMPLE;

		FMOD_RESULT result = m_pSystem->createSound(pBuffer,
													flags,
													&desc,
													&m_pSound);

		NGENE_DELETE_ARRAY(pBuffer);
		// Something went wrong
		if(result != FMOD_OK)
		{
			string temp = FMOD_ErrorString(result);
			wstring message(temp.begin(), temp.end());
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"FMOD error! %ls", message.c_str());
			return;
		}

		m_pSound->set3DMinMaxDistance(0.5f, 10000.0f);
	}
//----------------------------------------------------------------------
	void FMODSound::cleanup()
	{
		if(m_pSound)
		{
			m_pSound->release();
			m_pSound = NULL;
		}
	}
//----------------------------------------------------------------------
	void FMODSound::setLooped(bool _value)
	{
		m_bIsLooped = _value;
		m_pSound->setMode(m_bIsLooped ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	}
//----------------------------------------------------------------------
	FMOD::Sound* FMODSound::getSound() const
	{
		return m_pSound;
	}
//----------------------------------------------------------------------
	FMOD::System* FMODSound::getSystem() const
	{
		return m_pSystem;
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	FMODSoundInstance::FMODSoundInstance(FMODSound* _sound):
		m_pChannel(NULL),
		m_pSound(_sound),
		m_bIsPaused(true),
		m_fPan(0.0f),
		m_fFrequency(0.0f),
		m_fVolume(0.0f)
	{
	}
//----------------------------------------------------------------------
	FMODSoundInstance::~FMODSoundInstance()
	{
		m_pChannel->stop();
	}
//----------------------------------------------------------------------
	void FMODSoundInstance::play()
	{
		if(!m_pChannel)
		{
			FMOD_RESULT result = m_pSound->getSystem()->playSound(FMOD_CHANNEL_FREE,
																  m_pSound->getSound(),
																  m_bIsPaused,
																  &m_pChannel);
			// Something went wrong
			if(result != FMOD_OK)
			{
				string temp = FMOD_ErrorString(result);
				wstring message(temp.begin(), temp.end());
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"FMOD error! %ls", message.c_str());
				return;
			}
		}
		else
		{
			bool bPlay;
			m_pChannel->isPlaying(&bPlay);
			if(bPlay)
				m_pChannel->stop();

			FMOD_RESULT result = m_pSound->getSystem()->playSound(FMOD_CHANNEL_FREE,
																  m_pSound->getSound(),
																  m_bIsPaused,
																  &m_pChannel);

			// Something went wrong
			if(result != FMOD_OK)
			{
				string temp = FMOD_ErrorString(result);
				wstring message(temp.begin(), temp.end());
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"FMOD error! %ls", message.c_str());
				return;
			}
		}

		if(m_fFrequency > 0.0f)
			setFrequency(m_fFrequency);
		if(m_fPan > 0.0f)
			setPan(m_fPan);
		if(m_fVolume > 0.0f)
			setVolume(m_fVolume);

		
		pause(false);
	}
//----------------------------------------------------------------------
	void FMODSoundInstance::stop()
	{
		if(m_pChannel)
			m_pChannel->stop();
	}
//----------------------------------------------------------------------
	bool FMODSoundInstance::isPlaying() const
	{
		bool bPlay;
		FMOD_RESULT result = m_pChannel->isPlaying(&bPlay);
		if(result != FMOD_OK)
		{
			string temp = FMOD_ErrorString(result);
			wstring message(temp.begin(), temp.end());
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"FMOD error! %ls", message.c_str());
			return false;
		}

		return bPlay;
	}
//----------------------------------------------------------------------
	void FMODSoundInstance::pause(bool _value)
	{
		m_bIsPaused = _value;
		if(m_pChannel)
			m_pChannel->setPaused(m_bIsPaused);
	}
//----------------------------------------------------------------------
	void FMODSoundInstance::setPosition(Real _x, Real _y, Real _z)
	{
		m_vecPosition.x = _x;
		m_vecPosition.y = _y;
		m_vecPosition.z = _z;

		FMOD_VECTOR vecPosition = {_x, _y, _z};
		FMOD_VECTOR vecVelocity = {m_vecVelocity.x, m_vecVelocity.y, m_vecVelocity.z};

		m_pChannel->set3DAttributes(&vecPosition, &vecVelocity);
	}
//----------------------------------------------------------------------
	void FMODSoundInstance::setVelocity(Real _x, Real _y, Real _z)
	{
		m_vecVelocity.x = _x;
		m_vecVelocity.y = _y;
		m_vecVelocity.z = _z;

		FMOD_VECTOR vecPosition = {m_vecPosition.x, m_vecPosition.y, m_vecPosition.z};
		FMOD_VECTOR vecVelocity = {_x, _y, _z};

		m_pChannel->set3DAttributes(&vecPosition, &vecVelocity);
	}
//----------------------------------------------------------------------
	void FMODSoundInstance::setFrequency(Real _frequency)
	{
		m_fFrequency = _frequency;
		FMOD_RESULT result = m_pChannel->setFrequency(m_fFrequency);

		// Something went wrong
		if(result != FMOD_OK)
		{
			string temp = FMOD_ErrorString(result);
			wstring message(temp.begin(), temp.end());
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"FMOD error! %ls", message.c_str());
			return;
		}
	}
//----------------------------------------------------------------------
	Real FMODSoundInstance::getFrequency() const
	{
		m_pChannel->getFrequency((float*)&m_fFrequency);

		return m_fFrequency;
	}
//----------------------------------------------------------------------
	void FMODSoundInstance::setVolume(Real _volume)
	{
		m_fVolume = _volume;
		m_pChannel->setVolume(m_fVolume);
	}
//----------------------------------------------------------------------
	Real FMODSoundInstance::getVolume() const
	{
		m_pChannel->getVolume((float*)&m_fVolume);

		return m_fVolume;
	}
//----------------------------------------------------------------------
	void FMODSoundInstance::setPan(Real _pan)
	{
		m_fPan = _pan;
		m_pChannel->setPan(m_fPan);
	}
//----------------------------------------------------------------------
	Real FMODSoundInstance::getPan() const
	{
		return m_fPan;
	}
//----------------------------------------------------------------------
}