/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Character.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once


#include "Prerequisities.h"
#include "AIManager.h"
#include "Vector3.h"
#include "NodeLightning.h"

using namespace nGENE;
using namespace nGENE::AI;

using nGENE::Nature::NodeLightning;


enum ENTITY_TYPE
{
	GROUND_PLANE = 11,
	DEBRIS = 12,
	ENEMY = 13,
	ENEMY_PROJECTILE = 14,
	PROJECTILE = 15,
	ITEM = 16
};


class App;
class CPUCharacter;

class Item
{
public:
	virtual void use() {};
};

class Projectile
{
protected:
	static uint s_nProjectilesCount;

	uint m_nDamage;

	NodeVisible* m_pVisual;
	PhysicsActor* m_pPhysics;

	bool m_bToBeDisposed;
	bool m_bEnemy;

	Vector3 m_vecVelocity;

	wstring m_stName;

public:
	Projectile():
		m_bToBeDisposed(false)
	{
	}

	virtual ~Projectile();

	virtual void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material, Vector3& _position, bool _enemyShot,
		bool _movable, Vector3 _direction=Vector3::UNIT_Z);

	void dispose()
	{
		m_bToBeDisposed = true;
	}

	bool isDisposed() const
	{
		return m_bToBeDisposed;
	}

	wstring& getName();

	Vector3& getPosition();

	virtual void update(Real _delta);
};

class BombProjectile: public Projectile
{
public:
	virtual void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material, Vector3& _position, bool _enemyShot,
		bool _movable, Vector3 _direction=Vector3::UNIT_Z);
};


class MissileProjectile: public Projectile
{
protected:
	Vector3 m_vecPrevPos;
	Vector3 m_vecPrevDir;

	CPUCharacter* m_pEnemy;
	wstring m_stEnemyName;

	NodeParticleSystem* m_pPS;
	NodeLight* m_pLight;

public:
	virtual ~MissileProjectile();
	virtual void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material, Vector3& _position, bool _enemyShot,
		bool _movable, Vector3 _direction=Vector3::UNIT_Z);

	virtual void update(Real _delta);
};

inline wstring& Projectile::getName()
{
	return m_stName;
}

class Character;

class DebrisPart
{
protected:
	static uint s_nDebrisCount;

	NodeVisible* m_pVisual;
	PhysicsActor* m_pActor;

public:
	virtual ~DebrisPart();

	void init(SceneManager* _manager, Vector3& _position);
};

class Explosion
{
protected:
	static uint s_nExplosionsCount;

	NodeParticleSystem* m_pPS;
	NodeVisible* m_pDebris;
	NodeLight* m_pLight;
	PhysicsActor* m_pActor;

	DebrisPart m_Part1;
	DebrisPart m_Part2;
	DebrisPart m_Part3;
	/*DebrisPart m_Part4;
	DebrisPart m_Part5;
	DebrisPart m_Part6;*/

	bool m_bMovable;

	uint m_nPassed;
	uint m_nPrev;
	
	wstring m_stName;

public:
	Explosion():
		m_pPS(NULL),
		m_pDebris(NULL),
		m_pActor(NULL)
	{
	}
	virtual ~Explosion();

	virtual void init(SceneManager* _manager, Vector3& _position,
		Character* _character);

	void update();

	bool isDisposed() const;

	wstring& getName()
	{
		return m_stName;
	}
};


class Decorator
{
public:
	static uint s_nDecoratorsCount;

protected:
	NodeVisible* m_pVisual;
	//PhysicsActor* m_pActor;

	wstring m_stName;

	bool m_bToBeDisposed;

public:
	Decorator():
		m_bToBeDisposed(false)
	{
	}
	virtual ~Decorator();

	virtual void init(SceneManager* _manager, Vector3& _position);

	wstring& getName()
	{
		return m_stName;
	}

	void dispose()
	{
		m_bToBeDisposed = true;
	}

	bool isDisposed() const
	{
		return m_bToBeDisposed;
	}

	Vector3& getPosition();
};


class Weapon: public Item
{
protected:
	uint m_nClipSize;
	uint m_nShootSpeed;
	
	uint m_nAmmoLeft;
	bool m_bInfiniteAmmo;

	uint m_nMaxUses;
	uint m_nCurrentUses;

