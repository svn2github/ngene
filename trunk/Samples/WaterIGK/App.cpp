/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Info:		Official IGK'2009 (Polish Game Developers Conference) demo working
			as a proof-of-concept for "Realistic rendering of water effect in 
			the post-process stage" paper.
			This demo was later turned into official nGENE Tech demo.
Version:	0.06
---------------------------------------------------------------------------
*/

#include "App.h"
#include "nGENE.h"


App::App(bool _profile):
	FrameworkWin32(_profile),
	m_pCamera(NULL)
{
}
//----------------------------------------------------------------------
void App::createApplication()
{
	FrameworkWin32::createApplication();

	hideCursor();

	m_pMouse->setSensitivity(0.02f);

	QUAD_TREE_DESC quad_tree;
	quad_tree.bounds.left = -700.0f;
	quad_tree.bounds.right= 700.0f;
	quad_tree.bounds.top = -700.0f;
	quad_tree.bounds.bottom = 700.0f;
	quad_tree.minimumNodeSize = 25.0f;
	Assert(quad_tree.isValid(), "Invalid quad tree description!");
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

	// Create GUI
	m_pFont = new GUIFont(L"gui_default_font.fnt");

	GUIManager& gui = GUIManager::getSingleton();
	gui.setFont(m_pFont);
	m_Window.setVisible(false);
	gui.addWindow(&m_Window);
	gui.setActiveMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"gui_default_active"));
	gui.setInactiveMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"gui_default_active"));

	m_Splash = new SplashScreen();
	gui.addWindow(m_Splash);

	// Prepare splash screen
	Renderer::getSingleton().render();

	// Load map from file
	deserialize(L"tropicalIsland.nmp");
	
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);

	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"nGENE Tech Tropical Island Demo");

	// Enable HDR
	((LightStage*)renderer.getRenderStage(L"Light"))->setUseHDR(true);

	// Load post process effects
	createPostProcess();

	((ShadowStage*)Renderer::getSingleton().getRenderStage(L"Shadows"))->setRange(30.0f);
	Renderer::getSingleton().getRenderStage(L"Shadows")->setEnabled(false);

	((ReflectionStage*)Renderer::getSingleton().getRenderStage(L"Reflection"))->setReflectionDistance(Engine::getSingleton().getActiveCamera()->getFrustum().getFar());


	// Add plants
	ITexture* densityMap = TextureManager::getSingleton().createTextureFromFile(L"grass_density_map",
																				L"Islands.jpg");

	NodeMesh <MeshLoadPolicyXFile>* pPalm = sm->createMesh <MeshLoadPolicyXFile>(L"palm_tree.x");
	pPalm->setScale(1.3f, 1.3f, 1.3f);
	for(uint i = 0; i < pPalm->getSurfacesNum(); ++i)
	{
		Surface* pSurface = pPalm->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
		{
			Material* matPalm = NULL;
			if(i == 1)
				matPalm = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"palm_leaf");
			else
				matPalm = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"palm_bark");
			pSurface->setMaterial(matPalm);
			pSurface->flipWindingOrder();
		}
	}

	NodeMesh <MeshLoadPolicyXFile>* pPalm2 = sm->createMesh <MeshLoadPolicyXFile>(L"palm_tree_2.x");
	pPalm2->setScale(0.1f, 0.1f, 0.1f);
	for(uint i = 0; i < pPalm2->getSurfacesNum(); ++i)
	{
		Surface* pSurface = pPalm2->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
		{
			Material* matPalm = NULL;
			if(i == 1)
				matPalm = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"palm_leaf");
			else
				matPalm = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"palm_bark");
			pSurface->setMaterial(matPalm);
			pSurface->flipWindingOrder();
		}
	}

	NodeMesh <MeshLoadPolicy3dsFile>* pRock = sm->createMesh <MeshLoadPolicy3dsFile>(L"stone.3ds");
	pRock->setScale(2.8f, 2.8f, 2.8f);
	for(uint i = 0; i < pRock->getSurfacesNum(); ++i)
	{
		Surface* pSurface = pRock->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
		{
			Material* matRock = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"stones");
			pSurface->setMaterial(matRock);
		}
	}

	DECORATOR_DESC palms;
	palms.objectsCount = 65;
	palms.node = pPalm;
	palms.distributionMap = densityMap;
	palms.scaleSpread = 0.125f;

	DECORATOR_DESC palms2;
	palms2.objectsCount = 65;
	palms2.node = pPalm2;
	palms2.distributionMap = densityMap;
	palms2.undergroundLevel = -0.15f;
	palms2.scaleSpread = 0.125f;

	DECORATOR_DESC rocks;
	rocks.objectsCount = 230;
	rocks.node = pRock;
	rocks.distributionMap = densityMap;
	rocks.scaleSpread = 0.55f;

	PLANT_DESC grass1;
	grass1.plantsCount = 9000;
	grass1.width = 5.0f;
	grass1.widthSpread = 0.4f;
	grass1.height = 5.0f;
	grass1.heightSpread = 0.4f;
	grass1.undergroundLevel = -0.15f;
	grass1.material = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"meadow_grass_4");
	grass1.distributionMap = densityMap;

	NodeTerrain* pTerrain = (NodeTerrain*)sm->getNode(L"Landscape");

	meadow.setCreator(sm);
	meadow.setTerrain(pTerrain);
	meadow.addPlant(grass1);
	sm->getRootNode()->addChild(L"Meadow", meadow);

	forest.setCreator(sm);
	forest.setTerrain(pTerrain);
	forest.addDecorator(palms);
	forest.addDecorator(palms2);
	forest.addDecorator(rocks);
	//forest.addDecorator(ferns);
	sm->getRootNode()->addChild(L"Forest", forest);

	time.hours = 6;
	time.year = 2009;

	m_Window.setVisible(true);
	m_Splash->setVisible(false);
}
//----------------------------------------------------------------------
CameraFirstPerson* App::getCamera()
{
	if(!m_pCamera)
		m_pCamera = (CameraFirstPerson*)Engine::getSingleton().getActiveCamera();

	return m_pCamera;
}
//----------------------------------------------------------------------
SPriorityMaterial* App::getPostEffect(uint _index)
{
	return &m_Post[_index];
}
//----------------------------------------------------------------------
void App::createPostProcess()
{
	// Here we create a lot of post processing effect.
	// Names of the materials should be self-explanatory

	MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");
	Material* pMaterial = pMatLib->getMaterial(L"antialias");
	m_Post[0].priority = 100;
	m_Post[0].material = pMaterial;
	m_Post[0].material->setEnabled(true);

	m_Post[1].priority = 1;
	m_Post[1].material = pMatLib->getMaterial(L"depthOfField");
	m_Post[1].material->setEnabled(false);
	
	m_Post[2].priority = 1;
	m_Post[2].material = pMatLib->getMaterial(L"sepia");
	m_Post[2].material->setEnabled(false);

	m_Post[3].priority = 2;
	m_Post[3].material = pMatLib->getMaterial(L"inverse");
	m_Post[3].material->setEnabled(false);

	m_Post[4].priority = 3;
	m_Post[4].material = pMatLib->getMaterial(L"blackAndWhite");
	m_Post[4].material->setEnabled(false);

	m_Post[5].priority = 5;
	m_Post[5].material = pMatLib->getMaterial(L"fog");
	m_Post[5].material->setEnabled(false);

	m_Post[6].priority = 6;
	m_Post[6].material = pMatLib->getMaterial(L"ssao");
	ShaderInstance* pShader = m_Post[6].material->getActiveRenderTechnique()->getRenderPass(0).getPixelShader();
	Real radius = 0.225f;
	Real range = 0.5f;
	ShaderConstant* pConstant = pShader->getConstant("radius");
	if(pConstant)
		pConstant->setDefaultValue(&radius);
	pConstant = pShader->getConstant("range");
	if(pConstant)
		pConstant->setDefaultValue(&range);
	m_Post[6].material->setEnabled(false);

	m_Post[7].priority = 7;
	m_Post[7].material = pMatLib->getMaterial(L"edgeDetection");
	m_Post[7].material->setEnabled(false);

	m_Post[8].priority = 8;
	m_Post[8].material = pMatLib->getMaterial(L"thermalVision");
	m_Post[8].material->setEnabled(false);

	m_Post[9].priority = 20;
	m_Post[9].material = pMatLib->getMaterial(L"makeTiles");
	m_Post[9].material->setEnabled(false);

	m_Post[10].priority = 20;
	m_Post[10].material = pMatLib->getMaterial(L"waterColor");
	m_Post[10].material->setEnabled(false);

	m_Post[11].priority = 20;
	m_Post[11].material = pMatLib->getMaterial(L"pencilStroke");
	m_Post[11].material->setEnabled(false);

	m_Post[12].priority = 20;
	m_Post[12].material = pMatLib->getMaterial(L"monochrome");
	m_Post[12].material->setEnabled(false);

	m_Post[13].priority = 20;
	m_Post[13].material = pMatLib->getMaterial(L"nightVisionDevice");
	m_Post[13].material->setEnabled(false);

	m_Post[14].priority = 20;
	m_Post[14].material = pMatLib->getMaterial(L"oldMovie");
	m_Post[14].material->setEnabled(false);

	m_Post[15].priority = 20;
	m_Post[15].material = pMatLib->getMaterial(L"distanceHeatHaze");
	m_Post[15].material->setEnabled(false);

	m_Post[16].priority = 20;
	m_Post[16].material = pMatLib->getMaterial(L"vintageLook");
	m_Post[16].material->setEnabled(false);

	m_Post[17].priority = 20;
	m_Post[17].material = pMatLib->getMaterial(L"warmify");
	m_Post[17].material->setEnabled(false);

	m_Post[18].priority = 20;
	m_Post[18].material = pMatLib->getMaterial(L"orwig");
	m_Post[18].material->setEnabled(false);

	m_Post[19].priority = 20;
	m_Post[19].material = pMatLib->getMaterial(L"grading_orange_and_teal");
	m_Post[19].material->setEnabled(false);

	m_Post[20].priority = 20;
	m_Post[20].material = pMatLib->getMaterial(L"grading_high_contrast");
	m_Post[20].material->setEnabled(false);

	m_Post[21].priority = 20;
	m_Post[21].material = pMatLib->getMaterial(L"grading_low_contrast");
	m_Post[21].material->setEnabled(false);

	m_Post[22].priority = 20;
	m_Post[22].material = pMatLib->getMaterial(L"vintageLook_noBlue");
	m_Post[22].material->setEnabled(false);

	PostStage* postStage = ((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"));
	for(uint i = 0; i < POST_MATERIALS_COUNT; ++i)
		postStage->addToRender(&m_Post[i]);
}
//----------------------------------------------------------------------
NodeWater* App::getWater()
{
	return (NodeWater*)Engine::getSingleton().getSceneManager(0)->getNode(L"Water");
}
//----------------------------------------------------------------------
NodeSky* App::getSky() const
{
	return (NodeSky*)Engine::getSingleton().getSceneManager(0)->getNode(L"Sky");
}
//----------------------------------------------------------------------
NodeClouds* App::getClouds() const
{
	return (NodeClouds*)Engine::getSingleton().getSceneManager(0)->getNode(L"Clouds");
}
//----------------------------------------------------------------------
TimeDate& App::getTime()
{
	return time;
}
//----------------------------------------------------------------------
int main()
{
	App app(true);

	app.createApplication();
	app.run();

	app.shutdown();

	return 0;
}
//----------------------------------------------------------------------