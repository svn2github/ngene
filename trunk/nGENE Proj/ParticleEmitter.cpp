/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ParticleEmitter.cpp
Version:	0.14
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "NodeParticleSystem.h"
#include "Ray.h"
#include "Surface.h"
#include "VertexBufferManager.h"

#include <algorithm>


namespace nGENE
{
	// Initialize static members
	TypeInfo ParticleEmitter::Type(L"ParticleEmitter", NULL);
	TypeInfo SParticleForce::Type(L"SParticleForce", NULL);
	TypeInfo SParticleColour::Type(L"SParticleColour", NULL);
	TypeInfo SParticleDeflector::Type(L"SParticleDeflector", NULL);

	
	bool particleComparator(const Particle& _arg1, const Particle& _arg2)
	{
		Vector3 vecCamPosition = Engine::getSingleton().getActiveCamera()->getPositionLocal();
		Vector3 vecToParticle1 = _arg1.getPosition() - vecCamPosition;
		Vector3 vecToParticle2 = _arg2.getPosition() - vecCamPosition;

		Vector3 vecDir = Engine::getSingleton().getActiveCamera()->getForward();
		float fDistance1 = vecToParticle1.dotProduct(vecDir);
		float fDistance2 = vecToParticle2.dotProduct(vecDir);

		if(fDistance1 > fDistance2)
			return true;

		return false;
	}
//----------------------------------------------------------------------
	bool particleColourComparator(const PARTICLE_COLOUR& _arg1, const PARTICLE_COLOUR& _arg2)
	{
		if(_arg1.fLifeTimeLevel < _arg2.fLifeTimeLevel)
			return true;

		return false;
	}
//----------------------------------------------------------------------
	ParticleEmitter::ParticleEmitter():
		m_pParticleSystem(NULL),
		m_EmitterType(ET_POINT),
		m_nMaxParticles(2000),
		m_pSurface(NULL),
		m_pVB(NULL),
		m_pIVB(NULL),
		m_vecVelocity(Vector3::ZERO_VECTOR),
		m_vecVelocitySpread(Vector3::ZERO_VECTOR),
		m_vecAcceleration(Vector3::ZERO_VECTOR),
		m_vecAccelerationSpread(Vector3::ZERO_VECTOR),
		m_fLifeTime(1000.0f),
		m_fSize(0.0f),
		m_fSizeSpread(0.0f),
		m_fGrowth(0.0f),
		m_fGrowthSpread(0.0f),
		m_bIsDepthSorted(false),
		m_bEmitOnce(false),
		m_nSpawnRate(100),
		m_nAlreadySpawned(0),
		m_nWarmUpTime(0),
		m_nTimePassed(0),
		m_fAngularVelocity(0.0f),
		m_fAngularSpread(0.0f),
		m_fFadeSpeed(1.0f)
	{
	}
//----------------------------------------------------------------------
	ParticleEmitter::ParticleEmitter(const ParticleEmitter& src):
		ISerializable(src)
	{
		m_AABB = src.m_AABB;
		m_bIsDepthSorted = src.m_bIsDepthSorted;
		m_bEmitOnce = src.m_bEmitOnce;
		m_Colour = src.m_Colour;
		m_EmitterType = src.m_EmitterType;
		m_fAngularSpread = src.m_fAngularSpread;
		m_fAngularVelocity = src.m_fAngularVelocity;
		m_fFadeSpeed = src.m_fFadeSpeed;
		m_fGrowth = src.m_fGrowth;
		m_fGrowthSpread = src.m_fGrowthSpread;
		m_fLifeTime = src.m_fLifeTime;
		m_fLifeTimeSpread = src.m_fLifeTimeSpread;
		m_fSize = src.m_fSize;
		m_fSizeSpread = src.m_fSizeSpread;
		m_nMaxParticles = src.m_nMaxParticles;
		m_nSpawnRate = src.m_nSpawnRate;
		m_nAlreadySpawned = src.m_nAlreadySpawned;
		m_nWarmUpTime = src.m_nWarmUpTime;
		m_nTimePassed = 0;
		m_OBB = src.m_OBB;
		m_pIVB = src.m_pIVB;
		m_pParticleSystem = src.m_pParticleSystem;
		m_pSurface = src.m_pSurface;
		m_pVB = src.m_pVB;
		m_stName = src.m_stName;
		m_TargetColour = src.m_TargetColour;
		m_vColours = src.m_vColours;
		m_vecAcceleration = src.m_vecAcceleration;
		m_vecAccelerationSpread = src.m_vecAccelerationSpread;
		m_vecEmitterBox = src.m_vecEmitterBox;
		m_vecPosition = src.m_vecPosition;
		m_vecVelocity = src.m_vecVelocity;
		m_vecVelocitySpread = src.m_vecVelocitySpread;
		m_vForces = src.m_vForces;
		m_vDeflectors = src.m_vDeflectors;
		m_vParticles = src.m_vParticles;

		if(m_pSurface)
			m_pSurface->onUpdateAABB.bind(this, &ParticleEmitter::updateAABB);
	}
//----------------------------------------------------------------------
	ParticleEmitter& ParticleEmitter::operator=(const ParticleEmitter& rhs)
	{
		if(this == &rhs)
			return (*this);

		m_AABB = rhs.m_AABB;
		m_bIsDepthSorted = rhs.m_bIsDepthSorted;
		m_bEmitOnce = rhs.m_bEmitOnce;
		m_Colour = rhs.m_Colour;
		m_EmitterType = rhs.m_EmitterType;
		m_fAngularSpread = rhs.m_fAngularSpread;
		m_fAngularVelocity = rhs.m_fAngularVelocity;
		m_fFadeSpeed = rhs.m_fFadeSpeed;
		m_fGrowth = rhs.m_fGrowth;
		m_fGrowthSpread = rhs.m_fGrowthSpread;
		m_fLifeTime = rhs.m_fLifeTime;
		m_fLifeTimeSpread = rhs.m_fLifeTimeSpread;
		m_fSize = rhs.m_fSize;
		m_fSizeSpread = rhs.m_fSizeSpread;
		m_nMaxParticles = rhs.m_nMaxParticles;
		m_nSpawnRate = rhs.m_nSpawnRate;
		m_nAlreadySpawned = rhs.m_nAlreadySpawned;
		m_nWarmUpTime = rhs.m_nWarmUpTime;
		m_nTimePassed = 0;
		m_OBB = rhs.m_OBB;
		m_pIVB = rhs.m_pIVB;
		m_pParticleSystem = rhs.m_pParticleSystem;
		m_pSurface = rhs.m_pSurface;
		m_pVB = rhs.m_pVB;
		m_stName = rhs.m_stName;
		m_TargetColour = rhs.m_TargetColour;
		m_vColours = rhs.m_vColours;
		m_vecAcceleration = rhs.m_vecAcceleration;
		m_vecAccelerationSpread = rhs.m_vecAccelerationSpread;
		m_vecEmitterBox = rhs.m_vecEmitterBox;
		m_vecPosition = rhs.m_vecPosition;
		m_vecVelocity = rhs.m_vecVelocity;
		m_vecVelocitySpread = rhs.m_vecVelocitySpread;
		m_vForces = rhs.m_vForces;
		m_vDeflectors = rhs.m_vDeflectors;
		m_vParticles = rhs.m_vParticles;

		m_bIsSerializable = rhs.m_bIsSerializable;

		if(m_pSurface)
			m_pSurface->onUpdateAABB.bind(this, &ParticleEmitter::updateAABB);

		return (*this);
	}
//----------------------------------------------------------------------
	ParticleEmitter::~ParticleEmitter()
	{
		if(!m_vParticles.empty())
			m_vParticles.clear();

		if(!m_vForces.empty())
			m_vForces.clear();

		if(!m_vColours.empty())
			m_vColours.clear();

		if(!m_vDeflectors.empty())
			m_vDeflectors.clear();

		m_pParticleSystem = NULL;

		m_pVB = NULL;
		m_pIVB = NULL;
	}
//----------------------------------------------------------------------
	void ParticleEmitter::update(Real _delta)
	{
		if(m_nWarmUpTime)
		{
			if(m_nTimePassed < m_nWarmUpTime)
			{
				m_nTimePassed += FastFloat::floatToInt(_delta);
				if(m_nTimePassed < m_nWarmUpTime)
					return;
			}
		}

		// Spawn particles
		uint nSpawnCount = FastFloat::floatToInt(ceil(static_cast<float>(m_nSpawnRate) * _delta * 0.001f));
		if(m_bEmitOnce)
		{
			if(m_nAlreadySpawned != m_nMaxParticles)
			{
				m_nAlreadySpawned += nSpawnCount;
				uint toSpawn = nSpawnCount;
				if(m_nAlreadySpawned > m_nMaxParticles)
				{
					toSpawn = m_nAlreadySpawned - m_nMaxParticles;
					m_nAlreadySpawned = m_nMaxParticles;
				}

				spawn(toSpawn);
			}
		}
		else
			spawn(nSpawnCount);


		// Update existing particles
		for(uint i = 0; i < m_vParticles.size(); ++i)
		{
			Particle& particle = m_vParticles[i];
			particle.update(_delta);
			if(particle.getPassedTime() >= particle.getLifeTime())
				m_vParticles.erase(m_vParticles.begin() + i);
			else
			{
				// Update source/target colour data
				updateColour(particle);
				
				// Compute forces influence
				updateForces(particle, _delta);

				// Computes deflectors influence
				updateDeflectors(particle, _delta);
			}
		}

		// Depth-sort particles if necessary
		if(m_bIsDepthSorted)
			std::sort(m_vParticles.begin(), m_vParticles.end(), particleComparator);

		m_pSurface->setChanged(true);

		// Update geometry
		updateGeometry();
	}
//----------------------------------------------------------------------
	void ParticleEmitter::spawn(uint _count)
	{
		for(uint i = 0; i < _count; ++i)
		{
			if(m_vParticles.size() >= m_nMaxParticles)
				break;

			Particle particle;
			m_vParticles.push_back(particle);
			reset(m_vParticles.size() - 1);
		}
	}
//----------------------------------------------------------------------
	void ParticleEmitter::updateColour(Particle& _particle)
	{
		vector <PARTICLE_COLOUR>::iterator iter;
		vector <PARTICLE_COLOUR>::iterator iter2;

		for(iter = m_vColours.begin(); iter != m_vColours.end(); ++iter)
		{
			iter2 = iter;
			++iter2;

			if(iter2 == m_vColours.end())
				break;

			if(_particle.getPassedTime() >= iter->fLifeTimeLevel * _particle.getLifeTime() &&
			   _particle.getPassedTime() < iter2->fLifeTimeLevel * _particle.getLifeTime())
			{
				if(iter->colour != _particle.getStartColour())
				{
					_particle.setStartColour(iter->colour);
					_particle.setFadeStart(iter->fLifeTimeLevel);
				}

				if(iter2->colour != _particle.getTargetColour())
				{
					_particle.setTargetColour(iter2->colour);
					_particle.setFadeEnd(iter2->fLifeTimeLevel);
				}
			}
		}

	}
//----------------------------------------------------------------------
	void ParticleEmitter::updateForces(Particle& _particle, Real _delta)
	{
		for(uint i = 0; i < m_vForces.size(); ++i)
		{
			PARTICLE_FORCE& force = m_vForces[i];

			Vector3 vecDirection = _particle.getPosition() - force.vecPosition;
			float fDistanceSqr = vecDirection.dotProduct(vecDirection);
			float fForce = force.fStrength / (1.0f + sqrtf(fDistanceSqr) * force.fLinearAtt +
				fDistanceSqr * force.fQuadraticAtt);
			vecDirection *= (fForce * _delta * 0.001f);

			_particle.setVelocity(_particle.getVelocity() + vecDirection);
		}
	}
//----------------------------------------------------------------------
	void ParticleEmitter::updateDeflectors(Particle& _particle, Real _delta)
	{
		for(uint i = 0; i < m_vDeflectors.size(); ++i)
		{
			PARTICLE_DEFLECTOR& deflector = m_vDeflectors[i];

			Vector3 oldPos = _particle.getOldPosition();
			Vector3 dir = _particle.getPosition() - oldPos;
			float length = dir.getLength() + _particle.getSize() * 0.5f;
			dir.normalize();
			Point point(oldPos);
			Ray ray(point, dir);
			if(ray.intersectPlane(deflector.plane) && ray.getTValue() <= length)
			{
				Vector3 vecVelocity = _particle.getVelocity().deflect(deflector.plane);
				_particle.setVelocity(vecVelocity * deflector.restitution);
			}
		}
	}
//----------------------------------------------------------------------
	void ParticleEmitter::updateGeometry()
	{
		updateVertices();
		updateIndices();

		m_pSurface->updateBoundingBox();
	}
//----------------------------------------------------------------------
	void ParticleEmitter::updateVertices()
	{
		uint nVerticesNum = m_vParticles.size() * 4;
		SVertex* pVertices;

		m_pVB->lock(0, nVerticesNum, (void**)&pVertices);


		Vector3 vecMin = Vector3(Maths::Infinity, Maths::Infinity, Maths::Infinity);
		Vector3 vecMax = Vector3(Maths::MinusInfinity, Maths::MinusInfinity, Maths::MinusInfinity);


		for(uint i = 0; i < nVerticesNum; i += 4)
		{
			Particle& particle = m_vParticles[i / 4];
			float fSize = particle.getSize();
			float fSizeHalf = fSize * 0.5f;

			Vector3 vecPosition = particle.getPosition();

			if(vecPosition.x - fSizeHalf < vecMin.x)
				vecMin.x = vecPosition.x - fSizeHalf;
			if(vecPosition.y - fSizeHalf < vecMin.y)
				vecMin.y = vecPosition.y - fSizeHalf;
			if(vecPosition.z - fSizeHalf < vecMin.z)
				vecMin.z = vecPosition.z - fSizeHalf;

			if(vecPosition.x + fSizeHalf > vecMax.x)
				vecMax.x = vecPosition.x + fSizeHalf;
			if(vecPosition.y + fSizeHalf > vecMax.y)
				vecMax.y = vecPosition.y + fSizeHalf;
			if(vecPosition.z + fSizeHalf > vecMax.z)
				vecMax.z = vecPosition.z + fSizeHalf;


			float fAngle = particle.getRotation();

			pVertices[i + 0].vecPosition = vecPosition;
			pVertices[i + 0].vecNormal = Vector3(-1.0f, -1.0f, fSize);
			pVertices[i + 0].vecTexCoord = Vector2(fAngle, 1.0f);
			pVertices[i + 0].dwColour = particle.getColour();
			
			pVertices[i + 1].vecPosition = vecPosition;
			pVertices[i + 1].vecNormal = Vector3(1.0f, -1.0f, fSize);
			pVertices[i + 1].vecTexCoord = Vector2(fAngle, 1.0f);
			pVertices[i + 1].dwColour = particle.getColour();

			pVertices[i + 2].vecPosition = vecPosition;
			pVertices[i + 2].vecNormal = Vector3(1.0f, 1.0f, fSize);
			pVertices[i + 2].vecTexCoord = Vector2(fAngle, 1.0f);
			pVertices[i + 2].dwColour = particle.getColour();
			
			pVertices[i + 3].vecPosition = vecPosition;
			pVertices[i + 3].vecNormal = Vector3(-1.0f, 1.0f, fSize);
			pVertices[i + 3].vecTexCoord = Vector2(fAngle, 1.0f);
			pVertices[i + 3].dwColour = particle.getColour();
		}


		m_OBB.setOBB(vecMin, vecMax);
		Vector3 temp = vecMin - vecMax;
		if(temp.getLength() <= Maths::FloatEpsilon)
		{
			m_OBB.setOBB(Vector3(-1.0f, -1.0f, -1.0f),
						 Vector3(1.0f, 1.0f, 1.0f));
		}


		m_pVB->unlock();
		m_pVB->setPrimitivesNum(nVerticesNum / 2);
	}
//----------------------------------------------------------------------
	void ParticleEmitter::updateIndices()
	{
		uint nIndicesNum = m_vParticles.size() * 6;
		ushort* pIndices;

		m_pIVB->lock(0, nIndicesNum, (void**)&pIndices);


		ushort begin = 0;
		for(uint i = 0; i < nIndicesNum; i += 6)
		{
			pIndices[i + 0] = begin;
			pIndices[i + 1] = begin + 1;
			pIndices[i + 2] = begin + 3;
			pIndices[i + 3] = begin + 1;
			pIndices[i + 4] = begin + 2;
			pIndices[i + 5] = begin + 3;

			begin += 4;
		}


		m_pIVB->unlock();
	}
//----------------------------------------------------------------------
	void ParticleEmitter::setSurface(Surface* _surface)
	{
		uint nVerticesNum = m_vParticles.size() * 4;
		uint nIndicesNum = m_nMaxParticles * 6;

		SVertex* pVertices = new SVertex[4 * m_nMaxParticles];
		ushort* pIndices = new ushort[nIndicesNum];

		if(m_pVB)
			m_pVB = NULL;

		VertexBufferManager& manager = VertexBufferManager::getSingleton();


		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.type = IVB_DYNAMIC;
		ivbDesc.indicesNum = nIndicesNum;
		ivbDesc.indices = pIndices;
		m_pIVB = manager.createIndexedBuffer(ivbDesc);

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.type = VB_DYNAMIC;
		vbDesc.vertices = pVertices;
		vbDesc.verticesNum = 4 * m_nMaxParticles;
		vbDesc.primitivesNum = nVerticesNum / 2;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		m_pVB = manager.createVertexBuffer(vbDesc);
		m_pVB->setIndexedBuffer(m_pIVB);

		m_pSurface = _surface;
		m_pSurface->setVertexBuffer(m_pVB);
		m_pSurface->onUpdateAABB.bind(this, &ParticleEmitter::updateAABB);

		update(0.0f);

		NGENE_DELETE_ARRAY(pIndices);
		NGENE_DELETE_ARRAY(pVertices);
	}
//----------------------------------------------------------------------
	void ParticleEmitter::reset(uint _index)
	{
		if(m_vParticles.size() > 1)
			Maths::clamp <uint> (_index, 0, m_vParticles.size() - 1);


		Particle& particle = m_vParticles[_index];

		particle.reset();

		Vector3 vecPosition = m_vecPosition + m_pParticleSystem->getPositionWorld();
		switch(m_EmitterType)
		{
		case ET_POINT: particle.setPosition(vecPosition);
					   break;
		case ET_BOX:   particle.setPosition(vecPosition +
						  Vector3(m_vecEmitterBox.x * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000),
								  m_vecEmitterBox.y * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000),
								  m_vecEmitterBox.z * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000)));
					   break;
		}


		// Reset colour
		if(m_vColours.size() > 1)
		{
			vector <PARTICLE_COLOUR>::iterator iter;
			iter = m_vColours.begin();
			particle.setStartColour(iter->colour);
			particle.setTargetColour((++iter)->colour);
		}
		else if(m_vColours.size() == 1)
		{
			vector <PARTICLE_COLOUR>::iterator iter;
			iter = m_vColours.begin();
			particle.setStartColour(iter->colour);
			particle.setTargetColour(iter->colour);
		}
		else
		{
			particle.setStartColour(Colour::COLOUR_WHITE);
			particle.setTargetColour(Colour::COLOUR_WHITE);
		}

		float fSpread = m_fSizeSpread * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000);
		particle.setSize(m_fSize + fSpread);

		Vector3 vecSpread(m_vecVelocitySpread.x * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000),
						  m_vecVelocitySpread.y * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000),
						  m_vecVelocitySpread.z * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000));
		
		Vector3 vecVelocity = m_vecVelocity + vecSpread;

		particle.setVelocity(vecVelocity);
		particle.setFadeSpeed(m_fFadeSpeed);

		vecSpread = Vector3(m_vecAccelerationSpread.x * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000),
							Maths::abs <float> (m_vecAccelerationSpread.y * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000)),
							m_vecAccelerationSpread.z * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000));
		particle.setAcceleration(m_vecAcceleration + vecSpread);

		fSpread = m_fGrowthSpread * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000);
		particle.setGrowth(m_fGrowth + fSpread);

		fSpread = m_fAngularSpread * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000);
		particle.setAngularVelocity(m_fAngularVelocity + fSpread);

		fSpread = m_fLifeTimeSpread * Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000);
		particle.setLifeTime(m_fLifeTime + fSpread);
	}
