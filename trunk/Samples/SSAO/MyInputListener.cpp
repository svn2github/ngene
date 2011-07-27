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


MyInputListener::MyInputListener()
{
	m_Angle1 = 0.0f;
	m_Angle2 = 0.0f;
	m_fRadius = 1.025f;
	m_fRange = 1.0f;
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const MouseEvent& _evt)
{
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
		Maths::clamp <Real>(val, 5.0f, 150.0f);

		pCamera->setDistanceFromTarget(val);
	}

	m_Prev = Engine::getSingleton().getTimer().getMilliseconds();
}
//----------------------------------------------------------------------
void MyInputListener::handleEvent(const KeyboardEvent& _evt)
{
	if(_evt.isKeyPressed(KC_ESCAPE))
	{
		m_pApp->closeApplication();
		return;
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

	// Switch scene
	if(_evt.isKeyPressed(KC_1))
	{
		Engine::getSingleton().getSceneManager(0)->getNode(L"Angel")->setEnabled(true);
		Engine::getSingleton().getSceneManager(0)->getNode(L"House")->setEnabled(false);
	}

	if(_evt.isKeyPressed(KC_2))
	{
		Engine::getSingleton().getSceneManager(0)->getNode(L"Angel")->setEnabled(false);
		Engine::getSingleton().getSceneManager(0)->getNode(L"House")->setEnabled(true);
	}

	if(_evt.isKeyPressed(KC_H))
	{
		if(GUIManager::getSingleton().getWindow(1)->isVisible())
			GUIManager::getSingleton().getWindow(1)->setVisible(false);
		else
			GUIManager::getSingleton().getWindow(1)->setVisible(true);
	}

	// Modify SSAO
	Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"ssao");
	ShaderInstance* pShader = pMaterial->getActiveRenderTechnique()->getRenderPass(0).getPixelShader();
	if(_evt.isKeyDown(KC_U))
	{
		m_fRadius -= 0.01f;
		Maths::clamp(m_fRadius, 0.0f, 20.0f);
		ShaderConstant* pConstant = pShader->getConstant("radius");
		if(pConstant)
			pConstant->setDefaultValue(&m_fRadius);
	}
	if(_evt.isKeyDown(KC_I))
	{
		m_fRadius += 0.01f;
		Maths::clamp(m_fRadius, 0.0f, 20.0f);
		ShaderConstant* pConstant = pShader->getConstant("radius");
		if(pConstant)
			pConstant->setDefaultValue(&m_fRadius);
	}

	if(_evt.isKeyDown(KC_O))
	{
		m_fRange -= 0.1f;
		Maths::clamp(m_fRange, 0.0f, 200.0f);
		ShaderConstant* pConstant = pShader->getConstant("range");
		if(pConstant)
			pConstant->setDefaultValue(&m_fRange);
	}
	if(_evt.isKeyDown(KC_P))
	{
		m_fRange += 0.1f;
		Maths::clamp(m_fRange, 0.0f, 200.0f);
		ShaderConstant* pConstant = pShader->getConstant("range");
		if(pConstant)
			pConstant->setDefaultValue(&m_fRange);
	}
}
//----------------------------------------------------------------------