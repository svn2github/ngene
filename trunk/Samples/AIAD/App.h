/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once

#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"

#include "MyInputListener.h"
#include "MyUpdateListener.h"
#include "MyCollisionListener.h"
#include "MyWindow.h"
#include "MainMenu.h"
#include "LoadingScreen.h"
#include "HiScoresWindow.h"
#include "OptionsWindow.h"

#include "PlayerCharacter.h"
#include "PrefabPlane.h"
#include "GUIPromptWindow.h"
#include "GUICursor.h"
#include "Thread.h"

 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;
using nGENE::CharacterController;
using nGENE::GUIPromptWindow;
using nGENE::GUICursor;
using nGENE::Thread;


class LoadingThread;

 
class App: public FrameworkWin32, public Singleton <App>
{
private:
	CriticalSection m_CriticalSection;

	ScenePartitionerQuadTree* m_pPartitioner;

	MyInputListener* m_pInputListener;
	MyUpdateListener* m_pUpdateListener;
	MyCollisionListener* m_pTriggerListener;
	
	MainMenu m_MainMenuWindow;
	HiScoresWindow m_HiScoresWindow;
	LoadingScreen m_LoadingScreen;
	MyWindow m_Window;
	OptionsWindow m_OptionsWindow;
	GUIPromptWindow m_MsgBox;

	CameraThirdPerson* m_pCamera;
	GUIFont* m_pMenuFont;
	GUIFont* m_pGameFont;
	GUICursor m_Cursor;

	LoadingThread* m_pLoadingThread;

	// Weapons
	BasicWeapon* m_BasicWeapon;
	TwoShotsWeapon* m_TwoShotsWeapon;
	ThreeShotsWeapon* m_ThreeShotsWeapon;
	LightningWeapon* m_LightningWeapon;
	TwoLightningWeapon* m_TwoLightningWeapon;
	BombWeapon* m_BombWeapon;
	MissileWeapon* m_MissileWeapon;

	vector <SHiScoreEntry> m_vHiScores;

	PlayerCharacter m_Player;
	HashTable <wstring, CPUCharacter*> m_Enemies;
	HashTable <wstring, PickupItem*> m_PickupItems;
	HashTable <wstring, Projectile*> m_Projectiles;
	HashTable <wstring, Explosion*> m_Explosions;
	HashTable <wstring, Decorator*> m_Decorators;

	CPUActorFactory m_ActorFactory;
	AIManager m_AIManager;

	NodeTerrain* m_pTerrain;

	float m_fScrollStage;

	uint m_nEnemiesCount;
	uint m_nDecoratorsCount;
	uint m_nExplosionsCount;

	uint m_nCombo;
	uint m_nComboPause;
	uint m_nPrevComboTime;

	uint m_nPrevProjectileTime;

	NodeVisible* m_pEnemy;
	NodeVisible* m_pGhostShip;
	NodeVisible* m_pMirkhaShip;
	NodeVisible* m_pKanaraShip;

	NodeVisible* m_pDebris;
	NodeVisible* m_pGun;

	NodeVisible* m_pPlant;
	NodeVisible* m_pPlant2;

	NodeSound* m_pMenuMusic;
	NodeSound* m_pMenuSound;
	NodeSound* m_pKeySound;
	NodeSound* m_pSound;
	NodeSound* m_pElectricitySound;
	NodeSound* m_pShotSound;
	NodeSound* m_pMissileSound;
	NodeSound* m_pPowerUpSound;
	NodeSound* m_pEnemyHit;
	vector<NodeSound*> m_vMusic;
	NodeSound* m_pWeaponSwitchSound;
	NodeSound* m_pHeartSound;

	NodeSound* m_pActiveMusic;

	PrefabPlane* m_pClouds;

	SPriorityMaterial m_PostGlow;
	SPriorityMaterial m_PostSelective;
	SPriorityMaterial m_PostClouds;
	SPriorityMaterial m_PostRadialBlur;
	SPriorityMaterial m_PostOldMovie;
	SPriorityMaterial m_PostEdgeDection1;
	SPriorityMaterial m_PostEdgeDection2;

