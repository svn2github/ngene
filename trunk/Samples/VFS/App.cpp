/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "App.h"
#include "nGENE.h"


void App::createApplication()
{
	FrameworkWin32::createApplication();

	FileManager& mgr = FileManager::getSingleton();

	Renderer& renderer = Renderer::getSingleton();
	renderer.setClearColour(0);
	renderer.setCullingMode(CULL_CW);
	uint anisotropy = 0;
	renderer.getDeviceRender().testCapabilities(CAPS_MAX_ANISOTROPY, &anisotropy);
	for(uint i = 0; i < 8; ++i)
		renderer.setAnisotropyLevel(i, anisotropy);

	m_pMouse->setSensitivity(0.02f);

	// First create new archive
	FileArchive* pArchive = mgr.openArchive(L"Default", L"VFS2.nfs", OPEN_WRITE | OPEN_BINARY);

	IFile* pFile1 = mgr.openFile(L"angel.3ds", OPEN_READ | OPEN_BINARY, FT_NORMAL);
	IFile* pFile2 = mgr.openFile(L"palm_tree.x", OPEN_READ | OPEN_BINARY, FT_NORMAL);

	pArchive->insertFile(pFile1);
	pArchive->insertFile(pFile2);
	mgr.closeFile(pFile1->getFileName());
	mgr.closeFile(pFile2->getFileName());
	
	mgr.closeFile(pArchive->getFileName());


	// Now reopen it in 'reading' mode
	pArchive = mgr.openArchive(L"Default", L"VFS2.nfs", OPEN_READ | OPEN_BINARY);
	cout << pArchive->getEntriesCount() << endl;
	cout << ((DefaultFileArchive*)pArchive)->getVersion() << endl;
	
	// Now create scene
	SceneManager* sm = Engine::getSingleton().getSceneManager(0);

	CameraThirdPerson* cam;
	cam = (CameraThirdPerson*)sm->createCamera(L"CameraThirdPerson", L"Camera");
	cam->setVelocity(10.0f);
	cam->setPosition(Vector3(0.0f, 0.0f, 0.0f));


	// We're reading mesh from our archive!!! Note the path
	NodeMesh <MeshLoadPolicy3dsFile>* pAngel = sm->createMesh <MeshLoadPolicy3dsFile>(L"VFS2.nfs\\angel.3ds");
	pAngel->setPosition(0.0f, 0.0f, 9.0f);
	Surface* pSurface = NULL;
	pAngel->setScale(0.25f);
	Material* matAngel = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	for(uint i = 0; i < pAngel->getSurfacesNum(); ++i)
	{
		pSurface = pAngel->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(matAngel);
	}
	pAngel->setRotation(Maths::PI, -Maths::PI * 0.5f, 0.0f);
	sm->getRootNode()->addChild(L"Angel", *pAngel);

	// Now the same for .x file, well almost.
	// Note the difference though: we change current directory to the archive name.
	// nGENE treats archives as regular directories, so there is no need to specify
	// 'VFS.nfs' explicitly after that.
	mgr.changeDir("VFS2.nfs");
	string dir = mgr.getCurrentDir();
	cout << dir << endl;

	NodeMesh <MeshLoadPolicyXFile>* pPalm = sm->createMesh <MeshLoadPolicyXFile>(L"palm_tree.x");
	pPalm->setPosition(0.0f, 0.0f, 15.0f);
	pPalm->setScale(0.75f);
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

	Vector3 target = pAngel->getPositionLocal() + Vector3(0.0f, 3.0f, 0.0f);
	cam->setTarget(target);
	Engine::getSingleton().setActiveCamera(cam);
	pAngel->addChild(L"Camera", cam);
	m_pCamera = cam;

	NodeLight* pLight = sm->createLight(LT_SPOT);
	pLight->setPosition(-4.0f, 10.0f, 0.0f);
	pLight->setDirection(Vector3(0.7f, -0.9f, 1.0f));
	pLight->setDrawDebug(false);
	pLight->setRadius(500.0f);
	sm->getRootNode()->addChild(L"WorldLight", pLight);
	Renderer::getSingleton().addLight(pLight);

	// Create ground
	Plane pl(Vector3::UNIT_Y, Point(0.0f, 0.0f, 0.0f));
	PrefabPlane* pGround = sm->createPlane(pl, Vector2(800.0f, 800.0f));
	pGround->setPosition(0.0f, 0.0f, 0.0f);
	pSurface = pGround->getSurface(L"Base");
	Material* matGround = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"GroundPlane", pGround);

	// Add input listener
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);


	mgr.closeFile(pArchive->getFileName());

	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"Virtual File System");
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
	mgr.changeDir("../media");

	loadShaderConfig(L"ShaderConfig.xml");
	MaterialManager::getSingleton().createLibraryFromFile(L"core.lib", L"core");

	FileArchive* pArchive = mgr.openArchive(L"Default", L"VFS1.nfs", OPEN_READ | OPEN_BINARY);
	mgr.changeDir("VFS1.nfs");

	MaterialManager::getSingleton().createLibraryFromFile(L"vfs_demo.lib", L"default");

	mgr.closeFile(pArchive->getFileName());
	mgr.changeDir(".");
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