//----------------------------------------------------------------------
	void ParticleEmitter::removeForce(uint _index)
	{
		if(!m_vForces.size())
			return;

		Assert(_index < m_vForces.size(), "Particle force index >= forces vector size");

		m_vForces.erase(m_vForces.begin() + _index);
	}
//----------------------------------------------------------------------
	void ParticleEmitter::removeDeflector(uint _index)
	{
		if(!m_vDeflectors.size())
			return;

		Assert(_index < m_vDeflectors.size(), "Particle deflector index >= deflectors vector size");

		m_vDeflectors.erase(m_vDeflectors.begin() + _index);
	}
//----------------------------------------------------------------------
	PARTICLE_FORCE* ParticleEmitter::getForce(uint _index)
	{
		if(_index >= m_vForces.size())
			return NULL;

		return &m_vForces[_index];
	}
//----------------------------------------------------------------------
	PARTICLE_DEFLECTOR* ParticleEmitter::getDeflector(uint _index)
	{
		if(_index >= m_vDeflectors.size())
			return NULL;

		return &m_vDeflectors[_index];
	}
//----------------------------------------------------------------------
	void ParticleEmitter::addParticleColour(PARTICLE_COLOUR& _colour)
	{
		Maths::clamp <float>(_colour.fLifeTimeLevel, 0.0f, 1.0f);
		m_vColours.push_back(_colour);

		sortColours();
	}