	ShieldUsableItem m_ShieldItem;
	AgilityUsableItem m_AgilityItem;
	SlowMotionUsableItem m_SloMoItem;

	bool m_bSloMotion;
	bool m_bDied;
	bool m_bLoading;
	bool m_bStarted;

	float m_fSpeedRate;

	wstring m_stPlayerName;

public:
	App();
	~App() {}

	void msgBoxResult(PROMPT_WINDOW_RESULT_BUTTON _result);

	void createMenu();
	void createApplication();

	void loadHiScores();
	void loadResources();
	void createMap();
	void closeMap();

	void playerDied();

	void updateWeapons();
	void updateItems();
	void updateExplosions();

	void generateDecorators();
	void generateEnemies();
	void generateItem(Vector3& _position);
	void generateExplosion(Vector3& _position, Character* _character);

	void cleanDecorators(bool _force=false);
	void cleanEnemies(bool _force=false);
	void cleanPickupItems(bool _force=false);
	void cleanProjectiles(bool _force=false);
	void cleanExplosions(bool _force=false);

	PrefabBox* createBox(SceneManager* _manager, const wstring& _name,
		Vector3& _size, const Vector3& _position, PhysicsWorld* _world,
		PhysicsMaterial* _material);

	CameraThirdPerson* getCamera() const;
	PlayerCharacter& getPlayer();

	CPUCharacter* getEnemy(const wstring& _name);
	PickupItem* getItem(const wstring& _name);
	Projectile* getProjectile(const wstring& _name);

	void setScrollStage(float _stage);
	float getScrollStage() const;

	void addProjectile(wstring& _name, Projectile* _projectile);

	NodeVisible* getEnemy();
	NodeVisible* getDebris() {return m_pDebris;}
	NodeVisible* getGun() {return m_pGun;}

	NodeVisible* getDecorator();
	NodeTerrain* getTerrain() {return m_pTerrain;}

	NodeSound* getMenuSound() {return m_pMenuSound;}
	NodeSound* getKeySound() {return m_pKeySound;}
	NodeSound* getSound() {return m_pSound;}
	NodeSound* getElectricitySound() {return m_pElectricitySound;}
	NodeSound* getPowerUpSound() {return m_pPowerUpSound;}
	NodeSound* getEnemyHitSound() {return m_pEnemyHit;}
	NodeSound* getShotSound() {return m_pShotSound;}
	NodeSound* getMissileSound() {return m_pMissileSound;}
	NodeSound* getSwitchSound() {return m_pWeaponSwitchSound;}
	NodeSound* getHeartBeatSound() {return m_pHeartSound;}
	NodeSound* getMusic() {return m_pActiveMusic;}

	void playSoundTrack();
	
	MyWindow* getHUD() {return &m_Window;}
	MainMenu* getMainMenu() {return &m_MainMenuWindow;}
	HiScoresWindow* getHiScores() {return &m_HiScoresWindow;}
	OptionsWindow* getOptions() {return &m_OptionsWindow;}

	CPUCharacter* getNearestEnemy();
	void getNearestEnemies(CPUCharacter** _enemy1, CPUCharacter** _enemy2);
	bool hasEnemy(const wstring& _name);

	NodeVisible* getCloudsPlane() {return m_pClouds;}

	float getSpeedRate() const {return m_fSpeedRate;}

	void setSloMo(bool _value);
	bool getSloMo() const {return m_bSloMotion;}

	uint getCombo() const {return m_nCombo;}
	void calculateCombo();

	uint getExplosionsNum() {return m_nExplosionsCount;}

	bool isPlayerDead() const {return m_bDied;}

	void setPlayerName(const wstring& _name) {m_stPlayerName = _name;}

	void onLoadingRender();

	void setLoading(bool _value)
	{
		SharedObject sharedObj(&m_CriticalSection);

		m_bLoading = _value;
	}

	bool isLoading()
	{
		SharedObject sharedObj(&m_CriticalSection);

		return m_bLoading;
	}

	void showMsgBox();


	friend class LoadingThread;
};


class LoadingThread: public Thread
{
protected:
	App* m_pApp;

	void run();

public:
	LoadingThread(App* _app)
	{
		m_pApp = _app;
	}
};