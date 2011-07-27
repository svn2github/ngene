/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ParticleSystemWrapper.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "ParticleSystemWrapper.h"
#include "frmSceneGraph.h"

#include <vcclr.h>


using nGENE::Material;
using nGENE::MaterialLibrary;
using nGENE::MaterialManager;


namespace nGENEToolset
{
	void ParticleColourWrapper::refreshEmitter(float value)
	{
		m_pColour->fLifeTimeLevel = value;
		if(m_Emitter)
			m_Emitter->sortColours();
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	ParticleEmitterWrapper::ParticleEmitterWrapper()
	{
		m_pEmitter = new ParticleEmitter();
		m_pEmitter->setParticlesMaxCount(600);
		m_pEmitter->setSpawnRate(12);
		m_pEmitter->setSize(0.075f);
		m_pEmitter->setSizeSpread(0.025f);
		m_pEmitter->setGrowth(0.45f);
		m_pEmitter->setGrowthSpread(0.2f);
		m_pEmitter->setLifeTime(15000.0f);
		m_pEmitter->setLifeTimeSpread(0.0f);
		m_pEmitter->setFadeSpeed(1.5f);

		m_pEmitter->setPosition(nGENE::Vector3(0.0f, 0.0f, 0.0f));
		m_pEmitter->setVelocity(nGENE::Vector3(0.075f, 0.15f, 0.075f));
		m_pEmitter->setVelocitySpread(nGENE::Vector3(0.075f, 0.5f, 0.075f));
		m_pEmitter->setAccelerationSpread(nGENE::Vector3(0.0f, 0.0f, 0.2f));
		m_pEmitter->setAcceleration(nGENE::Vector3(-0.2f, -0.025f, 0.0f));
		m_pEmitter->setAngularVelocity(Maths::PI / 18.0f);
		m_pEmitter->setAngularVelocitySpread(0.02f);

		m_Position = gcnew Vector3(&m_pEmitter->getPosition());
		m_Velocity = gcnew Vector3(&m_pEmitter->getVelocity());
		m_VelocitySpread = gcnew Vector3(&m_pEmitter->getVelocitySpread());
		m_Accel = gcnew Vector3(&m_pEmitter->getAcceleration());
		m_AccelSpread = gcnew Vector3(&m_pEmitter->getAccelerationSpread());

		m_bOnHeap = true;


		this->Name = "ParticleEmitter" + (s_nEmittersCount);
		this->Key = "ParticleEmitter" + (s_nEmittersCount++);

		m_ColoursList = gcnew System::Collections::ArrayList();
	}
//----------------------------------------------------------------------
	ParticleEmitterWrapper::ParticleEmitterWrapper(ParticleEmitter* _emitter)
	{
		m_pEmitter = _emitter;
		s_nEmittersCount++;

		m_Position = gcnew Vector3(&_emitter->getPosition());
		m_Velocity = gcnew Vector3(&_emitter->getVelocity());
		m_VelocitySpread = gcnew Vector3(&_emitter->getVelocitySpread());
		m_Accel = gcnew Vector3(&_emitter->getAcceleration());
		m_AccelSpread = gcnew Vector3(&_emitter->getAccelerationSpread());

		m_bOnHeap = false;

		this->Name = gcnew String(_emitter->getName().c_str());

		m_ColoursList = gcnew System::Collections::ArrayList();
	}
//----------------------------------------------------------------------
	void ParticleEmitterWrapper::parseColours()
	{
		for(uint i = 0; i < m_pEmitter->getParticleColoursNum(); ++i)
		{
			ParticleColourWrapper^ colour = gcnew ParticleColourWrapper();
			colour->setColour(NULL);
			colour->setEmitter(this);

			frmSceneGraph::graph->add(colour);

			m_ColoursList->Add(colour);
		}

		sortColours();
	}
//----------------------------------------------------------------------
	void ParticleEmitterWrapper::parseForces()
	{
		for(uint i = 0; i < m_pEmitter->getForcesNum(); ++i)
		{
			ParticleForceWrapper^ force = gcnew ParticleForceWrapper(m_pEmitter->getForce(i));

			frmSceneGraph::graph->add(force);
		}
	}
//----------------------------------------------------------------------
	void ParticleEmitterWrapper::parseDeflectors()
	{
		for(uint i = 0; i < m_pEmitter->getDeflectorsNum(); ++i)
		{
			ParticleDeflectorWrapper^ deflector = gcnew ParticleDeflectorWrapper(m_pEmitter->getDeflector(i));

			frmSceneGraph::graph->add(deflector);
		}
	}
//----------------------------------------------------------------------
	System::Void ParticleEmitterWrapper::addColour(ParticleColourWrapper^ colour)
	{
		frmRenderTarget::engine->getLock();

		m_pEmitter->addParticleColour(*colour->getColour());
		colour->setColour(NULL);

		colour->setEmitter(this);
		frmSceneGraph::graph->addNode(colour);

		m_ColoursList->Add(colour);

		for(unsigned int i = 0; i < m_pEmitter->getParticleColoursNum(); ++i)
		{
			((ParticleColourWrapper^)m_ColoursList[i])->setColour(m_pEmitter->getParticleColour(i));
			((ParticleColourWrapper^)m_ColoursList[i])->Colour = Color::FromArgb(255,
						 m_pEmitter->getParticleColour(i)->colour.getRed(),
						 m_pEmitter->getParticleColour(i)->colour.getGreen(),
						 m_pEmitter->getParticleColour(i)->colour.getBlue());
		}

		frmRenderTarget::engine->releaseLock();
	}
//----------------------------------------------------------------------
	System::Void ParticleEmitterWrapper::addForce(ParticleForceWrapper^ force)
	{
		frmRenderTarget::engine->getLock();

		m_pEmitter->addForce(*force->getForce());
		frmSceneGraph::graph->addNode(force);
		force->setForce(m_pEmitter->getForce(m_pEmitter->getForcesNum() - 1));

		frmRenderTarget::engine->releaseLock();
	}
//----------------------------------------------------------------------
	System::Void ParticleEmitterWrapper::addDeflector(ParticleDeflectorWrapper^ deflector)
	{
		frmRenderTarget::engine->getLock();

		m_pEmitter->addDeflector(*deflector->getDeflector());
		frmSceneGraph::graph->addNode(deflector);
		deflector->setDeflector(m_pEmitter->getDeflector(m_pEmitter->getDeflectorsNum() - 1));

		frmRenderTarget::engine->releaseLock();
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	System::Void ParticleSystemWrapper::addEmitter(ParticleEmitterWrapper^ emitter)
	{
		System::String^ str = emitter->Name;
		pin_ptr <const wchar_t> str1 = PtrToStringChars(str);
		m_pPS->addParticleEmitter(*emitter->getEmitter(), str1);

		ParticleEmitterWrapper^ myEmitter = gcnew ParticleEmitterWrapper(m_pPS->getParticleEmitter(m_pPS->getParticleEmittersNum() - 1));
		frmSceneGraph::graph->addNode(myEmitter);

		Material* matSmoke = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"smoke");
		m_pPS->getSurface(str1)->setMaterial(matSmoke);
	}
//----------------------------------------------------------------------
	ParticleSystemWrapper::ParticleSystemWrapper(NodeParticleSystem* _ps, String^ _name):
		NodeVisibleWrapper(_ps, _name, 0)
	{
		m_pPS = _ps;
		s_nPSCount++;

		frmSceneGraph::graph->beginAdd();
		for(uint i = 0; i < m_pPS->getParticleEmittersNum(); ++i)
		{
			ParticleEmitter* pPE = m_pPS->getParticleEmitter(i);
			String^ str = gcnew String(pPE->getName().c_str());
			ParticleEmitterWrapper^ emitter = gcnew ParticleEmitterWrapper(pPE);
			frmSceneGraph::graph->add(emitter);
			frmSceneGraph::graph->selectNode(emitter->Key);

			emitter->parseColours();
			emitter->parseForces();
			emitter->parseDeflectors();

			frmSceneGraph::graph->selectParent();
		}
		frmSceneGraph::graph->endAdd();
	}
//----------------------------------------------------------------------
	ParticleSystemWrapper::ParticleSystemWrapper()
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		m_pPS = sm->createParticleSystem();
		m_pPS->setPosition(0.0f, 0.0f, 0.0f);

		m_Position = gcnew Vector3(&m_pPS->getPositionLocal());
		m_Dimensions = gcnew Vector3(&m_pPS->getScale());

		//m_PositionWorld->setVector(m_pNode->getPositionWorld());

		m_pNode = m_pPS;

		wostringstream buffer;
		buffer << "ParticleSystem";
		buffer << s_nPSCount;
		wstring text = buffer.str();
		if(frmSceneGraph::graph->getActive() != nullptr)
			frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pPS);

		this->Name = "ParticleSystem" + (s_nPSCount++);


		frmSceneGraph::graph->addNode(this);
	}
//----------------------------------------------------------------------
}