/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyInputListener.cpp
Version:	0.03
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

	CameraThirdPerson* pCamera = m_pApp->getCamera();
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
		Real val = pCamera->getDistanceFromTarget() - _evt.fvalue * 1.0f;
		Maths::clamp <Real>(val, 5.0f, 25.0f);

		pCamera->setDistanceFromTarget(val);
	}

	// Shoot the ball
	if(_evt.type == MET_BUTTON_LEFT && _evt.nvalue & 0x80)
	{
		PhysicsWorld* pWorld = Physics::getSingleton().getWorld(L"PhysicsWorld");

		wostringstream buffer;
		static uint m_BallsNum;
		buffer << L"Sphere_" << m_BallsNum++;

		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		PrefabSphere* pSphere = sm->createSphere(10, 10, 0.5f);
		Camera* pCam = Engine::getSingleton().getActiveCamera();
		Vector3 pos = pCam->getPositionLocal() + pCam->getForward() * 1.0f;
		pSphere->setPosition(pos);
		Surface* pSurface = pSphere->getSurface(L"Base");
		Material* matStone = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
		pSurface->setMaterial(matStone);
		sm->getRootNode()->addChild(buffer.str(), pSphere);

		PhysicsActor* pActor = pWorld->createPhysicsActor(L"Sphere", Vector3(0.5, 0.0f, 0.0f), PhysicsActor::BT_DYNAMIC);
		pActor->attachNode(pSphere);
		pActor->setMass(0.1f);
		pActor->setShapeMaterial(0, *pWorld->getPhysicsMaterial(L"Stone"));
		pWorld->addPhysicsActor(buffer.str(), pActor);
		FORCE_DESC force = {pCam->getForward(), 100.0f};
		pActor->applyForce(force);
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

	Vector3 movement;
	if(_evt.isKeyDown(KC_UP))
		movement.z = 1.0f;
	if(_evt.isKeyDown(KC_DOWN))
		movement.z = -1.0f;
	if(_evt.isKeyDown(KC_LEFT))
		movement.x = -1.0f;
	if(_evt.isKeyDown(KC_RIGHT))
		movement.x = 1.0f;

	// Hide/show menu
	if(_evt.isKeyPressed(KC_H))
	{
		if(GUIManager::getSingleton().getWindow(1)->isVisible())
			GUIManager::getSingleton().getWindow(1)->setVisible(false);
		else
			GUIManager::getSingleton().getWindow(1)->setVisible(true);
	}

	// Toggle debug info
	if(_evt.isKeyPressed(KC_D))
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		Node* pNode = (NodeVisible*)sm->getNode(L"SoundHelper");
		if(pNode)
			pNode->toggleEnabled();
		pNode = (NodeVisible*)sm->getNode(L"SoundPath");
		if(pNode)
			pNode->toggleEnabled();
	}

	// Disable SSAO
	if(_evt.isKeyPressed(KC_S))
	{
		if(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"ssao")->isEnabled())
			MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"ssao")->setEnabled(false);
		else
			MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"ssao")->setEnabled(true);
	}

	// Show only SSAO
	if(_evt.isKeyPressed(KC_M))
	{
		if(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"ssao")->isEnabled())
		{
			Material* matSSAO = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"ssao");
			RenderPass& pass = matSSAO->getActiveRenderTechnique()->getRenderPass(matSSAO->getActiveRenderTechnique()->getPassesNum() - 1);
			if(pass.isEnabled())
			{
				pass.setEnabled(false);
				matSSAO->getActiveRenderTechnique()->getRenderPass(matSSAO->getActiveRenderTechnique()->getPassesNum() - 2).getRenderTarget().setEnabled(false);
			}
			else
			{
				pass.setEnabled(true);
				matSSAO->getActiveRenderTechnique()->getRenderPass(matSSAO->getActiveRenderTechnique()->getPassesNum() - 2).getRenderTarget().setEnabled(true);
			}
		}
	}

	// Modify SSAO
	Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"ssao");
	ShaderInstance* pShader = pMaterial->getActiveRenderTechnique()->getRenderPass(0).getPixelShader();
	static Real radius = 1.025f;
	static Real range = 1.0f;
	if(_evt.isKeyDown(KC_U))
	{
		radius -= 0.01f;
		Maths::clamp(radius, 0.0f, 20.0f);
		ShaderConstant* pConstant = pShader->getConstant("radius");
		if(pConstant)
			pConstant->setDefaultValue(&radius);
	}
	if(_evt.isKeyDown(KC_I))
	{
		radius += 0.01f;
		Maths::clamp(radius, 0.0f, 20.0f);
		ShaderConstant* pConstant = pShader->getConstant("radius");
		if(pConstant)
			pConstant->setDefaultValue(&radius);
	}

	if(_evt.isKeyDown(KC_O))
	{
		range -= 0.25f;
		Maths::clamp(range, 0.0f, 200.0f);
		ShaderConstant* pConstant = pShader->getConstant("range");
		if(pConstant)
			pConstant->setDefaultValue(&range);
	}
	if(_evt.isKeyDown(KC_P))
	{
		range += 0.25f;
		Maths::clamp(range, 0.0f, 200.0f);
		ShaderConstant* pConstant = pShader->getConstant("range");
		if(pConstant)
			pConstant->setDefaultValue(&range);
	}
}
//----------------------------------------------------------------------