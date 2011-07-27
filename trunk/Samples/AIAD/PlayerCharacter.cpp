/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PlayerCharacter.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PlayerCharacter.h"

using nGENE::byte;

#include "nGENE.h"
#include "App.h"


PlayerCharacter::PlayerCharacter():
	m_pController(NULL),
	m_nPoints(0),
	m_fAgility(2.0f),
	m_fAngle(0.0f),
	m_bStrafing(false)
{
}
//----------------------------------------------------------------------
PlayerCharacter::~PlayerCharacter()
{
}
//----------------------------------------------------------------------
void PlayerCharacter::init(SceneManager* _manager, PhysicsWorld* _world)
{
	m_nMaxHP = 200;
	m_nCurrentHP = 200;

	Matrix4x4 matTrans;
	matTrans.scale(0.005f);
	m_pVisual = _manager->createMesh <MeshLoadPolicy3dsFile>(L"blackbird.3ds", matTrans, true);
	m_pVisual->setPosition(14.0f, 12.0f, 7.0f);
	
	for(uint i = 0; i < m_pVisual->getSurfacesNum(); ++i)
	{
		Surface* pSurface = m_pVisual->getSurface(i);
		if(pSurface->getName().find(L"Normals_") == wstring::npos)
			pSurface->setMaterial(MaterialManager::getSingleton().getLibrary(L"game")->getMaterial(L"playerShip"));
	}
	_manager->getRootNode()->addChild(L"PlayerNode", m_pVisual);
	Quaternion quatRot1(Vector3::UNIT_X, -Maths::PI_HALF);
	Quaternion quatRot2(Vector3::UNIT_Z, -Maths::PI);
	m_pVisual->setRotation(quatRot1 * quatRot2);

	CONTROLLER_DESC charDesc;
	charDesc.height = 0.5f;
	charDesc.obstacleLimit = 0.1f;
	charDesc.length = 1.0f;
	charDesc.width = 0.6f;
	charDesc.obstacleLimit = 0.0f;
	charDesc.shape = CCS_BOX;
	charDesc.sharpness = 0.1f;
	//charDesc.minDistance = 0.0001f;

	m_pController = _world->createController(charDesc);
	m_pController->attachNode(m_pVisual);
	m_pController->setNormalizeDir(false);
	m_pController->setVelocity(2.0f);
	m_pController->setCollisionFlag(GROUND_PLANE, true);
	m_pController->setCollisionFlag(ITEM, true);
	m_pController->setCollisionFlag(ENEMY, true);
	m_pController->setCollisionFlag(ENEMY_PROJECTILE, true);
	m_pController->setCollisionFlag(PROJECTILE, false);
	m_pController->setFlying(true);
	m_pController->setOrientationAppliesToMovementDir(false);
	_world->addController(L"Player", m_pController);


	ParticleEmitter fire;
	fire.setParticlesMaxCount(25);
	fire.setSpawnRate(5);
	fire.setPosition(Vector3(0.0, 0.0f, 0.0f));
	fire.setVelocity(Vector3(0.0f, 0.0f, -3.0f));
	fire.setVelocitySpread(Vector3(0.05f, 0.05f, 0.2f));
	fire.setAccelerationSpread(Vector3(0.025f, 0.025f, 0.1f));
	fire.setAcceleration(Vector3(0.0f, 0.0f, -0.2f));
	fire.setSize(1.0f);
	fire.setSizeSpread(0.025f);
	fire.setGrowth(0.9f);
	fire.setGrowthSpread(0.15f);
	fire.setLifeTime(500.0f);
	fire.setLifeTimeSpread(0.0f);
	fire.setFadeSpeed(2.5f);
	fire.setAngularVelocity(Maths::PI / 36.0f);
	fire.setAngularVelocitySpread(0.02f);


	// Create particle system
	/*NodeParticleSystem* m_pPS = _manager->createParticleSystem();
	m_pPS->setPosition(0.25f, -0.5f, 0.0f);
	m_pPS->addParticleEmitter(fire, L"fire_emitter");
	m_pVisual->addChild(L"PlayerFire_1", m_pPS);

	Material* matFire = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"fire");
	Surface* pSurface = m_pPS->getSurface(L"fire_emitter");
	pSurface->setMaterial(matFire);*/
}
//----------------------------------------------------------------------
void PlayerCharacter::move(const Vector3& _dir)
{
	m_pController->move(_dir);
}
//----------------------------------------------------------------------
void PlayerCharacter::update()
{
	vector <uint> vIndices;

	for(uint i = 0; i < m_vItems.size(); ++i)
	{
		m_vItems[i]->update();

		if(m_vItems[i]->isFinished())
			vIndices.push_back(i);
	}

	uint offset = 0;
	while(vIndices.size())
	{
		m_vItems.erase(m_vItems.begin() + vIndices[0] - offset);
		vIndices.erase(vIndices.begin());

		++offset;
	}
}
//----------------------------------------------------------------------
void PlayerCharacter::levelUp()
{

}
//----------------------------------------------------------------------
void PlayerCharacter::setRotation(const Quaternion& _quat)
{
	m_pController->setRotation(_quat);
}
//----------------------------------------------------------------------
Quaternion& PlayerCharacter::getRotation()
{
	return m_pController->getRotation();
}
//----------------------------------------------------------------------
void PlayerCharacter::onHit(uint _damagePoints)
{
	if(isShielded())
		return;

	// Player dead
	if(_damagePoints >= m_nCurrentHP)
	{
		if(m_nCurrentHP)
		{
			App::getSingleton().getHUD()->setMessage(L"You died!");
			m_nCurrentHP = 0;

			App::getSingleton().playerDied();
		}
	}
	else
	{
		m_nCurrentHP -= _damagePoints;
	}
}
//----------------------------------------------------------------------
void PlayerCharacter::reset()
{
	m_nMaxHP = 200;
	m_nCurrentHP = 200;
	m_nPoints = 0;
	m_fAgility = 2.0f;
	m_fAngle = 0.0f;
	m_bStrafing = false;
	m_nExperience = 0;

	m_vWeapons.clear();
	m_vItems.clear();
}
//----------------------------------------------------------------------
void PlayerCharacter::setActiveWeapon(Weapon* _weapon)
{
	Character::setActiveWeapon(_weapon);

	wstringstream name;
	name << L"icon_" << _weapon->getTypeName() << L".png";
	App::getSingleton().getHUD()->setWeaponImage(name.str());
}
//----------------------------------------------------------------------
void PlayerCharacter::weaponAvailable()
{
	if(!m_pActiveWeapon->getAmmoLeft() && !m_pActiveWeapon->isInfiniteAmmo())
		App::getSingleton().getHUD()->setNoAmmoImageEnabled(true);
	else if(m_pActiveWeapon->getAmmoLeft() && !m_pActiveWeapon->isInfiniteAmmo())
		App::getSingleton().getHUD()->setNoAmmoImageEnabled(false);
	else if(m_pActiveWeapon->isInfiniteAmmo())
		App::getSingleton().getHUD()->setNoAmmoImageEnabled(false);
}
//----------------------------------------------------------------------