	wstring m_stType;

public:
	Weapon():
		m_nMaxUses(0),
		m_nCurrentUses(0),
		m_bInfiniteAmmo(true),
		m_nAmmoLeft(10),
		m_nClipSize(10)
	{
	}

	void reset()
	{
		m_nAmmoLeft = m_nClipSize;
	}

	uint getAmmoLeft() const
	{
		return m_nAmmoLeft;
	}

	bool isInfiniteAmmo() const
	{
		return m_bInfiniteAmmo;
	}

	virtual void use(Vector3& _position, bool _enemyShot, bool _movable=true) {
	};

	void setMaxUses(uint _value)
	{
		m_nMaxUses = _value;
		m_nCurrentUses = _value;
	}

	uint getClipSize() const
	{
		return m_nClipSize;
	}

	virtual void update() {}
	virtual wstring& getTypeName()
	{
		return m_stType;
	}

	virtual void addAmmo(uint _count)
	{
		if(!m_bInfiniteAmmo)
			m_nAmmoLeft += _count;
	}
};

class BasicWeapon: public Weapon
{
protected:
	SceneManager* m_pSM;
	PhysicsWorld* m_pWorld;
	PhysicsMaterial* m_pMaterial;

public:
	virtual void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material)
	{
		m_pSM = _manager;
		m_pWorld = _world;
		m_pMaterial = _material;

		m_stType = L"one_shot";
	}

	virtual void use(Vector3& _position, bool _enemyShot, bool _movable=true);
};

class TwoShotsWeapon: public Weapon
{
protected:
	SceneManager* m_pSM;
	PhysicsWorld* m_pWorld;
	PhysicsMaterial* m_pMaterial;

public:
	virtual void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material)
	{
		m_pSM = _manager;
		m_pWorld = _world;
		m_pMaterial = _material;

		m_stType = L"two_shot";

		m_bInfiniteAmmo = false;
		m_nAmmoLeft = 50;
		m_nClipSize = 50;
	}

	virtual void use(Vector3& _position, bool _enemyShot, bool _movable=true);
};

class ThreeShotsWeapon: public Weapon
{
protected:
	SceneManager* m_pSM;
	PhysicsWorld* m_pWorld;
	PhysicsMaterial* m_pMaterial;

public:
	virtual void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material)
	{
		m_pSM = _manager;
		m_pWorld = _world;
		m_pMaterial = _material;

		m_stType = L"three_shot";

		m_bInfiniteAmmo = false;
		m_nAmmoLeft = 30;
		m_nClipSize = 30;
	}

	virtual void use(Vector3& _position, bool _enemyShot, bool _movable=true);
};

class LightningWeapon: public Weapon
{
protected:
	SceneManager* m_pSM;
	PhysicsWorld* m_pWorld;
	PhysicsMaterial* m_pMaterial;

	NodeLightning m_Bolt;
	wstring m_stEnemyName;

	CPUCharacter* m_pEnemy;

public:
	virtual void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material);

	virtual void use(Vector3& _position, bool _enemyShot, bool _movable=true);

	virtual void update();
};

class TwoLightningWeapon: public Weapon
{
protected:
	SceneManager* m_pSM;
	PhysicsWorld* m_pWorld;
	PhysicsMaterial* m_pMaterial;

	NodeLightning m_Bolt1;
	NodeLightning m_Bolt2;

	wstring m_stEnemyName1;
	wstring m_stEnemyName2;

	CPUCharacter* m_pEnemy1;
	CPUCharacter* m_pEnemy2;

public:
	virtual void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material);

	virtual void use(Vector3& _position, bool _enemyShot, bool _movable=true);

	virtual void update();
};

class BombWeapon: public Weapon
{
protected:
	SceneManager* m_pSM;
	PhysicsWorld* m_pWorld;
	PhysicsMaterial* m_pMaterial;

public:
	virtual void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material)
	{
		m_pSM = _manager;
		m_pWorld = _world;
		m_pMaterial = _material;

		m_stType = L"bomb";

		m_bInfiniteAmmo = false;
		m_nAmmoLeft = 5;
		m_nClipSize = 5;
	}

	virtual void use(Vector3& _position, bool _enemyShot, bool _movable=true);
};

