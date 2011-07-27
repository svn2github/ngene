/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

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

	CameraThirdPerson* cam;
	cam = (CameraThirdPerson*)sm->createCamera(L"CameraThirdPerson", L"Camera");
	cam->setVelocity(10.0f);
	cam->setPosition(Vector3(0.0f, 0.0f, 0.0f));
	//cam->getFrustum().setNear(0.01f);
	cam->getFrustum().setFar(3000.0f);
	m_pCamera = cam;

	Vector3 target;
	cam->setTarget(target);
	sm->getRootNode()->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);

	///////////////////////////////////////////////////
	// Creating terrain
	// Here we create base mesh.
	///////////////////////////////////////////////////
	ITexture* pTexture = TextureManager::getSingleton().createTextureFromFile(L"Terrain", L"terrain_height_map.jpg", TT_2D, TU_NORMAL, TF_DEFAULT);

	TEXTURE_DESC textureDesc = pTexture->getTextureDesc();
	TERRAIN_DESC terrainDesc;

	terrainDesc.columns = textureDesc.width;
	terrainDesc.rows = textureDesc.height;
	terrainDesc.heightScale = 0.35f;
	terrainDesc.step = 1.0f;
	terrainDesc.tiles_x = 16;
	terrainDesc.tiles_z = 16;
	terrainDesc.skirtSize = 10.0f;

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
	terrainDesc.LODs.push_back(make_pair(1, 1.0f));
	terrainDesc.LODs.push_back(make_pair(2, 20.0f));
	terrainDesc.LODs.push_back(make_pair(4, 80.0f));
	terrainDesc.LODs.push_back(make_pair(8, 160.0f));
	terrainDesc.LODs.push_back(make_pair(16, 1e9));

	NodeTerrain* pTerrain = sm->createTerrain(terrainDesc);
	pTerrain->setPosition(-256.0f, 0.0f, -256.0f);
	Material* matTerrain = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"terrain_morphable");
	Surface* pSurface = NULL;
	for(uint i = 0; i < 256; ++i)
	{
		pSurface = pTerrain->getSurface(i);
		pSurface->setMaterial(matTerrain);
	}

	sm->getRootNode()->addChild(L"Landscape", pTerrain);
	NGENE_DELETE_ARRAY(pHeights);

	///////////////////////////////////////////////////
	// Creating terrain morph target.
	// We create it normally - note that it has to have
	// same number of vertices!
	///////////////////////////////////////////////////
	pTexture = TextureManager::getSingleton().createTextureFromFile(L"TerrainMorph", L"terrain_height_map_target.bmp", TT_2D, TU_NORMAL, TF_DEFAULT);

	textureDesc = pTexture->getTextureDesc();
	
	terrainDesc.columns = textureDesc.width;
	terrainDesc.rows = textureDesc.height;
	terrainDesc.heightScale = 1.0f;
	terrainDesc.step = 1.0f;
	terrainDesc.tiles_x = 16;
	terrainDesc.tiles_z = 16;
	terrainDesc.skirtSize = 10.0f;

	pTempHeights = (nGENE::byte*)pTexture->getData();
	pHeights = new nGENE::byte[textureDesc.width * textureDesc.height];

	// We obtain only single channel from a texture
	index = 0;
	for(uint z = 0; z < textureDesc.height; ++z)
	{
		for(uint x = 0; x < textureDesc.width; ++x)
			pHeights[z * textureDesc.width + x] = pTempHeights[index++ * pTexture->getBitDepth()]; 
	}
	NGENE_DELETE_ARRAY(pTempHeights);
	terrainDesc.heights = boxFilter(pHeights,
									textureDesc.width,
									textureDesc.height);


	///////////////////////////////////////////////////
	// As we use tweening we have to extend standard
	// vertex declaration by adding data for 2nd
	// stream.
	///////////////////////////////////////////////////
	VERTEX_ELEMENT elements[8] =
	{
		{0, 0,  0, 3, VEU_POSITION},
		{0, 12, 0, 3, VEU_NORMAL},
		{0, 24, 0, 4, VEU_COLOUR},
		{0, 28, 0, 2, VEU_TEXCOORD},
		{1, 0,  1, 3, VEU_POSITION},
		{1, 12, 1, 3, VEU_NORMAL},
		{1, 24, 1, 4, VEU_COLOUR},
		{1, 28, 1, 2, VEU_TEXCOORD}
	};

	// Create new vertex declaration.
	VertexDeclaration* pDecl = VertexBufferManager::getSingleton().createVertexDeclaration(L"Default_2", elements, 8);

	NodeTerrain* pTerrain2 = sm->createTerrain(terrainDesc);
	pSurface = NULL;
	for(uint i = 0; i < 256; ++i)
	{
		// Get VertexBuffer from the target mesh
		VertexBuffer* pVB = pTerrain2->getSurface(i)->getVertexBuffer();
		// In this line we change vertex declaration for the vertex buffer
		pVB->setVertexDeclaration(pDecl);

		pSurface = pTerrain->getSurface(i);
		// Put VertexBuffer into the second stream of base mesh
		pSurface->setVertexBuffer(pVB, 1);
	}



	// Create sky and clouds
	sky.setCreator(sm);
	sky.init();
	sky.setSimulated(false);
	TimeDate time;
	time.hours = 15;
	sky.setDayTime(time);
	sky.setPosition(0.0f, -100.0f, 0.0f);
	sm->getRootNode()->addChild(L"Sky", sky);
	
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);

	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"Morphable terrain");

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
int main()
{
	App app;

	app.createApplication();
	app.run();

	app.shutdown();

	return 0;
}
//----------------------------------------------------------------------
