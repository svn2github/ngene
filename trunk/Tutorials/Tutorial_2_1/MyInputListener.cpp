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

	// Shoot the fireball
	if(_evt.type == MET_BUTTON_LEFT && _evt.nvalue & 0x80)
	{
		PhysicsWorld* pWorld = Physics::getSingleton().getWorld(L"MyPhysicsWorld");

		wostringstream buffer;
		buffer << L"Sphere_" << m_BallsNum;

		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		PrefabSphere* pSphere = sm->createSphere(10, 10, 0.5f);
		Camera* pCam = Engine::getSingleton().getActiveCamera();
		Vector3 pos = pCam->getPositionLocal() + pCam->getForward() * 5.0f;
		pSphere->setPosition(pos);
		Surface* pSurface = pSphere->getSurface(L"Base");
		sm->getRootNode()->addChild(buffer.str(), pSphere);

		PhysicsActor* pActor = pWorld->createPhysicsActor(L"Sphere", Vector3(0.5, 0.0f, 0.0f), PhysicsActor::BT_DYNAMIC);
		pActor->attachNode(pSphere);
		pActor->setMass(0.1f);
		pActor->setShapeMaterial(0, *pWorld->getPhysicsMaterial(L"DefaultMaterial"));
		pWorld->addPhysicsActor(buffer.str(), pActor);
		FORCE_DESC force = {pCam->getForward(), 100.0f};
		pActor->applyForce(force);

		// Create particle system
		ParticleEmitter fire;
		fire.setParticlesMaxCount(2400);
		fire.setSpawnRate(120);
		fire.setPosition(Vector3(0.0f, 0.0f, 0.0f));
		fire.setVelocity(Vector3(0.6f, 0.0f, 0.0f));
		fire.setVelocitySpread(Vector3(0.6f, 0.8f, 0.3f));
		fire.setAcceleration(Vector3(0.0f, 1.2f, 0.0f));
		fire.setAngularVelocity(Maths::PI / 3.0f);
		fire.setAngularVelocitySpread(0.2f);
		fire.setSize(1.2f);
		fire.setSizeSpread(0.4f);
		fire.setGrowth(0.24f);
		fire.setGrowthSpread(0.0f);
		fire.setLifeTime(2000.0f);
		fire.setLifeTimeSpread(1000.0f);
		fire.setColour(Colour(255, 255, 255, 125));
		fire.setFadeSpeed(1.0f);
		Colour target = Colour::COLOUR_WHITE;
		target.setAlpha(0);
		fire.setTargetColour(target);

		wostringstream buffer2;
		buffer2 << L"FireBall_" << m_BallsNum;
		NodeParticleSystem* pPS = sm->createParticleSystem();
		pPS->setPosition(0.0f, 0.0f, 0.0f);
		pPS->addParticleEmitter(fire, L"fire_emitter");
		pSphere->addChild(buffer2.str(), pPS);
		Material* matFire = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"fire");
		pSurface = pPS->getSurface(L"fire_emitter");
		pSurface->setMaterial(matFire);

		++m_BallsNum;
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