//----------------------------------------------------------------------
	void ParticleEmitter::addParticleColour(const Colour& _colour, Real _level)
	{
		PARTICLE_COLOUR colour(_colour, _level);
		addParticleColour(colour);
	}
//----------------------------------------------------------------------
	uint ParticleEmitter::getParticleColoursNum() const
	{
		return m_vColours.size();
	}
//----------------------------------------------------------------------
	PARTICLE_COLOUR* ParticleEmitter::getParticleColour(uint _index)
	{
		if(_index < m_vColours.size())
			return &m_vColours[_index];

		return NULL;
	}
//----------------------------------------------------------------------
	Surface* ParticleEmitter::getSurface()
	{
		return m_pSurface;
	}
//----------------------------------------------------------------------
	void ParticleEmitter::updateAABB()
	{
		if(m_pSurface && m_pParticleSystem)
		{
			Matrix4x4 mat;
			mat.translate(m_vecPosition);
			m_OBB.setTransform(mat);
			m_AABB.construct(m_OBB);

			m_pSurface->setOBB(m_OBB);
			m_pSurface->setAABB(m_AABB);
		}
	}
//----------------------------------------------------------------------
	void ParticleEmitter::sortColours()
	{
		if(m_vColours.size() > 1)
			std::sort(m_vColours.begin(), m_vColours.end(), particleColourComparator);
	}
