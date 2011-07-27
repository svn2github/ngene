/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Version:	0.03
Info:		This demo presents 2D non-photorealistic rendering.
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
	// In this demo we hide standard config dialog.
	FrameworkWin32::suppressConfigDialog();
	FrameworkWin32::createApplication();


	Maths::setAngleUnit(Maths::ANGLE_RADIAN);
	Renderer::getSingleton().setClearColour(Colour::COLOUR_BLACK);

	SceneManager* sm = Engine::getSingleton().getSceneManager(0);


	// Let's create camera with orthogonal projection.
	// This way we can easily achieve 2D look of the scene.
	// Important thing to note is from now on, we have to think
	// in pixels instead of abstract units.
	Camera2D* cam = (Camera2D*)sm->createCamera(L"Camera2D", L"Camera");
	cam->setVelocity(1000.0f);
	cam->setPosition(100.0f, -0.5f, -300.0f);
	cam->setUp(Vector3::UNIT_Y);

	Quaternion rot(Vector3(0.0f, 1.0f, 0.0f), Maths::PI / 4.0f);
	Quaternion rot2(Vector3(1.0f, 0.0f, 0.0f), Maths::PI / 4.0f);
	Quaternion rotFinal = rot * rot2;
	rotFinal.normalize();
	if(Maths::abs <float>(rotFinal.x - cam->getRotation().x) > 0.001f ||
	   Maths::abs <float>(rotFinal.y - cam->getRotation().y) > 0.001f ||
	   Maths::abs <float>(rotFinal.z - cam->getRotation().z) > 0.001f ||
	   Maths::abs <float>(rotFinal.w - cam->getRotation().w) > 0.001f)
	{
		cam->setRotation(rotFinal);
	}
	cam->getFrustumPtr()->setNear(0.0f);
	cam->getFrustumPtr()->setFar(12000.0f);
	cam->updateCamera();

	sm->getRootNode()->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(static_cast<Camera*>(sm->getNode(L"Camera")));

	// Create directional light
	NodeLight* pLight = sm->createLight(LT_DIRECTIONAL);
	pLight->setPosition(0.0f, 0.0f, 0.0f);
	pLight->setDirection(Vector3(-0.5f, -1.0f, 0.5f));
	Colour clrWorld(204, 204, 204);
	pLight->setColour(clrWorld);
	pLight->setRadius(10000.0f);
	pLight->setCastShadows(false);
	sm->getRootNode()->addChild(L"WorldLight", pLight);
	Renderer::getSingleton().addLight(pLight);

	pLight = sm->createLight(LT_POINT);
	pLight->setColour(Colour::COLOUR_GREEN);
	pLight->setPosition(150.0f, 100.0f, 0.0f);
	pLight->setRadius(1000.0f);
	pLight->setCastShadows(false);
	sm->getRootNode()->addChild(L"PointLightGreen", pLight);
	Renderer::getSingleton().addLight(pLight);

	pLight = sm->createLight(LT_POINT);
	pLight->setColour(Colour::COLOUR_MAGENTA);
	pLight->setPosition(800.0f, 100.0f, 0.0f);
	pLight->setRadius(1000.0f);
	pLight->setCastShadows(false);
	sm->getRootNode()->addChild(L"PointLightGreen2", pLight);
	Renderer::getSingleton().addLight(pLight);


	NodeMesh <MeshLoadPolicyXFile>* pWall = sm->createMesh <MeshLoadPolicyXFile>(L"wall_with_pillars.x");
	pWall->setScale(80.00001f, 80.1f, 80.00001f);
	Material* mat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	for(uint i = 0; i < 10; ++i)
		pWall->getSurface(i)->setMaterial(mat);
	Material* mat2 = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"parallaxOcclusionMap");
	Surface* surf = pWall->getSurface(L"surface_10");
	surf->setMaterial(mat2);
	surf = pWall->getSurface(L"surface_11");
	surf->setMaterial(mat);
	surf = pWall->getSurface(L"surface_12");
	surf->setMaterial(mat);
	sm->getRootNode()->addChild(L"Wall", *pWall);

	NodeMesh <MeshLoadPolicyXFile>* pSculp = sm->createMesh <MeshLoadPolicyXFile>(L"statue.x");
	pSculp->setPosition(700.0f, -0.5f, 2.0f);
	pSculp->setScale(20.0f, 20.0f, 20.0f);
	surf = pSculp->getSurface(L"surface_2");
	surf->setMaterial(mat);
	sm->getRootNode()->addChild(L"Statue", *pSculp);

	Engine::getSingleton().getRenderer().setCullingMode(CULL_NONE);

	// Add input listener
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);

	// Post-processing - that is what cause water color appearance of the image
	Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"waterColor");
	postWaterColor.priority = 0;
	postWaterColor.material = pMaterial;
	postWaterColor.material->setEnabled(true);

	pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"pencilStroke");
	postPencil.priority = 0;
	postPencil.material = pMaterial;
	postPencil.material->setEnabled(false);

	((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"))->addToRender(&postWaterColor);
	((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"))->addToRender(&postPencil);

	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"Non-photorealistic rendering");

	// Create GUI
	m_pFont = new GUIFont(L"gui_default_font.fnt");

	GUIManager::getSingleton().setFont(m_pFont);
	GUIManager::getSingleton().addWindow(&m_Window);
	GUIManager::getSingleton().setActiveMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"gui_default_active"));
	GUIManager::getSingleton().setInactiveMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"gui_default_inactive"));
}
//----------------------------------------------------------------------