class MissileWeapon: public Weapon
{
protected:
	SceneManager* m_pSM;
	PhysicsWorld* m_pWorld;
	PhysicsMaterial* m_pMaterial;

public:
	virtual void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material)
	{
		m_pSM = _manager;
		m_pWorld = _world;
		m_pMaterial = _material;

		m_stType = L"missile";

		m_bInfiniteAmmo = false;
		m_nAmmoLeft = 10;
		m_nClipSize = 10;
	}

	virtual void use(Vector3& _position, bool _enemyShot, bool _movable=true);

	//virtual void update();
};

class UsableItem;

/** Representation of the item on the map.
*/
class PickupItem
{
private:
	union
	{
		UsableItem* m_pItem;
		Weapon* m_pWeapon;
	};

	NodeVisible* m_pVisual;
	NodeLight* m_pLight;
	PhysicsActor* m_pActor;

	bool m_bWeapon;

	static uint s_nItemsCount;

	wstring m_stName;
	wstring m_stType;

	bool m_bToBeDisposed;

public:
	PickupItem():
		m_bToBeDisposed(false),
		m_pItem(NULL),
		m_bWeapon(false)
	{
	}

	virtual ~PickupItem();

	void setItem(UsableItem* _item);
	UsableItem* getItem() const;

	void setWeapon(Weapon* _weapon)
	{
		m_pWeapon = _weapon;
		m_bWeapon = true;
	}
	Weapon* getWeapon() const
	{
		if(m_bWeapon)
			return m_pWeapon;
		else
			return NULL;
	}

	void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material, Vector3& _position, wstring& _type);

	wstring& getName();

	wstring& getType()
	{
		return m_stType;
	}

	void dispose()
	{
		m_bToBeDisposed = true;
	}

	bool isDisposed() const
	{
		return m_bToBeDisposed;
	}

	Vector3& getPosition();

	virtual void pick();
};


class HealItem: public PickupItem
{
public:
	void pick();
};

class MoreHealthItem: public PickupItem
{
public:
	void pick();
};

class UsableItem: public Item
{
protected:
	uint m_nDuration;
	uint m_nPassed;
	uint m_nPrev;

	bool m_bFinished;

public:
	UsableItem()
	{
		m_nDuration = 0;
		m_nPassed = 0;
		m_bFinished = false;
	}

	virtual void onUse()
	{
		m_nPrev = Engine::getSingleton().getTimer().getMilliseconds();
		if(m_nPassed >= m_nDuration)
			m_nPassed = 0;
		m_bFinished = false;

		update();
	}

	virtual void onEnd()
	{
		m_bFinished = true;
	}

	virtual bool update()
	{
		if(m_nDuration)
		{
			uint now = Engine::getSingleton().getTimer().getMilliseconds();
			uint diff = now - m_nPrev;

			m_nPassed += diff;
			if(m_nPassed >= m_nDuration)
			{
				onEnd();
				return true;
			}

			m_nPrev = now;
		}

		return false;
	}

	bool isFinished() const
	{
		return m_bFinished;
	}

	void setDuration(uint _value)
	{
		m_nDuration = _value;
	}
};

class ShieldUsableItem: public UsableItem
{
public:
	ShieldUsableItem()
	{
		m_nDuration = 10000;
	}

	virtual bool update();

	virtual void onUse();
	virtual void onEnd();
};

class AgilityUsableItem: public UsableItem
{
public:
	AgilityUsableItem()
	{
		m_nDuration = 10000;
	}

	virtual bool update();

	virtual void onUse();
	virtual void onEnd();
};

class SlowMotionUsableItem: public UsableItem
{
public:
	SlowMotionUsableItem()
	{
		m_nDuration = 10000;
	}

	virtual bool update();

	virtual void onUse();
	virtual void onEnd();
};

class PointsItem: public PickupItem
{
public:
	void pick();
};


class Character
{
protected:
	uint m_nMaxHP;
	uint m_nCurrentHP;

	NodeVisible* m_pVisual;
	Weapon* m_pActiveWeapon;

	vector <Item> m_Equipment;

	bool m_bMovable;
	bool m_bShielded;

public:
	Character();
	virtual ~Character();

	virtual void move(const Vector2& _dir);
	
	void setCurrentHP(uint _value)
	{
		m_nCurrentHP = _value;
	}
	virtual uint getCurrentHP() const;

	virtual uint getMaxHP() const
	{
		return m_nMaxHP;
	}

	void setMaxHP(uint _value)
	{
		m_nMaxHP = _value;
	}

	void setShielded(bool _value)
	{
		m_bShielded = _value;
	}

	bool isShielded() const
	{
		return m_bShielded;
	}

