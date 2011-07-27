/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Version:	0.12
---------------------------------------------------------------------------
*/

#include "App.h"
#include "nGENE.h"

#include "NodeLightning.h"
#include "NodeVolumetric.h"
#include "NodeDebugFrustum.h"


void App::createApplication()
{
	FrameworkWin32::createApplication();

	// Let's load custom material library
	MaterialManager::getSingleton().createLibraryFromFile(L"SwayingPalmMatLib.lib", L"Demo");

	m_pMouse->setSensitivity(0.02f);

	// Set up default stuff
	QUAD_TREE_DESC quad_tree;
	quad_tree.bounds.left = -1000.0f;
	quad_tree.bounds.right= 1000.0f;
	quad_tree.bounds.top = -1000.0f;
	quad_tree.bounds.bottom = 1000.0f;
	quad_tree.minimumNodeSize = 20.0f;
	m_pPartitioner = new ScenePartitionerQuadTree(quad_tree);
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

	
	// Creating a skeletal animated mesh is just a matter of using
	// another loading policy class (in this example - AnimatedMeshLoadPolicyXFile,
	// instead of its static version - MeshLoadPolicyXFile).
	pTiny = sm->createMesh <AnimatedMeshLoadPolicyXFile>(L"tiny_4anim.x");
	pTiny->setPosition(0.0f, 3.0f, 2.0f);
	pTiny->setRotation(Vector3::UNIT_X, -Maths::PI_HALF);
	pTiny->setUpVector(UV_Z);
	Surface* pSurface = NULL;
	pTiny->setScale(0.0125f);
	Material* matTiny = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"skinned");
	for(uint i = 0; i < pTiny->getSurfacesNum(); ++i)
	{
		pSurface = pTiny->getSurface(i);
		if(i == 0)
			matTiny = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"skinned");
		pSurface->setMaterial(matTiny);
		pSurface->flipWindingOrder();
	}
	sm->getRootNode()->addChild(L"Tiny", *pTiny);

	// Skeletal animated meshes have skeleton defined for them. As we want
	// to animate the bones we have to obtain it and pass it to the
	// constructor of SkeletalAnimationController.
	m_pSkeletal = new SkeletalAnimationController <AnimatedMeshLoadPolicyXFile>(&pTiny->getSkeleton());
	m_pSkeletal->setConstants(matTiny->getActiveRenderTechnique()->getRenderPass(0).getVertexShader()->getConstant("boneMatrices"),
							  matTiny->getRenderTechnique(L"shadow")->getRenderPass(0).getVertexShader()->getConstant("boneMatrices"));
	
	// We have also to take animations from the mesh - we could use
	// different ones of course
	for(uint i = 0; i < pTiny->getAnimationSetsNum(); ++i)
	{
		m_pSkeletal->addAnimationSet(pTiny->getAnimationSet(i));
	}
	
	m_pSkeletal->getSkeleton()->getRoot()->print(cout);

	// We can also achieve blending by setting different weights for the
	// animations sets
	for(uint i = 0; i < m_pSkeletal->getAnimationSetsNum(); ++i)
	{
		// In this example we blend two animation sets - waving a hand and running.
		// To achieve this for each of the sets involved we set a per bone weights
		// for blending. For waving a hand set we apply 1.0 weights for right arm
		// and hand and 0 for the rest of the bones. For running it's completely
		// opposite.
		float weight = 0.0f;
		if(i == 0)
		{
			for(uint j = 0; j < m_pSkeletal->getBonesNum(); ++j)
			{
				BONE* pBone = m_pSkeletal->getBone(j);
				if((pBone->name.find("rm") != string::npos || pBone->name.find("Hand") != string::npos) &&
					pBone->name.find("_R_") != string::npos)
				{
					m_pSkeletal->getAnimationSet(i)->setWeight(pBone, 1.0f);
				}
				else
				{
					m_pSkeletal->getAnimationSet(i)->setWeight(pBone, 0.0f);
				}
			}
		}
		else if(i == 1)
		{
			for(uint j = 0; j < m_pSkeletal->getBonesNum(); ++j)
			{
				BONE* pBone = m_pSkeletal->getBone(j);
				if((pBone->name.find("rm") != string::npos || pBone->name.find("Hand") != string::npos) &&
					pBone->name.find("_R_") != string::npos)
				{
					m_pSkeletal->getAnimationSet(i)->setWeight(pBone, 0.0f);
				}
				else
				{
					m_pSkeletal->getAnimationSet(i)->setWeight(pBone, 1.0f);
				}
			}
		}
		else
		{
			// We're not using the rest of animations in this example.
			// However, this is to show that you can use same weights for all bones.
			// That is useful when you have animation blending friendly animation
			// sets or want to tween between different sets.
			m_pSkeletal->getAnimationSet(i)->setWeight(0.0f);
		}
	}

	// Call this method before starting animation
	pTiny->addAnimationController(m_pSkeletal);

	// Finally we can start it
	m_pSkeletal->startAnimation();

	// Here we create another mesh - not animated one (although we will change its
	// scale factor using another controller).
	NodeMesh <MeshLoadPolicyXFile>* pPalm = sm->createMesh <MeshLoadPolicyXFile>(L"palm_tree.x");
	pPalm->setPosition(0.0f, 0.0f, 9.0f);
	pSurface = NULL;
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


	// We will also change position of our animated mesh. Note that we use
	// Catmull-Rom interpolation and the animation is taking
	// 10000 milliseconds (10 seconds).
	m_pController = new PositionController <CatmullRomInterpolator>(10000.0f, true);
	m_pController->setFaceTarget(true);
	Vector3 pos = pPalm->getPositionLocal();
	pos.y = 3.0f;
	m_pController->addPoint(pos, 0.0f);
	Vector3 temp = pos + Vector3(3, 0, 3);
	m_pController->addPoint(temp, 0.0);
	temp = pos + Vector3(-3, 0, 3);
	m_pController->addPoint(temp, 0.32);
	temp = pos + Vector3(3, 0, 6);
	m_pController->addPoint(temp, 0.48);
	temp = pos + Vector3(5, 0, 3);
	m_pController->addPoint(temp, 0.64);
	temp = pos + Vector3(4, 0, -2);
	m_pController->addPoint(temp, 0.8);
	temp = pos + Vector3(-10, 0, -10);
	m_pController->addPoint(temp, 0.9);
	temp = pos + Vector3(-15, 0, 2);
	m_pController->addPoint(temp, 1);
	temp = pos + Vector3(15, 0, 3);
	m_pController->addPoint(temp, 1.0f);
	pTiny->addAnimationController(m_pController);
	m_pController->startAnimation();

	// Ands also we will scale a palm created a second before
	pScaleCont = new ScaleController <LinearInterpolator>(2000.0f, true);
	temp = Vector3::ONE_VECTOR;
	pScaleCont->addPoint(temp, 0);
	temp *= 0.5f;
	pScaleCont->addPoint(temp, 0.5);
	temp *= 2.0f;
	pScaleCont->addPoint(temp, 1);
	pPalm->addAnimationController(pScaleCont);
	pScaleCont->startAnimation();

	// This code is showing how to change rotation of the node.
	// However, it is not really used (we neither add it to the
	// node nor start the animation).
	pRotCont = new OrientationController <QuaternionInterpolator>(2000.0f, true);
	pRotCont->addPoint(pPalm->getRotation(), 0.0f);
	Quaternion quatRot(1.0f, 0.0f, 0.0f, Maths::PI * 0.5f);
	pRotCont->addPoint(quatRot, 0.65f);
	pRotCont->addPoint(pPalm->getRotation(), 1.0f);
	//pTiny->addAnimationController(pRotCont);
	//pRotCont->startAnimation();


	Vector3 target = Vector3(0.0f, 0.0f, 0.0f);
	cam->setTarget(target);
	cam->setDistanceFromTarget(40.0f);
	sm->getRootNode()->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);


	// For debugging purposes we can render path
	NodeDebugPath <CatmullRomInterpolator>* pLine = new NodeDebugPath <CatmullRomInterpolator>(m_pController, 250, Colour::COLOUR_BLUE);
	pLine->setCreator(sm);
	pLine->init();
	pLine->setPosition(0.0f, 1.5f, 0.0f);
	pLine->getSurface(L"Base")->setMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"debug_point"));
	pLine->getSurface(L"Points")->setMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"debug_point"));
	sm->getRootNode()->addChild(L"DebugPath", pLine);



	// Create ground
	Plane pl(Vector3::UNIT_Y, Point(0.0f, 0.0f, 0.0f));	
	PrefabPlane* pGround = sm->createPlane(pl, Vector2(800.0f, 800.0f));
	pGround->setPosition(0.0f, 0.0f, 0.0f);
	pSurface = pGround->getSurface(L"Base");
	Material* matGround = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"GroundPlane", pGround);

	// Create light
	NodeLight* pLight = sm->createLight(LT_POINT);
	pLight->setPosition(0.0f, 15.0f, 20.0f);
	pLight->setRadius(300.0f);
	Vector3 dir = Vector3(0.0f, 0.0f, 1.0f);
	dir.normalize();
	sm->getRootNode()->addChild(L"WorldLight", pLight);
	renderer.addLight(pLight);

	Plane plGround;
	plGround.setPlane(Vector3::UNIT_Y, Point::ZERO_POINT);
	
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);


	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"Animation introduction");

	// Create GUI
	m_pFont = new GUIFont(L"gui_default_font.fnt");

	GUIManager::getSingleton().setFont(m_pFont);
	GUIManager::getSingleton().addWindow(&m_Window);
	GUIManager::getSingleton().setActiveMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"gui_default_active"));
	GUIManager::getSingleton().setInactiveMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"gui_default_inactive"));
}
//----------------------------------------------------------------------
NodeMesh <AnimatedMeshLoadPolicyXFile>* App::getModel() const
{
	return pTiny;
}
//----------------------------------------------------------------------
SkeletalAnimationController <AnimatedMeshLoadPolicyXFile>* App::getSkeletalAnimation() const
{
	return m_pSkeletal;
}
//----------------------------------------------------------------------
PositionController <CatmullRomInterpolator>* App::getPositionController() const
{
	return m_pController;
}
//----------------------------------------------------------------------
CameraThirdPerson* App::getCamera() const
{
	return m_pCamera;
}
//----------------------------------------------------------------------
void App::closeApplication()
{
	// Stop animation
	if(m_pSkeletal)
	{
		m_pSkeletal->stopAnimation();
	}

	if(m_pController)
	{
		m_pController->stopAnimation();
	}

	if(pScaleCont)
	{
		pScaleCont->stopAnimation();
	}

	if(pRotCont)
	{
		pRotCont->stopAnimation();
	}


	FrameworkWin32::closeApplication();
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
App::App():
	pScaleCont(NULL),
	m_pController(NULL),
	pRotCont(NULL),
	m_pSkeletal(NULL)
{
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
