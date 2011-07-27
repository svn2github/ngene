#include "MyInputListener.h"
#include "nGENE.h"

#include "App.h"

using namespace nGENE;

MyInputListener::MyInputListener():
	m_pApp(NULL),
	m_bMenuMode(true)
{
}

MyInputListener::~MyInputListener()
{
}

void MyInputListener::handleEvent(const MouseEvent &_evt)
{
	if(m_bMenuMode)
		return;

	float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_prev) * 0.001f;

	Camera* pCamera = Engine::getSingleton().getActiveCamera();
	if(pCamera)
	{
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
	}

	m_prev = Engine::getSingleton().getTimer().getMilliseconds();
}

void MyInputListener::handleEvent(const KeyboardEvent& _evt)
{
	if(_evt.isKeyPressed(KC_ESCAPE))
	{
		m_pApp->closeApplication();
		return;
	}

	CameraFirstPerson* pCamera = (CameraFirstPerson*)Engine::getSingleton().getActiveCamera();
	if(pCamera)
	{
		Vector3 movement;
		if(_evt.isKeyDown(KC_UP))
			movement.z = 1.0f;
		if(_evt.isKeyDown(KC_DOWN))
			movement.z = -1.0f;
		if(_evt.isKeyDown(KC_LEFT))
			movement.x = -1.0f;
		if(_evt.isKeyDown(KC_RIGHT))
			movement.x = 1.0f;

		if(movement.x != 0.0f)
			pCamera->strafe(movement.x);
		if(movement.z != 0.0f)
			pCamera->move(movement.z);
	}
}