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


App::App():
	m_pController(NULL),
	m_pControllerLight(NULL)
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

	NodeMesh <MeshLoadPolicyXFile>* pPalm = sm->createMesh <MeshLoadPolicyXFile>(L"palm_tree.x");
	pPalm->setPosition(0.0f, 0.0f, 9.0f);
	Surface* pSurface = NULL;
	pPalm->setScale(0.5f);
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
	PrefabPlane* pGround = sm->createPlane(pl, Vector2(10.0f, 10.0f));
	pGround->setPosition(0.0f, 0.0f, 0.0f);
	pSurface = pGround->getSurface(L"Base");
	Material* matSteel = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"steel");
	pSurface->setMaterial(matSteel);
	sm->getRootNode()->addChild(L"GroundPlane", pGround);


	Material* matStone = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	Material* matCrate = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"crate");
	Material* matWood = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"parallaxOcclusionMap");
	// Lets change default material bump texture
	ITexture* pTexture = TextureManager::getSingleton().createTextureFromFile(L"relief", L"relief_bump.tga");
	matWood->getActiveRenderTechnique()->getRenderPass(0).getTextureSampler(1).setTexture(pTexture);

	PrefabBox* pBox = sm->createBox(2.0f, 10.0f, 100.0f, Vector2(10.0f, 1.0f));
	pBox->setPosition(-6.0f, 5.0f, 0.0f);
	uint surfaceID = 0;
	pBox->getSurface(surfaceID)->setMaterial(matStone);
	sm->getRootNode()->addChild(L"Box1", pBox);

	pBox = sm->createBox(2.0f, 10.0f, 100.0f, Vector2(5.0f, 1.0f));
	pBox->setPosition(6.0f, 5.0f, 0.0f);
	pBox->getSurface(surfaceID)->setMaterial(matWood);
	sm->getRootNode()->addChild(L"Box2", pBox);

	
	pBox = sm->createBox(12.0f, 10.0f, 1.0f);
	pBox->setPosition(0.0f, 5.0f, 50.0f);
	pBox->getSurface(surfaceID)->setMaterial(matWood);
	sm->getRootNode()->addChild(L"Box3", pBox);

	pBox = sm->createBox(12.0f, 10.0f, 1.0f);
	pBox->setPosition(0.0f, 5.0f, -50.0f);
	pBox->getSurface(surfaceID)->setMaterial(matStone);
	sm->getRootNode()->addChild(L"Box4", pBox);

	pBox = sm->createBox(12.0f, 2.0f, 100.0f, Vector2(1.0f, 10.0f));
	pBox->setPosition(0.0f, 10.0f, 0.0f);
	pBox->getSurface(surfaceID)->setMaterial(matStone);
	sm->getRootNode()->addChild(L"Box5", pBox);

	// Create light
	NodeLight* pLight = sm->createLight(LT_POINT);
	pLight->setPosition(-4.0f, 10.0f, 0.0f);
	pLight->setDirection(Vector3(0.7f, -0.9f, 1.0f));
	pLight->setDrawDebug(false);
	pLight->setRadius(500.0f);
	sm->getRootNode()->addChild(L"WorldLight", pLight);
	renderer.addLight(pLight);

	// Create physics
	PhysicsWorld* pWorld = Physics::getSingleton().createWorld(L"PhysicsWorld");
	pWorld->setGravity(Vector3(0.0f, -9.8f, 0.0f));
	pWorld->setSkinWidth(0.01f);

	PhysicsMaterial* physWood = pWorld->createPhysicsMaterial(L"Wood");
	physWood->setRestitution(0.5f);
	physWood->setDynamicFriction(0.5f);
	physWood->setStaticFriction(0.5f);

	PhysicsMaterial* physSteel = pWorld->createPhysicsMaterial(L"Steel");
	physSteel->setRestitution(0.5f);
	physSteel->setDynamicFriction(0.5f);
	physSteel->setStaticFriction(0.5f);

	PhysicsMaterial* physStone = pWorld->createPhysicsMaterial(L"Stone");
	physStone->setRestitution(0.5f);
	physStone->setDynamicFriction(0.5f);
	physStone->setStaticFriction(0.5f);


	PhysicsActor* pActor = NULL;
	pActor = pWorld->createPhysicsActor(L"Box", Vector3(2.0f, 10.0f, 100.0f), PhysicsActor::BT_STATIC);
	pActor->attachNode((NodeVisible*)sm->getNode(L"Box1"));
	pActor->setShapeMaterial(0, physStone);
	pWorld->addPhysicsActor(L"Box1", pActor);
	pActor->setMass(1.0f);

	pActor = pWorld->createPhysicsActor(L"Box", Vector3(2.0f, 10.0f, 100.0f), PhysicsActor::BT_STATIC);
	pActor->attachNode((NodeVisible*)sm->getNode(L"Box2"));
	pActor->setShapeMaterial(0, physWood);
	pWorld->addPhysicsActor(L"Box2", pActor);
	pActor->setMass(1.0f);

	pActor = pWorld->createPhysicsActor(L"Box", Vector3(12.0f, 10.0f, 1.0f), PhysicsActor::BT_STATIC);
	pActor->attachNode((NodeVisible*)sm->getNode(L"Box3"));
	pActor->setShapeMaterial(0, physWood);
	pWorld->addPhysicsActor(L"Box3", pActor);
	pActor->setMass(1.0f);

	pActor = pWorld->createPhysicsActor(L"Box", Vector3(12.0f, 10.0f, 1.0f), PhysicsActor::BT_STATIC);
	pActor->attachNode((NodeVisible*)sm->getNode(L"Box4"));
	pActor->setShapeMaterial(0, physStone);
	pWorld->addPhysicsActor(L"Box4", pActor);
	pActor->setMass(1.0f);

	pActor = pWorld->createPhysicsActor(L"Box", Vector3(12.0f, 2.0f, 100.0f), PhysicsActor::BT_STATIC);
	pActor->attachNode((NodeVisible*)sm->getNode(L"Box5"));
	pActor->setShapeMaterial(0, physStone);
	pWorld->addPhysicsActor(L"Box5", pActor);
	pActor->setMass(1.0f);

	pActor = pWorld->createPhysicsActor(L"Plane", Vector3(1000.0f, 1.0f, 1000.0f), PhysicsActor::BT_STATIC);
	pActor->attachNode((NodeVisible*)sm->getNode(L"GroundPlane"));
	pActor->setShapeMaterial(0, physSteel);
	pWorld->addPhysicsActor(L"Ground", pActor);
	pActor->setMass(1.0f);

	for(uint i = 0; i < pPalm->getSurfacesNum(); ++i)
	{
		Surface* pSurface = pPalm->getSurface(i);
		if(!i)
		{
			pActor = pWorld->createPhysicsActorMesh(Vector3(1.0f, 1.0f, 1.0f), 
				*pPalm->getSurface(i)->getVertexBuffer());
			pActor->attachNode(pPalm);
		}
		else
		{
			ShapeMesh* pShape = new ShapeMesh();
			pShape->setDimensions(Vector3::ONE_VECTOR);
			pShape->setGeometryData(*pPalm->getSurface(i)->getVertexBuffer(), Physics::getSingleton().getPhysX(), false);
			pActor->addShape(pShape);
		}
		pActor->setShapeMaterial(i, *physWood);
	}
	pWorld->addPhysicsActor(L"Palm", pActor);

	// Create crates
	uint count = 0;
	for(uint i = 0; i < 3; ++i)
	{
		for(uint j = 0; j < 3; ++j)
		{
			for(uint k = 0; k < 3; ++k)
			{
				wstringstream name;
				name << L"Crate_" << count++;

				pBox = sm->createBox(1.0f, 1.0f, 1.0f);
				pBox->setPosition(i, k + 1.5f, j);
				uint surfaceID = 0;
				pBox->getSurface(surfaceID)->setMaterial(matCrate);
				sm->getRootNode()->addChild(name.str(), pBox);

				pActor = pWorld->createPhysicsActor(L"Box", Vector3(1.0f, 1.0f, 1.0f), PhysicsActor::BT_DYNAMIC);
				pActor->attachNode((NodeVisible*)pBox);
				pActor->setShapeMaterial(0, *physWood);
				pWorld->addPhysicsActor(name.str(), pActor);
				pActor->setMass(1.0f);
				// Crates belong to different group!!!
				pActor->setGroup(1);
			}
		}
	}

	// Create listeners
	m_pCollisionListener = new MyCollisionListener();
	pWorld->addCollisionListener(m_pCollisionListener);
	pWorld->setGroupsFlags(0, 0, CT_START);
	pWorld->setGroupsFlags(0, 1, CT_START);


	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);

	// Create sound visualization for debugging purposes
	PrefabSphere* pSoundSphere = sm->createSphere(10, 10, 0.25f);
	pSurface = pSoundSphere->getSurface(L"Base");
	pSurface->setMaterial(matSteel);
	sm->getRootNode()->addChild(L"SoundHelper", pSoundSphere);

	// Create moving sound
	SOUND_DESC soundDesc;
	soundDesc.isUsingHardware = false;
	soundDesc.is3D = true;
	soundDesc.fileName = L"drumloop.wav";
	NodeSound* pSound = sm->createSound(L"Drums", soundDesc);
	ISoundInstance* pInternalSound = pSound->getSound();
	pInternalSound->getSound()->setLooped(true);
	pSound->setPosition(0.0f, 0.0f, 0.0f);
	pSoundSphere->addChild(L"Sound1", pSound);
	pInternalSound->play();

	m_pController = new PositionController <LinearInterpolator>(15000.0f, true);
	Vector3 temp = Vector3(0, 2, 0);
	m_pController->addPoint(temp, 0.0f);
	temp = Vector3(6, 2, 100);
	m_pController->addPoint(temp, 0.25f);
	temp = Vector3(0, 2, 0);
	m_pController->addPoint(temp, 0.50f);
	temp = Vector3(-6, 2, -100);
	m_pController->addPoint(temp, 0.75f);
	temp = Vector3(0, 2, 0);
	m_pController->addPoint(temp, 1.0f);
	pSoundSphere->addAnimationController(m_pController);
	m_pController->startAnimation();

	NodeDebugPath <LinearInterpolator>* pLine = new NodeDebugPath <LinearInterpolator>(m_pController, 250, Colour::COLOUR_BLUE);
	pLine->setCreator(sm);
	pLine->init();
	pLine->setPosition(0.0f, 0.0f, 0.0f);
	pLine->getSurface(L"Base")->setMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"debug_point"));
	pLine->getSurface(L"Points")->setMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"debug_point"));
	sm->getRootNode()->addChild(L"SoundPath", pLine);

	// Move light
	m_pControllerLight = new PositionController <LinearInterpolator>(50000.0f, true);
	temp = Vector3(-4, 8, 0);
	m_pControllerLight->addPoint(temp, 0.0f);
	temp = Vector3(1, 7, 10);
	m_pControllerLight->addPoint(temp, 0.25f);
	temp = Vector3(-4, 8, 0);
	m_pControllerLight->addPoint(temp, 0.50f);
	temp = Vector3(1, 7, -10);
	m_pControllerLight->addPoint(temp, 0.75f);
	temp = Vector3(-4, 8, 0);
	m_pControllerLight->addPoint(temp, 1.0f);
	pLight->addAnimationController(m_pControllerLight);
	m_pControllerLight->startAnimation();

	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"Sound");

	// Post-processing - SSAO is done this way
	Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"ssao");
	postSSAO.priority = 0;
	postSSAO.material = pMaterial;
	postSSAO.material->setEnabled(true);

	ShaderInstance* pShader = pMaterial->getActiveRenderTechnique()->getRenderPass(0).getPixelShader();
	Real radius = 1.025f;
	Real range = 1.0f;
	ShaderConstant* pConstant = pShader->getConstant("radius");
	if(pConstant)
		pConstant->setDefaultValue(&radius);
	pConstant = pShader->getConstant("range");
	if(pConstant)
		pConstant->setDefaultValue(&range);

	((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"))->addToRender(&postSSAO);

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
void App::closeApplication()
{
	// Stop controllers before leaving
	if(m_pController)
	{
		m_pController->stopAnimation();
	}

	if(m_pControllerLight)
	{
		m_pControllerLight->stopAnimation();
	}

	FrameworkWin32::closeApplication();
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