	void addItem(const Item& _item);
	
	virtual void setActiveWeapon(Weapon* _weapon);
	Weapon* getActiveWeapon() const;

	Vector3& getPosition();

	bool isMovable() const
	{
		return m_bMovable;
	}
};


class CPUCharacter;


enum EVENT_TYPE
{
	ET_HIT,
	ET_NOHITPOINTS
};


class MyState: public AIActorFSMState
{
protected:
	CPUCharacter* m_pCharacter;

public:
	void setCharacter(CPUCharacter* _character);
};



inline void MyState::setCharacter(CPUCharacter* _character)
{
	m_pCharacter = _character;
}
//----------------------------------------------------------------------


class StateMoving: public MyState
{
public:
	void update();
};

class StateDead: public MyState
{
public:
	void onEnter();
	void update();
};

enum MOVEMENT_TYPE
{
	MT_BASIC,
	MT_PATH_SIN,
	MT_PATH_COS,
	MT_PATH_CHAIN
};

struct SHiScoreEntry
{
	wstring player;
	uint points;
};

class CPUCharacter: public Character, public AIActor
{
	EXPOSE_TYPE
public:
	static uint s_nEnemiesCount;

private:
	PhysicsActor* m_pActor;
	Material* m_pBase;

	Vector3 m_vecStartPos;

	wstring m_stName;

	StateMoving m_StateMoving;
	StateDead m_StateDead;

	FSMBrain m_Brain;
	FiniteStateMachine m_FSM;

	bool m_bToBeDisposed;
	bool m_bHit;

	float m_fVelocity;

	dword m_nPrevTime;
	dword m_nHitPassed;

	float m_fAmplitude;
	float m_fShootDelta;
	float m_fShootFrequency;
	float m_fMovementPassed;
	float m_fFreq;

	MOVEMENT_TYPE m_MovementType;


	virtual void makeShot(float _delta);
	virtual void makeMovement(float _delta);

public:
	CPUCharacter():
		m_bToBeDisposed(false),
		m_fVelocity(1.8f),
		m_fShootDelta(0.0f),
		m_fShootFrequency(1000.0f),
		m_bHit(false),
		m_fMovementPassed(0.0f)
	{
	}

	CPUCharacter(IBrain* _brain):
		AIActor(_brain),
		m_bToBeDisposed(false),
		m_fVelocity(1.8f),
		m_fShootDelta(0.0f),
		m_fShootFrequency(1000.0f),
		m_bHit(false),
		m_fMovementPassed(0.0f)
	{
	}

	virtual ~CPUCharacter();

	void init(SceneManager* _manager, PhysicsWorld* _world,
		PhysicsMaterial* _material, bool _movable, Vector3& _position, Vector3& _size,
		NodeVisible* _enemy, Vector3& _offset, MOVEMENT_TYPE _movement,
		float _amplitude, float _frequency, float _speedMult);

	void cleanup();
	void update();

	wstring& getName();

	virtual void onHit(uint _damagePoints);

	void dispose()
	{
		m_bToBeDisposed = true;
	}

	bool isDisposed() const
	{
		return m_bToBeDisposed;
	}

	void setPosition(Vector3& _pos);
};


inline void Character::setActiveWeapon(Weapon* _weapon)
{
	m_pActiveWeapon = _weapon;
}
//----------------------------------------------------------------------
inline Weapon* Character::getActiveWeapon() const
{
	return m_pActiveWeapon;
}
//----------------------------------------------------------------------
inline void PickupItem::setItem(UsableItem* _item)
{
	m_pItem = _item;
}
//----------------------------------------------------------------------
inline UsableItem* PickupItem::getItem() const
{
	return m_pItem;
}
//----------------------------------------------------------------------
inline wstring& PickupItem::getName()
{
	return m_stName;
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
inline wstring& CPUCharacter::getName()
{
	return m_stName;
}
//----------------------------------------------------------------------
inline uint Character::getCurrentHP() const
{
	return m_nCurrentHP;
}
//----------------------------------------------------------------------


/// CPUActor class factory.
class CPUActorFactory: public IAIActorFactory
{
public:
	virtual AIActor* createAIActor()
	{
		return new CPUCharacter();
	}

	virtual AIActor* createAIActor(IBrain* _brain);

	void removeAIActor(AIActor* _actor)
	{
		//NGENE_DELETE(_actor);
	}
};