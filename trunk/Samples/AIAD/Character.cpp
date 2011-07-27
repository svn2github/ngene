/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Character.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "Character.h"
#include "PlayerCharacter.h"

using nGENE::byte;

#include "nGENE.h"
#include "App.h"


// Initialize static members
uint CPUCharacter::s_nEnemiesCount = 0;
uint Projectile::s_nProjectilesCount = 0;
uint PickupItem::s_nItemsCount = 0;
uint Explosion::s_nExplosionsCount = 0;
uint Decorator::s_nDecoratorsCount = 0;
uint DebrisPart::s_nDebrisCount = 0;

TypeInfo CPUCharacter::Type(L"CPUActor", &AIActor::Type);


Character::Character():
	m_pVisual(NULL),
	m_pActiveWeapon(NULL),
	m_bMovable(true),
	m_bShielded(false)
{
}
//----------------------------------------------------------------------
Character::~Character()
{
}
//----------------------------------------------------------------------
void Character::move(const Vector2& _dir)
{
	if(!m_pVisual)
		return;

	m_pVisual->translate(_dir.x, 0.0f, _dir.y);
}
//----------------------------------------------------------------------
void Character::addItem(const Item& _item)
{
	m_Equipment.push_back(_item);
}
//----------------------------------------------------------------------
Projectile::~Projectile()
{
	PhysicsWorld* pWorld = m_pPhysics->getPhysicsWorld();
	pWorld->removePhysicsActor(m_pPhysics);

	SceneManager* pSM = m_pVisual->getCreator();

	Node* pParent = m_pVisual->getParent();
	pParent->removeChild(m_pVisual);
	m_pVisual = NULL;
}
//----------------------------------------------------------------------
Vector3& Decorator::getPosition()
{
	return m_pVisual->getPositionLocal();
}
//----------------------------------------------------------------------
Decorator::~Decorator()
{
	Node* pParent = m_pVisual->getParent();
	pParent->removeChild(m_pVisual);
	m_pVisual = NULL;
}
//----------------------------------------------------------------------
void Decorator::init(SceneManager* _manager, Vector3& _position)
{
	float y = -1.0f;

	m_pVisual = new NodeVisible();
	m_pVisual->setCreator(_manager);
	m_pVisual->init();
	
	INTERSECTION_DESC intersectDesc;
	Point origin;
	Ray ray;
	Vector3 direction(0.0f, -2000.0f, 0.0f);
	NodeTerrain* pTerrain = App::getSingleton().getTerrain();
	TERRAIN_DESC& terrainDesc = pTerrain->getTerrainDesc();
	Matrix4x4 matTerrain = pTerrain->getTransform();

	float step = terrainDesc.step;
	float halfStep = step * 0.5f;
	float width = float(terrainDesc.columns) * terrainDesc.step * 0.5f;
	float height = float(terrainDesc.rows) * terrainDesc.step * 0.5f;
	int halfCols = terrainDesc.columns >> 1;
	int halfRows = terrainDesc.rows >> 1;

	float xDiff = width + pTerrain->getPositionLocal().x;
	float zDiff = height + pTerrain->getPositionLocal().z;

	int pointPerTileX = terrainDesc.columns / terrainDesc.tiles_x;
	int pointPerTileZ = terrainDesc.rows / terrainDesc.tiles_z;


	uint iter = 0;
	while(y < 3.3f && iter < 5)
	{
		float x = _position.x + 16.0f;
		float z = _position.z - 4096.0f;

		int nx = (int)floor(x / step) + halfCols;
		int nz = (int)floor(z / step) + halfRows;

		origin.set(x + xDiff, 1000.0f, z + zDiff);
		ray.set(origin, direction);
		ray.moveToObjectSpace(matTerrain);

		uint index = (nz / pointPerTileZ) * terrainDesc.tiles_x + (nx / pointPerTileX);
		Surface* terrainSurf = pTerrain->getSurface(index);
		while(!ray.intersectSurface(*terrainSurf, intersectDesc));
		y = ray.getIntersectionPoint().y + matTerrain._m42 + 0.4f;

		if(y < 3.3f)
		{
			_position.set(14.0f + Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * 10.0f,
						  -12,
						  App::getSingleton().getScrollStage() + 12.0f + Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * 7.0f + 7.0f);
		}

		++iter;
	}

	if(iter == 5)
		y = -12.0f;


	m_pVisual->setPosition(_position.x, y, _position.z);
	
	NodeVisible* pDecorator = App::getSingleton().getDecorator();
	for(uint i = 0; i < App::getSingleton().getDecorator()->getSurfacesNum(); ++i)
	{
		Surface* pSurface = pDecorator->getSurface(i);
		m_pVisual->addSurface(pSurface->getName(), *pSurface);
	}
	m_pVisual->setRotation(pDecorator->getRotation());
	m_pVisual->setScale(pDecorator->getScale().x);

	wstringstream name;
	name << L"Decorator_" << s_nDecoratorsCount;

	m_stName = name.str();

	_manager->getRootNode()->addChild(name.str(), m_pVisual);

	++s_nDecoratorsCount;
}
//----------------------------------------------------------------------
void Explosion::init(SceneManager* _manager, Vector3& _position,
					 Character* _character)
{
	ParticleEmitter fire;
	fire.setParticlesMaxCount(70);
	fire.setSpawnRate(30);
	fire.setPosition(Vector3(0.0, 0.0f, 0.0f));
	fire.setVelocity(Vector3(0.0f, 0.0f, 0.0f));
	fire.setVelocitySpread(Vector3(1.5f, 1.5f, 1.5f));
	fire.setAccelerationSpread(Vector3(0.2f, 0.2f, 0.2f));
	fire.setAcceleration(Vector3(0.0f, 0.0f, 0.0f));
	fire.setSize(1.0f);
	fire.setSizeSpread(0.025f);
	fire.setGrowth(1.0f);
	fire.setGrowthSpread(0.2f);
	fire.setLifeTime(1500.0f);
	fire.setLifeTimeSpread(0.0f);
	fire.setFadeSpeed(2.5f);
	fire.setAngularVelocity(Maths::PI / 18.0f);
	fire.setAngularVelocitySpread(0.02f);
	fire.setSpawnOnce(true);

	ParticleEmitter smoke;
	smoke.setParticlesMaxCount(90);
	smoke.setSpawnRate(30);
	smoke.setPosition(Vector3(0.0, 0.0f, 0.0f));
	smoke.setVelocity(Vector3(0.0f, 0.0f, 0.0f));
	smoke.setVelocitySpread(Vector3(0.5f, 0.5f, 0.5f));
	smoke.setAccelerationSpread(Vector3(0.3f, 0.3f, 0.3f));
	smoke.setAcceleration(Vector3(0.0f, 0.5f, 0.0f));
	smoke.setSize(1.0f);
	smoke.setSizeSpread(0.025f);
	smoke.setGrowth(0.8f);
	smoke.setGrowthSpread(0.2f);
	smoke.setLifeTime(4000.0f);
	smoke.setLifeTimeSpread(0.0f);
	smoke.setFadeSpeed(2.5f);
	smoke.setAngularVelocity(Maths::PI / 4.0f);
	smoke.setAngularVelocitySpread(0.06f);
	smoke.setSpawnOnce(true);
	smoke.setWarmUpTime(700);

	Colour target = Colour::COLOUR_WHITE;
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
	smoke.setTargetColour(target);

	wstringstream name;
	name << L"Explosion_" << ++s_nExplosionsCount;

	m_stName = name.str();

	// Create particle system
	m_pPS = _manager->createParticleSystem();
	m_pPS->setPosition(_position);
	m_pPS->addParticleEmitter(fire, L"fire_emitter");
	m_pPS->addParticleEmitter(smoke, L"smoke_emitter");
	if(_character->isMovable())
	{
		// Create falling debris
		m_pDebris = new NodeVisible();
		m_pDebris->setCreator(_manager);
		m_pDebris->init();

		NodeVisible* pEnemy = App::getSingleton().getDebris();

		Material* matDebris = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"debris");
		for(uint i = 0; i < pEnemy->getSurfacesNum(); ++i)
		{
			Surface* pSurface = pEnemy->getSurface(i);
			m_pDebris->addSurface(pSurface->getName(), *pSurface);
		}
		m_pDebris->setPosition(_character->getPosition());
		m_pPS->setPosition(0.0f, 0.0f, 0.0f);
		m_pDebris->addChild(name.str(), m_pPS);

		name << L"_Debris" << endl;
		_manager->getRootNode()->addChild(name.str(), m_pDebris);

		PhysicsWorld* pWorld = Physics::getSingleton().getWorld(0);
		PhysicsMaterial* physMat = pWorld->getPhysicsMaterial(L"Debris");
		m_pActor = pWorld->createPhysicsActor(L"Box", Vector3(0.8f, 0.4f, 0.8f),
			PhysicsActor::BT_DYNAMIC, false, Vector3(0.0f, 0.0f, -1.5f));
		m_pActor->attachNode((NodeVisible*)m_pDebris);
		m_pActor->setShapeMaterial(0, *physMat);
		m_pActor->setMass(1.0f);
		m_pActor->setCollidable(DEBRIS);
		pWorld->addPhysicsActor(name.str(), m_pActor);

		/*Vector3 vecDir(rand() % 20 - 10, rand() % 20 - 8, rand() % 20 - 13);
		vecDir.normalize();
		FORCE_DESC force = {vecDir, (rand() % 5 + 1) * 70};

		force.fValue *= 0.05f;
		m_pActor->applyTorque(force);*/
	}
	else
		_manager->getRootNode()->addChild(name.str(), m_pPS);
	
	Material* matFire = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"fire");
	Surface* pSurface = m_pPS->getSurface(L"fire_emitter");
	pSurface->setMaterial(matFire);
	Material* matSmoke = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"smoke");
	pSurface = m_pPS->getSurface(L"smoke_emitter");
	pSurface->setMaterial(matSmoke);

	// Play sound
	NodeSound* pSound = App::getSingleton().getSound();
	pSound->setPosition(_position);
	pSound->getSound()->play();

	// Create light
	name << "_light" << endl;
	Renderer& renderer = Renderer::getSingleton();
	m_pLight = _manager->createLight(LT_POINT);
	m_pLight->setDrawDebug(false);
	m_pLight->setCastShadows(false);
	m_pLight->setColour(Colour::COLOUR_ORANGE);
	m_pLight->setRadius(2.0f);
	m_pLight->setIntensity(5.0f);
	if(_character->isMovable())
	{
		m_pDebris->addChild(name.str(), m_pLight);
	}
	else
		m_pPS->addChild(name.str(), m_pLight);
	renderer.addLight(m_pLight);

	m_nPrev = Engine::getSingleton().getTimer().getMilliseconds();
	m_nPassed = 0;

	m_Part1.init(_manager, _position);
	m_Part2.init(_manager, _position);
	m_Part3.init(_manager, _position);
	/*m_Part4.init(_manager, _position);
	m_Part5.init(_manager, _position);
	m_Part6.init(_manager, _position);*/

	m_bMovable = _character->isMovable();
}
//----------------------------------------------------------------------
void Explosion::update()
{
	uint now = Engine::getSingleton().getTimer().getMilliseconds();
	uint diff = now - m_nPrev;

	m_nPassed += diff;
	if(m_nPassed <= 400)
	{
		float radius = Maths::lerp(0.0f, 4.0f, (float)m_nPassed / 400.0f);
		float intensity = Maths::lerp(0.0f, 5.0f, (float)m_nPassed / 400.0f);

		if(!m_bMovable)
		{
			radius *= 0.5f;
			intensity *= 0.5f;
		}

		m_pLight->setRadius(radius);
		m_pLight->setIntensity(intensity);
	}
	else if(m_nPassed >= 800 && m_nPassed <= 6300)
	{
		float radius = Maths::lerp(4.0f, 0.0f, (float)(m_nPassed - 800) / 5500.0f);
		float intensity = Maths::lerp(5.0f, 0.1f, (float)(m_nPassed - 800) / 5500.0f);

		if(!m_bMovable)
		{
			radius *= 0.5f;
			intensity *= 0.5f;
		}

		m_pLight->setRadius(radius);
		m_pLight->setIntensity(intensity);
	}

	if(m_nPassed < 300)
	{
		if(App::getSingleton().getExplosionsNum() == 1)
		{
			MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");
			pMatLib->getMaterial(L"radialBlur")->setEnabled(true);
		}
		float blurStrength = Maths::lerp(0.0f, 1.4f, (float)m_nPassed / 300.0f);
		MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");
		pMatLib->getMaterial(L"radialBlur")->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getShader()->getConstant("blur_strength")->setDefaultValue(&blurStrength);
	}
	else if(m_nPassed > 400 && m_nPassed < 700)
	{
		float blurStrength = Maths::lerp(1.4f, 0.0f, (float)(m_nPassed - 400) / 300.0f);
		MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");
		pMatLib->getMaterial(L"radialBlur")->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getShader()->getConstant("blur_strength")->setDefaultValue(&blurStrength);
	}
	else if(m_nPassed > 700 && App::getSingleton().getExplosionsNum() == 1)
	{
		//MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");
		//pMatLib->getMaterial(L"radialBlur")->setEnabled(false);
	}

	m_nPrev = now;
}
//----------------------------------------------------------------------
Explosion::~Explosion()
{
	SceneManager* pSM = m_pPS->getCreator();

	if(m_pDebris)
	{
		Renderer::getSingleton().removeLight(m_pLight);

		PhysicsWorld* pWorld = m_pActor->getPhysicsWorld();
		pWorld->removePhysicsActor(m_pActor);

		Node* pParent = m_pDebris->getParent();
		pParent->removeChild(m_pDebris);
		m_pDebris = NULL;
	}
	else
	{
		Renderer::getSingleton().removeLight(m_pLight);

		Node* pParent = m_pPS->getParent();
		pParent->removeChild(m_pPS);
		m_pPS = NULL;
	}
}
//----------------------------------------------------------------------
DebrisPart::~DebrisPart()
{
	PhysicsWorld* pWorld = m_pActor->getPhysicsWorld();
	pWorld->removePhysicsActor(m_pActor);

	Node* pParent = m_pVisual->getParent();
	pParent->removeChild(m_pVisual);
	m_pVisual = NULL;
}
//----------------------------------------------------------------------
void DebrisPart::init(SceneManager* _manager, Vector3& _position)
{
	m_pVisual = _manager->createBox(0.25f, 0.3f, 0.1f);
	m_pVisual->setPosition(_position);
	wstringstream name;
	name << L"DebrisPart_" << s_nDebrisCount++ << endl;
	_manager->getRootNode()->addChild(name.str(), m_pVisual);

	Material* matDebris = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"enemyShip");
	Surface* pSurface = m_pVisual->getSurface(L"Base");
	pSurface->setMaterial(matDebris);

	PhysicsWorld* pWorld = Physics::getSingleton().getWorld(0);
	PhysicsMaterial* physMat = pWorld->getPhysicsMaterial(L"Debris");
	m_pActor = pWorld->createPhysicsActor(L"Box", Vector3(0.5f, 0.6f, 0.2f),
		PhysicsActor::BT_DYNAMIC, false, Vector3(0.0f, 0.0f, 0.0f));
	m_pActor->attachNode((NodeVisible*)m_pVisual);
	m_pActor->setShapeMaterial(0, *physMat);
	m_pActor->setMass(1.0f);
	m_pActor->setCollidable(DEBRIS);
	pWorld->addPhysicsActor(name.str(), m_pActor);

	Vector3 vecDir(rand() % 20 - 10, rand() % 20 - 8, rand() % 20 - 13);
	vecDir.normalize();
	FORCE_DESC force = {vecDir, (rand() % 5 + 1) * 70};
	m_pActor->applyForce(force);

	
	force.fValue *= 0.25f;
	m_pActor->applyTorque(force);
}
//----------------------------------------------------------------------
bool Explosion::isDisposed() const
{
	return m_pPS->getParticleEmitter(0)->hasFinished() &&
		   m_pPS->getParticleEmitter(1)->hasFinished();
}
//----------------------------------------------------------------------
Vector3& Projectile::getPosition()
{
	return m_pVisual->getPositionLocal();
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
CPUCharacter::~CPUCharacter()
{
}
//----------------------------------------------------------------------
void CPUCharacter::cleanup()
{
	PhysicsWorld* pWorld = m_pActor->getPhysicsWorld();
	pWorld->removePhysicsActor(m_pActor);

	SceneManager* pSM = m_pVisual->getCreator();

	Node* pParent = m_pVisual->getParent();
	pParent->removeChild(m_pVisual);
	m_pVisual = NULL;
}
//----------------------------------------------------------------------
void CPUCharacter::init(SceneManager* _manager, PhysicsWorld* _world,
						PhysicsMaterial* _material, bool _movable,
						Vector3& _position, Vector3& _size,
						NodeVisible* _enemy, Vector3& _offset,
						MOVEMENT_TYPE _movement,
						float _amplitue, float _frequency, float _speedMult)
{
	m_nCurrentHP = 20;
	m_nMaxHP = 20;
	m_bMovable = _movable;
	m_vecStartPos = _position;

	m_MovementType = _movement;

	m_fAmplitude = _amplitue;
	m_fFreq = _frequency;

	m_fVelocity *= _speedMult;


	m_pVisual = new NodeVisible();
	m_pVisual->setCreator(_manager);
	m_pVisual->init();
	if(_movable)
		m_pVisual->setPosition(_position.x, 12.0f, _position.z);
	else
	{
		INTERSECTION_DESC intersectDesc;
		Point origin;
		Ray ray;
		Vector3 direction(0.0f, -2000.0f, 0.0f);
		NodeTerrain* pTerrain = App::getSingleton().getTerrain();
		TERRAIN_DESC& terrainDesc = pTerrain->getTerrainDesc();
		Matrix4x4 matTerrain = pTerrain->getTransform();

		float step = terrainDesc.step;
		float halfStep = step * 0.5f;
		float width = float(terrainDesc.columns) * terrainDesc.step * 0.5f;
		float height = float(terrainDesc.rows) * terrainDesc.step * 0.5f;
		int halfCols = terrainDesc.columns >> 1;
		int halfRows = terrainDesc.rows >> 1;

		float xDiff = width + pTerrain->getPositionLocal().x;
		float zDiff = height + pTerrain->getPositionLocal().z;

		// @todo check why i have to use something like that? i mean this - 1024?
		float z = _position.z - 4096.0f;
		float x = _position.x + 16.0f;

		int pointPerTileX = terrainDesc.columns / terrainDesc.tiles_x;
		int pointPerTileZ = terrainDesc.rows / terrainDesc.tiles_z;
		int nx = (int)floor(x / step) + halfCols;
		int nz = (int)floor(z / step) + halfRows;

		origin.set(x + xDiff, 1000.0f, z + zDiff);
		ray.set(origin, direction);
		ray.moveToObjectSpace(matTerrain);

		uint index = (nz / pointPerTileZ) * terrainDesc.tiles_x + (nx / pointPerTileX);
		Surface* terrainSurf = pTerrain->getSurface(index);
		while(!ray.intersectSurface(*terrainSurf, intersectDesc));
		float y = ray.getIntersectionPoint().y + matTerrain._m42 + 0.75f;
		
		m_pVisual->setPosition(_position.x, y, _position.z);
	}


	NodeVisible* pEnemy = NULL;
	if(_movable)
		pEnemy = _enemy;
	else
		pEnemy = App::getSingleton().getGun();

	for(uint i = 0; i < pEnemy->getSurfacesNum(); ++i)
	{
		Surface* pSurface = pEnemy->getSurface(i);
		if(!pSurface->getMaterialPtr())
			continue;

		m_pVisual->addSurface(pSurface->getName(), *pSurface);
		
		m_pBase = &pSurface->getMaterial();
	}

	m_pVisual->setRotation(pEnemy->getRotation());


	wstringstream name;
	name << L"Enemy_" << s_nEnemiesCount;

	_manager->getRootNode()->addChild(name.str(), m_pVisual);

	Vector3 vecSize;
	if(_movable)
		vecSize.set(_size);
	else
		vecSize.set(2.0f, 1.5f, 1.5f);
	m_pActor = _world->createPhysicsActor(L"Box", vecSize,
		PhysicsActor::BT_KINEMATIC, false);
	m_pActor->attachNode((NodeVisible*)m_pVisual);
	m_pActor->setShapeMaterial(0, *_material);
	m_pActor->setMass(1.0f);
	m_pActor->setCollidable(ENEMY);
	m_pActor->getShape(0)->setLocalPos(_offset);

	_world->addPhysicsActor(name.str(), m_pActor);

	m_stName = name.str();


	/*NodePrefab* pBox = _manager->createBox(_size);
	name << L"DebrisPart_aaa" << endl;
	m_pVisual->addChild(name.str(), pBox);

	Material* matDebris = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"shield");
	Surface* pSurface = pBox->getSurface(L"Base");
	pSurface->setMaterial(matDebris);*/

	if(!_movable)
	{
		/*NodeVisible* pBox = _manager->createBox(2.0f, 1.5f, 1.5f);
		pBox->setPosition(m_pVisual->getPositionLocal());
		name << L"_BOX" << endl;
		_manager->getRootNode()->addChild(name.str(), pBox);

		Material* matDebris = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"debris");
		uint i = 0;
		Surface* pSurface = pBox->getSurface(i);
		pSurface->setMaterial(matDebris);*/
	}

	++s_nEnemiesCount;

	// Create AI
	m_StateMoving.setCharacter(this);
	m_StateDead.setCharacter(this);

	m_StateMoving.addTransition(&m_StateDead, ET_NOHITPOINTS);

	m_FSM.setActiveState(&m_StateMoving);
	m_Brain.setFSM(&m_FSM);

	setBrain(&m_Brain);

	m_fShootDelta = rand() % FastFloat::floatToInt(m_fShootFrequency * 0.3f);

	m_nPrevTime = Engine::getSingleton().getTimer().getMilliseconds();
}
//----------------------------------------------------------------------
void CPUCharacter::onHit(uint _damagePoints)
{
	if(isShielded())
		return;

	if(_damagePoints >= m_nCurrentHP)
	{
		m_nCurrentHP = 0;

		AIEvent evt;
		evt.receiver = this;
		evt.messageID = ET_NOHITPOINTS;

		AIManager::getSingleton().handleEvent(evt);
	}
	else
	{
		m_nCurrentHP -= _damagePoints;
		m_bHit = true;

		for(uint i = 0; i < m_pVisual->getSurfacesNum(); ++i)
		{
			m_pVisual->getSurface(i)->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"enemyHit"));
		}
		m_nHitPassed = 0;

		App::getSingleton().getEnemyHitSound()->getSound()->play();
	}
}
//----------------------------------------------------------------------
void CPUCharacter::setPosition(Vector3& _pos)
{
	m_pVisual->setPosition(_pos);
}
//----------------------------------------------------------------------
Vector3& Character::getPosition()
{
	return m_pVisual->getPositionLocal();
}
//----------------------------------------------------------------------
Vector3& PickupItem::getPosition()
{
	return m_pVisual->getPositionLocal();
}
//----------------------------------------------------------------------
void CPUCharacter::update()
{
	dword now = Engine::getSingleton().getTimer().getMilliseconds();
	float diff = (float)(now - m_nPrevTime) * App::getSingleton().getSpeedRate();

	AIActor::update();

	makeMovement(diff);
	if(!App::getSingleton().isPlayerDead())
		makeShot(diff);

	if(m_bHit)
	{
		m_nHitPassed += diff;
		if(m_nHitPassed >= 250)
		{
			m_bHit = false;
			for(uint i = 0; i < m_pVisual->getSurfacesNum(); ++i)
			{
				m_pVisual->getSurface(i)->setMaterial(m_pBase);
			}
		}
	}


	m_nPrevTime = now;
}
//----------------------------------------------------------------------
void CPUCharacter::makeMovement(float _delta)
{
	if(!m_bMovable)
		return;

	Vector3 pos = m_pVisual->getPositionLocal();
	if(m_MovementType == MT_BASIC)
	{
		pos.z -= m_fVelocity * (_delta * 0.001f);
	}
	else if(m_MovementType == MT_PATH_SIN)
	{
		pos.z -= m_fVelocity * 0.25f * (_delta * 0.001f);
		m_fMovementPassed += _delta;
		pos.x = m_vecStartPos.x + Maths::fsin(m_fFreq * (m_fMovementPassed * m_fVelocity * 0.25f * 0.001f + m_vecStartPos.x)) * m_fAmplitude;
	}
	else if(m_MovementType == MT_PATH_COS)
	{
		pos.z -= m_fVelocity * 0.25f * (_delta * 0.001f);
		m_fMovementPassed += _delta;
		pos.x = m_vecStartPos.x + Maths::fcos(m_fFreq * (m_fMovementPassed * m_fVelocity * 0.25f * 0.001f + m_vecStartPos.x)) * m_fAmplitude;
	}
	else if(m_MovementType == MT_PATH_CHAIN)
	{
		pos.z -= m_fVelocity * 0.25f * (_delta * 0.001f);
		m_fMovementPassed += _delta;
		pos.x = m_vecStartPos.x + Maths::fcos(m_fFreq * (m_fMovementPassed * m_fVelocity * 0.25f * 0.001f + m_vecStartPos.x + m_vecStartPos.z * 0.05f)) * m_fAmplitude;
	}

	m_pVisual->setPosition(pos);
}
//----------------------------------------------------------------------
void CPUCharacter::makeShot(float _delta)
{
	if(!m_pActiveWeapon)
		return;

	m_fShootDelta += _delta;
	if(m_fShootDelta >= m_fShootFrequency)
	{
		m_pActiveWeapon->use(m_pVisual->getPositionLocal(), true, m_bMovable);
		m_fShootDelta = 0.0f;
	}
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void StateMoving::update()
{
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void StateDead::onEnter()
{
	// Calculate combo effect
	App::getSingleton().calculateCombo();

	// Give player points
	uint nCombo = App::getSingleton().getCombo();
	if(!nCombo)
		nCombo = 1;
	PlayerCharacter::getSingleton().addPoints(10 * nCombo);

	// Generate a power-up/bonus
	uint probability = rand() % 100;
	if(probability < 30)
	{
		App::getSingleton().generateItem(m_pCharacter->getPosition());
	}

	// Generate boom :P
	App::getSingleton().generateExplosion(m_pCharacter->getPosition(), m_pCharacter);

	// And finally dispose character
	m_pCharacter->dispose();
}
//----------------------------------------------------------------------
void StateDead::update()
{
}
//----------------------------------------------------------------------
AIActor* CPUActorFactory::createAIActor(IBrain* _brain)
{
	return new CPUCharacter(_brain);
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void TwoShotsWeapon::use(Vector3& _position, bool _enemyShot, bool _movable)
{
	if(!_enemyShot)
	{
		if(!m_bInfiniteAmmo && !m_nAmmoLeft)
		{
			App::getSingleton().getSwitchSound()->getSound()->play();
			App::getSingleton().getHUD()->setNoAmmoImageEnabled(true);
			App::getSingleton().getHUD()->setMessage(L"No Ammo!", Colour::COLOUR_RED);
			return;
		}
		else if(m_nAmmoLeft)
			--m_nAmmoLeft;
	}

	Projectile* pProjectile = new Projectile();
	Vector3 vecPos = _position - Vector3(0.25f, 0.0f, 0.0f);
	pProjectile->init(m_pSM, m_pWorld, m_pMaterial, vecPos, _enemyShot, _movable);

	pProjectile = new Projectile();
	vecPos = _position + Vector3(0.25f, 0.0f, 0.0f);
	pProjectile->init(m_pSM, m_pWorld, m_pMaterial, vecPos, _enemyShot, _movable);

	if(!_enemyShot)
		App::getSingleton().getShotSound()->getSound()->play();
}
//----------------------------------------------------------------------
void ThreeShotsWeapon::use(Vector3& _position, bool _enemyShot, bool _movable)
{
	if(!_enemyShot)
	{
		if(!m_bInfiniteAmmo && !m_nAmmoLeft)
		{
			App::getSingleton().getSwitchSound()->getSound()->play();
			App::getSingleton().getHUD()->setNoAmmoImageEnabled(true);
			App::getSingleton().getHUD()->setMessage(L"No Ammo!", Colour::COLOUR_RED);
			return;
		}
		else if(m_nAmmoLeft)
			--m_nAmmoLeft;
	}

	Projectile* pProjectile = new Projectile();
	Vector3 vecPos = _position - Vector3(0.15f, 0.0f, 0.0f);
	Vector3 vecDir(-0.15f, 0.0f, 1.0f);
	pProjectile->init(m_pSM, m_pWorld, m_pMaterial, vecPos, _enemyShot, _movable, vecDir);

	pProjectile = new Projectile();
	vecPos = _position + Vector3(0.15f, 0.0f, 0.0f);
	vecDir.x = 0.15f;
	pProjectile->init(m_pSM, m_pWorld, m_pMaterial, vecPos, _enemyShot, _movable, vecDir);

	pProjectile = new Projectile();
	vecPos = _position;
	pProjectile->init(m_pSM, m_pWorld, m_pMaterial, vecPos, _enemyShot, _movable);

	if(!_enemyShot)
		App::getSingleton().getShotSound()->getSound()->play();
}
//----------------------------------------------------------------------
void BombWeapon::use(Vector3& _position, bool _enemyShot, bool _movable)
{
	if(!_enemyShot)
	{
		if(!m_bInfiniteAmmo && !m_nAmmoLeft)
		{
			App::getSingleton().getSwitchSound()->getSound()->play();
			App::getSingleton().getHUD()->setNoAmmoImageEnabled(true);
			App::getSingleton().getHUD()->setMessage(L"No Ammo!", Colour::COLOUR_RED);
			return;
		}
		else if(m_nAmmoLeft)
			--m_nAmmoLeft;
	}

	Projectile* pProjectile = new BombProjectile();
	Vector3 vecPos = _position - Vector3(0.0f, 0.1f, 0.0f);
	pProjectile->init(m_pSM, m_pWorld, m_pMaterial, vecPos, _enemyShot, _movable);
}
//----------------------------------------------------------------------
void MissileWeapon::use(Vector3& _position, bool _enemyShot, bool _movable)
{
	if(!_enemyShot)
	{
		if(!m_bInfiniteAmmo && !m_nAmmoLeft)
		{
			App::getSingleton().getSwitchSound()->getSound()->play();
			App::getSingleton().getHUD()->setNoAmmoImageEnabled(true);
			App::getSingleton().getHUD()->setMessage(L"No Ammo!", Colour::COLOUR_RED);
			return;
		}
		else if(m_nAmmoLeft)
			--m_nAmmoLeft;
	}

	Projectile* pProjectile = new MissileProjectile();
	Vector3 vecPos = _position + Vector3(0.0f, -0.1f, 0.5f);
	pProjectile->init(m_pSM, m_pWorld, m_pMaterial, vecPos, _enemyShot, _movable);

	App::getSingleton().getMissileSound()->getSound()->play();
}
//----------------------------------------------------------------------
void BasicWeapon::use(Vector3& _position, bool _enemyShot, bool _movable)
{
	if(!_enemyShot)
	{
		if(!m_bInfiniteAmmo && !m_nAmmoLeft)
		{
			App::getSingleton().getSwitchSound()->getSound()->play();
			App::getSingleton().getHUD()->setNoAmmoImageEnabled(true);
			App::getSingleton().getHUD()->setMessage(L"No Ammo!", Colour::COLOUR_RED);
			return;
		}
		else if(m_nAmmoLeft)
			--m_nAmmoLeft;
	}

	Projectile* pProjectile = new Projectile();
	pProjectile->init(m_pSM, m_pWorld, m_pMaterial, _position, _enemyShot, _movable);

	if(!_enemyShot)
		App::getSingleton().getShotSound()->getSound()->play();
}
//----------------------------------------------------------------------
void MissileProjectile::init(SceneManager* _manager, PhysicsWorld* _world,
						  PhysicsMaterial* _material, Vector3& _position, bool _enemyShot,
						  bool _movable, Vector3 _dir)
{
	CPUCharacter* pEnemy = App::getSingleton().getNearestEnemy();
	if(!pEnemy)
		return;

	m_pEnemy = pEnemy;
	m_stEnemyName = pEnemy->getName();

	m_pVisual = _manager->createCapsule(10, 10, 0.06f, 0.3f);
	m_pVisual->setPosition(_position);
	m_pVisual->setRotation(Vector3::UNIT_X, Maths::PI_HALF);

	Surface* pSurface = m_pVisual->getSurface(L"Base");
	pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"projectile"));

	wstringstream name;
	if(_enemyShot)
		name << L"E_Projectile_" << s_nProjectilesCount;
	else
		name << L"Projectile_" << s_nProjectilesCount;

	m_stName = name.str();

	_manager->getRootNode()->addChild(name.str(), m_pVisual);

	Vector3 vecDir = pEnemy->getPosition() - _position;
	vecDir.normalize();
	m_vecPrevDir = vecDir;

	m_pPhysics = _world->createPhysicsActor(L"Capsule", Vector3(0.06f, 0.3f, 0.0f),
		PhysicsActor::BT_DYNAMIC, false, vecDir * 10);
	m_pPhysics->attachNode((NodeVisible*)m_pVisual);
	m_pPhysics->setShapeMaterial(0, *_material);
	m_pPhysics->setMass(1.0f);
	if(_enemyShot)
		m_pPhysics->setCollidable(0, ENEMY_PROJECTILE);
	else
		m_pPhysics->setCollidable(0, PROJECTILE);

	_world->addPhysicsActor(name.str(), m_pPhysics);
	m_vecPrevPos = _position;


	App::getSingleton().addProjectile(m_stName, this);


	ParticleEmitter fire;
	fire.setParticlesMaxCount(25);
	fire.setSpawnRate(5);
	fire.setPosition(Vector3(0.0, 0.0f, 0.0f));
	fire.setVelocity(Vector3(0.0f, 0.0f, 0.0f));
	fire.setVelocitySpread(Vector3(0.5f, 0.5f, 0.5f));
	fire.setAccelerationSpread(Vector3(0.1f, 0.1f, 0.1f));
	fire.setAcceleration(Vector3(0.0f, -0.1f, 0.0f));
	fire.setSize(1.0f);
	fire.setSizeSpread(0.025f);
	fire.setGrowth(0.9f);
	fire.setGrowthSpread(0.15f);
	fire.setLifeTime(500.0f);
	fire.setLifeTimeSpread(0.0f);
	fire.setFadeSpeed(2.5f);
	fire.setAngularVelocity(Maths::PI / 36.0f);
	fire.setAngularVelocitySpread(0.02f);


	name << L"_fire";

	// Create particle system
	m_pPS = _manager->createParticleSystem();
	m_pPS->setPosition(0.0f, 0.0f, 0.0f);
	m_pPS->addParticleEmitter(fire, L"fire_emitter");
	m_pVisual->addChild(name.str(), m_pPS);

	Material* matFire = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"fire");
	pSurface = m_pPS->getSurface(L"fire_emitter");
	pSurface->setMaterial(matFire);

	// Create light
	name << "_light" << endl;
	Renderer& renderer = Renderer::getSingleton();
	m_pLight = _manager->createLight(LT_POINT);
	m_pLight->setDrawDebug(false);
	m_pLight->setCastShadows(false);
	m_pLight->setColour(Colour::COLOUR_ORANGE);
	m_pLight->setRadius(2.0f);
	m_pLight->setIntensity(2.0f);
	m_pVisual->addChild(name.str(), m_pLight);
	renderer.addLight(m_pLight);

	++s_nProjectilesCount;
}
//----------------------------------------------------------------------
MissileProjectile::~MissileProjectile()
{
	Renderer::getSingleton().removeLight(m_pLight);
}
//----------------------------------------------------------------------
void Projectile::update(Real _delta)
{
	if(!m_bEnemy)
		return;

	Vector3 pos = m_pVisual->getPositionLocal();
	pos += (m_vecVelocity * _delta);

	m_pVisual->setPosition(pos);
}
//----------------------------------------------------------------------
void MissileProjectile::update(Real _delta)
{
	if(m_pEnemy && App::getSingleton().hasEnemy(m_stEnemyName))
	{
		Vector3 vecDir = m_pEnemy->getPosition() - m_pVisual->getPositionLocal();
		vecDir.normalize();

		m_pPhysics->setVelocity(vecDir * 3);
		m_vecPrevDir = vecDir;

		vecDir.y = 0.0f;
		Vector3 unitz = Vector3::UNIT_Z;

		float dp = vecDir.dotProduct(unitz);
		float len = (vecDir.length() * unitz.length()) + 0.0001f;

		float angle = (acos(dp / len));

		Quaternion quatRot1(Vector3::UNIT_X, Maths::PI_HALF);
		if(vecDir.x > 0.0f)
			angle *= -1.0f;
		Quaternion quatRot2(Vector3::UNIT_Z, angle);

		m_pPhysics->setRotationWorld(quatRot1 * quatRot2);
	}
	else
	{
		m_pEnemy = App::getSingleton().getNearestEnemy();
		m_stEnemyName = m_pEnemy->getName();
	}
}
//----------------------------------------------------------------------
void LightningWeapon::use(Vector3& _position, bool _enemyShot, bool _movable)
{
	if(!_enemyShot)
	{
		if(!m_bInfiniteAmmo && !m_nAmmoLeft)
		{
			App::getSingleton().getSwitchSound()->getSound()->play();
			App::getSingleton().getHUD()->setNoAmmoImageEnabled(true);
			App::getSingleton().getHUD()->setMessage(L"No Ammo!", Colour::COLOUR_RED);
			return;
		}
		else if(m_nAmmoLeft)
			--m_nAmmoLeft;
	}

	if(m_Bolt.getNumberOfRunsLeft() && m_Bolt.isEnabled())
		return;

	m_Bolt.setStartPoint(_position);
	m_Bolt.setNumberOfRuns(10);

	CPUCharacter* pEnemy = App::getSingleton().getNearestEnemy();
	if(pEnemy)
	{
		m_Bolt.setEndPoint(pEnemy->getPosition());
		pEnemy->onHit(10);
		m_stEnemyName = pEnemy->getName();
		
		m_Bolt.setEnabled(true);

		App::getSingleton().getElectricitySound()->getSound()->play();
	}
	m_pEnemy = pEnemy;
}
//----------------------------------------------------------------------
void LightningWeapon::init(SceneManager* _manager, PhysicsWorld* _world, PhysicsMaterial* _material)
{
	m_pSM = _manager;
	m_pWorld = _world;
	m_pMaterial = _material;

	m_pEnemy = NULL;

	m_Bolt.setCreator(_manager);
	m_Bolt.init();
	m_Bolt.setMinBranchLength(0.35f);
	m_Bolt.setPosition(0.0f, 0.0f, 0.0f);
	m_Bolt.setEndPoint(Vector3(0.0f, 0.0f, 0.0f));
	m_Bolt.setBranchesNum(30);
	m_Bolt.setOffset(4.5f);
	m_Bolt.setColour(Colour(100, 100, 255));
	m_Bolt.setStartPoint(Vector3(-70.0f, 70.0f, 0.0f));
	m_Bolt.setPause(50);
	m_Bolt.setWidth(0.035f);
	m_Bolt.setBolt2Alpha(0.35f);
	m_Bolt.setSegmentsNum(6);
	m_Bolt.setUpdateFrequency(100);
	m_Bolt.setBranchSpread(0.5f);
	m_Bolt.setMaxFlash(0.0f);
	m_Bolt.setEnabled(false);

	_manager->getRootNode()->addChild(L"Bolt", m_Bolt);

	m_stType = L"lightning";

	m_bInfiniteAmmo = false;
	m_nAmmoLeft = 20;
	m_nClipSize = 20;
}
//----------------------------------------------------------------------
void LightningWeapon::update()
{
	Vector3 start = PlayerCharacter::getSingleton().getPosition();
	start += Vector3(0.0f, 0.0f, 0.5f);
	m_Bolt.setStartPoint(start);

	if(m_pEnemy && App::getSingleton().hasEnemy(m_stEnemyName))
	{
		Vector3 vecEnemyPos = m_pEnemy->getPosition();
		Vector3 vecDir = vecEnemyPos - start;
		float fLength = vecDir.length();
		float fOffset = 3.5f;
		if(fLength <= 7.0f)
			fOffset = Maths::lerp(0.5f, fOffset, fLength / 7.0f);

		m_Bolt.setEndPoint(vecEnemyPos);
		m_Bolt.setOffset(fOffset);
	}
	else
	{
		m_pEnemy = NULL;
		m_Bolt.setEnabled(false);

		if(PlayerCharacter::getSingleton().getActiveWeapon() == this)
			App::getSingleton().getElectricitySound()->getSound()->stop();
	}

	if(m_Bolt.hasFinished())
	{
		if(PlayerCharacter::getSingleton().getActiveWeapon() == this)
			App::getSingleton().getElectricitySound()->getSound()->stop();
	}
}
//----------------------------------------------------------------------
void BombProjectile::init(SceneManager* _manager, PhysicsWorld* _world,
					  PhysicsMaterial* _material, Vector3& _position, bool _enemyShot,
					  bool _movable, Vector3 _dir)
{
	m_pVisual = _manager->createCapsule(10, 10, 0.1f, 0.25f);
	m_pVisual->setPosition(_position);
	m_pVisual->setRotation(Vector3::UNIT_X, Maths::PI_HALF);

	Surface* pSurface = m_pVisual->getSurface(L"Base");
	pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"projectile"));

	wstringstream name;
	if(_enemyShot)
		name << L"E_Projectile_" << s_nProjectilesCount;
	else
		name << L"Projectile_" << s_nProjectilesCount;

	m_stName = name.str();

	_manager->getRootNode()->addChild(name.str(), m_pVisual);

	Vector3 vecVelocity(0.0f, 0.0f, 0.2f * PlayerCharacter::getSingleton().getController()->getVelocity());

	m_pPhysics = _world->createPhysicsActor(L"Capsule", Vector3(0.2f, 0.5f, 0.0f),
		PhysicsActor::BT_DYNAMIC, false, vecVelocity);
	m_pPhysics->attachNode((NodeVisible*)m_pVisual);
	m_pPhysics->setShapeMaterial(0, *_material);
	m_pPhysics->setMass(1.0f);
	if(_enemyShot)
		m_pPhysics->setCollidable(0, ENEMY_PROJECTILE);
	else
		m_pPhysics->setCollidable(0, PROJECTILE);

	_world->addPhysicsActor(name.str(), m_pPhysics);


	App::getSingleton().addProjectile(m_stName, this);

	++s_nProjectilesCount;
}
//----------------------------------------------------------------------
void Projectile::init(SceneManager* _manager, PhysicsWorld* _world,
					  PhysicsMaterial* _material, Vector3& _position, bool _enemyShot,
					  bool _movable, Vector3 _dir)
{
	m_pVisual = _manager->createSphere(10, 10, 0.05f);
	m_pVisual->setPosition(_position);

	Surface* pSurface = m_pVisual->getSurface(L"Base");
	pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"projectile"));

	wstringstream name;
	if(_enemyShot)
		name << L"E_Projectile_" << s_nProjectilesCount;
	else
		name << L"Projectile_" << s_nProjectilesCount;

	m_stName = name.str();

	_manager->getRootNode()->addChild(name.str(), m_pVisual);

	if(_enemyShot)
	{
		m_pPhysics = _world->createPhysicsActor(L"Box", Vector3(0.5f, 0.5f, 0.5f),
			PhysicsActor::BT_KINEMATIC);
	}
	else
	{
		m_pPhysics = _world->createPhysicsActor(L"Box", Vector3(0.1f, 0.1f, 0.1f),
			PhysicsActor::BT_DYNAMIC);
	}
	m_pPhysics->attachNode((NodeVisible*)m_pVisual);
	m_pPhysics->setShapeMaterial(0, *_material);
	m_pPhysics->setMass(1.0f);
	if(_enemyShot)
		m_pPhysics->setCollidable(0, ENEMY_PROJECTILE);
	else
		m_pPhysics->setCollidable(0, PROJECTILE);

	_world->addPhysicsActor(name.str(), m_pPhysics);

	_dir.normalize();
	FORCE_DESC force = {_dir, 1000.0f};
	if(_enemyShot)
	{
		if(_movable)
			force.fValue *= -1.0f;
		else
		{
			Vector3 vecDir = PlayerCharacter::getSingleton().getPosition() -
				_position;
			vecDir.normalize();
			force.vecDirection = vecDir;
		}
	}
	if(!_enemyShot)
		m_pPhysics->setVelocity(force.vecDirection * force.fValue * 0.01f);
	else
		m_vecVelocity = force.vecDirection * force.fValue * 0.005f;

	m_bEnemy = _enemyShot;

	App::getSingleton().addProjectile(m_stName, this);

	++s_nProjectilesCount;
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void PickupItem::init(SceneManager* _manager, PhysicsWorld* _world,
					  PhysicsMaterial* _material, Vector3& _position,
					  wstring& _type)
{
	m_stType = _type;

	m_pVisual = _manager->createSphere(12, 12, 0.2f);
	Vector3 vecPos = _position;
	vecPos.y = PlayerCharacter::getSingleton().getPosition().y;
	m_pVisual->setPosition(vecPos);

	Surface* pSurface = m_pVisual->getSurface(L"Base");
	pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"powerup"));

	wstringstream name;
	name << L"Item_" << s_nItemsCount;

	_manager->getRootNode()->addChild(name.str(), m_pVisual);

	m_pActor = _world->createPhysicsActor(L"Sphere", Vector3(0.2f, 0.2f, 0.2f),
		PhysicsActor::BT_KINEMATIC, false);
	m_pActor->attachNode((NodeVisible*)m_pVisual);
	m_pActor->setShapeMaterial(0, *_material);
	m_pActor->setMass(1.0f);
	m_pActor->setCollidable(ITEM);

	_world->addPhysicsActor(name.str(), m_pActor);

	m_stName = name.str();

	name << L"_light";
	m_pLight = _manager->createLight(LT_POINT);
	m_pLight->setDrawDebug(false);
	m_pLight->setCastShadows(false);
	m_pLight->setColour(Colour::COLOUR_BLUE);
	m_pLight->setRadius(3.0f);
	m_pLight->setIntensity(4.0f);
	m_pLight->setPosition(0.0f, 0.5f, 0.0f);
	m_pVisual->addChild(name.str(), m_pLight);
	Renderer::getSingleton().addLight(m_pLight);

	++s_nItemsCount;
}
//----------------------------------------------------------------------
PickupItem::~PickupItem()
{
	Renderer::getSingleton().removeLight(m_pLight);

	PhysicsWorld* pWorld = m_pActor->getPhysicsWorld();
	pWorld->removePhysicsActor(m_pActor);

	SceneManager* pSM = m_pVisual->getCreator();

	Node* pParent = m_pVisual->getParent();
	pParent->removeChild(m_pVisual);
	m_pVisual = NULL;
}
//----------------------------------------------------------------------
void PickupItem::pick()
{
	App::getSingleton().getPowerUpSound()->getSound()->play();

	if(m_pItem && !m_bWeapon)
	{
		m_pItem->onUse();
		PlayerCharacter::getSingleton().addItem(m_pItem);
	}
	else if(m_pWeapon && m_bWeapon)
	{
		PlayerCharacter::getSingleton().addWeapon(m_pWeapon);
	}

	dispose();
}
//----------------------------------------------------------------------
void HealItem::pick()
{
	PlayerCharacter::getSingleton().heal(20);

	PickupItem::pick();
}
//----------------------------------------------------------------------
void MoreHealthItem::pick()
{
	PlayerCharacter& player = PlayerCharacter::getSingleton();

	player.setMaxHP(player.getMaxHP() + 10);
	player.setCurrentHP(player.getMaxHP());

	PickupItem::pick();
}
//----------------------------------------------------------------------
void PointsItem::pick()
{
	PlayerCharacter::getSingleton().addPoints(500);

	PickupItem::pick();
}
//----------------------------------------------------------------------
void ShieldUsableItem::onUse()
{
	UsableItem::onUse();


	PlayerCharacter& player = PlayerCharacter::getSingleton();
	if(player.isShielded())
		return;

	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	if(sm->getNode(L"ShieldNode"))
	{
		Node* pVisual = sm->getNode(L"ShieldNode");
		pVisual->setEnabled(true);
	}
	else
	{
		NodeVisible* pVisual = sm->createSphere(32, 32, 0.95f);

		Surface* pSurface = pVisual->getSurface(L"Base");
		pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"shield"));

		sm->getNode(L"PlayerNode")->addChild(L"ShieldNode", pVisual);
	}

	player.setShielded(true);
}
//----------------------------------------------------------------------
void ShieldUsableItem::onEnd()
{
	UsableItem::onEnd();


	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	Node* pVisual = sm->getNode(L"ShieldNode");

	// We don't destroy as we may need it later on
	pVisual->setEnabled(false);

	PlayerCharacter& player = PlayerCharacter::getSingleton();
	player.setShielded(false);
}
//----------------------------------------------------------------------
bool ShieldUsableItem::update()
{
	if(UsableItem::update())
		return true;

	ShaderConstant* pConstant = MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"shield")->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getConstant("colour");
	float values[4];
	memcpy(values, (float*)pConstant->getValues(), 4 * sizeof(float));
	float alpha = 1.0f;
	if(m_nPassed <= 1500.0f)
	{
		alpha = Maths::lerp(0.0f, 1.0f, (float)m_nPassed / 1500.0f);
	}
	else if(m_nPassed > 8500)
	{
		alpha = Maths::lerp(1.0f, 0.0f, (float)(m_nPassed - 8500) / 1500.0f);
	}

	values[3] = alpha;
	pConstant->setValues(values);

	return false;
}
//----------------------------------------------------------------------
void AgilityUsableItem::onUse()
{
	UsableItem::onUse();


	PlayerCharacter& player = PlayerCharacter::getSingleton();
	player.setAgility(3.0f);
}
//----------------------------------------------------------------------
void AgilityUsableItem::onEnd()
{
	UsableItem::onEnd();


	PlayerCharacter& player = PlayerCharacter::getSingleton();
	player.setAgility(1.5f);
}
//----------------------------------------------------------------------
bool AgilityUsableItem::update()
{
	if(UsableItem::update())
		return true;

	return false;
}
//----------------------------------------------------------------------
void SlowMotionUsableItem::onUse()
{
	UsableItem::onUse();

	App::getSingleton().setSloMo(true);
}
//----------------------------------------------------------------------
void SlowMotionUsableItem::onEnd()
{
	UsableItem::onEnd();

	App::getSingleton().setSloMo(false);
}
//----------------------------------------------------------------------
bool SlowMotionUsableItem::update()
{
	if(UsableItem::update())
		return true;

	MaterialLibrary* pMatLib = MaterialManager::getSingleton().getLibrary(L"default");
	PlayerCharacter& player = PlayerCharacter::getSingleton();
	float fLevel = (float)player.getCurrentHP() / (float)player.getMaxHP();

	if(m_nPassed <= 1000.0f)
	{
		fLevel = Maths::lerp(1.0f - fLevel, 1.0f, (float)m_nPassed / 1000.0f);
	}
	else if(m_nPassed >= 9000)
	{
		fLevel = Maths::lerp(1.0f, 1.0f - fLevel, (float)(m_nPassed - 9000) / 1000.0f);
	}
	else
		fLevel = 1.0f;

	pMatLib->getMaterial(L"selectiveColouring")->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getShader()->getConstant("level")->setDefaultValue(&fLevel);


	return false;
}
//----------------------------------------------------------------------
void TwoLightningWeapon::use(Vector3& _position, bool _enemyShot, bool _movable)
{
	if(!_enemyShot)
	{
		if(!m_bInfiniteAmmo && !m_nAmmoLeft)
		{
			App::getSingleton().getSwitchSound()->getSound()->play();
			App::getSingleton().getHUD()->setNoAmmoImageEnabled(true);
			App::getSingleton().getHUD()->setMessage(L"No Ammo!", Colour::COLOUR_RED);
			return;
		}
		else if(m_nAmmoLeft)
			--m_nAmmoLeft;
	}

	if(m_Bolt1.getNumberOfRunsLeft() && m_Bolt1.isEnabled())
		return;

	m_Bolt1.setStartPoint(_position);
	m_Bolt1.setNumberOfRuns(10);


	if(m_Bolt2.getNumberOfRunsLeft() && m_Bolt2.isEnabled())
		return;

	m_Bolt2.setStartPoint(_position);
	m_Bolt2.setNumberOfRuns(10);


	App::getSingleton().getNearestEnemies(&m_pEnemy1, &m_pEnemy2);
	if(m_pEnemy1)
	{
		m_Bolt1.setEndPoint(m_pEnemy1->getPosition());
		m_pEnemy1->onHit(10);
		m_stEnemyName1 = m_pEnemy1->getName();

		m_Bolt1.setEnabled(true);

		App::getSingleton().getElectricitySound()->getSound()->play();
	}
	if(m_pEnemy2)
	{
		m_Bolt2.setEndPoint(m_pEnemy2->getPosition());
		m_pEnemy2->onHit(10);
		m_stEnemyName2 = m_pEnemy2->getName();

		m_Bolt2.setEnabled(true);

		App::getSingleton().getElectricitySound()->getSound()->play();
	}
}
//----------------------------------------------------------------------
void TwoLightningWeapon::init(SceneManager* _manager, PhysicsWorld* _world, PhysicsMaterial* _material)
{
	m_pSM = _manager;
	m_pWorld = _world;
	m_pMaterial = _material;

	m_pEnemy1 = NULL;
	m_pEnemy2 = NULL;

	m_Bolt1.setCreator(_manager);
	m_Bolt1.init();
	m_Bolt1.setMinBranchLength(0.35f);
	m_Bolt1.setPosition(0.0f, 0.0f, 0.0f);
	m_Bolt1.setEndPoint(Vector3(0.0f, 0.0f, 0.0f));
	m_Bolt1.setBranchesNum(30);
	m_Bolt1.setOffset(4.5f);
	m_Bolt1.setColour(Colour(255, 100, 200));
	m_Bolt1.setStartPoint(Vector3(-70.0f, 70.0f, 0.0f));
	m_Bolt1.setPause(50);
	m_Bolt1.setWidth(0.04f);
	m_Bolt1.setBolt2Alpha(0.35f);
	m_Bolt1.setSegmentsNum(6);
	m_Bolt1.setUpdateFrequency(100);
	m_Bolt1.setBranchSpread(0.5f);
	m_Bolt1.setMaxFlash(0.0f);
	m_Bolt1.setEnabled(false);

	_manager->getRootNode()->addChild(L"TwoBolt_1", m_Bolt1);


	m_Bolt2.setCreator(_manager);
	m_Bolt2.init();
	m_Bolt2.setMinBranchLength(0.35f);
	m_Bolt2.setPosition(0.0f, 0.0f, 0.0f);
	m_Bolt2.setEndPoint(Vector3(0.0f, 0.0f, 0.0f));
	m_Bolt2.setBranchesNum(30);
	m_Bolt2.setOffset(4.5f);
	m_Bolt2.setColour(Colour(255, 100, 200));
	m_Bolt2.setStartPoint(Vector3(-70.0f, 70.0f, 0.0f));
	m_Bolt2.setPause(50);
	m_Bolt2.setWidth(0.04f);
	m_Bolt2.setBolt2Alpha(0.35f);
	m_Bolt2.setSegmentsNum(6);
	m_Bolt2.setUpdateFrequency(100);
	m_Bolt2.setBranchSpread(0.5f);
	m_Bolt2.setMaxFlash(0.0f);
	m_Bolt2.setEnabled(false);

	_manager->getRootNode()->addChild(L"TwoBolt_2", m_Bolt2);

	m_stType = L"two_lightning";

	m_bInfiniteAmmo = false;
	m_nAmmoLeft = 15;
	m_nClipSize = 10;
}
//----------------------------------------------------------------------
void TwoLightningWeapon::update()
{
	Vector3 start = PlayerCharacter::getSingleton().getPosition();
	start += Vector3(0.0f, 0.0f, 0.5f);
	
	m_Bolt1.setStartPoint(start);

	if(m_pEnemy1 && App::getSingleton().hasEnemy(m_stEnemyName1))
	{
		Vector3 vecEnemyPos = m_pEnemy1->getPosition();
		Vector3 vecDir = vecEnemyPos - start;
		float fLength = vecDir.length();
		float fOffset = 3.5f;
		if(fLength <= 7.0f)
			fOffset = Maths::lerp(0.5f, fOffset, fLength / 7.0f);

		m_Bolt1.setEndPoint(vecEnemyPos);
		m_Bolt1.setOffset(fOffset);
	}
	else
	{
		m_pEnemy1 = NULL;
		m_Bolt1.setEnabled(false);

		if(PlayerCharacter::getSingleton().getActiveWeapon() == this)
			App::getSingleton().getElectricitySound()->getSound()->stop();
	}



	m_Bolt2.setStartPoint(start);

	if(m_pEnemy2 && App::getSingleton().hasEnemy(m_stEnemyName2))
	{
		Vector3 vecEnemyPos = m_pEnemy2->getPosition();
		Vector3 vecDir = vecEnemyPos - start;
		float fLength = vecDir.length();
		float fOffset = 3.5f;
		if(fLength <= 7.0f)
			fOffset = Maths::lerp(0.5f, fOffset, fLength / 7.0f);

		m_Bolt2.setEndPoint(vecEnemyPos);
		m_Bolt2.setOffset(fOffset);
	}
	else
	{
		m_pEnemy2 = NULL;
		m_Bolt2.setEnabled(false);

		if(PlayerCharacter::getSingleton().getActiveWeapon() == this)
			App::getSingleton().getElectricitySound()->getSound()->stop();
	}


	if(m_Bolt1.hasFinished() && m_Bolt2.hasFinished())
	{
		if(PlayerCharacter::getSingleton().getActiveWeapon() == this)
			App::getSingleton().getElectricitySound()->getSound()->stop();
	}
}
//----------------------------------------------------------------------