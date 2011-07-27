/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SoundManager.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FMODSoundManager.h"
#include "FMODSound.h"
#include "Camera.h"
#include "SoundManagerEnumerator.h"

#include "FMOD.hpp"
#include "FMOD_Errors.h"


namespace nGENE
{
	FMODSoundManager::FMODSoundManager():
		m_pSoundSystem(NULL),
		m_bEnabled(true)
	{
	}
//----------------------------------------------------------------------
	FMODSoundManager::~FMODSoundManager()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	ISoundInstance* FMODSoundManager::createSound(const wstring& _name, const SOUND_DESC& _desc)
	{
		// File already exists, so return it
		if(m_Sounds.find(_name) != m_Sounds.end())
		{
			return new FMODSoundInstance((FMODSound*)m_Sounds[_name]);
		}

		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating sound: %ls from file: %ls", _name.c_str(), _desc.fileName.c_str());

		// Create new sound
		FMODSound* pSound = new FMODSound(m_pSoundSystem, _desc);
		pSound->init();
		pSound->setName(_name);
		m_Sounds[_name] = pSound;

		return new FMODSoundInstance(pSound);
	}
//----------------------------------------------------------------------
	ISound* FMODSoundManager::getSound(const wstring& _name)
	{
		if(m_Sounds.find(_name) != m_Sounds.end())
			return m_Sounds[_name];
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Sound: %ls does not exist", _name.c_str());
			return NULL;
		}
	}
//----------------------------------------------------------------------
	bool FMODSoundManager::hasSound(const wstring& _name) const
	{
		if(m_Sounds.find(_name) != m_Sounds.end())
			return true;
		return false;
	}
//----------------------------------------------------------------------
	void FMODSoundManager::removeSound(const wstring& _name)
	{
		// Sound doesn't exist so return
		if(m_Sounds.find(_name) == m_Sounds.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Sound: %ls does not exist", _name.c_str());
			return;
		}

		NGENE_DELETE(m_Sounds[_name]);
		m_Sounds.erase(_name);
	}
//----------------------------------------------------------------------
	void FMODSoundManager::init()
	{
		FMOD_RESULT result = FMOD::System_Create(&m_pSoundSystem);
		if(result != FMOD_OK)
		{
			string temp = FMOD_ErrorString(result);
			wstring message(temp.begin(), temp.end());
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"FMOD error! %ls", message.c_str());
			
			return;
		}

		int nNumDrivers = 0;
		m_pSoundSystem->getNumDrivers(&nNumDrivers);

		// No sound
		if(!nNumDrivers)
			m_pSoundSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		else
		{
			FMOD_CAPS caps;
			FMOD_SPEAKERMODE speakerMode;

			m_pSoundSystem->getDriverCaps(0, &caps, 0, 0, &speakerMode);
			m_pSoundSystem->setSpeakerMode(speakerMode);

			if(caps & FMOD_CAPS_HARDWARE_EMULATED)
			{
				m_pSoundSystem->setDSPBufferSize(1024, 10);
				Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"No hardware acceleration present for sound!");
			}
		}

		result = m_pSoundSystem->init(100, FMOD_INIT_NORMAL, 0);
		if(result == FMOD_ERR_OUTPUT_CREATEBUFFER)
		{
			m_pSoundSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
			result = m_pSoundSystem->init(100, FMOD_INIT_NORMAL, 0);

			if(result != FMOD_OK)
			{
				string temp = FMOD_ErrorString(result);
				wstring message(temp.begin(), temp.end());
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"FMOD error! %ls", message.c_str());

				return;
			}
		}

		// Using metric measures
		m_pSoundSystem->set3DSettings(1.0f, 1.0f, 1.0f);


		Timer& timer = Engine::getSingleton().getTimer();
		m_dwLast = timer.getMilliseconds();
	}
//----------------------------------------------------------------------
	void FMODSoundManager::cleanup()
	{
		if(m_Sounds.size())
		{
			HashTable <wstring, ISound*>::iterator iter;
			for(iter = m_Sounds.begin(); iter != m_Sounds.end(); ++iter)
				NGENE_DELETE(iter->second);
			m_Sounds.clear();
		}

		m_pSoundSystem->close();
		m_pSoundSystem->release();
	}
//----------------------------------------------------------------------
	void FMODSoundManager::update()
	{
		// Sound manager disabled, so return
		if(!m_bEnabled)
			return;

		Camera* pCamera = Engine::getSingleton().getActiveCamera();
		if(!pCamera)
			return;

		Timer& timer = Engine::getSingleton().getTimer();
		dword now = timer.getMilliseconds();
		dword delta = now - m_dwLast;

		Vector3& pos = pCamera->getPositionLocal();
		Vector3& forward = pCamera->getForward();
		Vector3& up = pCamera->getUp();
		Vector3 velocity = (pos - m_vecLastPos) / (float)delta;

		// Store current position
		m_vecLastPos = pos;

		FMOD_VECTOR vecPos = {pos.x, pos.y, pos.z};
		FMOD_VECTOR vecVel = {velocity.x, velocity.y, velocity.z};
		FMOD_VECTOR vecForward = {forward.x, forward.y, forward.z};
		FMOD_VECTOR vecUp = {up.x, up.y, up.z};

		m_pSoundSystem->set3DListenerAttributes(0, &vecPos, &vecVel, &vecForward, &vecUp);
		m_pSoundSystem->update();

		m_dwLast = now;
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	FMODSoundManagerFactory::FMODSoundManagerFactory()
	{
	}
//----------------------------------------------------------------------
	ISoundManager* FMODSoundManagerFactory::createSoundManager()
	{
		return (new FMODSoundManager());
	}
//----------------------------------------------------------------------
}