/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyFrameListener.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "MyUpdateListener.h"
#include "nGENE.h"

#include "App.h"

#include <sstream>

using namespace nGENE;


MyUpdateListener::MyUpdateListener():
	m_pApp(NULL),
	m_fScrollSpeed(0.0f),
	m_nPrev(0.0f),
	m_bNeedsRotating(false)
{
}
//----------------------------------------------------------------------
MyUpdateListener::~MyUpdateListener()
{
}
//----------------------------------------------------------------------
void MyUpdateListener::handleEvent(const EngineUpdateEvent& _evt)
{
	NodeSound* pMusic = m_pApp->getMusic();
	if(!pMusic->getSound()->isPlaying())
		m_pApp->playSoundTrack();

	m_fScrollSpeed = Maths::lerp(0.2f, 3.0f, (m_pApp->getScrollStage() + 5.0) / 2000.0f) * 
		m_pApp->getSpeedRate();

	MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"game");
	float fWind = m_fScrollSpeed / 0.2f;
	pMatLib->getMaterial(L"clouds")->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getShader()->getConstant("wind")->setDefaultValue(&fWind);

	// Make movement (vertical scrolling of the map)
	Vector3 movement(0.0f, 0.0f, m_fScrollSpeed);
	PlayerCharacter& player = m_pApp->getPlayer();

	float stage = (float)player.getCurrentHP() / (float)player.getMaxHP();
	if(stage <= 0.5f)
	{
		float fVolume = Maths::lerp(0.35f, 1.0f, 1.0f - stage * 2.0f);
		if(!m_pApp->isPlayerDead())
		{
			m_pApp->getHeartBeatSound()->getSound()->setVolume(fVolume);
			m_pApp->getMusic()->getSound()->setVolume(1.0f - fVolume);
		}
		else
		{
			m_pApp->getHeartBeatSound()->getSound()->stop();
			m_pApp->getMusic()->getSound()->setVolume(0.2f);
		}


		if(!m_pApp->getHeartBeatSound()->getSound()->isPlaying())
			m_pApp->getHeartBeatSound()->getSound()->play();
	}
	else
	{
		m_pApp->getHeartBeatSound()->getSound()->stop();
		m_pApp->getMusic()->getSound()->setVolume(1.0f);
	}

	if(!m_pApp->isPlayerDead())
	{
		player.move(movement);
		m_pApp->getCamera()->move(m_fScrollSpeed);
	}

	// Move the camera to show currently active map region
	Vector3 vecTarget = m_pApp->getCamera()->getPositionLocal();


	m_pApp->setScrollStage(m_pApp->getCamera()->getPositionLocal().z - 5.0f);
	
	Vector3 vecLight = vecTarget;
	NodeLight* pLight = (NodeLight*)Engine::getSingleton().getSceneManager(0)->getNode(L"ShadowLight");
	vecLight.y = 20.0f;
	pLight->setPosition(vecLight);

	if(!m_pApp->getSloMo())
	{
		MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");
		float fLevel = 1.0f - (float)player.getCurrentHP() / (float)player.getMaxHP();
		pMatLib->getMaterial(L"selectiveColouring")->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getShader()->getConstant("level")->setDefaultValue(&fLevel);
	}

	if(player.isStrafing())
	{
		player.setStrafing(false);

		m_nPrev = Engine::getSingleton().getTimer().getMilliseconds();
		m_nPassedRotate = 0;

		m_bNeedsRotating = true;
	}
	else if(m_bNeedsRotating)
	{
		uint now = Engine::getSingleton().getTimer().getMilliseconds();
		uint diff = now - m_nPrev;
		m_nPassedRotate += diff;
		if(m_nPassedRotate > 250)
		{
			m_nPassedRotate = 250;
			m_bNeedsRotating = false;
		}

		float angle = Maths::lerp(player.getAngle(), 0.0f, (float)m_nPassedRotate / 500.0f);
		player.setAngle(angle);

		Quaternion quatRot(Vector3::UNIT_Z, angle);
		player.getController()->setRotation(quatRot);

		m_nPrev = now;
	}


	// Update AI manager
	AIManager::getSingleton().update();


	if(!m_pApp->isPlayerDead())
	{
		// Remove enemies
		m_pApp->cleanEnemies();
		// Remove pickups
		m_pApp->cleanPickupItems();
		// Remove projectiles
		m_pApp->cleanProjectiles();
		// Remove explosions
		m_pApp->cleanExplosions();
		// Remove decorators
		m_pApp->cleanDecorators();


		// Generate new enemies
		m_pApp->generateEnemies();
		// Generate new decorators
		m_pApp->generateDecorators();
	}


	// Update weapons
	m_pApp->updateWeapons();
	// Update usable items
	m_pApp->updateItems();
	// Update explosions
	m_pApp->updateExplosions();

	//cout << Engine::getSingleton().getSceneManager(0)->getScenePartitioner()->getNodesVisibleCount() << endl;
}
//----------------------------------------------------------------------