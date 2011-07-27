#include "App.h"
#include "nGENE.h"


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

	CameraFirstPerson* cam;
	cam = (CameraFirstPerson*)sm->createCamera(L"CameraFirstPerson", L"Camera");
	cam->setVelocity(80.0f);
	cam->setPosition(Vector3(0.0f, 25.0f, -10.0f));
	sm->getRootNode()->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);


	// Creating terrain
	ITexture* pTexture = TextureManager::getSingleton().createTextureFromFile(L"Terrain", L"terrain_height_map.jpg", TT_2D, TU_NORMAL, TF_DEFAULT);

	TEXTURE_DESC textureDesc = pTexture->getTextureDesc();
	TERRAIN_DESC terrainDesc;

	terrainDesc.columns = textureDesc.width;
	terrainDesc.rows = textureDesc.height;
	terrainDesc.heightScale = 0.3f;
	terrainDesc.step = 1.0f;
	terrainDesc.tiles_x = 32;
	terrainDesc.tiles_z = 32;
	terrainDesc.skirtSize = 1.0f;

	FuncFilterBox <nGENE::byte, float> boxFilter;
	nGENE::byte* pTempHeights = (nGENE::byte*)pTexture->getData();
	nGENE::byte* pHeights = new nGENE::byte[textureDesc.width * textureDesc.height];

	// We obtain only single channel from a texture
	uint index = 0;
	for(uint z = 0; z < textureDesc.height; ++z)
	{
		for(uint x = 0; x < textureDesc.width; ++x)
			pHeights[z * textureDesc.width + x] = pTempHeights[index++ * pTexture->getBitDepth()]; 
	}
	NGENE_DELETE_ARRAY(pTempHeights);
	terrainDesc.heights = boxFilter(pHeights,
									textureDesc.width,
									textureDesc.height);

	// Now let's add LOD
	terrainDesc.LODs.push_back(make_pair(1, 80.0f));
	terrainDesc.LODs.push_back(make_pair(2, 120.0f));
	terrainDesc.LODs.push_back(make_pair(4, 180.0f));
	terrainDesc.LODs.push_back(make_pair(8, 320.0f));
	terrainDesc.LODs.push_back(make_pair(16, 1e9));

	NodeTerrain* pTerrain = sm->createTerrain(terrainDesc);
	pTerrain->setPosition(-256.0f, -9.0f, -128.0f);
	Material* matTerrain = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"terrain");
	Surface* pSurface = NULL;
	for(uint i = 0; i < 1024; ++i)
	{
		pSurface = pTerrain->getSurface(i);
		pSurface->setMaterial(matTerrain);
	}

	sm->getRootNode()->addChild(L"Landscape", pTerrain);
	NGENE_DELETE_ARRAY(pHeights);


	ITexture* densityMap = TextureManager::getSingleton().createTextureFromFile(L"grass_density_map", L"grass_density.jpg");

	// Create meadow
	PLANT_DESC grass;
	grass.plantsCount = 350000;
	grass.width = 1.5f;
	grass.widthSpread = 0.4f;
	grass.height = 1.5f;
	grass.heightSpread = 0.4f;
	grass.undergroundLevel = 0.0f;
	grass.material = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"meadow_grass_1");
	grass.distributionMap = densityMap;

	meadow.setCreator(sm);
	meadow.setTerrain(pTerrain);
	meadow.addPlant(grass);
	sm->getRootNode()->addChild(L"Meadow", meadow);

	TextureManager::getSingleton().removeTexture(L"grass_density_map");

	// Create sky and clouds
	sky.setCreator(sm);
	sky.init();
	sky.setSimulated(false);
	TimeDate time;
	time.hours = 15;
	sky.setDayTime(time);
	sky.setPosition(0.0f, 0.0f, 0.0f);
	sm->getRootNode()->addChild(L"Sky", sky);

	clouds.setSimulated(false);
	clouds.setCreator(sm);
	clouds.init();
	clouds.setPosition(0.0f, 150.0f, 0.0f);
	clouds.setDayTime(time);
	sm->getRootNode()->addChild(L"Clouds", clouds);
	
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);


	PhysicsWorld* pWorld = Physics::getSingleton().createWorld(L"MyPhysicsWorld");
	pWorld->setGravity(Vector3(0.0f, -9.8f, 0.0f));

	PhysicsMaterial* physMat = pWorld->createPhysicsMaterial(L"DefaultMaterial");
	physMat->setRestitution(0.5f);
	physMat->setDynamicFriction(0.5f);
	physMat->setStaticFriction(0.5f);

	PhysicsActor* pActor = pWorld->createTerrain(pTerrain->getTerrainDesc());
	pActor->attachNode((NodeVisible*)sm->getNode(L"Landscape"));
	pActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(L"Terrain", pActor);

	CONTROLLER_DESC charDesc;
	charDesc.height = 2.0f;
	charDesc.slopeLimit = 0.5f;
	charDesc.obstacleLimit = 0.7f;
	charDesc.radius = 0.8f;
	m_pController = pWorld->createController(charDesc);
	m_pController->attachNode(cam);
	m_pController->setVelocity(8.0f);
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
