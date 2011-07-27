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

#include "NodeLightning.h"

using nGENE::Nature::NodeLightning;

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

	// Create boxes
	PrefabBox* pBox = NULL;
	Material* matCrate = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"crate");
	uint count = 0;

	for(float z = -12.0f; z < 12.0f; z += 3.0f)
	{
		for(float x = -12.0f; x < 12.0f; x += 3.0f)
		{
			wstringstream name;
			name << L"Box_" << count++;

			pBox = sm->createBox(4.0f, 4.0f, 4.0f);
			pBox->setPosition(x * 5.0f, 1.5f, z * 5.0f);
			uint surfaceID = 0;
			pBox->getSurface(surfaceID)->setMaterial(matCrate);
			sm->getRootNode()->addChild(name.str(), pBox);
		}
	}
	
	// Create lightning
	static NodeLightning bolt;
	bolt.setCreator(sm);
	bolt.init();
	bolt.setMinBranchLength(8.0f);
	bolt.setPosition(0.0f, 0.0f, 0.0f);
	bolt.setEndPoint(Vector3(0.0f, 0.0f, 0.0f));
	bolt.setBranchesNum(400);
	bolt.setOffset(45.0f);
	bolt.setColour(Colour(128, 255, 128));
	bolt.setStartPoint(Vector3(-70.0f, 70.0f, 0.0f));
	bolt.setPause(50.0f);
	bolt.setWidth(0.25f);
	bolt.setBolt2Alpha(4.0f);
	bolt.setSegmentsNum(8);
	bolt.setUpdateFrequency(200);
	bolt.setBranchSpread(55.0f);
	bolt.setMaxFlash(0.0f);
	sm->getRootNode()->addChild(L"Bolt", bolt);


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

	// Create light
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

	// Add frame listener
	m_pFrameListener = new MyFrameListener();
	Renderer::getSingleton().addFrameListener(m_pFrameListener);

	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"Lightning Bolts");

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