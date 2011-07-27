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

#include "LevelGenerator.h"
#include "NodeWater.h"
#include "NodeSky.h"
#include "FuncSplit.h"
#include "Console.h"


using nGENE::Nature::NodeWater;
using nGENE::Nature::NodeSky;
using nGENE::FuncSplit;


void App::createApplication()
{
	m_pLoadingThread = NULL;
	m_pActiveMusic = NULL;

	m_stPlayerName = L"Player";

	//FrameworkWin32::suppressConfigDialog();
	FrameworkWin32::createApplication();

	GUIManager::getSingleton().setBaseGUISize(800, 600);


	srand(Engine::getSingleton().getTimer().getMicroseconds());

	// Disable loggers
	m_LogConsole.setEnabled(false);
	m_LogXML.setEnabled(false);
	m_pConsoleWnd->setEnabled(false);

	// @todo showing cursor not working?!
	//hideCursor();
	showCursor();

	m_fScrollStage = 0.0f;
	m_nEnemiesCount = 0;
	m_nDecoratorsCount = 0;

	AIActorEnumerator::getSingleton().addFactory(m_ActorFactory, CPUCharacter::Type.Type());

	Renderer::getSingleton().setVSync(false);
	Renderer::getSingleton().removeRenderStage(L"Reflection");

	m_pMouse->setSensitivity(0.02f);

	Renderer& renderer = Renderer::getSingleton();
	renderer.setClearColour(0);
	renderer.setCullingMode(CULL_CW);
	uint anisotropy = 0;
	renderer.getDeviceRender().testCapabilities(CAPS_MAX_ANISOTROPY, &anisotropy);
	for(uint i = 0; i < 8; ++i)
		renderer.setAnisotropyLevel(i, anisotropy);


	// Set window caption
	Window* pWindow = Renderer::getSingleton().getWindow(0);
	pWindow->setWindowCaption(L"As Inevitable As Death");

	// Create GUI
	m_pMenuFont = new GUIFont(L"gui_aiad_menu_font.fnt");
	m_pGameFont = new GUIFont(L"gui_aiad_font.fnt");

	// We disable possibility to loose focus by a window
	GUIManager::getSingleton().setFont(m_pMenuFont);
	GUIManager::getSingleton().setActiveMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"gui_default_active"));
	GUIManager::getSingleton().setInactiveMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"gui_default_active"));

	m_MsgBox.hide();
	m_MsgBox.onButtonPressed.bind(this, &App::msgBoxResult);

	m_MainMenuWindow.addControls();
	m_LoadingScreen.addControls();
	m_Window.addControls();
	m_HiScoresWindow.addControls();
	m_OptionsWindow.addControls();

	GUIManager::getSingleton().addWindow(&m_MainMenuWindow, true);
	GUIManager::getSingleton().addWindow(&m_HiScoresWindow, false);
	GUIManager::getSingleton().addWindow(&m_OptionsWindow, false);
	GUIManager::getSingleton().addWindow(&m_MsgBox, false);
	GUIManager::getSingleton().addWindow(&m_LoadingScreen, false);
	GUIManager::getSingleton().addWindow(&m_Window, false);

	m_HiScoresWindow.hide();
	m_OptionsWindow.hide();
	m_LoadingScreen.hide();
	m_Window.hide();

	m_Cursor.setSize(20.0f, 20.0f);
	//Colour clr = Colour::COLOUR_YELLOW;
	//m_Cursor.setColour(clr);
	GUIManager::getSingleton().setCursor(&m_Cursor);


	// Load hi-scores
	FileManager& mgr = FileManager::getSingleton();
	IFile* pFile = mgr.openFile(L"hiscores.dat", OPEN_READ, FT_UNICODE);

	while(!pFile->eof())
	{
		wstring stLine = pFile->getLine();
		if(stLine == L"")
			break;

		vector <wstring> vWords;
		FuncSplit <wstring> tokenizer;
		tokenizer(stLine, vWords, L",");

		uint points;
		swscanf(vWords[1].c_str(), L"%d", &points);

		SHiScoreEntry entry = {vWords[0], points};

		m_vHiScores.push_back(entry);
	}

	mgr.closeFile(pFile->getFileName());

	loadHiScores();

	setLoading(true);


	// Load menu sound
	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	SOUND_DESC soundDesc;
	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"menu_select.wav";
	m_pMenuSound = sm->createSound(L"Menu", soundDesc);
	ISoundInstance* pInternalSound = m_pMenuSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	pInternalSound->setVolume(1.0f);
	sm->getRootNode()->addChild(L"MenuSound", m_pMenuSound);

	// Load key pressed sound
	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"key_press.wav";
	m_pKeySound = sm->createSound(L"Key", soundDesc);
	pInternalSound = m_pKeySound->getSound();
	pInternalSound->getSound()->setLooped(false);
	pInternalSound->setVolume(1.0f);
	sm->getRootNode()->addChild(L"KeySound", m_pKeySound);

	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"music/menu.mp3";
	m_pMenuMusic = sm->createSound(L"MenuMusic", soundDesc);
	pInternalSound = m_pMenuMusic->getSound();
	pInternalSound->getSound()->setLooped(true);
	m_pMenuMusic->setPosition(0.0f, 0.0f, 0.0f);
	sm->getRootNode()->addChild(L"MenuMusic", m_pMenuMusic);
	pInternalSound->play();
}
//----------------------------------------------------------------------
void App::loadHiScores()
{
	// Add hi-score items
	for(uint i = 0; i < m_vHiScores.size(); ++i)
	{
		if(i == 10)
			break;

		SHiScoreEntry& entry = m_vHiScores[i];
		wstringstream msgPlayer;
		msgPlayer << (i + 1) << L". " << entry.player;
		wstringstream msgScore;
		msgScore << entry.points;

		m_HiScoresWindow.addHiScoreItem(msgPlayer.str(), msgScore.str());
	}
}
//----------------------------------------------------------------------
void App::msgBoxResult(PROMPT_WINDOW_RESULT_BUTTON _result)
{
	switch(_result)
	{
	case PWRB_YES:
		{
			m_Window.setNoAmmoImageEnabled(false);

			m_bDied = false;
			Engine::getSingleton().getSceneManager(0)->getNode(L"PlayerNode")->setEnabled(true);

			m_Player.reset();

			m_Player.addWeapon(m_BasicWeapon);
			m_Player.setActiveWeapon(m_BasicWeapon);

			m_bSloMotion = false;
			m_fSpeedRate = 1.0f;
			m_fScrollStage = 0.0f;
			m_nCombo = 0;

			m_Player.getController()->setPosition(14.0f, 12.0f, 7.0f);

			Vector3 target(14.0f, 15.5f, 10.0f);
			m_pCamera->setPosition(0.0f, 0.0f, 0.0f);
			m_pCamera->setTarget(target);

			cleanDecorators(true);
			cleanEnemies(true);
			cleanPickupItems(true);
			cleanProjectiles(true);
			cleanExplosions(true);

			m_nDecoratorsCount = 0;
			m_nEnemiesCount = 0;

			m_BasicWeapon->reset();
			m_TwoShotsWeapon->reset();
			m_ThreeShotsWeapon->reset();
			m_LightningWeapon->reset();
			m_TwoLightningWeapon->reset();
			m_BombWeapon->reset();
			m_MissileWeapon->reset();

			hideCursor();
		}
		
		break;

	case PWRB_NO:
		//closeApplication();
		createMenu();
		closeMap();
		break;
	}
}
//----------------------------------------------------------------------
void App::createMenu()
{
	m_MainMenuWindow.show();
	//GUIManager::getSingleton().addWindow(&m_MainMenuWindow, true);
}
//----------------------------------------------------------------------
void App::closeMap()
{
	m_vMusic.clear();


	m_Window.hide();
	//GUIManager::getSingleton().removeWindow(&m_Window);
	GUIManager::getSingleton().setFont(m_pMenuFont);

	cleanDecorators(true);
	cleanEnemies(true);
	cleanPickupItems(true);
	cleanProjectiles(true);
	cleanExplosions(true);

	m_Player.reset();

	m_fScrollStage = 0.0f;
	m_nDecoratorsCount = 0;
	m_nEnemiesCount = 0;

	AIManager::getSingleton().removeAllActors();

	PostStage* postStage = ((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"));
	postStage->removeAllMaterials();

	Physics::getSingleton().removeWorld(0);
	
	Engine::getSingleton().getSceneManager(0)->cleanup();
	Engine::getSingleton().setActiveCamera(NULL);

	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	Node* pRoot = new Node();
	pRoot->setName(L"SceneRoot");
	pRoot->setCreator(sm);
	sm->init(pRoot);

	SOUND_DESC soundDesc;
	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"menu_select.wav";
	m_pMenuSound = sm->createSound(L"Menu", soundDesc);
	ISoundInstance* pInternalSound = m_pMenuSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	pInternalSound->setVolume(2.1f);
	sm->getRootNode()->addChild(L"MenuSound", m_pMenuSound);

	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"key_press.wav";
	m_pKeySound = sm->createSound(L"Key", soundDesc);
	pInternalSound = m_pKeySound->getSound();
	pInternalSound->getSound()->setLooped(false);
	pInternalSound->setVolume(1.0f);
	sm->getRootNode()->addChild(L"KeySound", m_pKeySound);

	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"music/1.mp3";
	m_pMenuMusic = sm->createSound(L"MenuMusic", soundDesc);
	pInternalSound = m_pMenuMusic->getSound();
	pInternalSound->getSound()->setLooped(true);
	m_pMenuMusic->setPosition(0.0f, 0.0f, 0.0f);
	sm->getRootNode()->addChild(L"MenuMusic", m_pMenuMusic);
	pInternalSound->play();


	Engine::getSingleton().removeAllUpdateListeners();
	InputSystem::getSingleton().removeAllInputListeners();


	m_HiScoresWindow.clearHiScore();
	loadHiScores();
}
//----------------------------------------------------------------------
void App::playerDied()
{
	uint nPoints = m_Player.getPoints();
	bool hiScore = false;
	uint i = 0;
	for(i = 0; i < m_vHiScores.size(); ++i)
	{
		if(m_vHiScores[i].points < nPoints)
		{
			hiScore = true;
			break;
		}
	}

	if(i >= 10)
		hiScore = false;

	if(!m_vHiScores.size() || m_vHiScores.size() < 10)
		hiScore = true;
	

	if(hiScore)
	{
		SHiScoreEntry entry = {m_stPlayerName, nPoints};
		m_vHiScores.insert(m_vHiScores.begin() + i, entry);

		wstringstream msg;
		msg << L"YOU REACHED HI-SCORE #" << (i + 1) << L"!";

		//m_Window.setMessage(msg.str(), Colour::COLOUR_YELLOW);

		FileManager& mgr = FileManager::getSingleton();
		IFile* pFile = mgr.openFile(L"hiscores.dat", OPEN_WRITE, FT_UNICODE);

		for(uint i = 0; i < m_vHiScores.size(); ++i)
		{
			if(i > 9)
				break;

			pFile->write(L"%s,%d\n", m_vHiScores[i].player.c_str(), m_vHiScores[i].points);
		}

		mgr.closeFile(pFile->getFileName());

		m_Window.setHiScore(msg.str());
	}
	else
	{
		showMsgBox();
	}

	m_bDied = true;

	showCursor();

	generateExplosion(m_Player.getPosition(), &m_Player);

	Engine::getSingleton().getSceneManager(0)->getNode(L"PlayerNode")->setEnabled(false);
}
//----------------------------------------------------------------------
void App::showMsgBox()
{
	m_MsgBox.setPosition(0, 0);
	Colour clr;
	clr.setAlpha(100);
	m_MsgBox.setColour(clr);
	m_MsgBox.slideIn(500);
	m_MsgBox.activate();
}
//----------------------------------------------------------------------
void App::createMap()
{
	m_pMenuMusic->getSound()->stop();

	//GUIManager::getSingleton().setFont(m_pGameFont);

	// Remove main menu window
	m_MainMenuWindow.hide();
	//GUIManager::getSingleton().removeWindow(&m_MainMenuWindow);

	m_LoadingScreen.show();

	//Physics::getSingleton().setEnabled(false);
	//ISoundManager::getSingleton().setEnabled(false);

	hideCursor();


	// We need to bind custom rendering routine to render GUI only while loading.
	Renderer::getSingleton().onRender.bind(this, &App::onLoadingRender);
	if(m_pLoadingThread)
		NGENE_DELETE(m_pLoadingThread);
	m_pLoadingThread = new LoadingThread(this);
	m_pLoadingThread->start();

	m_pWeaponSwitchSound = NULL;
	m_nExplosionsCount = 0;

	QUAD_TREE_DESC quadTreeDesc;
	quadTreeDesc.bounds.left = -64.0f;
	quadTreeDesc.bounds.right = 64.0f;
	quadTreeDesc.bounds.top = 4096.0f;
	quadTreeDesc.bounds.bottom = 0;
	m_pPartitioner = new ScenePartitionerQuadTree();
	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	sm->setScenePartitioner(m_pPartitioner);

	CameraThirdPerson* cam;
	cam = (CameraThirdPerson*)sm->createCamera(L"CameraThirdPerson", L"Camera");
	cam->setVelocity(2.0f);
	cam->setPosition(Vector3(0.0f, 0.0f, 0.0f));
	cam->setTarget(Vector3(0.0f, 0.0f, 0.0f));

	Quaternion quatRot(Vector3(1.0f, 0.0f, 0.0f), Maths::PI_HALF);
	cam->setRotation(quatRot);
	m_pCamera = cam;


	PhysicsWorld* pWorld = Physics::getSingleton().createWorld(L"PhysicsWorld");
	pWorld->setGravity(Vector3(0.0f, -1.8f, 0.0f));
	pWorld->setSkinWidth(0.01f);

	PhysicsActor* pActor = NULL;

	// Create physics materials
	PhysicsMaterial* physMat = pWorld->createPhysicsMaterial(L"DefaultMaterial");
	physMat->setRestitution(0.5f);
	physMat->setDynamicFriction(0.5f);
	physMat->setStaticFriction(0.5f);

	physMat = pWorld->createPhysicsMaterial(L"Debris");
	physMat->setRestitution(0.0f);
	physMat->setDynamicFriction(30.9f);
	physMat->setStaticFriction(30.9f);

	// Create player character
	m_Player.init(sm, pWorld);

	// Create weapons
	m_BasicWeapon = new BasicWeapon();
	m_TwoShotsWeapon = new TwoShotsWeapon();
	m_ThreeShotsWeapon = new ThreeShotsWeapon();
	m_LightningWeapon = new LightningWeapon();
	m_TwoLightningWeapon = new TwoLightningWeapon();
	m_BombWeapon = new BombWeapon();
	m_MissileWeapon = new MissileWeapon();

	m_BasicWeapon->init(sm, pWorld, physMat);
	m_TwoShotsWeapon->init(sm, pWorld, physMat);
	m_ThreeShotsWeapon->init(sm, pWorld, physMat);
	m_LightningWeapon->init(sm, pWorld, physMat);
	m_TwoLightningWeapon->init(sm, pWorld, physMat);
	m_BombWeapon->init(sm, pWorld, physMat);
	m_MissileWeapon->init(sm, pWorld, physMat);

	m_Player.addWeapon(m_BasicWeapon);
	m_Player.setActiveWeapon(m_BasicWeapon);


	Vector3 target = m_Player.getVisual()->getPositionLocal() +
		Vector3(0.0f, 3.5f, 3.0f);
	cam->setTarget(target);
	cam->setDistanceFromTarget(6.0f);
	sm->getRootNode()->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);

	// Create directional light (temporary)
	Renderer& renderer = Renderer::getSingleton();

	NodeLight* pLight = sm->createLight(LT_DIRECTIONAL);
	pLight->setPosition(10.0f, 15.0f, 8.0f);
	// @todo check why z has to be non-zero?
	Vector3 vecDir(0.0f, -1.0f, 0.01f);
	vecDir.normalize();
	pLight->setDirection(vecDir);
	pLight->setDrawDebug(false);
	pLight->setCastShadows(true);
	Colour clrWorld = Colour::COLOUR_WHITE;
	pLight->setColour(clrWorld);
	//pLight->setRadius(1800.0f);
	pLight->setIntensity(0.001f);
	pLight->setRadius(40.0f);
	pLight->setShadowResolution(1024);
	pLight->setShadowDensity(0.35f);
	sm->getRootNode()->addChild(L"ShadowLight", pLight);
	renderer.addLight(pLight);


	/// @todo check why it looks that bad...
	//((LightStage*)Renderer::getSingleton().getRenderStage(L"Light"))->toggleHDR();

	/*pLight = sm->createLight(LT_DIRECTIONAL);
	pLight->setPosition(10.0f, 15.0f, 8.0f);
	// @todo check why z has to be non-zero?
	vecDir.set(0.0f, -1.0f, 1.0f);
	vecDir.normalize();
	pLight->setDirection(vecDir);
	pLight->setDrawDebug(false);
	pLight->setCastShadows(false);
	pLight->setColour(clrWorld);
	pLight->setRadius(1800.0f);
	sm->getRootNode()->addChild(L"WorldLight", pLight);
	renderer.addLight(pLight);*/

	NodeSky* m_pSky = new NodeSky(3500.0f);
	m_pSky->setCreator(sm);
	m_pSky->init();
	m_pSky->setSimulated(false);
	m_pSky->setCastShadows(false);
	m_pSky->setPosition(0.0f, -13.0f, 0.0f);
	m_pSky->setDayLength(60.0f);
	TimeDate time;
	time.hours = 10;
	m_pSky->setDayTime(time);
	sm->getRootNode()->addChild(L"Sky", m_pSky);

	sm->getNode(L"SkyDome")->setEnabled(false);

	((ShadowStage*)Renderer::getSingleton().getRenderStage(L"Shadows"))->setRange(15.0f);
	((ShadowStage*)Renderer::getSingleton().getRenderStage(L"Shadows"))->setShadowOffset(Vector3(0.0f, 0.0f, 10.0f));

	// Create terrain
	ITexture* pTexture = TextureManager::getSingleton().createTextureFromFile(L"Terrain", L"canyon.jpg", TT_2D, TU_NORMAL, TF_DEFAULT);

	TEXTURE_DESC textureDesc = pTexture->getTextureDesc();
	TERRAIN_DESC terrainDesc;

	terrainDesc.columns = textureDesc.width;
	terrainDesc.rows = textureDesc.height;
	terrainDesc.heightScale = 0.06f;
	terrainDesc.step = 2.0f;
	terrainDesc.tiles_x = 4;
	terrainDesc.tiles_z = 128;
	terrainDesc.skirtSize = 10.0f;
	terrainDesc.texScaleX = 4.0f;
	terrainDesc.texScaleY = 8.0f;

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
	terrainDesc.LODs.push_back(make_pair(1, 100.0f));
	terrainDesc.LODs.push_back(make_pair(2, 200.0f));
	terrainDesc.LODs.push_back(make_pair(4, 800.0f));
	terrainDesc.LODs.push_back(make_pair(8, 1600.0f));
	terrainDesc.LODs.push_back(make_pair(16, 1e9));

	m_pTerrain = sm->createTerrain(terrainDesc);
	m_pTerrain->setPosition(-144, 0.0f, 0.0f);
	Material* matTerrain = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"terrain");
	Surface* pSurface = NULL;
	for(uint i = 0; i < 128; ++i)
	{
		for(uint j = 0; j < 4; ++j)
		{
			pSurface = m_pTerrain->getSurface(i * 4 + j);
			if(j == 0 || j == 1 || j == 3)
				pSurface->setEnabled(false);
			pSurface->setMaterial(matTerrain);
		}
	}

	sm->getRootNode()->addChild(L"Landscape", m_pTerrain);
	NGENE_DELETE_ARRAY(pHeights);

	pActor = pWorld->createTerrain(m_pTerrain->getTerrainDesc());
	pActor->attachNode(m_pTerrain);
	pActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(L"Terrain", pActor);


	// Create listeners
	m_pInputListener = new MyInputListener();
	m_pInputListener->setApp(this);
	InputSystem::getSingleton().addInputListener(m_pInputListener);

	m_pUpdateListener = new MyUpdateListener();
	m_pUpdateListener->setApp(this);
	m_pUpdateListener->setScrollSpeed(0.1f);
	Engine::getSingleton().addUpdateListener(m_pUpdateListener);

	// Set collision flags
	pWorld->setCollisionFlags(GROUND_PLANE, PhysicsActor::GROUP_CHARACTER_CONTROLLER, true);
	pWorld->setCollisionFlags(ITEM, PhysicsActor::GROUP_CHARACTER_CONTROLLER, true);
	pWorld->setCollisionFlags(ENEMY, PhysicsActor::GROUP_CHARACTER_CONTROLLER, true);
	pWorld->setCollisionFlags(DEBRIS, PhysicsActor::GROUP_CHARACTER_CONTROLLER, false);
	pWorld->setCollisionFlags(PROJECTILE, PhysicsActor::GROUP_CHARACTER_CONTROLLER, false);
	pWorld->setCollisionFlags(ENEMY_PROJECTILE, PhysicsActor::GROUP_CHARACTER_CONTROLLER, true);

	pWorld->setCollisionFlags(GROUND_PLANE, PROJECTILE, false);
	pWorld->setCollisionFlags(DEBRIS, PROJECTILE, false);
	pWorld->setCollisionFlags(ENEMY, PROJECTILE, true);

	pWorld->setCollisionFlags(GROUND_PLANE, ENEMY_PROJECTILE, false);
	pWorld->setCollisionFlags(DEBRIS, ENEMY_PROJECTILE, false);
	pWorld->setCollisionFlags(ENEMY, ENEMY_PROJECTILE, false);

	pWorld->setCollisionFlags(GROUND_PLANE, ENEMY, false);
	pWorld->setCollisionFlags(DEBRIS, ENEMY, false);

	pWorld->setCollisionFlags(DEBRIS, GROUND_PLANE, false);
	pWorld->setCollisionFlags(ITEM, GROUND_PLANE, false);
	pWorld->setCollisionFlags(ITEM, DEBRIS, false);
	pWorld->setCollisionFlags(ITEM, ENEMY, false);
	pWorld->setCollisionFlags(ENEMY, ENEMY, false);
	pWorld->setCollisionFlags(ITEM, PROJECTILE, false);
	pWorld->setCollisionFlags(ITEM, ENEMY_PROJECTILE, false);
	pWorld->setCollisionFlags(PROJECTILE, PROJECTILE, false);
	pWorld->setCollisionFlags(ENEMY_PROJECTILE, ENEMY_PROJECTILE, false);
	pWorld->setCollisionFlags(PROJECTILE, ENEMY_PROJECTILE, false);
	pWorld->setCollisionFlags(DEBRIS, DEBRIS, false);

	for(uint i = 0; i < 16; i++)
	{
		for(uint j = 0; j < 16; j++)
		{
			pWorld->setGroupsFlags(i, j, CT_START);
		}
	}

	m_pTriggerListener = new MyCollisionListener();
	m_pTriggerListener->setApp(this);
	pWorld->addCollisionListener(m_pTriggerListener);


	// Create enemies
	Matrix4x4 matTrans;
	matTrans.scale(0.005f);
	m_pEnemy = sm->createMesh <MeshLoadPolicy3dsFile>(L"blackbird.3ds", matTrans);
	for(uint i = 0; i < m_pEnemy->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pEnemy->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"enemyShip"));
	}
	m_pEnemy->setRotation(0.0f, -Maths::PI * 0.5f, 0.0f);


	m_pGhostShip = sm->createMesh <MeshLoadPolicy3dsFile>(L"ghostship.3ds", matTrans);
	for(uint i = 0; i < m_pGhostShip->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pGhostShip->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"ghostShip"));
	}
	m_pGhostShip->setRotation(-Maths::PI * 0.5f, -Maths::PI * 0.5f, 0.0f);


	m_pDebris = sm->createMesh <MeshLoadPolicy3dsFile>(L"blackbird.3ds", matTrans);
	for(uint i = 0; i < m_pDebris->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pDebris->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"enemyShip"));
	}


	matTrans.scale(1.0f);
	m_pMirkhaShip = sm->createMesh <MeshLoadPolicy3dsFile>(L"mirkha.3ds", matTrans);
	for(uint i = 0; i < m_pMirkhaShip->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pMirkhaShip->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"mirkhaShip"));
	}
	m_pMirkhaShip->setRotation(-Maths::PI * 0.5f, -Maths::PI * 0.5f, 0.0f);

	matTrans.scale(1.0f);
	m_pKanaraShip = sm->createMesh <MeshLoadPolicy3dsFile>(L"kanara.3ds", matTrans);
	for(uint i = 0; i < m_pKanaraShip->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pKanaraShip->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"kanaraShip"));
	}
	m_pKanaraShip->setRotation(-Maths::PI, 0.0f, 0.0f);


	matTrans.identity();
	Quaternion quatRotY(Vector3::UNIT_Z, Maths::PI_HALF);
	matTrans.scale(0.025f);
	matTrans *= quatRotY.toMatrix();
	m_pGun = sm->createMesh <MeshLoadPolicy3dsFile>(L"hammerhead.3ds", matTrans);
	for(uint i = 0; i < m_pGun->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pGun->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"enemyShip"));
	}
	m_pGun->setRotation(0.0f, -Maths::PI * 0.5f, 0.0f);


	matTrans.identity();
	matTrans.scale(0.3f);
	m_pPlant = sm->createMesh <MeshLoadPolicyXFile>(L"palm_tree.x", matTrans, true);
	for(uint i = 0; i < m_pPlant->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pPlant->getSurface(i);
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
	//m_pPlant->setScale(0.2f);

	matTrans.identity();
	m_pPlant2 = sm->createMesh <MeshLoadPolicy3dsFile>(L"fern.3ds", matTrans, true);
	for(uint i = 0; i < m_pPlant2->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pPlant2->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
		{
			Material* matPalm = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"fern");
			pSurface->setMaterial(matPalm);
			pSurface->flipWindingOrder();
		}
	}
	m_pPlant2->setRotation(Vector3::UNIT_X, -Maths::PI_HALF);
	m_pPlant2->setScale(2.2f);


	m_nCombo = 0;
	m_nComboPause = 2000;

	// Add post-processing effects
	PostStage* postStage = ((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"));
	MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");

	Material* pMaterial = pMatLib->getMaterial(L"glow");
	m_PostGlow.priority = 100;
	m_PostGlow.material = pMaterial;
	m_PostGlow.material->setEnabled(true);
	postStage->addToRender(&m_PostGlow);

	pMaterial = pMatLib->getMaterial(L"selectiveColouring");
	m_PostSelective.priority = 100;
	m_PostSelective.material = pMaterial;
	m_PostSelective.material->setEnabled(true);
	postStage->addToRender(&m_PostSelective);

	pMaterial = pMatLib->getMaterial(L"radialBlur");
	m_PostRadialBlur.priority = 100;
	m_PostRadialBlur.material = pMaterial;
	m_PostRadialBlur.material->setEnabled(false);
	postStage->addToRender(&m_PostRadialBlur);

	/*pMaterial = pMatLib->getMaterial(L"oldMovie");
	m_PostOldMovie.priority = 200;
	m_PostOldMovie.material = pMaterial;
	m_PostOldMovie.material->setEnabled(true);
	postStage->addToRender(&m_PostOldMovie);*/

	/*pMaterial = pMatLib->getMaterial(L"sobelFilter");
	m_PostEdgeDection1.priority = 200;
	m_PostEdgeDection1.material = pMaterial;
	m_PostEdgeDection1.material->setEnabled(true);
	postStage->addToRender(&m_PostEdgeDection1);*/

	/*pMaterial = pMatLib->getMaterial(L"waterColor");
	m_PostEdgeDection2.priority = 201;
	m_PostEdgeDection2.material = pMaterial;
	m_PostEdgeDection2.material->setEnabled(true);
	postStage->addToRender(&m_PostEdgeDection2);*/

	pMatLib = MaterialManager::getSingleton().getLibrary(L"game");
	pMaterial = pMatLib->getMaterial(L"clouds");
	m_PostClouds.priority = 100;
	m_PostClouds.material = pMaterial;
	m_PostClouds.material->setEnabled(true);
	postStage->addToRender(&m_PostClouds);


	NodeWater* m_pWater = new NodeWater();
	m_pWater->setCreator(sm);
	m_pWater->init();
	m_pWater->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"water"));
	m_pWater->setSimulated(false);
	m_pWater->setPosition(0.0f, 2.9f, 0.0f);
	m_pWater->setDayLength(60.0f);
	m_pWater->setMuddiness(0.8f);
	m_pWater->setDisplacement(1.7f);
	m_pWater->setWind(Vector2(-1.5f, 3.0f));
	m_pWater->setHorizontalVisibility(0.5f);
	m_pWater->setNormalScale(1.5f);
	m_pWater->setWavesAmplitude(0.15f);
	m_pWater->setWavesScale(Vector2(0.1f, 0.1f));
	m_pWater->setExtinction(Vector3(1.4f, 6.0f, 8.0f));
	m_pWater->setDepthColour(Colour(105, 105, 105));
	m_pWater->setFoam(Vector3(0.15f, 0.85f, 20.7f));
	m_pWater->setDayTime(time);
	sm->getRootNode()->addChild(L"Water", m_pWater);

	m_fSpeedRate = 1.0f;

	// Create sounds
	SOUND_DESC soundDesc;
	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"explode.wav";
	m_pSound = sm->createSound(L"Explosion", soundDesc);
	ISoundInstance* pInternalSound = m_pSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	pInternalSound->setVolume(2.1f);
	sm->getRootNode()->addChild(L"ExplosionSound", m_pSound);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"electricity.wav";
	m_pElectricitySound = sm->createSound(L"Electricity", soundDesc);
	pInternalSound = m_pElectricitySound->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"ElectricitySound", m_pElectricitySound);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"powerup.wav";
	m_pPowerUpSound = sm->createSound(L"PowerUp", soundDesc);
	pInternalSound = m_pPowerUpSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"PowerUpSound", m_pPowerUpSound);

	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"heart_beat.mp3";
	m_pHeartSound = sm->createSound(L"Heart", soundDesc);
	pInternalSound = m_pHeartSound->getSound();
	pInternalSound->getSound()->setLooped(true);
	sm->getRootNode()->addChild(L"HeartSound", m_pHeartSound);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"switch.wav";
	m_pWeaponSwitchSound = sm->createSound(L"Switch", soundDesc);
	pInternalSound = m_pWeaponSwitchSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"SwitchSound", m_pWeaponSwitchSound);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"hit_enemy.wav";
	m_pEnemyHit = sm->createSound(L"HitEnemy", soundDesc);
	pInternalSound = m_pEnemyHit->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"HitEnemySound", m_pEnemyHit);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"shot.wav";
	m_pShotSound = sm->createSound(L"Shot", soundDesc);
	pInternalSound = m_pShotSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"ShotSound", m_pShotSound);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"missile.wav";
	m_pMissileSound = sm->createSound(L"Missile", soundDesc);
	pInternalSound = m_pMissileSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"MissileSound", m_pMissileSound);

	// Create music
	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	wstringstream fileName;
	for(uint i = 0; i < 7; ++i)
	{
		fileName.str(L"");
		fileName << L"music/" << (i + 1) << L".mp3";

		soundDesc.fileName = fileName.str();
		NodeSound* pMusic = sm->createSound(fileName.str(), soundDesc);
		pInternalSound = pMusic->getSound();
		pInternalSound->getSound()->setLooped(false);
		pMusic->setPosition(0.0f, 0.0f, 0.0f);
		sm->getRootNode()->addChild(fileName.str(), pMusic);

		m_vMusic.push_back(pMusic);
	}

	playSoundTrack();


	ParticleEmitter smoke;
	smoke.setParticlesMaxCount(60);
	smoke.setSpawnRate(1);
	smoke.setPosition(Vector3(0.0, 0.0f, 0.0f));
	smoke.setVelocity(Vector3(0.0f, 0.0f, 0.0f));
	smoke.setVelocitySpread(Vector3(0.4f, 0.2f, 0.4f));
	smoke.setAccelerationSpread(Vector3(0.15f, 0.1f, 0.2f));
	smoke.setAcceleration(Vector3(0.0f, 0.0f, 0.0f));
	smoke.setSize(2.0f);
	smoke.setSizeSpread(0.025f);
	smoke.setGrowth(0.3f);
	smoke.setGrowthSpread(0.2f);
	smoke.setLifeTime(3000.0f);
	smoke.setLifeTimeSpread(0.0f);
	smoke.setFadeSpeed(1.0f);
	smoke.setAngularVelocity(Maths::PI / 32.0f);
	smoke.setAngularVelocitySpread(0.09f);
	smoke.setSpawnOnce(false);
	//smoke.setWarmUpTime(700);

	/*Colour target = Colour::COLOUR_WHITE;
	target.setAlpha(1);
	smoke.setColour(target);
	target = Colour::COLOUR_BLACK;
	target.setAlpha(100);
	smoke.addParticleColour(target, 0.4f);
	//target = Colour::COLOUR_BLACK;
	target = Colour::COLOUR_WHITE;
	target.setAlpha(150);
	smoke.addParticleColour(target, 0.7f);

	//target = Colour::COLOUR_WHITE;
	//target.setRGBA(100, 100, 100);
	target.setAlpha(0);
	smoke.setTargetColour(target);*/

	/*wstringstream name;/
	name << L"Explosion_" << ++s_nExplosionsCount;

	m_stName = name.str();*/

	// Create particle system
	/*NodeParticleSystem* m_pPS = sm->createParticleSystem();
	m_pPS->setPosition(14.0f, 12.0f, 10.0f);
	m_pPS->addParticleEmitter(smoke, L"smoke_emitter");
	sm->getRootNode()->addChild(L"Clouds", m_pPS);

	Material* matSmoke = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"clouds_particle");
	pSurface = m_pPS->getSurface(L"smoke_emitter");
	pSurface->setMaterial(matSmoke);*/

	m_bSloMotion = false;
	m_bDied = false;

	m_nPrevProjectileTime = Engine::getSingleton().getTimer().getMilliseconds();

	setLoading(false);
	while(!isLoading())
	{
		//cout << ".";
	}
}
//----------------------------------------------------------------------
void App::playSoundTrack()
{
	if(m_pActiveMusic)
		m_pActiveMusic->getSound()->stop();

	uint track = rand() % m_vMusic.size();
	m_vMusic[track]->getSound()->play();

	m_pActiveMusic = m_vMusic[track];
}
//----------------------------------------------------------------------
NodeVisible* App::getDecorator()
{
	uint prob = rand() % 10;
	if(prob <= 5)
		return m_pPlant;
	else
		return m_pPlant2;
}
//----------------------------------------------------------------------
void App::generateExplosion(Vector3& _position, Character* _character)
{
	SceneManager* sm = Engine::getSingleton().getSceneManager(0);

	Explosion* pExplosion = new Explosion();
	pExplosion->init(sm, _position, _character);
	m_Explosions[pExplosion->getName()] = pExplosion;


	MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");
	Renderer& renderer = Renderer::getSingleton();
	Matrix4x4 matWorld;
	matWorld.translate(_position);
	Matrix4x4 matProj = Engine::getSingleton().getActiveCamera()->getFrustum().getProjectionMatrix();
	Matrix4x4 matView = Engine::getSingleton().getActiveCamera()->getViewMatrix();

	Vector4 pos(0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4x4 matViewProj = matWorld * matView * matProj;
	pos *= matViewProj;

	Vector2 centre((pos.x / pos.w + 1.0f) * 0.5f, 1.0f - (pos.y / pos.w + 1.0f) * 0.5f);
	float fRadius = 0.25f;

	pMatLib->getMaterial(L"radialBlur")->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getShader()->getConstant("centre")->setDefaultValue(centre.getData());
	//pMatLib->getMaterial(L"radialBlur")->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getShader()->getConstant("radius")->setDefaultValue(&fRadius);

	++m_nExplosionsCount;
}
//----------------------------------------------------------------------
CPUCharacter* App::getEnemy(const wstring& _name)
{
	return m_Enemies[_name];
}
//----------------------------------------------------------------------
PickupItem* App::getItem(const wstring& _name)
{
	return m_PickupItems[_name];
}
//----------------------------------------------------------------------
PrefabBox* App::createBox(SceneManager* _manager, const wstring& _name,
						  Vector3& _size, const Vector3& _position, PhysicsWorld* _world,
						  PhysicsMaterial* _material)
{
	PrefabBox* pBox = _manager->createBox(_size);
	pBox->setPosition(_position);
	_manager->getRootNode()->addChild(_name, pBox);

	PhysicsActor* pActor = _world->createPhysicsActor(L"Box", _size, PhysicsActor::BT_STATIC);
	pActor->attachNode((NodeVisible*)pBox);
	pActor->setShapeMaterial(0, *_material);
	pActor->setMass(1.0f);

	_world->addPhysicsActor(_name, pActor);

	return pBox;
}
//----------------------------------------------------------------------
CameraThirdPerson* App::getCamera() const
{
	return m_pCamera;
}
//----------------------------------------------------------------------
PlayerCharacter& App::getPlayer()
{
	return m_Player;
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
	MaterialManager::getSingleton().createLibraryFromFile(L"aiad.lib", L"game");
}
//----------------------------------------------------------------------
void App::cleanEnemies(bool _force)
{
	HashTable <wstring, CPUCharacter*>::iterator iter;
	for(iter = m_Enemies.begin(); iter != m_Enemies.end(); ++iter)
	{
		CPUCharacter* pCPU = iter->second;
		if(pCPU && !pCPU->isDisposed())
		{
			if(pCPU->getPosition().z < m_fScrollStage - 2.0f)
			{
				pCPU->dispose();
				//cout << "Enemy out of bounds" << endl;
			}
		}
		if(pCPU && (pCPU->isDisposed() || _force))
		{
			pCPU->cleanup();

			AIManager::getSingleton().removeActor(pCPU);
			NGENE_DELETE(pCPU);
			pCPU = NULL;

			m_Enemies[iter->first] = NULL;

			--m_nEnemiesCount;
		}
	}
}
//----------------------------------------------------------------------
void App::cleanDecorators(bool _force)
{
	HashTable <wstring, Decorator*>::iterator iter;
	for(iter = m_Decorators.begin(); iter != m_Decorators.end(); ++iter)
	{
		Decorator* pDecorator = iter->second;
		if(pDecorator && !pDecorator->isDisposed())
		{
			if(pDecorator->getPosition().z < m_fScrollStage - 3.0f)
			{
				pDecorator->dispose();
				//cout << "Decorator out of bounds" << endl;
			}
		}
		if(pDecorator && (pDecorator->isDisposed() || _force))
		{
			//cout << "Decorator removed" << endl;
			NGENE_DELETE(pDecorator);
			pDecorator = NULL;

			m_Decorators[iter->first] = NULL;

			--m_nDecoratorsCount;
		}
	}
}
//----------------------------------------------------------------------
void App::cleanPickupItems(bool _force)
{
	HashTable <wstring, PickupItem*>::iterator iter;
	for(iter = m_PickupItems.begin(); iter != m_PickupItems.end(); ++iter)
	{
		PickupItem* pItem = iter->second;
		if(pItem && !pItem->isDisposed())
		{
			if(pItem->getPosition().z < m_fScrollStage)
			{
				pItem->dispose();
				//cout << "Item out of bounds" << endl;
			}
		}
		if(pItem && (pItem->isDisposed()|| _force))
		{
			NGENE_DELETE(pItem);
			pItem = NULL;

			m_PickupItems[iter->first] = NULL;
		}
	}
}
//----------------------------------------------------------------------
void App::cleanProjectiles(bool _force)
{
	HashTable <wstring, Projectile*>::iterator iter;
	for(iter = m_Projectiles.begin(); iter != m_Projectiles.end(); ++iter)
	{
		Projectile* pProjectile = iter->second;
		if(pProjectile && !pProjectile->isDisposed())
		{
			if(pProjectile->getPosition().z < m_fScrollStage ||
			   pProjectile->getPosition().z > m_fScrollStage + 10.0f)
			{
				pProjectile->dispose();
				//cout << "Projectile out of bounds" << endl;
			}
		}
		if(pProjectile && (pProjectile->isDisposed() || _force))
		{
			NGENE_DELETE(pProjectile);
			pProjectile = NULL;

			m_Projectiles[iter->first] = NULL;
		}
	}
}
//----------------------------------------------------------------------
void App::cleanExplosions(bool _force)
{
	HashTable <wstring, Explosion*>::iterator iter;
	for(iter = m_Explosions.begin(); iter != m_Explosions.end(); ++iter)
	{
		Explosion* pExplosion = iter->second;
		if(pExplosion && (pExplosion->isDisposed() || _force))
		{
			//cout << "Explosion removed" << endl;

			NGENE_DELETE(pExplosion);
			pExplosion = NULL;

			m_Explosions[iter->first] = NULL;

			--m_nExplosionsCount;
		}
	}
}
//----------------------------------------------------------------------
void App::generateItem(Vector3& _position)
{
	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	PhysicsWorld* pWorld = Physics::getSingleton().getWorld(0);
	PhysicsMaterial* physMat = pWorld->getPhysicsMaterial(L"DefaultMaterial");

	PickupItem* pItem = NULL;
	uint prob = rand() % 12;
	wstring stType;

	//prob = 10;
	switch(prob)
	{
	case 0:
		pItem = new PickupItem();
		pItem->setItem(&m_AgilityItem);
		stType = L"Agility";
		break;
	case 1:
		pItem = new HealItem();
		stType = L"MedPack";
		break;
	case 2:
		pItem = new PointsItem();
		stType = L"500 Points";
		break;
	case 3:
		pItem = new PickupItem();
		pItem->setItem(&m_ShieldItem);
		stType = L"Shield";
		break;
	case 4:
		pItem = new PickupItem();
		pItem->setItem(&m_SloMoItem);
		stType = L"Slo-Motion";
		break;
	case 5:
		pItem = new PickupItem();
		pItem->setWeapon(m_TwoShotsWeapon);
		stType = L"Double Rifle";
		break;
	case 6:
		pItem = new PickupItem();
		pItem->setWeapon(m_ThreeShotsWeapon);
		stType = L"Triple Rifle";
		break;
	case 7:
		pItem = new PickupItem();
		pItem->setWeapon(m_LightningWeapon);
		stType = L"Lightning";
		break;
	case 8:
		pItem = new PickupItem();
		pItem->setWeapon(m_TwoLightningWeapon);
		stType = L"Double Lightning";
		break;
	case 9:
		pItem = new PickupItem();
		pItem->setWeapon(m_BombWeapon);
		stType = L"Bomb";
		break;
	case 10:
		pItem = new PickupItem();
		pItem->setWeapon(m_MissileWeapon);
		stType = L"Homing Missile";
		break;
	case 11:
		pItem = new MoreHealthItem();
		stType = L"+10 Max Health";
		break;
	}
	
	pItem->init(sm, pWorld, physMat, _position, stType);
	m_PickupItems[pItem->getName()] = pItem;
}
//----------------------------------------------------------------------
void App::generateEnemies()
{
	uint maxEnemies = Maths::lerp(4, 22, (m_fScrollStage + 5.0) / 2000.0f);
	maxEnemies += rand() % FastFloat::floatToInt(0.3f * maxEnemies);
	while(m_nEnemiesCount < maxEnemies && m_fScrollStage > 1.0f)
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		PhysicsWorld* pWorld = Physics::getSingleton().getWorld(0);
		PhysicsMaterial* physMat = pWorld->getPhysicsMaterial(L"DefaultMaterial");

		bool bMovable = true;
		if(rand() % 10 < 2)
			bMovable = false;

		NodeVisible* pMesh = NULL;
		MOVEMENT_TYPE movementType = MT_BASIC;
		Vector3 vecSize(0.5f, 0.5f, 0.5f);
		Vector3 vecOffset;
		float speedMult = 1.0f;
		if(bMovable)
		{
			uint movement = rand() % 10;
			if(movement <= 7)
				movementType = MT_BASIC;
			else
			{
				uint movement = rand() % 13;
				if(movement < 5)
					movementType = MT_PATH_SIN;
				else if(movement < 10)
					movementType = MT_PATH_COS;
				else
					movementType = MT_PATH_CHAIN;
			}

			uint prob = rand() % 40;
			if(prob <= 10)
			{
				pMesh = m_pEnemy;
				vecSize.set(0.6f, 1.0f, 0.5f);
				speedMult = 1.5f;
			}
			else if(prob <= 20)
			{
				pMesh = m_pGhostShip;
				vecSize.set(3.0f, 1.0f, 0.5f);
				vecOffset.set(1.0f, 0.0f, 0.0f);
				speedMult = 1.2f;
			}
			else if(prob <= 30)
			{
				pMesh = m_pMirkhaShip;
				vecSize.set(1.2f, 1.6f, 1.0f);
				vecOffset.set(-0.6f, 0.0f, 0.0f);
				speedMult = 0.9f;
			}
			else
			{
				pMesh = m_pKanaraShip;
				vecSize.set(1.8f, 0.6f, 1.8f);
				vecOffset.set(0.0f, 0.0f, -0.6f);
				speedMult = 0.7f;
			}
		}

		float fAmplitude = 2.0f * (rand() % 3 + 1);
		float fFreq = (rand() % 3 + 1);
		if(movementType != MT_PATH_CHAIN)
		{
			wstringstream buf;
			buf << L"Enemy_" << CPUCharacter::s_nEnemiesCount;

			// Create new CPU character
			CPUCharacter* pEnemy = AIManager::getSingleton().createActor <CPUCharacter>(buf.str());
			uint prob = rand() % 10;
			if(prob <= 6)
				pEnemy->setActiveWeapon(m_BasicWeapon);
			else if(prob <= 8)
				pEnemy->setActiveWeapon(m_TwoShotsWeapon);
			else
				pEnemy->setActiveWeapon(m_ThreeShotsWeapon);

			Vector3 vecPos(14.0f + Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * 6.0f,
						   0.0f,
							m_fScrollStage + 14.0f + Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * 4.0f + 4.0f);

			pEnemy->init(sm, pWorld, physMat, bMovable,
				vecPos,
				vecSize, pMesh, vecOffset, movementType, fAmplitude, fFreq,
				speedMult);
			m_Enemies[pEnemy->getName()] = pEnemy;

			++m_nEnemiesCount;
		}
		else
		{
			pMesh = m_pEnemy;
			vecSize.set(0.6f, 1.0f, 0.5f);
			vecOffset.set(0.0f, 0.0f, 0.0f);

			Vector3 vecPos(14.0f + Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * 6.0f,
				0.0f,
				m_fScrollStage + 14.0f + Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * 4.0f + 4.0f);

			// Create chain enemy
			for(uint i = 0; i < 6; ++i)
			{
				wstringstream buf;
				buf << L"Enemy_" << CPUCharacter::s_nEnemiesCount;

				// Create new CPU character
				CPUCharacter* pEnemy = AIManager::getSingleton().createActor <CPUCharacter>(buf.str());
				if(!i)
				{
					uint prob = rand() % 10;
					if(prob <= 6)
						pEnemy->setActiveWeapon(m_BasicWeapon);
					else if(prob <= 8)
						pEnemy->setActiveWeapon(m_TwoShotsWeapon);
					else
						pEnemy->setActiveWeapon(m_ThreeShotsWeapon);
				}

				pEnemy->init(sm, pWorld, physMat, bMovable,
					vecPos,
					vecSize, pMesh, vecOffset, movementType,
					fAmplitude, fFreq,
					1.0f);
				m_Enemies[pEnemy->getName()] = pEnemy;

				vecPos.z += 1.1f;

				++m_nEnemiesCount;
			}
		}
	}
}
//----------------------------------------------------------------------
void App::generateDecorators()
{
	uint maxDecorators = 30;
	maxDecorators += rand() % 11;
	while(m_nDecoratorsCount < maxDecorators && m_fScrollStage > 1.0f)
	{
		wstringstream buf;
		buf << L"Decorator_" << Decorator::s_nDecoratorsCount;

		// Create new decorator
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);

		Decorator* pDecorator = new Decorator();
		
		Vector3 vecPos(14.0f + Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * 10.0f,
					   0.0f,
					   m_fScrollStage + 12.0f + Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * 7.0f + 7.0f);

		pDecorator->init(sm, vecPos);
		m_Decorators[pDecorator->getName()] = pDecorator;
		
		++m_nDecoratorsCount;
	}
}
//----------------------------------------------------------------------
void App::setScrollStage(float _stage)
{
	m_fScrollStage = _stage;
}
//----------------------------------------------------------------------
float App::getScrollStage() const
{
	return m_fScrollStage;
}
//----------------------------------------------------------------------
void App::addProjectile(wstring& _name, Projectile* _projectile)
{
	m_Projectiles[_name] = _projectile;
}
//----------------------------------------------------------------------
Projectile* App::getProjectile(const wstring& _name)
{
	return m_Projectiles[_name];
}
//----------------------------------------------------------------------
void App::calculateCombo()
{
	if(!m_nCombo++)
	{
		m_nPrevComboTime = Engine::getSingleton().getTimer().getMilliseconds();
		return;
	}

	uint now = Engine::getSingleton().getTimer().getMilliseconds();

	if(now - m_nPrevComboTime <= m_nComboPause)
	{
		if(m_nCombo > 1)
		{
			wstringstream msg;
			msg << L"Combo x" << m_nCombo;
			m_Window.setMessage(msg.str());
		}

		m_nPrevComboTime = now;
	}
	else
		m_nCombo = 0;
}
//----------------------------------------------------------------------
CPUCharacter* App::getNearestEnemy()
{
	HashTable <wstring, CPUCharacter*>::iterator iter;
	float fMin = 1e9;
	CPUCharacter* pEnemy = NULL;

	for(iter = m_Enemies.begin(); iter != m_Enemies.end(); ++iter)
	{
		CPUCharacter* pCPU = iter->second;
		if(pCPU && !pCPU->isDisposed())
		{
			Vector3 dir = pCPU->getPosition() - m_Player.getPosition();
			dir.y = 0.0f;
			float length = dir.length();
			if(length < fMin && pCPU->getPosition().z >= m_fScrollStage)
			{
				fMin = length;
				pEnemy = pCPU;
			}
		}
	}

	return pEnemy;
}
//----------------------------------------------------------------------
void App::getNearestEnemies(CPUCharacter** _enemy1, CPUCharacter** _enemy2)
{
	HashTable <wstring, CPUCharacter*>::iterator iter;
	float fMin1 = 1e9;
	float fMin2 = 1e9;

	*_enemy1 = NULL;
	*_enemy2 = NULL;

	for(iter = m_Enemies.begin(); iter != m_Enemies.end(); ++iter)
	{
		CPUCharacter* pCPU = iter->second;
		if(pCPU && !pCPU->isDisposed())
		{
			Vector3 dir = pCPU->getPosition() - m_Player.getPosition();
			dir.y = 0.0f;
			float length = dir.length();
			if(length < fMin1 && pCPU->getPosition().z >= m_fScrollStage)
			{
				if(_enemy2)
				{
					fMin2 = fMin1;
					*_enemy2 = *_enemy1;
				}

				fMin1 = length;
				*_enemy1 = pCPU;
			}
			else if(length < fMin2 && pCPU->getPosition().z >= m_fScrollStage)
			{
				fMin2 = length;
				*_enemy2 = pCPU;
			}
		}
	}
}
//----------------------------------------------------------------------
bool App::hasEnemy(const wstring& _name)
{
	if(m_Enemies.find(_name) != m_Enemies.end() && m_Enemies[_name] != NULL)
		return true;
	
	return false;
}
//----------------------------------------------------------------------
void App::updateWeapons()
{
	m_LightningWeapon->update();
	m_TwoLightningWeapon->update();
	m_MissileWeapon->update();

	uint now = Engine::getSingleton().getTimer().getMilliseconds();
	uint diff = now - m_nPrevProjectileTime;

	HashTable <wstring, Projectile*>::iterator iter;
	for(iter = m_Projectiles.begin(); iter != m_Projectiles.end(); ++iter)
	{
		Projectile* pProjectile = iter->second;
		if(pProjectile && !pProjectile->isDisposed())
			pProjectile->update(0.001f * (float)diff * App::getSingleton().getSpeedRate());
	}

	m_nPrevProjectileTime = now;
}
//----------------------------------------------------------------------
void App::updateItems()
{
	m_Player.update();
}
//----------------------------------------------------------------------
void App::updateExplosions()
{
	HashTable <wstring, Explosion*>::iterator iter;
	for(iter = m_Explosions.begin(); iter != m_Explosions.end(); ++iter)
	{
		Explosion* pExplosion = iter->second;
		if(pExplosion && !pExplosion->isDisposed())
		{
			pExplosion->update();
		}
	}
}
//----------------------------------------------------------------------
void App::setSloMo(bool _value)
{
	if(_value)
	{
		m_fSpeedRate = 0.1f;

		m_pMissileSound->getSound()->setFrequency(25000);
		m_pSound->getSound()->setFrequency(25000);
		m_pElectricitySound->getSound()->setFrequency(25000);
		m_pShotSound->getSound()->setFrequency(25000);
		m_pMissileSound->getSound()->setFrequency(25000);
		m_pPowerUpSound->getSound()->setFrequency(25000);
		m_pEnemyHit->getSound()->setFrequency(25000);
		m_pActiveMusic->getSound()->setFrequency(25000);
		m_pHeartSound->getSound()->setFrequency(25000);
	}
	else
	{
		m_fSpeedRate = 1.0f;

		m_pMissileSound->getSound()->setFrequency(44100);
		m_pSound->getSound()->setFrequency(44100);
		m_pElectricitySound->getSound()->setFrequency(44100);
		m_pShotSound->getSound()->setFrequency(44100);
		m_pMissileSound->getSound()->setFrequency(44100);
		m_pPowerUpSound->getSound()->setFrequency(44100);
		m_pEnemyHit->getSound()->setFrequency(44100);
		m_pActiveMusic->getSound()->setFrequency(44100);
		m_pHeartSound->getSound()->setFrequency(44100);
	}

	m_bSloMotion = _value;

	Physics::getSingleton().getWorld(0)->setSlowMotionRate(m_fSpeedRate);
}
//----------------------------------------------------------------------
NodeVisible* App::getEnemy()
{
	uint prob = rand() % 40;
	if(prob <= 10)
		return m_pEnemy;
	else if(prob <= 20)
		return m_pGhostShip;
	else if(prob <= 30)
		return m_pMirkhaShip;
	else
		return m_pKanaraShip;
}
//----------------------------------------------------------------------
App::App():
	FrameworkWin32(false, true),
	m_MsgBox(PWB_YES_NO, L"You died! Would you like to start again?", L"You died!")
{
}
//----------------------------------------------------------------------
void App::onLoadingRender()
{
	GUIStage* pStage = ((GUIStage*)Renderer::getSingleton().getRenderStage(L"GUI"));

	pStage->onBeginFrame();
	pStage->render();
}
//----------------------------------------------------------------------
void LoadingThread::run()
{
	Thread::setName("LoadingThread");

	while(m_pApp->isLoading())
	{
		InputSystem::getSingleton().processData();
		Renderer::getSingleton().render();
	}

	m_pApp->setLoading(true);
	Renderer::getSingleton().onRender = NULL;


	m_pApp->m_LoadingScreen.hide();
	m_pApp->m_Window.show();
	//GUIManager::getSingleton().removeWindow(&m_pApp->m_LoadingScreen);
	//GUIManager::getSingleton().addWindow(&m_pApp->m_Window, true);


	/*m_pApp->m_pWeaponSwitchSound = NULL;
	m_pApp->m_nExplosionsCount = 0;

	QUAD_TREE_DESC quadTreeDesc;
	quadTreeDesc.bounds.left = -64.0f;
	quadTreeDesc.bounds.right = 64.0f;
	quadTreeDesc.bounds.top = 4096.0f;
	quadTreeDesc.bounds.bottom = 0;
	m_pApp->m_pPartitioner = new ScenePartitionerQuadTree();
	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	sm->setScenePartitioner(m_pApp->m_pPartitioner);

	CameraThirdPerson* cam;
	cam = (CameraThirdPerson*)sm->createCamera(L"CameraThirdPerson", L"Camera");
	cam->setVelocity(2.0f);
	cam->setPosition(Vector3(0.0f, 0.0f, 0.0f));
	cam->setTarget(Vector3(0.0f, 0.0f, 0.0f));

	Quaternion quatRot(Vector3(1.0f, 0.0f, 0.0f), Maths::PI_HALF);
	cam->setRotation(quatRot);
	m_pApp->m_pCamera = cam;

	Physics::getSingleton().setEnabled(false);
	ISoundManager::getSingleton().setEnabled(false);

	PhysicsWorld* pWorld = Physics::getSingleton().createWorld(L"PhysicsWorld");
	pWorld->setGravity(Vector3(0.0f, -1.8f, 0.0f));
	pWorld->setSkinWidth(0.01f);

	PhysicsActor* pActor = NULL;

	// Create physics materials
	PhysicsMaterial* physMat = pWorld->createPhysicsMaterial(L"DefaultMaterial");
	physMat->setRestitution(0.5f);
	physMat->setDynamicFriction(0.5f);
	physMat->setStaticFriction(0.5f);

	physMat = pWorld->createPhysicsMaterial(L"Debris");
	physMat->setRestitution(0.0f);
	physMat->setDynamicFriction(30.9f);
	physMat->setStaticFriction(30.9f);

	// Create player character
	m_pApp->m_Player.init(sm, pWorld);

	// Create weapons
	m_pApp->m_BasicWeapon = new BasicWeapon();
	m_pApp->m_TwoShotsWeapon = new TwoShotsWeapon();
	m_pApp->m_ThreeShotsWeapon = new ThreeShotsWeapon();
	m_pApp->m_LightningWeapon = new LightningWeapon();
	m_pApp->m_TwoLightningWeapon = new TwoLightningWeapon();
	m_pApp->m_BombWeapon = new BombWeapon();
	m_pApp->m_MissileWeapon = new MissileWeapon();

	m_pApp->m_BasicWeapon->init(sm, pWorld, physMat);
	m_pApp->m_TwoShotsWeapon->init(sm, pWorld, physMat);
	m_pApp->m_ThreeShotsWeapon->init(sm, pWorld, physMat);
	m_pApp->m_LightningWeapon->init(sm, pWorld, physMat);
	m_pApp->m_TwoLightningWeapon->init(sm, pWorld, physMat);
	m_pApp->m_BombWeapon->init(sm, pWorld, physMat);
	m_pApp->m_MissileWeapon->init(sm, pWorld, physMat);

	m_pApp->m_Player.addWeapon(m_pApp->m_BasicWeapon);
	m_pApp->m_Player.setActiveWeapon(m_pApp->m_BasicWeapon);


	Vector3 target = m_pApp->m_Player.getVisual()->getPositionLocal() +
		Vector3(0.0f, 3.5f, 3.0f);
	cam->setTarget(target);
	cam->setDistanceFromTarget(6.0f);
	sm->getRootNode()->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);

	// Create directional light (temporary)
	Renderer& renderer = Renderer::getSingleton();

	NodeLight* pLight = sm->createLight(LT_DIRECTIONAL);
	pLight->setPosition(10.0f, 15.0f, 8.0f);
	// @todo check why z has to be non-zero?
	Vector3 vecDir(0.0f, -1.0f, 0.01f);
	vecDir.normalize();
	pLight->setDirection(vecDir);
	pLight->setDrawDebug(false);
	pLight->setCastShadows(true);
	Colour clrWorld = Colour::COLOUR_WHITE;
	pLight->setColour(clrWorld);
	//pLight->setRadius(1800.0f);
	pLight->setIntensity(0.001f);
	pLight->setRadius(40.0f);
	pLight->setShadowResolution(1024);
	pLight->setShadowDensity(0.35f);
	sm->getRootNode()->addChild(L"ShadowLight", pLight);
	renderer.addLight(pLight);


	/// @todo check why it looks that bad...
	//((LightStage*)Renderer::getSingleton().getRenderStage(L"Light"))->toggleHDR();

	/*pLight = sm->createLight(LT_DIRECTIONAL);
	pLight->setPosition(10.0f, 15.0f, 8.0f);
	// @todo check why z has to be non-zero?
	vecDir.set(0.0f, -1.0f, 1.0f);
	vecDir.normalize();
	pLight->setDirection(vecDir);
	pLight->setDrawDebug(false);
	pLight->setCastShadows(false);
	pLight->setColour(clrWorld);
	pLight->setRadius(1800.0f);
	sm->getRootNode()->addChild(L"WorldLight", pLight);
	renderer.addLight(pLight);*/

	/*NodeSky* m_pSky = new NodeSky(3500.0f);
	m_pSky->setCreator(sm);
	m_pSky->init();
	m_pSky->setSimulated(false);
	m_pSky->setCastShadows(false);
	m_pSky->setPosition(0.0f, -13.0f, 0.0f);
	m_pSky->setDayLength(60.0f);
	TimeDate time;
	time.hours = 10;
	m_pSky->setDayTime(time);
	sm->getRootNode()->addChild(L"Sky", m_pSky);

	sm->getNode(L"SkyDome")->setEnabled(false);

	((ShadowStage*)Renderer::getSingleton().getRenderStage(L"Shadows"))->setRange(15.0f);
	((ShadowStage*)Renderer::getSingleton().getRenderStage(L"Shadows"))->setShadowOffset(Vector3(0.0f, 0.0f, 10.0f));

	// Create terrain
	ITexture* pTexture = TextureManager::getSingleton().createTextureFromFile(L"Terrain", L"canyon.jpg", TT_2D, TU_NORMAL, TF_DEFAULT);

	TEXTURE_DESC textureDesc = pTexture->getTextureDesc();
	TERRAIN_DESC terrainDesc;

	terrainDesc.columns = textureDesc.width;
	terrainDesc.rows = textureDesc.height;
	terrainDesc.heightScale = 0.06f;
	terrainDesc.step = 2.0f;
	terrainDesc.tiles_x = 4;
	terrainDesc.tiles_z = 128;
	terrainDesc.skirtSize = 10.0f;
	terrainDesc.texScaleX = 4.0f;
	terrainDesc.texScaleY = 8.0f;

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
	terrainDesc.LODs.push_back(make_pair(1, 100.0f));
	terrainDesc.LODs.push_back(make_pair(2, 200.0f));
	terrainDesc.LODs.push_back(make_pair(4, 800.0f));
	terrainDesc.LODs.push_back(make_pair(8, 1600.0f));
	terrainDesc.LODs.push_back(make_pair(16, 1e9));

	m_pApp->m_pTerrain = sm->createTerrain(terrainDesc);
	m_pApp->m_pTerrain->setPosition(-144, 0.0f, 0.0f);
	Material* matTerrain = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"terrain");
	Surface* pSurface = NULL;
	for(uint i = 0; i < 128; ++i)
	{
		for(uint j = 0; j < 4; ++j)
		{
			pSurface = m_pApp->m_pTerrain->getSurface(i * 4 + j);
			if(j == 0 || j == 1 || j == 3)
				pSurface->setEnabled(false);
			pSurface->setMaterial(matTerrain);
		}
	}

	sm->getRootNode()->addChild(L"Landscape", m_pApp->m_pTerrain);
	NGENE_DELETE_ARRAY(pHeights);

	pActor = pWorld->createTerrain(m_pApp->m_pTerrain->getTerrainDesc());
	pActor->attachNode(m_pApp->m_pTerrain);
	pActor->setShapeMaterial(0, *physMat);
	pWorld->addPhysicsActor(L"Terrain", pActor);

	// Set collision flags
	pWorld->setCollisionFlags(GROUND_PLANE, PhysicsActor::GROUP_CHARACTER_CONTROLLER, true);
	pWorld->setCollisionFlags(ITEM, PhysicsActor::GROUP_CHARACTER_CONTROLLER, true);
	pWorld->setCollisionFlags(ENEMY, PhysicsActor::GROUP_CHARACTER_CONTROLLER, true);
	pWorld->setCollisionFlags(DEBRIS, PhysicsActor::GROUP_CHARACTER_CONTROLLER, false);
	pWorld->setCollisionFlags(PROJECTILE, PhysicsActor::GROUP_CHARACTER_CONTROLLER, false);
	pWorld->setCollisionFlags(ENEMY_PROJECTILE, PhysicsActor::GROUP_CHARACTER_CONTROLLER, true);

	pWorld->setCollisionFlags(GROUND_PLANE, PROJECTILE, false);
	pWorld->setCollisionFlags(DEBRIS, PROJECTILE, false);
	pWorld->setCollisionFlags(ENEMY, PROJECTILE, true);

	pWorld->setCollisionFlags(GROUND_PLANE, ENEMY_PROJECTILE, false);
	pWorld->setCollisionFlags(DEBRIS, ENEMY_PROJECTILE, false);
	pWorld->setCollisionFlags(ENEMY, ENEMY_PROJECTILE, false);

	pWorld->setCollisionFlags(GROUND_PLANE, ENEMY, false);
	pWorld->setCollisionFlags(DEBRIS, ENEMY, false);

	pWorld->setCollisionFlags(DEBRIS, GROUND_PLANE, false);
	pWorld->setCollisionFlags(ITEM, GROUND_PLANE, false);
	pWorld->setCollisionFlags(ITEM, DEBRIS, false);
	pWorld->setCollisionFlags(ITEM, ENEMY, false);
	pWorld->setCollisionFlags(ENEMY, ENEMY, false);
	pWorld->setCollisionFlags(ITEM, PROJECTILE, false);
	pWorld->setCollisionFlags(ITEM, ENEMY_PROJECTILE, false);
	pWorld->setCollisionFlags(PROJECTILE, PROJECTILE, false);
	pWorld->setCollisionFlags(ENEMY_PROJECTILE, ENEMY_PROJECTILE, false);
	pWorld->setCollisionFlags(PROJECTILE, ENEMY_PROJECTILE, false);
	pWorld->setCollisionFlags(DEBRIS, DEBRIS, false);

	for(uint i = 0; i < 16; i++)
	{
		for(uint j = 0; j < 16; j++)
		{
			pWorld->setGroupsFlags(i, j, CT_START);
		}
	}


	m_pApp->m_Window.addControls();
	GUIManager::getSingleton().addWindow(&m_pApp->m_Window, true);

	// Create enemies
	Matrix4x4 matTrans;
	matTrans.scale(0.005f);
	m_pApp->m_pEnemy = sm->createMesh <MeshLoadPolicy3dsFile>(L"blackbird.3ds", matTrans);
	for(uint i = 0; i < m_pApp->m_pEnemy->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pApp->m_pEnemy->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"enemyShip"));
	}
	m_pApp->m_pEnemy->setRotation(0.0f, -Maths::PI * 0.5f, 0.0f);


	m_pApp->m_pGhostShip = sm->createMesh <MeshLoadPolicy3dsFile>(L"ghostship.3ds", matTrans);
	for(uint i = 0; i < m_pApp->m_pGhostShip->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pApp->m_pGhostShip->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"ghostShip"));
	}
	m_pApp->m_pGhostShip->setRotation(-Maths::PI * 0.5f, -Maths::PI * 0.5f, 0.0f);


	m_pApp->m_pDebris = sm->createMesh <MeshLoadPolicy3dsFile>(L"blackbird.3ds", matTrans);
	for(uint i = 0; i < m_pApp->m_pDebris->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pApp->m_pDebris->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"enemyShip"));
	}


	matTrans.scale(1.0f);
	m_pApp->m_pMirkhaShip = sm->createMesh <MeshLoadPolicy3dsFile>(L"mirkha.3ds", matTrans);
	for(uint i = 0; i < m_pApp->m_pMirkhaShip->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pApp->m_pMirkhaShip->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"mirkhaShip"));
	}
	m_pApp->m_pMirkhaShip->setRotation(-Maths::PI * 0.5f, -Maths::PI * 0.5f, 0.0f);

	matTrans.scale(1.0f);
	m_pApp->m_pKanaraShip = sm->createMesh <MeshLoadPolicy3dsFile>(L"kanara.3ds", matTrans);
	for(uint i = 0; i < m_pApp->m_pKanaraShip->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pApp->m_pKanaraShip->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"kanaraShip"));
	}
	m_pApp->m_pKanaraShip->setRotation(-Maths::PI, 0.0f, 0.0f);


	matTrans.identity();
	Quaternion quatRotY(Vector3::UNIT_Z, Maths::PI_HALF);
	matTrans.scale(0.025f);
	matTrans *= quatRotY.toMatrix();
	m_pApp->m_pGun = sm->createMesh <MeshLoadPolicy3dsFile>(L"hammerhead.3ds", matTrans);
	for(uint i = 0; i < m_pApp->m_pGun->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pApp->m_pGun->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"enemyShip"));
	}
	m_pApp->m_pGun->setRotation(0.0f, -Maths::PI * 0.5f, 0.0f);


	matTrans.identity();
	matTrans.scale(0.3f);
	m_pApp->m_pPlant = sm->createMesh <MeshLoadPolicyXFile>(L"palm_tree.x", matTrans, true);
	for(uint i = 0; i < m_pApp->m_pPlant->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pApp->m_pPlant->getSurface(i);
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
	//m_pPlant->setScale(0.2f);

	matTrans.identity();
	m_pApp->m_pPlant2 = sm->createMesh <MeshLoadPolicy3dsFile>(L"fern.3ds", matTrans, true);
	for(uint i = 0; i < m_pApp->m_pPlant2->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pApp->m_pPlant2->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
		{
			Material* matPalm = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"fern");
			pSurface->setMaterial(matPalm);
			pSurface->flipWindingOrder();
		}
	}
	m_pApp->m_pPlant2->setRotation(Vector3::UNIT_X, -Maths::PI_HALF);
	m_pApp->m_pPlant2->setScale(2.2f);


	m_pApp->m_nCombo = 0;
	m_pApp->m_nComboPause = 2000;

	// Add post-processing effects
	PostStage* postStage = ((PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"));
	MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");

	Material* pMaterial = pMatLib->getMaterial(L"glow");
	m_pApp->m_PostGlow.priority = 100;
	m_pApp->m_PostGlow.material = pMaterial;
	m_pApp->m_PostGlow.material->setEnabled(true);
	postStage->addToRender(&m_pApp->m_PostGlow);

	pMaterial = pMatLib->getMaterial(L"selectiveColouring");
	m_pApp->m_PostSelective.priority = 100;
	m_pApp->m_PostSelective.material = pMaterial;
	m_pApp->m_PostSelective.material->setEnabled(true);
	postStage->addToRender(&m_pApp->m_PostSelective);

	pMaterial = pMatLib->getMaterial(L"radialBlur");
	m_pApp->m_PostRadialBlur.priority = 100;
	m_pApp->m_PostRadialBlur.material = pMaterial;
	m_pApp->m_PostRadialBlur.material->setEnabled(false);
	postStage->addToRender(&m_pApp->m_PostRadialBlur);

	pMatLib = MaterialManager::getSingleton().getLibrary(L"game");
	pMaterial = pMatLib->getMaterial(L"clouds");
	m_pApp->m_PostClouds.priority = 100;
	m_pApp->m_PostClouds.material = pMaterial;
	m_pApp->m_PostClouds.material->setEnabled(true);
	postStage->addToRender(&m_pApp->m_PostClouds);


	NodeWater* m_pWater = new NodeWater();
	m_pWater->setCreator(sm);
	m_pWater->init();
	m_pWater->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"water"));
	m_pWater->setSimulated(false);
	m_pWater->setPosition(0.0f, 2.9f, 0.0f);
	m_pWater->setDayLength(60.0f);
	m_pWater->setMuddiness(0.8f);
	m_pWater->setDisplacement(1.7f);
	m_pWater->setWind(Vector2(-1.0f, 2.0f));
	m_pWater->setHorizontalVisibility(0.5f);
	m_pWater->setNormalScale(1.5f);
	m_pWater->setWavesAmplitude(0.15f);
	m_pWater->setWavesScale(Vector2(0.1f, 0.1f));
	m_pWater->setExtinction(Vector3(1.4f, 6.0f, 8.0f));
	m_pWater->setDepthColour(Colour(105, 105, 105));
	m_pWater->setFoam(Vector3(0.15f, 0.85f, 20.7f));
	m_pWater->setDayTime(time);
	sm->getRootNode()->addChild(L"Water", m_pWater);

	m_pApp->m_fSpeedRate = 1.0f;

	// Create sounds
	SOUND_DESC soundDesc;
	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"explode.wav";
	m_pApp->m_pSound = sm->createSound(L"Explosion", soundDesc);
	ISoundInstance* pInternalSound = m_pApp->m_pSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	pInternalSound->setVolume(2.1f);
	sm->getRootNode()->addChild(L"ExplosionSound", m_pApp->m_pSound);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"electricity.wav";
	m_pApp->m_pElectricitySound = sm->createSound(L"Electricity", soundDesc);
	pInternalSound = m_pApp->m_pElectricitySound->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"ElectricitySound", m_pApp->m_pElectricitySound);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"powerup.wav";
	m_pApp->m_pPowerUpSound = sm->createSound(L"PowerUp", soundDesc);
	pInternalSound = m_pApp->m_pPowerUpSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"PowerUpSound", m_pApp->m_pPowerUpSound);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"switch.wav";
	m_pApp->m_pWeaponSwitchSound = sm->createSound(L"Switch", soundDesc);
	pInternalSound = m_pApp->m_pWeaponSwitchSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"SwitchSound", m_pApp->m_pWeaponSwitchSound);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"hit_enemy.wav";
	m_pApp->m_pEnemyHit = sm->createSound(L"HitEnemy", soundDesc);
	pInternalSound = m_pApp->m_pEnemyHit->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"HitEnemySound", m_pApp->m_pEnemyHit);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"shot.wav";
	m_pApp->m_pShotSound = sm->createSound(L"Shot", soundDesc);
	pInternalSound = m_pApp->m_pShotSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"ShotSound", m_pApp->m_pShotSound);


	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"missile.wav";
	m_pApp->m_pMissileSound = sm->createSound(L"Missile", soundDesc);
	pInternalSound = m_pApp->m_pMissileSound->getSound();
	pInternalSound->getSound()->setLooped(false);
	sm->getRootNode()->addChild(L"MissileSound", m_pApp->m_pMissileSound);

	// Create music
	soundDesc.isUsingHardware = false;
	soundDesc.is3D = false;
	soundDesc.fileName = L"music/8.mp3";
	m_pApp->m_pMusic = sm->createSound(L"Music", soundDesc);
	pInternalSound = m_pApp->m_pMusic->getSound();
	pInternalSound->getSound()->setLooped(true);
	m_pApp->m_pMusic->setPosition(0.0f, 0.0f, 0.0f);
	sm->getRootNode()->addChild(L"Music", m_pApp->m_pMusic);
	pInternalSound->play();


	ParticleEmitter smoke;
	smoke.setParticlesMaxCount(60);
	smoke.setSpawnRate(1);
	smoke.setPosition(Vector3(0.0, 0.0f, 0.0f));
	smoke.setVelocity(Vector3(0.0f, 0.0f, 0.0f));
	smoke.setVelocitySpread(Vector3(0.4f, 0.2f, 0.4f));
	smoke.setAccelerationSpread(Vector3(0.15f, 0.1f, 0.2f));
	smoke.setAcceleration(Vector3(0.0f, 0.0f, 0.0f));
	smoke.setSize(2.0f);
	smoke.setSizeSpread(0.025f);
	smoke.setGrowth(0.3f);
	smoke.setGrowthSpread(0.2f);
	smoke.setLifeTime(3000.0f);
	smoke.setLifeTimeSpread(0.0f);
	smoke.setFadeSpeed(1.0f);
	smoke.setAngularVelocity(Maths::PI / 32.0f);
	smoke.setAngularVelocitySpread(0.09f);
	smoke.setSpawnOnce(false);
	//smoke.setWarmUpTime(700);

	/*Colour target = Colour::COLOUR_WHITE;
	target.setAlpha(1);
	smoke.setColour(target);
	target = Colour::COLOUR_BLACK;
	target.setAlpha(100);
	smoke.addParticleColour(target, 0.4f);
	//target = Colour::COLOUR_BLACK;
	target = Colour::COLOUR_WHITE;
	target.setAlpha(150);
	smoke.addParticleColour(target, 0.7f);

	//target = Colour::COLOUR_WHITE;
	//target.setRGBA(100, 100, 100);
	target.setAlpha(0);
	smoke.setTargetColour(target);*/

	/*wstringstream name;/
	name << L"Explosion_" << ++s_nExplosionsCount;

	m_stName = name.str();*/

	// Create particle system
	/*NodeParticleSystem* m_pPS = sm->createParticleSystem();
	m_pPS->setPosition(14.0f, 12.0f, 10.0f);
	m_pPS->addParticleEmitter(smoke, L"smoke_emitter");
	sm->getRootNode()->addChild(L"Clouds", m_pPS);

	Material* matSmoke = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"clouds_particle");
	pSurface = m_pPS->getSurface(L"smoke_emitter");
	pSurface->setMaterial(matSmoke);*/

	/*m_pApp->m_bSloMotion = false;
	m_pApp->m_bDied = false;

	m_pApp->m_nPrevProjectileTime = Engine::getSingleton().getTimer().getMilliseconds();


	// Create listeners
	m_pApp->m_pInputListener = new MyInputListener();
	m_pApp->m_pInputListener->setApp(m_pApp);
	InputSystem::getSingleton().addInputListener(m_pApp->m_pInputListener);

	m_pApp->m_pUpdateListener = new MyUpdateListener();
	m_pApp->m_pUpdateListener->setApp(m_pApp);
	m_pApp->m_pUpdateListener->setScrollSpeed(0.1f);
	Engine::getSingleton().addUpdateListener(m_pApp->m_pUpdateListener);

	m_pApp->m_pTriggerListener = new MyCollisionListener();
	m_pApp->m_pTriggerListener->setApp(m_pApp);
	pWorld->addCollisionListener(m_pApp->m_pTriggerListener);


	Physics::getSingleton().setEnabled(true);
	ISoundManager::getSingleton().setEnabled(true);*/
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