//----------------------------------------------------------------------
	void ParticleEmitter::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		// Not serializable so return
		if(!isSerializable())
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			
			Property <wstring> prName(m_stName);
			_serializer->addProperty("Name", prName);

			Property <uint> prMaxParticles(m_nMaxParticles);
			_serializer->addProperty("ParticlesCount", prMaxParticles);
			Property <uint> prSpawnRate(m_nSpawnRate);
			_serializer->addProperty("SpawnRate", prSpawnRate);
			Property <uint> prWarmUpTime(m_nWarmUpTime);
			_serializer->addProperty("WarmUpTime", prWarmUpTime);

			Property <bool> prDepthSorted(m_bIsDepthSorted);
			_serializer->addProperty("DephtSorted", prDepthSorted);
			Property <bool> prSpawnOnce(m_bEmitOnce);
			_serializer->addProperty("SpawnOnce", prSpawnOnce);

			Property <float> prLifeTime(m_fLifeTime);
			_serializer->addProperty("LifeTime", prLifeTime);
			Property <float> prLifeTimeSpread(m_fLifeTimeSpread);
			_serializer->addProperty("LifeTimeSpread", prLifeTimeSpread);
			Property <float> prSize(m_fSize);
			_serializer->addProperty("Size", prSize);
			Property <float> prSizeSpread(m_fSizeSpread);
			_serializer->addProperty("SizeSpread", prSizeSpread);
			Property <float> prGrowth(m_fGrowth);
			_serializer->addProperty("Growth", prGrowth);
			Property <float> prGrowthSpread(m_fGrowthSpread);
			_serializer->addProperty("GrowthSpread", prGrowthSpread);
			Property <float> prAngular(m_fAngularVelocity);
			_serializer->addProperty("AngularVelocity", prAngular);
			Property <float> prAngularSpread(m_fAngularSpread);
			_serializer->addProperty("AngularSpread", prAngularSpread);
			Property <float> prFadeSpeed(m_fFadeSpeed);
			_serializer->addProperty("FadeSpeed", prFadeSpeed);

			Property <Vector3> prEmitterBox(m_vecEmitterBox);
			_serializer->addProperty("EmitterBox", prEmitterBox);
			Property <Vector3> prPosition(m_vecPosition);
			_serializer->addProperty("Position", prPosition);
			Property <Vector3> prVelocity(m_vecVelocity);
			_serializer->addProperty("Velocity", prVelocity);
			Property <Vector3> prVelocitySpread(m_vecVelocitySpread);
			_serializer->addProperty("VelocitySpread", prVelocitySpread);
			Property <Vector3> prAccel(m_vecAcceleration);
			_serializer->addProperty("Acceleration", prAccel);
			Property <Vector3> prAccelSpread(m_vecAccelerationSpread);
			_serializer->addProperty("AccelerationSpread", prAccelSpread);

			// Serialize forces, deflectors and colours
			uint count = m_vForces.size();
			Property <uint> prForcesCount(count);
			_serializer->addProperty("ForcesCount", prForcesCount);

			count = m_vDeflectors.size();
			Property <uint> prDeflectorsCount(count);
			_serializer->addProperty("DeflectorsCount", prDeflectorsCount);

			count = m_vColours.size();
			Property <uint> prColoursCount(count);
			_serializer->addProperty("ColoursCount", prColoursCount);

			for(uint i = 0; i < m_vForces.size(); ++i)
				m_vForces[i].serialize(_serializer, true, _serializeChildren);

			for(uint i = 0; i < m_vDeflectors.size(); ++i)
				m_vDeflectors[i].serialize(_serializer, true, _serializeChildren);

			for(uint i = 0; i < m_vColours.size(); ++i)
				m_vColours[i].serialize(_serializer, true, _serializeChildren);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void ParticleEmitter::deserialize(ISerializer* _serializer)
	{
		Property <wstring> prName(m_stName);
		_serializer->getProperty("Name", prName);

		Property <uint> prMaxParticles(m_nMaxParticles);
		_serializer->getProperty("ParticlesCount", prMaxParticles);
		Property <uint> prSpawnRate(m_nSpawnRate);
		_serializer->getProperty("SpawnRate", prSpawnRate);
		Property <uint> prWarmUpTime(m_nWarmUpTime);
		_serializer->getProperty("WarmUpTime", prWarmUpTime);

		Property <bool> prDepthSorted(m_bIsDepthSorted);
		_serializer->getProperty("DephtSorted", prDepthSorted);
		Property <bool> prSpawnOnce(m_bEmitOnce);
		_serializer->getProperty("SpawnOnce", prSpawnOnce);

		Property <float> prLifeTime(m_fLifeTime);
		_serializer->getProperty("LifeTime", prLifeTime);
		Property <float> prLifeTimeSpread(m_fLifeTimeSpread);
		_serializer->getProperty("LifeTimeSpread", prLifeTimeSpread);
		Property <float> prSize(m_fSize);
		_serializer->getProperty("Size", prSize);
		Property <float> prSizeSpread(m_fSizeSpread);
		_serializer->getProperty("SizeSpread", prSizeSpread);
		Property <float> prGrowth(m_fGrowth);
		_serializer->getProperty("Growth", prGrowth);
		Property <float> prGrowthSpread(m_fGrowthSpread);
		_serializer->getProperty("GrowthSpread", prGrowthSpread);
		Property <float> prAngular(m_fAngularVelocity);
		_serializer->getProperty("AngularVelocity", prAngular);
		Property <float> prAngularSpread(m_fAngularSpread);
		_serializer->getProperty("AngularSpread", prAngularSpread);
		Property <float> prFadeSpeed(m_fFadeSpeed);
		_serializer->getProperty("FadeSpeed", prFadeSpeed);

		Property <Vector3> prEmitterBox(m_vecEmitterBox);
		_serializer->getProperty("EmitterBox", prEmitterBox);
		Property <Vector3> prPosition(m_vecPosition);
		_serializer->getProperty("Position", prPosition);
		Property <Vector3> prVelocity(m_vecVelocity);
		_serializer->getProperty("Velocity", prVelocity);
		Property <Vector3> prVelocitySpread(m_vecVelocitySpread);
		_serializer->getProperty("VelocitySpread", prVelocitySpread);
		Property <Vector3> prAccel(m_vecAcceleration);
		_serializer->getProperty("Acceleration", prAccel);
		Property <Vector3> prAccelSpread(m_vecAccelerationSpread);
		_serializer->getProperty("AccelerationSpread", prAccelSpread);

		// Deserialize particle forces and colours
		uint count;
		Property <uint> prForcesCount(count);
		_serializer->getProperty("ForcesCount", prForcesCount);

		uint countDfl = m_vDeflectors.size();
		Property <uint> prDeflectorsCount(countDfl);
		_serializer->getProperty("DeflectorsCount", prDeflectorsCount);

		uint countClr;
		Property <uint> prColoursCount(countClr);
		_serializer->getProperty("ColoursCount", prColoursCount);

		for(uint i = 0; i < count; ++i)
		{
			SParticleForce force;
			_serializer->getNextObject();
			force.deserialize(_serializer);

			addForce(force);
		}

		for(uint i = 0; i < countDfl; ++i)
		{
			SParticleDeflector deflector;
			_serializer->getNextObject();
			deflector.deserialize(_serializer);

			addDeflector(deflector);
		}

		for(uint i = 0; i < countClr; ++i)
		{
			SParticleColour colour;
			_serializer->getNextObject();
			colour.deserialize(_serializer);

			addParticleColour(colour);
		}

		// Now add emitter to the particle system
		if(_serializer->getParent())
			((NodeParticleSystem*)_serializer->getParent())->addParticleEmitter(*this, getName());
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	void SParticleForce::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		// Not serializable so return
		if(!isSerializable())
			return;

		if(_serializeType) _serializer->addObject(this->Type);

			Property <Vector3> prPosition(vecPosition);
			_serializer->addProperty("Position", prPosition);

			Property <Real> prStrength(fStrength);
			_serializer->addProperty("Strength", prStrength);
			Property <Real> prLinear(fLinearAtt);
			_serializer->addProperty("LinearAtt", prLinear);
			Property <Real> prQuadratic(fQuadraticAtt);
			_serializer->addProperty("QuadraticAtt", prQuadratic);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void SParticleForce::deserialize(ISerializer* _serializer)
	{
		Property <Vector3> prPosition(vecPosition);
		_serializer->getProperty("Position", prPosition);

		Property <Real> prStrength(fStrength);
		_serializer->getProperty("Strength", prStrength);
		Property <Real> prLinear(fLinearAtt);
		_serializer->getProperty("LinearAtt", prLinear);
		Property <Real> prQuadratic(fQuadraticAtt);
		_serializer->getProperty("QuadraticAtt", prQuadratic);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	void SParticleColour::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		// Not serializable so return
		if(!isSerializable())
			return;

		if(_serializeType) _serializer->addObject(this->Type);

			Property <Colour> prColour(colour);
			_serializer->addProperty("Colour", prColour);

			Property <Real> prLife(fLifeTimeLevel);
			_serializer->addProperty("Level", prLife);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void SParticleColour::deserialize(ISerializer* _serializer)
	{
		Property <Colour> prColour(colour);
		_serializer->getProperty("Colour", prColour);

		Property <Real> prLife(fLifeTimeLevel);
		_serializer->getProperty("Level", prLife);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	void SParticleDeflector::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		// Not serializable so return
		if(!isSerializable())
			return;

		if(_serializeType) _serializer->addObject(this->Type);

			Property <Plane> prPlane(plane);
			_serializer->addProperty("Plane", prPlane);

			Property <Real> prBounciness(restitution);
			_serializer->addProperty("Restitution", prBounciness);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void SParticleDeflector::deserialize(ISerializer* _serializer)
	{
		Property <Plane> prPlane(plane);
		_serializer->getProperty("Plane", prPlane);

		Property <Real> prBounciness(restitution);
		_serializer->getProperty("Restitution", prBounciness);
	}
//----------------------------------------------------------------------
}