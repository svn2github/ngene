/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "App.h"
#include "nGENE.h"

#include <sstream>


void App::createApplication()
{
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

	CameraThirdPerson* cam;
	cam = (CameraThirdPerson*)sm->createCamera(L"CameraThirdPerson", L"Camera");
	cam->setVelocity(10.0f);
	cam->setPosition(Vector3(0.0f, 1.0f, 0.0f));
	cam->setDistanceFromTarget(50.0f);
	m_pCamera = cam;

	PhysicsWorld* pWorld = Physics::getSingleton().createWorld(L"PhysicsWorld");
	pWorld->setGravity(Vector3(0.0f, -9.8f, 0.0f));
	pWorld->setSkinWidth(0.01f);

	PhysicsActor* pActor = NULL;
	PhysicsMaterial* physMat = pWorld->createPhysicsMaterial(L"DefaultMaterial");
	physMat->setRestitution(0.5f);
	physMat->setDynamicFriction(0.5f);
	physMat->setStaticFriction(0.5f);

	// Create boxes
	PrefabBox* pBox = NULL;
	Material* matCrate = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"crate");
	uint count = 0;
	for(float z = -8.0f; z < 8.0f; z += 2.0f)
	{
		for(float y = 0.0f; y < 16.0f; y += 2.0f)
		{
			for(float x = -8.0f; x < 8.0f; x += 2.0f)
			{
				wstringstream name;
				name << L"Box_" << count++;

				pBox = sm->createBox(2.0f, 2.0f, 2.0f);
				pBox->setPosition(x, y + 1.5f, z);
				uint surfaceID = 0;
				pBox->getSurface(surfaceID)->setMaterial(matCrate);
				sm->getRootNode()->addChild(name.str(), pBox);

				pActor = pWorld->createPhysicsActor(L"Box", Vector3(2.0f, 2.0f, 2.0f), PhysicsActor::BT_DYNAMIC);
				pActor->attachNode((NodeVisible*)pBox);
				pActor->setShapeMaterial(0, *physMat);
				pWorld->addPhysicsActor(name.str(), pActor);
				pActor->setMass(1.0f);
			}
		}
	}
	

	cam->setTarget(Vector3(0.0f, 0.0f, 0.0f));
	Engine::getSingleton().setActiveCamera(cam);

	// Create ground
	Plane pl(Vector3::UNIT_Y, Point(0.0f, 0.0f, 0.0f));
	PrefabPlane* pGround = sm->createPlane(pl, Vector2(800.0f, 800.0f));
	pGround->setPosition(0.0f, 0.0, 0.0f);
	Surface* pSurface = pGround->getSurface(L"Base");
	Material* matGround = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"parallaxMap");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"GroundPlane", pGround);

	ShapePlane* pShape = (ShapePlane*)pWorld->createShape(L"Plane");
	pShape->setNormal(Vector3::UNIT_Y);
	pShape->setDistanceFromOrigin(0.0f);
	pActor = pWorld->createPhysicsActor(pShape);
	pActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(L"Plane", pActor);

	// Create spot light
	NodeLight* pLight = sm->createLight(LT_POINT);
	pLight->setPosition(-14.0f, 18.0f, -20.0f);
	pLight->setDirection(Vector3(0.7f, -0.9f, 1.0f));
	pLight->setDrawDebug(false);
	Colour clrWorld = Colour::COLOUR_WHITE;
	pLight->setColour(clrWorld);
	pLight->setRadius(180.0f);
	sm->getRootNode()->addChild(L"Light", pLight);
	renderer.addLight(pLight);
	
	// Add input listener
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);

	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"Tornado");

	// Vortex force field
	ForceFieldShape* pBoxFF = pWorld->createForceFieldShape(L"Box");
	pBoxFF->setDimensions(Vector3(15.0f, 17.0f, 15.0f));
	pBoxFF->setLocalPos(Vector3(0.0f, 3.5f, 0.0f));

	ForceFieldShapesGroup* pGroup = pWorld->createForceFieldShapesGroup(true);
	pGroup->addShape(pBoxFF);

	Matrix3x3 velMult;

	Matrix3x3 posMult;
	posMult.zero();
	posMult._m11 = 10.0f;
	posMult._m12 = -5.0f;

	FORCE_FIELD_KERNEL_DESC kernelDesc;
	kernelDesc.constant = Vector3(-30.0f, 4.0f, 0.0f);
	kernelDesc.velocityTarget = Vector3(0.0f, 0.0f, 30.0f);
	kernelDesc.velocityMultiplier = velMult;
	kernelDesc.positionTarget = Vector3(3.0f, 0.0f, 0.0f);
	kernelDesc.positionMultiplier = posMult;
	kernelDesc.noise = Vector3(5.0f, 5.0f, 5.0f);

	FORCE_FIELD_DESC fieldDesc;
	fieldDesc.coordinates = FFC_CYLINDRICAL;
	fieldDesc.kernel = &kernelDesc;
	fieldDesc.volumeScalingFlags = 0;
	ForceField* pForceField = pWorld->createForceField(fieldDesc);
	pForceField->addShapeGroup(pGroup);
	pWorld->addForceField(L"Vortex", pForceField);

	pActor = pWorld->createPhysicsActor(L"Box", Vector3(1.0f, 1.0f, 1.0f), PhysicsActor::BT_DYNAMIC);
	pWorld->addPhysicsActor(L"Dummy", pActor);

	pForceField->attachNode(pActor);

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
int main()
{
	App app;

	app.createApplication();
	app.run();

	app.shutdown();

	return 0;
}
//----------------------------------------------------------------------