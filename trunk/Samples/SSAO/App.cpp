/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Version:	0.03
Info:		This demo presents Screen Space Ambient Occlusion.
---------------------------------------------------------------------------
*/

#include "App.h"
#include "nGENE.h"

#include <sstream>


App::App()
{
}
//----------------------------------------------------------------------
App::~App()
{
	cleanup();
}
//----------------------------------------------------------------------
void App::cleanup()
{
}
//----------------------------------------------------------------------
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
	cam->setPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_pCamera = cam;


	// Create directional light
	NodeLight* pLight = sm->createLight(LT_DIRECTIONAL);
	pLight->setPosition(0.0f, 0.0f, 0.0f);
	pLight->setDirection(Vector3(0.5f, -1.0f, -0.85f));
	Colour clrWorld(204, 204, 204);
	pLight->setColour(clrWorld);
	pLight->setRadius(1000.0f);
	pLight->setCastShadows(false);
	sm->getRootNode()->addChild(L"WorldLight", pLight);
	Renderer::getSingleton().addLight(pLight);

	
	// Create angel
	NodeMesh <MeshLoadPolicy3dsFile>* pAngel = sm->createMesh <MeshLoadPolicy3dsFile>(L"angel.3ds");
	pAngel->setPosition(0.0f, 0.0f, 7.0f);
	pAngel->setScale(0.75f);
	Surface* pSurface = NULL;
	Material* matAngel = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	for(uint i = 0; i < pAngel->getSurfacesNum(); ++i)
	{
		pSurface = pAngel->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(matAngel);
	}
	// Rotate using Euler angles
	pAngel->setRotation(0.0f, -Maths::PI * 0.5f, 0.0f);
	sm->getRootNode()->addChild(L"Angel", *pAngel);

	Vector3 target = pAngel->getPositionLocal() + Vector3(0.0f, 10.0f, 0.0f);
	cam->setTarget(target);
	pAngel->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);

	// Create house
	NodeMesh <MeshLoadPolicy3dsFile>* pHouse = sm->createMesh <MeshLoadPolicy3dsFile>(L"house_ssao.3ds");
	pHouse->setPosition(0.0f, 30.0f, 7.0f);
	pHouse->setScale(0.07f);
	Material* matHouse = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	for(uint i = 0; i < pHouse->getSurfacesNum(); ++i)
	{
		pSurface = pHouse->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(matHouse);
	}
	// Rotate using Euler angles
	pHouse->setRotation(0.0f, -Maths::PI * 0.5f, 0.0f);
	sm->getRootNode()->addChild(L"House", *pHouse);
	pHouse->setEnabled(false);


	// Create ground
	Plane pl(Vector3::UNIT_Y, Point(0.0f, 0.0f, 0.0f));
	PrefabPlane* pGround = sm->createPlane(pl, Vector2(800.0f, 800.0f));
	pGround->setPosition(0.0f, 0.0f, 0.0f);
	pSurface = pGround->getSurface(L"Base");
	Material* matGround = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"parallaxMap");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"GroundPlane", pGround);

	// Add input listener
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);

	// Post-processing - SSAO is done this way
	Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"ssao");
	postSSAO.priority = 0;
	postSSAO.material = pMaterial;
	postSSAO.material->setEnabled(true);

	((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"))->addToRender(&postSSAO);

	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"Screen Space Ambient Occlusion");

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