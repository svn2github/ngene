/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ParticleEmitter.h
Version:	0.09
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PARTICLEEMITTER_H_
#define __INC_PARTICLEEMITTER_H_


#include "Prerequisities.h"
#include "AABB.h"
#include "Colour.h"
#include "ISerializable.h"
#include "Particle.h"
#include "OBB.h"
#include "Plane.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Point force, affecting speed (and thus direction) of particle
		movement.
	*/
	typedef struct nGENEDLL SParticleForce: public ISerializable
	{
		EXPOSE_TYPE

		Vector3 vecPosition;				///< Position of the force

		Real fStrength;						///< Strength of the force
		Real fLinearAtt;					///< Linear attenuation of strength (0.0 - 1.0)
		Real fQuadraticAtt;					///< Quadratic attenuation of strength (0.0 - 1.0)

		SParticleForce():
			fStrength(0.0f),
			fLinearAtt(0.0f),
			fQuadraticAtt(0.0f)
		{}

		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	} PARTICLE_FORCE;

	/** Deflector
		@par
			Deflects particles.
	*/
	typedef struct nGENEDLL SParticleDeflector: public ISerializable
	{
		EXPOSE_TYPE

		Plane plane;						///< Plane representing deflector
		Real restitution;					/**< How much will the particle bounce off.
												 Values in most cases should range [0; 1]
												 although other values will also work. */

		SParticleDeflector():
			plane(Plane::UNIT_Y),
			restitution(1.0f)
		{}

		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	} PARTICLE_DEFLECTOR;

	/// Type of the emitter
	enum nGENEDLL EMITTER_TYPE
	{
		ET_POINT,							///< Default particle emitter 
		ET_BOX
	};

	/** Colour of the particle.
		@remarks
			Multiple colours can be used and interpolated between
			according to the time passed.
	*/
	typedef struct nGENEDLL SParticleColour: public ISerializable
	{
		EXPOSE_TYPE

		Colour colour;						///< Colour of the particle
		float fLifeTimeLevel;				/** Value from 0.0 to 1.0 representing
												passed lifetime fraction. */

		SParticleColour() {}
		SParticleColour(const Colour& _colour, Real _level):
			colour(_colour),
			fLifeTimeLevel(_level)
		{
		}

		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	} PARTICLE_COLOUR;


	/** Particle emitter emits particles.
		@par
			If you want to give emitter some time without emitting any particles
			you can do this by calling ParticleEmitter::setWarmUpTime()
			method and passing number of milliseconds.
		@remarks
			If you change any of the existing emitter colours' fLifeTimeLevel
			member's value, then you have to manually sort colours by invoking
			ParticleEmitter::sortColours() method.
	*/
	class nGENEDLL ParticleEmitter: public ISerializable
	{
		EXPOSE_TYPE
	private:
		AABB m_AABB;							///< Particle emitter helper AABB
		OBB m_OBB;								///< Particle emitter helper OBB

		NodeParticleSystem* m_pParticleSystem;	///< Pointer to the particle system


		EMITTER_TYPE m_EmitterType;				///< Type of the emitter

		uint m_nMaxParticles;					///< Maximum number of particles

		uint m_nSpawnRate;						/**< Spawn rate (i.e. how many new particles
													 are created each second). */
		uint m_nAlreadySpawned;					///< Number of particles already spawned
		uint m_nWarmUpTime;						/**< Time in milliseconds after which emitter starts
													 emitting particles. */
		uint m_nTimePassed;

		Vector3 m_vecEmitterBox;				///< Useful for box emitter

		/// Vector holding all particles.
		vector <Particle> m_vParticles;

		/// Vector holding all point forces.
		vector <PARTICLE_FORCE> m_vForces;

		/// Vector holding all deflectors.
		vector <PARTICLE_DEFLECTOR> m_vDeflectors;

		/// List holding all colours
		vector <PARTICLE_COLOUR> m_vColours;

		Colour m_Colour;
		Colour m_TargetColour;

		Surface* m_pSurface;					///< Pointer to the surface object

		VertexBuffer* m_pVB;					///< Vertex buffer with particles
		IndexedBuffer* m_pIVB;					///< Indices buffer

		Vector3 m_vecPosition;					///< Emitter position

		Vector3 m_vecVelocity;					///< Average particle velocity
		Vector3 m_vecVelocitySpread;			///< Maximum difference in velocity

		Vector3 m_vecAcceleration;				///< Average particle acceleration
		Vector3 m_vecAccelerationSpread;		///< Maximum difference in acceleration

		float m_fLifeTime;						///< Average particle life time
		float m_fLifeTimeSpread;				///< Maximum difference in life time

		float m_fSize;							///< Average particle base size
		float m_fSizeSpread;					///< Maximum difference in size
		float m_fGrowth;						///< Average particle growth
		float m_fGrowthSpread;					///< Maximum difference in growth

		float m_fAngularVelocity;				///< Average angular velocity of the particle
		float m_fAngularSpread;					///< Maximum difference in angular velocity

		float m_fFadeSpeed;						///< Colour fade speed

		bool m_bIsDepthSorted;					///< Are particles Z-sorted?
		bool m_bEmitOnce;						///< Should emit the particles only once?

		wstring m_stName;						///< Emitter name


		/// Resets single particle.
		virtual void reset(uint _index);

		/// Spawns new particles.
		void spawn(uint _count);

		/// Updates colour of the given particle.
		void updateColour(Particle& _particle);
		/// Updates forces influence on the particle.
		void updateForces(Particle& _particle, Real _delta);
		/// Updates deflectors influence on the particle.
		void updateDeflectors(Particle& _particle, Real _delta);

		/// Updates geometry data.
		void updateGeometry();

		/// Updates vertices data.
		void updateVertices();
		/// Updates indices data.
		void updateIndices();

		void updateAABB();

	public:
		ParticleEmitter();
		ParticleEmitter(const ParticleEmitter& src);
		virtual ~ParticleEmitter();

		/// Assignment operator.
		ParticleEmitter& operator=(const ParticleEmitter& rhs);


		/// Updates all particles of the emitter.
		void update(Real _delta);

		/// Sets surface.
		void setSurface(Surface* _surface);
		Surface* getSurface();


		/// Adds new force.
		void addForce(const PARTICLE_FORCE& _force);
		/// Removes given force.
		void removeForce(uint _index);
		/// Returns specified force.
		PARTICLE_FORCE* getForce(uint _index);
		/// Returns number of forces.
		uint getForcesNum() const;

		/// Adds new deflector.
		void addDeflector(const PARTICLE_DEFLECTOR& _deflector);
		/// Removes given deflector.
		void removeDeflector(uint _index);
		/// Returns specified deflector.
		PARTICLE_DEFLECTOR* getDeflector(uint _index);
		/// Returns number of deflectors.
		uint getDeflectorsNum() const;

		/// Adds new colour.
		void addParticleColour(PARTICLE_COLOUR& _colour);
		/// Adds new colour.
		void addParticleColour(const Colour& _colour, Real _level);
		/// Returns colour with the specified index.
		PARTICLE_COLOUR* getParticleColour(uint _index);
		/// Returns total number of colours.
		uint getParticleColoursNum() const;


		// Underneath is a whole bunch of accessors


		/// Sets particle system with which this emitter is associated.
		void setParticleSystem(NodeParticleSystem* _ps);


		/// Sets emitter position relative to particle system position.
		void setPosition(const Vector3& _position);
		/// Sets emitter position relative to particle system position.
		void setPosition(Real _x, Real _y, Real _z);
		/// Returns emitter position relative to particle system position.
		Vector3& getPosition();

		/// Sets average particle velocity.
		void setVelocity(const Vector3& _velocity);
		/// Sets average particle velocity.
		void setVelocity(Real _x, Real _y, Real _z);
		/// Returns velocity vector.
		Vector3& getVelocity();

		/// Sets deviation of particles velocity.
		void setVelocitySpread(const Vector3& _spread);
		/// Sets deviation of particles velocity.
		void setVelocitySpread(Real _x, Real _y, Real _z);
		/// Returns velocity spread.
		Vector3& getVelocitySpread();

		/// Sets average angular velocity of a particle.
		void setAngularVelocity(Real _velocity);
		/// Returns angular velocity.
		Real getAngularVelocity() const;

		/// Sets angular velocity spread.
		void setAngularVelocitySpread(Real _spread);
		/// Returns angular velocity spread.
		Real getAngularVelocitySpread() const;

		/// Sets acceleration vector.
		void setAcceleration(const Vector3& _acceleration);
		/// Sets acceleration vector.
		void setAcceleration(Real _x, Real _y, Real _z);
		/// Returns acceleration.
		Vector3& getAcceleration();

		/// Sets deviation of acceleration spread.
		void setAccelerationSpread(const Vector3& _spread);
		/// Sets deviation of acceleration spread.
		void setAccelerationSpread(Real _x, Real _y, Real _z);
		/// Returns acceleration spread.
		Vector3& getAccelerationSpread();

		/// Sets average particle size.
		void setSize(Real _size);
		/// Returns average particle size.
		Real getSize() const;

		/// Sets deviation of particle size.
		void setSizeSpread(Real _spread);
		/// Returns particles size spread.
		Real getSizeSpread() const;

		/// Sets average particle growth.
		void setGrowth(Real _growth);
		/// Returns particle growth.
		Real getGrowth() const;

		/// Sets deviation of particle growth.
		void setGrowthSpread(Real _spread);
		/// Returns growth spread.
		Real getGrowthSpread() const;

		/// Sets average particle life time.
		void setLifeTime(Real _life);
		/// Returns life time length.
		Real getLifeTime() const;

		/// Sets deviation of particle life time.
		void setLifeTimeSpread(Real _spread);
		/// Returns life time spread.
		Real getLifeTimeSpread() const;

		/// Sets rate of generating new particles per second.
		void setSpawnRate(uint _rate);
		/// Returns spawn rate.
		uint getSpawnRate() const;

		/// Sets maximum number of active particles.
		void setParticlesMaxCount(uint _count);
		/// Returns maximum number of active particles.
		uint getParticlesMaxCount() const;

		void setColour(const Colour& _colour);
		void setTargetColour(const Colour& _colour);

		void setFadeSpeed(Real _speed);
		Real getFadeSpeed() const;

		/// Specifies if particles are depth sorted.
		void setDepthSorted(bool _value);
		/// Checks if depth sorting is used for particles.
		bool isDepthSorted() const;

		/// Specifies if it should emit particles only once.
		void setSpawnOnce(bool _spawn);
		/// Checks if the particles are emitted only once.
		bool getSpawnOnce() const;

		/// Has finished emitting?
		bool hasFinished() const;


		void setEmitterType(EMITTER_TYPE _type);
		EMITTER_TYPE getEmitterType() const;


		void setBoxEmitterSize(const Vector3& _size);


		void sortColours();


		/// Sets surface name.
		void setName(const wstring& _name);
		/// Returns surface name.
		wstring& getName();

		/// Sets warm up time.
		void setWarmUpTime(uint _value);
		/// Returns warm up time.
		uint getWarmUpTime() const;


		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};



	inline void ParticleEmitter::addForce(const PARTICLE_FORCE& _force)
	{
		m_vForces.push_back(_force);
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::addDeflector(const PARTICLE_DEFLECTOR& _deflector)
	{
		m_vDeflectors.push_back(_deflector);
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setParticleSystem(NodeParticleSystem* _ps)
	{
		m_pParticleSystem = _ps;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setPosition(const Vector3& _position)
	{
		m_vecPosition = _position;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setPosition(Real _x, Real _y, Real _z)
	{
		m_vecPosition.set(_x, _y, _z);
	}
//----------------------------------------------------------------------
	inline Vector3& ParticleEmitter::getPosition()
	{
		return m_vecPosition;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setVelocity(const Vector3& _velocity)
	{
		m_vecVelocity = _velocity;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setVelocity(Real _x, Real _y, Real _z)
	{
		m_vecVelocity.set(_x, _y, _z);
	}
//----------------------------------------------------------------------
	inline Vector3& ParticleEmitter::getVelocity()
	{
		return m_vecVelocity;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setVelocitySpread(const Vector3& _spread)
	{
		m_vecVelocitySpread = _spread;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setVelocitySpread(Real _x, Real _y, Real _z)
	{
		m_vecVelocitySpread.set(_x, _y, _z);
	}
//----------------------------------------------------------------------
	inline Vector3& ParticleEmitter::getVelocitySpread()
	{
		return m_vecVelocitySpread;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setAngularVelocity(Real _velocity)
	{
		m_fAngularVelocity = _velocity;
	}
//----------------------------------------------------------------------
	inline Real ParticleEmitter::getAngularVelocity() const
	{
		return m_fAngularVelocity;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setAngularVelocitySpread(Real _spread)
	{
		m_fAngularSpread = _spread;
	}
//----------------------------------------------------------------------
	inline Real ParticleEmitter::getAngularVelocitySpread() const
	{
		return m_fAngularSpread;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setAcceleration(const Vector3& _acceleration)
	{
		m_vecAcceleration = _acceleration;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setAcceleration(Real _x, Real _y, Real _z)
	{
		m_vecAcceleration.set(_x, _y, _z);
	}
//----------------------------------------------------------------------
	inline Vector3& ParticleEmitter::getAcceleration()
	{
		return m_vecAcceleration;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setAccelerationSpread(const Vector3& _spread)
	{
		m_vecAccelerationSpread = _spread;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setAccelerationSpread(Real _x, Real _y, Real _z)
	{
		m_vecAccelerationSpread.set(_x, _y, _z);
	}
//----------------------------------------------------------------------
	inline Vector3& ParticleEmitter::getAccelerationSpread()
	{
		return m_vecAccelerationSpread;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setSize(Real _size)
	{
		m_fSize = _size;
	}
//----------------------------------------------------------------------
	inline Real ParticleEmitter::getSize() const
	{
		return m_fSize;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setSizeSpread(Real _spread)
	{
		m_fSizeSpread = _spread;
	}
//----------------------------------------------------------------------
	inline Real ParticleEmitter::getSizeSpread() const
	{
		return m_fSizeSpread;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setGrowth(Real _growth)
	{
		m_fGrowth = _growth;
	}
//----------------------------------------------------------------------
	inline Real ParticleEmitter::getGrowth() const
	{
		return m_fGrowth;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setGrowthSpread(Real _spread)
	{
		m_fGrowthSpread = _spread;
	}
//----------------------------------------------------------------------
	inline Real ParticleEmitter::getGrowthSpread() const
	{
		return m_fGrowthSpread;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setLifeTime(Real _life)
	{
		m_fLifeTime = _life;
	}
//----------------------------------------------------------------------
	inline Real ParticleEmitter::getLifeTime() const
	{
		return m_fLifeTime;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setLifeTimeSpread(Real _spread)
	{
		m_fLifeTimeSpread = _spread;
	}
//----------------------------------------------------------------------
	inline Real ParticleEmitter::getLifeTimeSpread() const
	{
		return m_fLifeTimeSpread;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setSpawnRate(uint _rate)
	{
		m_nSpawnRate = _rate;
	}
//----------------------------------------------------------------------
	inline uint ParticleEmitter::getSpawnRate() const
	{
		return m_nSpawnRate;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setParticlesMaxCount(uint _count)
	{
		m_nMaxParticles = _count;
		if(m_pSurface)
		{
			m_vParticles.clear();
			setSurface(m_pSurface);
		}
	}
//----------------------------------------------------------------------
	inline uint ParticleEmitter::getParticlesMaxCount() const
	{
		return m_nMaxParticles;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setColour(const Colour& _colour)
	{
		addParticleColour(_colour, 0.0f);
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setTargetColour(const Colour& _colour)
	{
		addParticleColour(_colour, 1.0f);
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setFadeSpeed(Real _speed)
	{
		m_fFadeSpeed = _speed;
	}
//----------------------------------------------------------------------
	inline Real ParticleEmitter::getFadeSpeed() const
	{
		return m_fFadeSpeed;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setDepthSorted(bool _value)
	{
		m_bIsDepthSorted = _value;
	}
//----------------------------------------------------------------------
	inline bool ParticleEmitter::isDepthSorted() const
	{
		return m_bIsDepthSorted;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setEmitterType(EMITTER_TYPE _type)
	{
		m_EmitterType = _type;
	}
//---------------------------------------------------------------------- 
	inline EMITTER_TYPE ParticleEmitter::getEmitterType() const
	{
		return m_EmitterType;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setBoxEmitterSize(const Vector3& _size)
	{
		m_vecEmitterBox = _size;
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setName(const wstring& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
	inline wstring& ParticleEmitter::getName()
	{
		return m_stName;
	}
//----------------------------------------------------------------------
	inline uint ParticleEmitter::getForcesNum() const
	{
		return m_vForces.size();
	}
//----------------------------------------------------------------------
	inline uint ParticleEmitter::getDeflectorsNum() const
	{
		return m_vDeflectors.size();
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setSpawnOnce(bool _spawn)
	{
		m_bEmitOnce = _spawn;
	}
//----------------------------------------------------------------------
	inline bool ParticleEmitter::getSpawnOnce() const
	{
		return m_bEmitOnce;
	}
//----------------------------------------------------------------------
	inline bool ParticleEmitter::hasFinished() const
	{
		return (m_nAlreadySpawned >= m_nMaxParticles &&
				!m_vParticles.size());
	}
//----------------------------------------------------------------------
	inline void ParticleEmitter::setWarmUpTime(uint _value)
	{
		m_nWarmUpTime = _value;
		m_nTimePassed = 0;
	}
//----------------------------------------------------------------------
	inline uint ParticleEmitter::getWarmUpTime() const
	{
		return m_nWarmUpTime;
	}
//----------------------------------------------------------------------
}


#endif