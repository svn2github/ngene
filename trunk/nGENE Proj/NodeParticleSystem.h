/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeParticleSystem.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEPARTICLESYSTEM_H_
#define __INC_NODEPARTICLESYSTEM_H_


#include "Prerequisities.h"
#include "NodeVisible.h"
#include "TypeInfo.h"
#include "ParticleEmitter.h"


namespace nGENE
{
	/** Container for particle emitters.
	*/
	class nGENEDLL NodeParticleSystem: public NodeVisible,
		public Listener <DeviceEvent>
	{
		EXPOSE_TYPE
	private:
		vector <ParticleEmitter> m_vParticleEmitters;

		dword m_dwLastTime;						///< Last update time

		bool m_bValid;							///< Is vertex data valid?


		void updateSurfaceAABB();

		/// Updates particle system.
		void onUpdate();

	public:
		NodeParticleSystem();
		virtual ~NodeParticleSystem();

		/** Adds new particle emitter.
			@remarks
				Particle system can have multiple emitters to create more
				interesting effects eg. in case of fire one emitter can
				emit fire particles, and another ones of smoke.
		*/
		void addParticleEmitter(ParticleEmitter& _emitter, const wstring& _name);
		ParticleEmitter* getParticleEmitter(uint _index);
		uint getParticleEmittersNum() const;

		void handleEvent(const DeviceEvent& _evt);

		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};
}


#endif