/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "App.h"
#include "nGENE.h"


void App::createApplication()
{
	pSelectedActor = NULL;

	FrameworkWin32::createApplication();

	m_pMouse->setSensitivity(0.02f);

	m_pPartitioner = new ScenePartitionerQuadTree();
	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	sm->setScenePartitioner(m_pPartitioner);

	Renderer& renderer = Renderer::getSingleton();
	renderer.setClearColour(0);
	renderer.setCullingMode(CULL_CW);
	uint anisotropy = 0;
	renderer.getDeviceRender().testCapabilities(CAPS_MAX_ANISOTROPY, &anisotropy);
	for(uint i = 0; i < 8; ++i)
		renderer.setAnisotropyLevel(i, anisotropy);

	pWorld = Physics::getSingleton().createWorld(L"MyPhysicsWorld");
	pWorld->setGravity(Vector3(0.0f, -9.8f, 0.0f));

	physMat = pWorld->createPhysicsMaterial(L"DefaultMaterial");
	physMat->setRestitution(0.5f);
	physMat->setDynamicFriction(0.5f);
	physMat->setStaticFriction(0.5f);

	// Create ragdoll guy
	Quaternion qRotLeft(Vector3::UNIT_Z, Maths::PI_HALF);
	Quaternion qRotRight(Vector3::UNIT_Z, -Maths::PI_HALF);
	Quaternion qRotAround(Vector3::UNIT_Z, Maths::PI);
	
	PhysicsActor* pHead = createSphere(sm, L"head", 0.5f, Vector3(0, 8.8f, 0));
	PhysicsActor* pTorso = createSphere(sm, L"torso", 0.95f, Vector3(0, 7.35f, 0));
	PhysicsActor* pPelvis = createSphere(sm, L"pelvis", 0.7f, Vector3(0, 5.7f, 0));

	PhysicsActor* pLeftUArm = createCapsule(sm, L"leftUpperArm", Vector3(0.5, 8.0, 0), &qRotRight, 1.0, 0.4);
	PhysicsActor* pLeftFArm = createCapsule(sm, L"leftForeArm", Vector3(2.0, 8.0, 0), &qRotRight, 1.0, 0.3);

	PhysicsActor* pRightUArm = createCapsule(sm, L"rightUpperArm", Vector3(-0.5, 8.0, 0), &qRotLeft, 1.0, 0.4);
	PhysicsActor* pRightFArm = createCapsule(sm, L"rightForeArm", Vector3(-2.0, 8.0, 0), &qRotLeft, 1.0, 0.3);

	PhysicsActor* pLeftThigh = createCapsule(sm, L"leftThigh", Vector3(0.6, 5.2, 0), &qRotAround, 1.5, 0.5);
	PhysicsActor* pLeftCalf = createCapsule(sm, L"leftCalf", Vector3(0.6, 2.7, 0), &qRotAround, 1.5, 0.35);
	PhysicsActor* pLeftFoot = createBox(sm, L"leftFoot", Vector3(0.6, 0.5, 0.2), &qRotAround, Vector3(0.4, 0.2, 0.75));

	PhysicsActor* pRightThigh = createCapsule(sm, L"rightThigh", Vector3(-0.6, 5.2, 0), &qRotAround, 1.5, 0.5);
	PhysicsActor* pRightCalf = createCapsule(sm, L"rightCalf", Vector3(-0.6, 2.7, 0), &qRotAround, 1.5, 0.35);
	PhysicsActor* pRightFoot = createBox(sm, L"rightFoot", Vector3(-0.6, 0.5, 0.2), &qRotAround, Vector3(0.4, 0.2, 0.75));


	createSphericalJoint(pHead, pTorso, Vector3(0, 8.8f, 0), Vector3::UNIT_Y);
	createSphericalJoint(pLeftUArm, pTorso, Vector3(0.5, 8.0, 0), Vector3::UNIT_X);
	createSphericalJoint(pRightUArm, pTorso, Vector3(-0.5, 8.0, 0), Vector3::NEGATIVE_UNIT_X);
	createSphericalJoint(pTorso, pPelvis, Vector3(0, 7.35f, 0), Vector3::UNIT_X);
	createSphericalJoint(pLeftThigh, pPelvis, Vector3(0.6, 5.2, 0), Vector3::UNIT_Y);
	createSphericalJoint(pRightThigh, pPelvis, Vector3(-0.6, 5.2, 0), Vector3::UNIT_Y);

	createHingeJoint(pLeftFArm, pLeftUArm, Vector3(2.0, 8.0, 0), Vector3::NEGATIVE_UNIT_Z);
	createHingeJoint(pRightFArm, pRightUArm, Vector3(-2.0, 8.0, 0), Vector3::NEGATIVE_UNIT_Z);
	createHingeJoint(pLeftCalf, pLeftThigh, Vector3(0.6, 2.7, 0.0f), Vector3::UNIT_X);
	createHingeJoint(pRightCalf, pRightThigh, Vector3(-0.6, 2.7, 0), Vector3::NEGATIVE_UNIT_X);
	createHingeJoint(pLeftFoot, pLeftCalf, Vector3(0.6, 0.5, 0.0), Vector3::UNIT_X);
	createHingeJoint(pRightFoot, pRightCalf, Vector3(-0.6, 0.5, 0.0), Vector3::NEGATIVE_UNIT_X);


	pSelectedActor = pPelvis;

	for (uint i = 0; i < prefabs.size(); ++i)
	{
		NodePrefab* prefab = prefabs[i];
		Surface* pSurface = prefab->getSurface(L"Base");
		Material* pMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"dotCel");
		pSurface->setMaterial(pMat);

		if(actors[i] == pSelectedActor)
		{
			Material* pMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"dotCelBlue");
			pSurface->setMaterial(pMat);
		}
	}

	CameraThirdPerson* cam;
	cam = (CameraThirdPerson*)sm->createCamera(L"CameraThirdPerson", L"Camera");
	cam->setVelocity(10.0f);
	cam->setPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_pCamera = cam;

	Vector3 target = Vector3(0.0f, 3.0f, 0.0f);
	cam->setTarget(target);
	sm->getRootNode()->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);

	// Create ground
	Plane pl(Vector3::UNIT_Y, Point(0.0f, 0.0f, 0.0f));
	PrefabPlane* pGround = sm->createPlane(pl, Vector2(800.0f, 800.0f));
	pGround->setPosition(0.0f, 0.0f, 0.0f);
	Surface* pSurface = pGround->getSurface(L"Base");
	Material* matGround = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"GroundPlane", pGround);

	PhysicsActor* pActor = pWorld->createPhysicsActor(L"Plane", Vector3(100.0f, 0.5f, 100.0f));
	pActor->attachNode(pGround);
	pActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(L"Plane", pActor);

	NodeLight* pLight = sm->createLight(LT_POINT);
	pLight->setPosition(-4.0f, 10.0f, 2.0f);
	pLight->setDirection(Vector3(0.7f, -0.9f, 1.0f));
	pLight->setDrawDebug(false);
	pLight->setRadius(180.0f);
	sm->getRootNode()->addChild(L"WorldLight", pLight);
	renderer.addLight(pLight);
	
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);

	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"Ragdoll");

	pWorld->setEnabled(false);

	// Create GUI
	m_pFont = new GUIFont(L"gui_default_font.fnt");

	GUIManager::getSingleton().setFont(m_pFont);
	GUIManager::getSingleton().addWindow(&m_Window);
	GUIManager::getSingleton().setActiveMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"gui_default_active"));
	GUIManager::getSingleton().setInactiveMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"gui_default_inactive"));
}
//----------------------------------------------------------------------
CameraThirdPerson* App::getCamera() const
{
	return m_pCamera;
}
//----------------------------------------------------------------------
PhysicsActor* App::getSelectedActor() const
{
	return pSelectedActor;
}
//----------------------------------------------------------------------
void App::selectNextActor()
{
	uint index = 0;
	for(index = 0; index < actors.size(); ++index)
	{
		if(actors[index] == pSelectedActor)
		{
			Material* pMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"dotCel");
			Surface* pSurface = prefabs[index]->getSurface(L"Base");
			pSurface->setMaterial(pMat);

			pMat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"dotCelBlue");
			if(index == actors.size() - 1)
			{
				pSelectedActor = actors[0];

				pSurface = prefabs[0]->getSurface(L"Base");
				pSurface->setMaterial(pMat);
			}
			else
			{
				pSelectedActor = actors[index + 1];

				pSurface = prefabs[index + 1]->getSurface(L"Base");
				pSurface->setMaterial(pMat);
			}

			break;
		}
	}
}
//----------------------------------------------------------------------
void App::createSphericalJoint(PhysicsActor* actor1, PhysicsActor* actor2, Vector3& pos, const Vector3& axis)
{
	SPHERE_JOINT_DESC desc;
	desc.actor1 = actor1;
	desc.actor2 = actor2;
	desc.globalAnchor = pos;
	desc.globalDirection = axis;

	desc.flags |= JSF_TWIST_LIMIT_ENABLED;
	desc.twistLimit.lowValue = -Maths::PI * 0.025f;
	desc.twistLimit.lowRestitution = 0.5f;
	desc.twistLimit.highValue = Maths::PI * 0.025f;
	desc.twistLimit.highRestitution = 0.5f;

	desc.flags |= JSF_SWING_LIMIT_ENABLED;
	desc.swingLimit.value = Maths::PI * 0.25f;
	desc.swingLimit.restitution = 0.5f;

	desc.flags |= JSF_TWIST_SPRING_ENABLED;
	desc.twistSpring.springCoeff = 0.5f;
	desc.twistSpring.damperCoeff = 1.0f;

	desc.flags |= JSF_SWING_SPRING_ENABLED;
	desc.swingSpring.springCoeff = 0.5f;
	desc.swingSpring.damperCoeff = 1.0f;

	desc.projectionDistance = 0.15f;


	pWorld->createJoint(L"Sphere", desc);
}
//----------------------------------------------------------------------
void App::createHingeJoint(PhysicsActor* actor1, PhysicsActor* actor2, Vector3& pos, const Vector3& axis)
{
	JOINT_DESC desc;
	desc.actor1 = actor1;
	desc.actor2 = actor2;
	desc.globalAnchor = pos;
	desc.globalDirection = axis;


	pWorld->createJoint(L"Hinge", desc);
}
//----------------------------------------------------------------------
PhysicsActor* App::createBox(SceneManager* sm, const wstring& name, Vector3& pos, Quaternion* rot, Vector3& size)
{
	PrefabBox* box = sm->createBox(size.x, size.y, size.z);
	box->setPosition(pos);
	if(rot)
		box->setRotation(*rot);
	prefabs.push_back(box);
	sm->getRootNode()->addChild(name, box);

	ShapeBox* pShape = new ShapeBox();
	pShape->setDimensions(size);
	pShape->setLocalPos(Vector3(0.0f, size.y, 0.0f));
	PhysicsActor* pActor = pWorld->createPhysicsActor(pShape, PhysicsActor::BT_DYNAMIC);
	pActor->attachNode(box);
	pActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(name, pActor);

	actors.push_back(pActor);

	return pActor;
}
//----------------------------------------------------------------------
PhysicsActor* App::createSphere(SceneManager* sm, const wstring& name, float radius, Vector3& pos)
{
	PrefabSphere* sphere = sm->createSphere(10, 10, radius);
	sphere->setPosition(pos);
	prefabs.push_back(sphere);
	sm->getRootNode()->addChild(name, sphere);

	PhysicsActor* pActor = pWorld->createPhysicsActor(L"Sphere", Vector3(radius, 0.0f, 0.0f), PhysicsActor::BT_DYNAMIC);
	pActor->attachNode(sphere);
	pActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(name, pActor);

	actors.push_back(pActor);

	return pActor;
}
//----------------------------------------------------------------------
PhysicsActor* App::createCapsule(SceneManager* sm, const wstring& name, Vector3& pos, Quaternion* rot, float height, float radius)
{
	PrefabCapsule* capsule = sm->createCapsule(10, 10, radius, height);
	capsule->setPosition(pos);
	if(rot)
		capsule->setRotation(*rot);
	prefabs.push_back(capsule);
	sm->getRootNode()->addChild(name, capsule);

	Vector3 size;
	size.x = radius;
	size.y = height;
	ShapeCapsule* pShape = new ShapeCapsule();
	pShape->setDimensions(size);
	pShape->setLocalPos(Vector3(0.0f, radius + 0.5f * height, 0.0f));
	PhysicsActor* pActor = pWorld->createPhysicsActor(pShape, PhysicsActor::BT_DYNAMIC);
	pActor->attachNode(capsule);
	pActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(name, pActor);

	actors.push_back(pActor);

	return pActor;
}
//----------------------------------------------------------------------
int main()
{
	App app;

	app.createApplication();
	app.run();

	app.shutdown();

	return 0;
}
//----------------------------------------------------------------------
