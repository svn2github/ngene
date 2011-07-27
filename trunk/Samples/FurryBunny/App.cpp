/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "App.h"
#include "nGENE.h"


void App::createApplication()
{
	FrameworkWin32::createApplication();

	Renderer::getSingleton().setVSync(false);

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

	// Create bunny
	NodeMesh <MeshLoadPolicyXFile>* pBunny = sm->createMesh <MeshLoadPolicyXFile>(L"bunny.x");
	pBunny->setPosition(0.0f, 4.0f, 7.0f);
	Surface* pSurface = NULL;
	Material* matFur = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"fur_base");
	uint index = 0; 
	pSurface = pBunny->getSurface(index);
	pSurface->flipWindingOrder();
	pSurface->setMaterial(matFur);
	sm->getRootNode()->addChild(L"Bunny", pBunny);

	m_Fur.setCreator(sm);
	m_Fur.init();
	m_Fur.addSurface(L"Fur", *pSurface);
	pBunny->addChild(L"Fur", m_Fur);

	cam->setTarget(pBunny->getPositionLocal());
	pBunny->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);

	// Create ground
	Plane pl(Vector3::UNIT_Y, Point(0.0f, 0.0f, 0.0f));
	PrefabPlane* pGround = sm->createPlane(pl, Vector2(800.0f, 800.0f));
	pGround->setPosition(0.0f, 1.2, 0.0f);
	pSurface = pGround->getSurface(L"Base");
	Material* matGround = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"GroundPlane", pGround);

	// Create spot light
	NodeLight* pLight = sm->createLight(LT_SPOT);
	pLight->setPosition(-4.0f, 8.0f, 0.0f);
	pLight->setDirection(Vector3(0.7f, -0.9f, 1.0f));
	pLight->setDrawDebug(false);
	Colour clrWorld = Colour::COLOUR_WHITE;
	pLight->setColour(clrWorld);
	pLight->setRadius(180.0f);
	sm->getRootNode()->addChild(L"WorldLight", pLight);
	renderer.addLight(pLight);
	
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);

	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"Furry bunny");

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
	ShaderManager::getSingleton().setDebugEnabled(true);

	// Mount '../media' path as current directory
	FileManager::getSingleton().changeDir("../media");

	loadShaderConfig(L"ShaderConfig.xml");
	MaterialManager::getSingleton().createLibraryFromFile(L"core.lib", L"core");
	MaterialManager::getSingleton().createLibraryFromFile(L"default.lib", L"default");
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