#include "App.h"
#include "nGENE.h"


void App::createApplication()
{
	FrameworkWin32::createApplication();

	hideCursor();

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

	CameraFirstPerson* cam;
	cam = (CameraFirstPerson*)sm->createCamera(L"CameraFirstPerson", L"Camera");
	cam->setVelocity(10.0f);
	cam->setPosition(Vector3(0.0f, 5.0f, -10.0f));
	cam->getFrustum().setNear(0.01f);
	sm->getRootNode()->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);


	NodeMesh <MeshLoadPolicyXFile>* pSculp = sm->createMesh <MeshLoadPolicyXFile>(L"statue.x");
	pSculp->setPosition(0.0f, 0.0f, 7.0f);
	pSculp->setScale(0.25f, 0.25f, 0.25f);
	Surface* pSurface = pSculp->getSurface(L"surface_2");
	pSurface->flipWindingOrder();
	Material* matstone = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"dotCel");
	pSurface->setMaterial(matstone);
	sm->getRootNode()->addChild(L"Sculpture", *pSculp);

	PrefabBox* pBox = sm->createBox(100.0f, 0.5f, 100.0f, Vector2(80.0f, 80.0f));
	pBox->setPosition(0.0f, 1.2, 0.0f);
	pSurface = pBox->getSurface(L"Base");
	Material* matGround = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"Plane", pBox);

	PrefabBox* pFrame = sm->createBox(1.0f, 5.0f, 1.0f);
	pFrame->setPosition(-1.65f, 4.0, -5.0f);
	pSurface = pFrame->getSurface(L"Base");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"DoorFrame", pFrame);

	PrefabBox* pDoor = sm->createBox(2.0f, 5.0f, 0.25f);
	pDoor->setPosition(0.0f, 3.95f, -5.0f);
	pSurface = pDoor->getSurface(L"Base");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"Door", pDoor);

	PrefabSphere* pSphere = sm->createSphere(20, 20, 1.5f);
	pSphere->setPosition(5.0f, 8.0f, 15.0f);
	pSurface = pSphere->getSurface(L"Base");
	sm->getRootNode()->addChild(L"Ball", pSphere);
	pSurface->setMaterial(matGround);

	PrefabSphere* pSphereSmall = sm->createSphere(20, 20, 0.5f);
	pSphereSmall->setPosition(7.0f, 8.0f, 15.0f);
	pSurface = pSphereSmall->getSurface(L"Base");
	sm->getRootNode()->addChild(L"BallSmall", pSphereSmall);
	pSurface->setMaterial(matGround);

	NodeLight* pLight = sm->createLight(LT_DIRECTIONAL);
	pLight->setPosition(-4.0f, 5.0f, 0.0f);
	pLight->setDirection(Vector3(0.7f, -0.5f, 1.0f));
	pLight->setDrawDebug(false);
	Colour clrWorld(204, 204, 0);
	pLight->setColour(clrWorld);
	pLight->setRadius(180.0f);
	sm->getRootNode()->addChild(L"WorldLight", *pLight);
	renderer.addLight(pLight);
	
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);


	// Create PhysicsWorld object
	PhysicsWorld* pWorld = Physics::getSingleton().createWorld(L"MyPhysicsWorld");
	pWorld->setGravity(Vector3(0.0f, -9.8f, 0.0f));

	// Create default material
	PhysicsMaterial* physMat = pWorld->createPhysicsMaterial(L"DefaultMaterial");
	physMat->setRestitution(0.5f);
	physMat->setDynamicFriction(0.5f);
	physMat->setStaticFriction(0.5f);

	PhysicsActor* pActor = pWorld->createPhysicsActorMesh(pSculp->getScale(), 
		*pSculp->getSurface(L"surface_2")->getVertexBuffer());
	pActor->attachNode(pSculp);
	pActor->setShapeMaterial(0, *physMat);

	pActor = pWorld->createPhysicsActor(L"Box", Vector3(100.0f, 0.5f, 100.0f));
	pActor->attachNode(pBox);
	pActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(L"Plane", pActor);

	pActor = pWorld->createPhysicsActor(L"Sphere", Vector3(1.5f, 0.0f, 0.0f), PhysicsActor::BT_DYNAMIC);
	pActor->attachNode(pSphere);
	pActor->setShapeMaterial(0, *physMat);
	pActor->setMass(1.0f);
	pWorld->addPhysicsActor(L"Ball", pActor);

	// Make larger ball swing (create hinge joint)
	JOINT_DESC desc;
	desc.actor1 = pActor;
	desc.actor2 = NULL;
	Joint* pJoint = pWorld->createJoint(L"Hinge", desc);
	Vector3 vecPos(0.0f, 15.0f, 15.0f);
	pJoint->setPosition(vecPos);
	pJoint->setDirection(Vector3::UNIT_Z);
	// Make this joint breakable
	pJoint->setBreakable(6.0f, 100000.0f);

	PhysicsActor* pActor2 = pWorld->createPhysicsActor(L"Sphere", Vector3(0.5f, 0.0f, 0.0f), PhysicsActor::BT_DYNAMIC);
	pActor2->attachNode(pSphereSmall);
	pActor2->setShapeMaterial(0, *physMat);
	pActor2->setMass(1.0f);
	pWorld->addPhysicsActor(L"BallSmall", pActor2);

	// Bind two balls together
	desc.actor1 = pActor;
	desc.actor2 = pActor2;
	pJoint = pWorld->createJoint(L"Fixed", desc);
	pJoint->setPosition(vecPos);	

	PhysicsActor* pFrameActor = pWorld->createPhysicsActor(L"Box", Vector3(1.0f, 5.0f, 1.0f));
	pFrameActor->attachNode(pFrame);
	pFrameActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(L"DoorFrame", pFrameActor);

	// Create hinge to simulate doors
	pActor = pWorld->createPhysicsActor(L"Box", Vector3(2.0f, 5.0f, 0.25f), PhysicsActor::BT_DYNAMIC);
	pActor->attachNode(pDoor);
	pActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(L"Door", pActor);

	desc.actor1 = pActor;
	desc.actor2 = NULL;
	pJoint = pWorld->createJoint(L"Hinge", desc);
	vecPos.set(-1.0f, 5.0f, -5.0f);
	pJoint->setPosition(vecPos);
	pJoint->setDirection(Vector3::UNIT_Y);


	// Create character controller
	CONTROLLER_DESC charDesc;
	charDesc.height = 2.0f;
	charDesc.obstacleLimit = 0.7f;
	charDesc.radius = 0.8f;
	m_pController = pWorld->createController(charDesc);
	m_pController->attachNode(cam);
	pWorld->addController(L"Player", m_pController);
}

CharacterController* App::getController() const
{
	return m_pController;
}

int main()
{
	App app;

	app.createApplication();
	app.run();

	app.shutdown();

	return 0;
}
