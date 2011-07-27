/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "App.h"
#include "nGENE.h"


void App::createApplication(HWND* hwnd)
{
	FrameworkWin32::suppressConfigDialog();
	FrameworkWin32::createApplication(hwnd);

	// Let's load custom material library
	MaterialManager::getSingleton().createLibraryFromFile(L"SwayingPalmMatLib.lib", L"Demo");

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

	NodeMesh <MeshLoadPolicyXFile>* pPalm = sm->createMesh <MeshLoadPolicyXFile>(L"palm_tree_2.x");
	pPalm->setPosition(10.0f, 0.0f, 10.0f);
	Surface* pSurface = NULL;
	pPalm->setScale(0.05f);
	for(uint i = 0; i < pPalm->getSurfacesNum(); ++i)
	{
		Surface* pSurface = pPalm->getSurface(i);
		Material* matPalm = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"palm_leaf");
		if(i == 0)
			matPalm = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"palm_bark");
		pSurface->setMaterial(matPalm);
		pSurface->flipWindingOrder();
	}
	sm->getRootNode()->addChild(L"Palm", *pPalm);

	Vector3 target = pPalm->getPositionLocal() + Vector3(0.0f, 3.0f, 0.0f);
	cam->setTarget(target);
	pPalm->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);

	// Create ground
	Plane pl(Vector3::UNIT_Y, Point(0.0f, 0.0f, 0.0f));
	PrefabPlane* pGround = sm->createPlane(pl, Vector2(800.0f, 800.0f));
	pGround->setPosition(0.0f, 0.0f, 0.0f);
	pSurface = pGround->getSurface(L"Base");
	Material* matGround = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"GroundPlane", pGround);

	NodeLight* pLight = sm->createLight(LT_SPOT);
	pLight->setPosition(-4.0f, 10.0f, 0.0f);
	pLight->setDirection(Vector3(0.7f, -0.9f, 1.0f));
	pLight->setDrawDebug(false);
	pLight->setRadius(180.0f);
	sm->getRootNode()->addChild(L"WorldLight", pLight);
	renderer.addLight(pLight);
	
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);

	// Set window caption
	//Window* pWindow = Renderer::getSingleton().getWindow(0);
	//pWindow->setWindowCaption(L"Swaying palm");

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
void App::loadResources()
{
	FileManager& mgr = FileManager::getSingleton();

	// Mount '../media' path as current directory
	//mgr.changeDir("D:\\nGENE");

	FileArchive* pArchive = mgr.openArchive(L"Default", L"media.nfs", OPEN_READ | OPEN_BINARY);
	mgr.changeDir("media.nfs");

	loadShaderConfig(L"ShaderConfig.xml");
	MaterialManager::getSingleton().createLibraryFromFile(L"core.lib", L"core");
	MaterialManager::getSingleton().createLibraryFromFile(L"default.lib", L"default");
}
//----------------------------------------------------------------------