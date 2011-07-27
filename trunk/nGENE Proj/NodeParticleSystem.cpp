/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeParticleSystem.cpp
Version:	0.09
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DeviceRender.h"
#include "NodeParticleSystem.h"

#include "AABB.h"
#include "OBB.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo NodeParticleSystem::Type(L"NodeParticleSystem", &NodeVisible::Type);


	NodeParticleSystem::NodeParticleSystem():
		m_bValid(true)
	{
		m_dwLastTime = Engine::getSingleton().getTimer().getMilliseconds();

		ListenerRegistry <DeviceEvent>::addListener(this);
	}
//----------------------------------------------------------------------
	NodeParticleSystem::~NodeParticleSystem()
	{
		if(!m_vParticleEmitters.empty())
			m_vParticleEmitters.clear();

		ListenerRegistry <DeviceEvent>::removeListener(this);
	}
//----------------------------------------------------------------------
	void NodeParticleSystem::onUpdate()
	{
		// If node is invalid, return
		if(!m_bValid)
			return;

		Timer& timer = Engine::getSingleton().getTimer();
		float delta = static_cast<float>(timer.getMilliseconds() - m_dwLastTime);

		// First update particles
		for(uint i = 0; i < m_vParticleEmitters.size(); ++i)
			m_vParticleEmitters[i].update(delta);

		NodeVisible::onUpdate();

		m_dwLastTime = timer.getMilliseconds();
	}
//----------------------------------------------------------------------
	void NodeParticleSystem::addParticleEmitter(ParticleEmitter& _emitter, const wstring& _name)
	{
		Surface surface;
		addSurface(_name, surface);

		_emitter.setSurface(&m_Surfaces[_name]);
		_emitter.setParticleSystem(this);
		_emitter.setName(_name);

		m_vParticleEmitters.push_back(_emitter);
	}
//----------------------------------------------------------------------
	ParticleEmitter* NodeParticleSystem::getParticleEmitter(uint _index)
	{
		if(_index < m_vParticleEmitters.size())
			return &m_vParticleEmitters[_index];
		else
			return NULL;
	}
//----------------------------------------------------------------------
	uint NodeParticleSystem::getParticleEmittersNum() const
	{
		return m_vParticleEmitters.size();
	}
//----------------------------------------------------------------------
	void NodeParticleSystem::handleEvent(const DeviceEvent& _evt)
	{
		if(_evt.type == DET_DESTROYED || _evt.type == DET_LOST)
		{
			m_bValid = false;
		}
		else if(_evt.type == DET_RESET || _evt.type == DET_CREATED)
		{
			m_bValid = true;
		}
	}
//----------------------------------------------------------------------
	void NodeParticleSystem::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);

			NodeVisible::serialize(_serializer, false, _serializeChildren);

			/// Serialize particle emitters
			for(uint i = 0; i < m_vParticleEmitters.size(); ++i)
				m_vParticleEmitters[i].serialize(_serializer, true, true);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeParticleSystem::deserialize(ISerializer* _serializer)
	{
		NodeVisible::deserialize(_serializer);
	}
//----------------------------------------------------------------------
}