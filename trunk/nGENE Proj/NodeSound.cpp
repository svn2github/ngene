/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeSound.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "NodeSound.h"
#include "ISound.h"
#include "ISoundManager.h"
#include "ISerializer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo NodeSound::Type(L"NodeSound", &Node::Type);


	NodeSound::NodeSound()
	{
	}
//----------------------------------------------------------------------
	NodeSound::NodeSound(const wstring& _name, const SOUND_DESC& _desc)
	{
		createSound(_name, _desc);
	}
//----------------------------------------------------------------------
	NodeSound::~NodeSound()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void NodeSound::cleanup()
	{
		NGENE_DELETE(m_pSound);
	}
//----------------------------------------------------------------------
	void NodeSound::translate(const Vector3& _translate)
	{
		m_vecPosition += _translate;
		m_pSound->setPosition(m_vecPosition.x, m_vecPosition.y, m_vecPosition.z);

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void NodeSound::translate(Real _x, Real _y, Real _z)
	{
		m_vecPosition.x += _x;
		m_vecPosition.y += _y;
		m_vecPosition.z += _z;

		m_pSound->setPosition(m_vecPosition.x, m_vecPosition.y, m_vecPosition.z);

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void NodeSound::setPosition(const Vector3& _pos)
	{
		m_vecPosition = _pos;
		m_pSound->setPosition(m_vecPosition.x, m_vecPosition.y, m_vecPosition.z);

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void NodeSound::setPosition(Real _x, Real _y, Real _z)
	{
		m_vecPosition.set(_x, _y, _z);
		m_pSound->setPosition(_x, _y, _z);

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void NodeSound::onUpdate()
	{
		Node::onUpdate();

		dword now = Engine::getSingleton().getTimer().getMilliseconds();
		dword delta = now - m_nPrevTime;
		m_nPrevTime = now;

		if(m_bHasChanged)
		{
			Vector3 position = getPositionWorld();
			m_pSound->setPosition(position.x, position.y, position.z);

			Vector3 velocity = (m_vecPosition - m_vecLastPos) / (float)delta;
			m_pSound->setVelocity(velocity.x, velocity.y, velocity.z);

			m_vecLastPos = m_vecPosition;
		}
	}
//----------------------------------------------------------------------
	void NodeSound::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
		Node::serialize(_serializer, false, _serializeChildren);

		Property <wstring> prFile(m_pDesc->fileName);
		_serializer->addProperty("FileName", prFile);
		Property <wstring> prName(*m_pName);
		_serializer->addProperty("SoundName", prName);

		Property <bool> prIs3D(m_pDesc->is3D);
		_serializer->addProperty("Is3D", prIs3D);
		Property <bool> prIsCompressed(m_pDesc->isCompressed);
		_serializer->addProperty("IsCompressed", prIsCompressed);
		Property <bool> prIsUsingHardware(m_pDesc->isUsingHardware);
		_serializer->addProperty("IsUsingHardware", prIsUsingHardware);

		Real fTmp = m_pSound->getFrequency();
		Property <Real> prFrequency(fTmp);
		_serializer->addProperty("Frequency", prFrequency);

		fTmp = m_pSound->getVolume();
		Property <Real> prVolume(fTmp);
		_serializer->addProperty("Volume", prVolume);

		fTmp = m_pSound->getPan();
		Property <Real> prPan(fTmp);
		_serializer->addProperty("Pan", prPan);


		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeSound::deserialize(ISerializer* _serializer)
	{
		SOUND_DESC desc;

		Property <wstring> prFile(desc.fileName);
		_serializer->getProperty("FileName", prFile);

		wstring name;
		Property <wstring> prName(name);
		_serializer->getProperty("SoundName", prName);

		Property <bool> prIs3D(desc.is3D);
		_serializer->getProperty("Is3D", prIs3D);

		Property <bool> prIsCompressed(desc.isCompressed);
		_serializer->getProperty("IsCompressed", prIsCompressed);

		Property <bool> prIsUsingHardware(desc.isUsingHardware);
		_serializer->getProperty("IsUsingHardware", prIsUsingHardware);


		createSound(name, desc);

		m_pSound->play();


		Real fTmp;
		Property <Real> prFrequency(fTmp);
		_serializer->getProperty("Frequency", prFrequency);
		m_pSound->setFrequency(fTmp);

		Property <Real> prVolume(fTmp);
		_serializer->getProperty("Volume", prVolume);
		m_pSound->setVolume(fTmp);

		Property <Real> prPan(fTmp);
		_serializer->getProperty("Pan", prPan);
		m_pSound->setPan(fTmp);


		Node::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	void NodeSound::createSound(const wstring& _name, const SOUND_DESC& _desc)
	{
		ISoundManager& mgr = ISoundManager::getSingleton();
		m_pSound = mgr.createSound(_name, _desc);

		m_pDesc = &m_pSound->getSound()->getDesc();
		m_pName = &m_pSound->getSound()->getName();
	}
//----------------------------------------------------------------------
}