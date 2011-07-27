#include "App.h"
#include "nGENE.h"


App::~App()
{
	NGENE_DELETE(m_pFactory);
}

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

	// Before adding camera we have to register factory for it
	m_pFactory = new SlidingCameraFactory();

	m_pCamera = (CameraFirstPersonSliding*)sm->createCamera(L"CameraFirstPersonSliding", L"Camera");
	m_pCamera->setVelocity(10.0f);
	m_pCamera->setPosition(Vector3(0.0f, 5.0f, -10.0f));
	sm->getRootNode()->addChild(L"Camera", m_pCamera);
	Engine::getSingleton().setActiveCamera(m_pCamera);


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


	PhysicsWorld* pWorld = Physics::getSingleton().createWorld(L"MyPhysicsWorld");
	pWorld->setGravity(Vector3(0.0f, -9.8f, 0.0f));

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
}

CameraFirstPersonSliding* App::getCamera() const
{
	return m_pCamera;
}

int main()
{
	App app;

	app.createApplication();
	app.run();

	app.shutdown();

	return 0;
}
