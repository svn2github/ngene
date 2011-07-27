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

MyInputListener::~MyInputListener()
{
}

void MyInputListener::handleEvent(const MouseEvent& _evt)
{
	float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_prev) * 0.001f;

	CharacterController* pController = m_pApp->getController();
	if(_evt.type == MET_MOVE_X || _evt.type == MET_MOVE_Y)
	{
		if(_evt.type == MET_MOVE_X)
			m_Angle1 += _evt.fvalue;
		else if(_evt.type == MET_MOVE_Y)
			m_Angle2 += _evt.fvalue;

		Quaternion quatRot1(Vector3(0.0f, 1.0f, 0.0f), m_Angle1);
		Quaternion quatRot2(Vector3(1.0f, 0.0f, 0.0f), m_Angle2);

		pController->setRotation(quatRot1 * quatRot2);
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

	if(_evt.isKeyPressed(KC_Q))
	{
		PhysicsWorld* pWorld = Physics::getSingleton().getWorld(L"MyPhysicsWorld");

		MaterialLibrary* pLibrary = MaterialManager::getSingleton().getLibrary(L"default");
		Material* pMaterial = pLibrary->getMaterial(L"normalMap");

		wostringstream buffer;
		buffer << L"Box_" << m_BoxesNum++;

		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		PrefabBox* pBox = sm->createBox(0.5f, 0.5f, 0.5f);
		pBox->setPosition(0.0f, 7.0f, 7.0f);
		Surface* pSurface = pBox->getSurface(L"Base");
		pSurface->setMaterial(pMaterial);
		sm->getRootNode()->addChild(buffer.str(), pBox);

		PhysicsActor* pActor = pWorld->createPhysicsActor(L"Box", Vector3(0.5, 0.5f, 0.5f), PhysicsActor::BT_DYNAMIC);
		pActor->attachNode(pBox);
		pActor->setShapeMaterial(0, *pWorld->getPhysicsMaterial(L"DefaultMaterial"));
		pWorld->addPhysicsActor(buffer.str(), pActor);
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

	if(movement.x != 0.0f || movement.z != 0.0f)
		m_pApp->getController()->move(movement);
}