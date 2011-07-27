/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyInputListener.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "MyInputListener.h"
#include "nGENE.h"

#include "App.h"

#include <sstream>

using namespace nGENE;


MyInputListener::MyInputListener():
	m_pApp(NULL),
	m_BoxesNum(0),
	m_BoltsNum(0),
	m_Water(0),
	m_bFreezeMouseMovement(false),
	m_fNear(2.5f),
	m_fFar(10.0f),
	m_fFocus(5.0f),
	m_Angle1(0.0f),
	m_Angle2(0.0f)
{
}
//----------------------------------------------------------------------
MyInputListener::~MyInputListener()
{
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const MouseEvent& _evt)
{
	float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_prev) * 0.001f;

	if(!m_bFreezeMouseMovement)
	{
		CameraFirstPerson* pCamera = m_pApp->getCamera();
		if(_evt.type == MET_MOVE_X || _evt.type == MET_MOVE_Y)
		{
			if(_evt.type == MET_MOVE_X)
				m_Angle1 += _evt.fvalue;
			else if(_evt.type == MET_MOVE_Y)
				m_Angle2 += _evt.fvalue;

			Quaternion quatRot1(Vector3(0.0f, 1.0f, 0.0f), m_Angle1);
			Quaternion quatRot2(Vector3(1.0f, 0.0f, 0.0f), m_Angle2);

			pCamera->setRotation(quatRot1 * quatRot2);
		}

		if(_evt.type == MET_MOVE_Z)
		{
			float fov = pCamera->getFrustumPtr()->getFOV() - _evt.fvalue * 0.01f;
			Maths::clamp(fov, Maths::PI / 15.0f, Maths::PI * (0.66f) - 0.01f);
			pCamera->getFrustumPtr()->setFOV(fov);
			pCamera->requestUpdate();
		}
	}

	m_prev = Engine::getSingleton().getTimer().getMilliseconds();
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const KeyboardEvent& _evt)
{
	if(_evt.isKeyPressed(KC_ESCAPE))
	{
		m_pApp->closeApplication();
		return;
	}

	if(_evt.isKeyPressed(KC_C))
	{
		m_pApp->toggleConsole();
		m_pApp->toggleCursor();
		m_bFreezeMouseMovement = (m_bFreezeMouseMovement ? false : true);
	}

	// Move camera
	Vector3 movement;
	CameraFirstPerson* pCamera = m_pApp->getCamera();
	if(_evt.isKeyDown(KC_W))
		pCamera->move(1.0f);
	if(_evt.isKeyDown(KC_S))
		pCamera->move(-1.0f);
	if(_evt.isKeyDown(KC_A))
		pCamera->strafe(-1.0f);
	if(_evt.isKeyDown(KC_D))
		pCamera->strafe(1.0f);

	// Modify DoF
	Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"depthOfField");
	Shader* pShader = pMaterial->getActiveRenderTechnique()->getRenderPass(3).getPixelShader()->getShader();
	if(_evt.isKeyDown(KC_J))
	{
		m_fNear -= 2.0f;
		Maths::clamp(m_fNear, 0.0f, m_fFocus);
		ShaderConstant* pConstant = pShader->getConstant("near");
		if(pConstant)
			pConstant->setDefaultValue(&m_fNear);
	}
	if(_evt.isKeyDown(KC_M))
	{
		m_fNear += 2.0f;
		Maths::clamp(m_fNear, 0.0f, m_fFocus);
		ShaderConstant* pConstant = pShader->getConstant("near");
		if(pConstant)
			pConstant->setDefaultValue(&m_fNear);
	}

	if(_evt.isKeyDown(KC_K))
	{
		m_fFar -= 2.0f;
		Maths::clamp(m_fFar, m_fFocus, 3000.0f);
		ShaderConstant* pConstant = pShader->getConstant("far");
		if(pConstant)
			pConstant->setDefaultValue(&m_fFar);
	}
	if(_evt.isKeyDown(KC_COMMA))
	{
		m_fFar += 2.0f;
		Maths::clamp(m_fFar, m_fFocus, 3000.0f);
		ShaderConstant* pConstant = pShader->getConstant("far");
		if(pConstant)
			pConstant->setDefaultValue(&m_fFar);
	}

	if(_evt.isKeyDown(KC_L))
	{
		m_fFocus -= 2.0f;
		Maths::clamp(m_fFocus, m_fNear + 1.0f, m_fFar - 1.0f);
		ShaderConstant* pConstant = pShader->getConstant("focus");
		if(pConstant)
			pConstant->setDefaultValue(&m_fFocus);
	}
	if(_evt.isKeyDown(KC_PERIOD))
	{
		m_fFocus += 2.0f;
		Maths::clamp(m_fFocus, m_fNear + 1.0f, m_fFar - 1.0f);
		ShaderConstant* pConstant = pShader->getConstant("focus");
		if(pConstant)
			pConstant->setDefaultValue(&m_fFocus);
	}

	// Post process effects
	// Toggle each effect when specified key is pressed
	if(_evt.isKeyPressed(KC_F1))
		m_pApp->getPostEffect(0)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F2))
		m_pApp->getPostEffect(1)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F3))
		m_pApp->getPostEffect(2)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F4))
		m_pApp->getPostEffect(3)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F5))
		m_pApp->getPostEffect(4)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F6))
		m_pApp->getPostEffect(5)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F7))
		m_pApp->getPostEffect(6)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F8))
		m_pApp->getPostEffect(7)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F9))
		m_pApp->getPostEffect(8)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F10))
		m_pApp->getPostEffect(9)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F11))
		m_pApp->getPostEffect(10)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_F12))
		m_pApp->getPostEffect(11)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_1))
		m_pApp->getPostEffect(12)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_2))
		m_pApp->getPostEffect(13)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_3))
		m_pApp->getPostEffect(14)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_4))
		m_pApp->getPostEffect(15)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_5))
		m_pApp->getPostEffect(16)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_6))
		m_pApp->getPostEffect(17)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_7))
		m_pApp->getPostEffect(18)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_8))
		m_pApp->getPostEffect(19)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_9))
		m_pApp->getPostEffect(20)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_0))
		m_pApp->getPostEffect(21)->material->toggleEnabled();
	if(_evt.isKeyPressed(KC_Q))
		m_pApp->getPostEffect(22)->material->toggleEnabled();

	// Change water preset
	if(_evt.isKeyPressed(KC_N))
	{
		++m_Water;
		Maths::clamp_roll (m_Water, 0, 3);

		NodeWater* water = m_pApp->getWater();
		switch(m_Water)
		{
		case 0:
			// Normal sea water
			water->setWind(Vector2(-0.3f, 1.5f));
			water->setDisplacement(1.7f);
			water->setWavesScale(Vector2(0.005f, 0.0025f));
			water->setWaterLevel(14.0f);
			water->setExtinction(Vector3(7.0f, 30.0f, 40.0f));
			water->setRefractionStrength(0.0f);
			water->setNormalScale(1.5f);
			water->setRefractionScale(0.002f);
			water->setDepthColour(Colour(105, 105, 105));
			water->setHorizontalVisibility(4.0f);
			water->setWavesAmplitude(1.5f);
			water->setMuddiness(0.5f);
			water->setSurfaceColour(Colour(240, 255, 255));
			water->setShininess(0.7f);
			water->setFoam(Vector3(0.35f, 1.05f, 0.7f));

			break;

		case 1:
			// Muddy water
			water->setWind(Vector2(0.3f, 0.3f));
			water->setDisplacement(1.7f);
			water->setWavesScale(Vector2(0.005f, 0.0025f));
			water->setWaterLevel(14.0f);
			water->setExtinction(Vector3(30.0f, 20.0f, 10.0f));
			water->setRefractionStrength(0.23f);
			water->setNormalScale(2.0f);
			water->setRefractionScale(0.002f);
			water->setDepthColour(Colour(105, 105, 105));
			water->setHorizontalVisibility(1.0f);
			water->setWavesAmplitude(1.0f);
			water->setMuddiness(0.9f);
			water->setSurfaceColour(Colour(205, 133, 63));
			water->setShininess(0.1f);
			water->setFoam(Vector3(0.05f, 0.95f, 0.8f));

			break;

		case 2:
			// Tropical water
			water->setWind(Vector2(-0.3f, 3.4f));
			water->setDisplacement(1.7f);
			water->setWavesScale(Vector2(0.005f, 0.0025f));
			water->setWaterLevel(14.0f);
			water->setExtinction(Vector3(4.5f, 30.0f, 50.0f));
			water->setRefractionStrength(0.23f);
			water->setNormalScale(3.5f);
			water->setRefractionScale(0.001f);
			water->setDepthColour(Colour(47, 79, 79));
			water->setHorizontalVisibility(4.0f);
			water->setWavesAmplitude(4.5f);
			water->setMuddiness(0.5f);
			water->setSurfaceColour(Colour(32, 178, 170));
			water->setShininess(0.7f);
			water->setFoam(Vector3(0.65f, 1.35f, 2.0f));

			break;

		case 3:
			// Wavy water
			water->setWind(Vector2(-0.3f, 1.5f));
			water->setDisplacement(1.7f);
			water->setWavesScale(Vector2(0.0025f, 0.0025f));
			water->setWaterLevel(14.0f);
			water->setExtinction(Vector3(4.5f, 70.0f, 300.0f));
			water->setRefractionStrength(0.23f);
			water->setNormalScale(3.0f);
			water->setRefractionScale(0.001f);
			water->setDepthColour(Colour(0, 0, 0));
			water->setHorizontalVisibility(3.0f);
			water->setWavesAmplitude(3.0f);
			water->setMuddiness(0.15f);
			water->setSurfaceColour(Colour(192, 255, 255));
			water->setShininess(0.5f);
			water->setFoam(Vector3(0.65f, 1.35f, 1.5f));

			break;
		}
	}

	// Change time of day
	TimeDate& time = m_pApp->getTime();
	TimeDate offset;
	offset.minutes = 2;
	bool timeset = false;

	if(_evt.isKeyDown(KC_EQUALS))
	{
		timeset = true;

		time += offset;
		if(time.hours > 22)
			time.hours = 6;
	}
	if(_evt.isKeyDown(KC_MINUS))
	{
		timeset = true;

		time -= offset;
		if(time.hours < 6)
			time.hours = 22;
	}

	// Toggle shadows
	if(_evt.isKeyPressed(KC_Z))
	{
		Renderer::getSingleton().getRenderStage(L"Shadows")->toggleEnabled();
	}

	// Toggle HDR
	if(_evt.isKeyPressed(KC_X))
	{
		((LightStage*)Renderer::getSingleton().getRenderStage(L"Light"))->toggleHDR();
	}

	// Toggle grass
	if(_evt.isKeyPressed(KC_G))
		Engine::getSingleton().getSceneManager(0)->getNode(L"Meadow")->toggleEnabled();

	// Move water up/down
	if(_evt.isKeyDown(KC_PGUP))
	{
		if(m_pApp->getWater())
			m_pApp->getWater()->setWaterLevel(m_pApp->getWater()->getWaterLevel() + 0.1f);
	}

	if(_evt.isKeyDown(KC_PGDOWN))
	{
		if(m_pApp->getWater())
			m_pApp->getWater()->setWaterLevel(m_pApp->getWater()->getWaterLevel() - 0.1f);
	}

	// Apply bleach effect
	if(_evt.isKeyPressed(KC_B))
	{
		MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");
		Material* pMaterial = pMatLib->getMaterial(L"bleach");
		SPriorityMaterial* pMat = new SPriorityMaterial();
		pMat->priority = 99;
		pMat->material = pMaterial;
		pMat->material->setEnabled(true);

		PostStage* postStage = ((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"));
		postStage->addToRender(pMat);
	}

	// Change clouds density
	if(_evt.isKeyDown(KC_U))
	{
		if(m_pApp->getClouds())
		{
			Real fDensity = m_pApp->getClouds()->getCloudDensity() - 0.001f;
			Maths::clamp(fDensity, 0.0f, 1.0f);
			m_pApp->getClouds()->setCloudDensity(fDensity);
		}
	}

	if(_evt.isKeyDown(KC_I))
	{
		if(m_pApp->getClouds())
		{
			Real fDensity = m_pApp->getClouds()->getCloudDensity() + 0.001f;
			Maths::clamp(fDensity, 0.0f, 1.0f);
			m_pApp->getClouds()->setCloudDensity(fDensity);
		}
	}

	// Change clouds cover
	if(_evt.isKeyDown(KC_O))
	{
		if(m_pApp->getClouds())
		{
			Real fCover = m_pApp->getClouds()->getCloudCover() - 0.001f;
			Maths::clamp(fCover, 0.0f, 1.0f);
			m_pApp->getClouds()->setCloudCover(fCover);
		}
	}

	// New lightning bolt
	if(_evt.isKeyPressed(KC_F))
	{
		int index = m_BoltsNum % 10;
		NodeLightning* pBolt = m_pLightnings[index];
		NodeSound* pSound;
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		if(m_BoltsNum < 10)
		{
			pBolt = new NodeLightning();
			pBolt->setCreator(sm);
			pBolt->init();
			wstringstream name;
			name << "Bolt" << m_BoltsNum;
			sm->getRootNode()->addChild(name.str(), pBolt);

			pBolt->setStartPoint(Vector3(0.0f, 170.0f, 0.0f));
			pBolt->setEndPoint(Vector3(0.0f, -10.0f, 0.0f));
			Colour col(255, 182, 255, 255);
			pBolt->setColour(col);
			pBolt->setBranchesNum(75);
			pBolt->setOffset(26);
			pBolt->setMaxFlash(0.0f);
			pBolt->setSegmentsNum(6);
			pBolt->setWidth(0.25f);
			pBolt->setUpdateFrequency(1000);

			m_pLightnings[index] = pBolt;


			name << "Thunder" << m_BoltsNum;
			if(!sm->getNode(name.str()))
			{
				SOUND_DESC soundDesc;
				soundDesc.isUsingHardware = false;
				soundDesc.is3D = true;
				soundDesc.fileName = L"thunder.wav";

				pSound = sm->createSound(name.str(), soundDesc);
				ISoundInstance* pInternalSound = pSound->getSound();
				pInternalSound->getSound()->setLooped(false);
				sm->getRootNode()->addChild(name.str(), pSound);
				pInternalSound->play();
			}
			else
				pSound = (NodeSound*)sm->getNode(name.str());
		}

		++m_BoltsNum;

		Vector3 pos = Engine::getSingleton().getActiveCamera()->getPositionLocal();
		Vector3 dir = Engine::getSingleton().getActiveCamera()->getForward();
		dir.y = 0.0f;
		Vector3 right = Engine::getSingleton().getActiveCamera()->getRight();
		right.y = 0.0f;

		pos = pos + //Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000) * 30.0f * right +
				(abs(Maths::perlinNoiseGenerator(rand() % 1000, rand() % 1000, rand() % 1000)) * 100.0f + 100.0f) * dir;

		pBolt->setPosition(pos);


		pBolt->setPause(500);
		pBolt->setNumberOfRuns(0);

		pSound->setPosition(pBolt->getPositionLocal());
	}


	if(_evt.isKeyDown(KC_P))
	{
		if(m_pApp->getClouds())
		{
			Real fCover = m_pApp->getClouds()->getCloudCover() + 0.001f;
			Maths::clamp(fCover, 0.0f, 1.0f);
			m_pApp->getClouds()->setCloudCover(fCover);
		}
	}

	// Change time of day
	if(timeset)
	{
		if(m_pApp->getWater())
			m_pApp->getWater()->setDayTime(time);
		if(m_pApp->getSky())
			m_pApp->getSky()->setDayTime(time);
		if(m_pApp->getClouds())
			m_pApp->getClouds()->setDayTime(time);
	}

	// Show/hide window.
	if(_evt.isKeyPressed(KC_H))
	{
		GUIManager::getSingleton().getWindow(1)->toggleVisible();
	}
}
//----------------------------------------------------------------------