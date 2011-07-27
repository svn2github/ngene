#include "MyInputListener.h"
#include "nGENE.h"

#include "App.h"

#include <sstream>

using namespace nGENE;

MyInputListener::MyInputListener():
	m_pApp(NULL),
	m_BallsNum(0),
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

	if((_evt.type == MET_BUTTON_LEFT || _evt.type == MET_BUTTON_RIGHT) && _evt.nvalue & 0x80)
	{
		Camera* pCam = Engine::getSingleton().getActiveCamera();
		PhysicsActor* pActor = createSphere();

		if(_evt.type == MET_BUTTON_LEFT)
		{
			FORCE_DESC force = {pCam->getForward(), 1000.0f};
			pActor->applyForce(force);
		}
		else
		{
			// When right button is pressed create spring-like joint between two "bullets"
			PhysicsActor* pActor2 = createSphere();
			DISTANCE_JOINT_DESC desc;
			desc.minDistance = 0.5f;
			desc.maxDistance = 1.0f;
			desc.spring.springCoeff = 1.5f;
			desc.spring.damperCoeff = 0.0f;
			desc.flags = JDF_MIN_DISTANCE_ENABLED | JDF_MAX_DISTANCE_ENABLED | JDF_SPRING_ENABLED;
			desc.actor1 = pActor;
			desc.actor2 = pActor2;
			PhysicsWorld* pWorld = Physics::getSingleton().getWorld(L"MyPhysicsWorld");
			pWorld->createJoint(L"Distance", desc);

			FORCE_DESC force = {pCam->getForward(), 1000.0f};
			pActor->applyForce(force);
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

PhysicsActor* MyInputListener::createSphere()
{
	PhysicsWorld* pWorld = Physics::getSingleton().getWorld(L"MyPhysicsWorld");

	MaterialLibrary* pLibrary = MaterialManager::getSingleton().getLibrary(L"default");
	Material* pMaterial = pLibrary->getMaterial(L"normalMap");

	wostringstream buffer;
	buffer << L"Sphere_" << m_BallsNum;
	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	PrefabSphere* pSphere = sm->createSphere(10, 10, 0.25f);
	Camera* pCam = Engine::getSingleton().getActiveCamera();
	Vector3 pos = pCam->getPositionLocal() + pCam->getForward();
	pSphere->setPosition(pos);
	Surface* pSurface = pSphere->getSurface(L"Base");
	sm->getRootNode()->addChild(buffer.str(), pSphere);
	pSurface->setMaterial(pMaterial);

	PhysicsActor* pActor = pWorld->createPhysicsActor(L"Sphere", Vector3(0.25, 0.0f, 0.0f), PhysicsActor::BT_DYNAMIC);
	pActor->attachNode(pSphere);
	pActor->setMass(1.0f);
	pActor->setShapeMaterial(0, *pWorld->getPhysicsMaterial(L"DefaultMaterial"));
	pWorld->addPhysicsActor(buffer.str(), pActor);

	++m_BallsNum;

	return pActor;
}