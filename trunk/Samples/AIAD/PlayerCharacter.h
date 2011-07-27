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
#include "Character.h"
#include "Singleton.h"


class PlayerCharacter: public Character, public Singleton <PlayerCharacter>
{
private:
	uint m_nExperience;
	uint m_nPoints;

	float m_fAgility;
	float m_fAngle;

	bool m_bStrafing;

	CharacterController* m_pController;

	vector <Weapon*> m_vWeapons;
	vector <UsableItem*> m_vItems;


	void levelUp();

public:
	PlayerCharacter();
	virtual ~PlayerCharacter();

	void init(SceneManager* _manager, PhysicsWorld* _world);
	void update();

	void move(const Vector3& _dir);

	void setRotation(const Quaternion& _quat);
	Quaternion& getRotation();

	NodeVisible* getVisual() const;

	void addPoints(uint _points);
	uint getPoints() const;

	void reset();

	virtual void onHit(uint _damagePoints);

	void setAgility(float _agility)
	{
		m_fAgility = _agility;
	}

	void setAngle(float _angle)
	{
		m_fAngle = _angle;
	}

	float getAngle() const
	{
		return m_fAngle;
	}

	void setStrafing(bool _value)
	{
		m_bStrafing = _value;
	}

	bool isStrafing() const
	{
		return m_bStrafing;
	}

	void heal(uint _points)
	{
		m_nCurrentHP += _points;
		if(m_nCurrentHP >= m_nMaxHP)
			m_nCurrentHP = m_nMaxHP;
	}

	float getAgility()
	{
		return m_fAgility;
	}

	void addWeapon(Weapon* _weapon)
	{
		uint i = 0;
		for(i = 0; i < m_vWeapons.size(); ++i)
		{
			if(m_vWeapons[i] == _weapon)
				break;
		}

		// Add weapon if player haven't it yet
		if(i == m_vWeapons.size())
			m_vWeapons.push_back(_weapon);
		else
		{
			// Add ammo if player already have this weapon
			_weapon->addAmmo(_weapon->getClipSize());
			if(_weapon == m_pActiveWeapon)
				weaponAvailable();
		}
	}

	void addItem(UsableItem* _item)
	{
		m_vItems.push_back(_item);
	}

	void weaponAvailable();

	void selectNextWeapon()
	{
		uint i;
		for(i = 0; i < m_vWeapons.size(); ++i)
		{
			if(m_vWeapons[i] == m_pActiveWeapon)
				break;
		}

		if(i != m_vWeapons.size() && i != m_vWeapons.size() - 1)
		{
			setActiveWeapon(m_vWeapons[i + 1]);
		}
		else if(i != m_vWeapons.size() && i == m_vWeapons.size() - 1)
		{
			setActiveWeapon(m_vWeapons[0]);
		}

		weaponAvailable();
	}

	void selectPrevWeapon()
	{
		uint i;
		for(i = 0; i < m_vWeapons.size(); ++i)
		{
			if(m_vWeapons[i] == m_pActiveWeapon)
				break;
		}

		if(i != m_vWeapons.size() && i != 0)
		{
			setActiveWeapon(m_vWeapons[i - 1]);
		}
		else if(i != m_vWeapons.size() && i == 0)
		{
			setActiveWeapon(m_vWeapons[m_vWeapons.size() - 1]);
		}

		weaponAvailable();
	}

	void setActiveWeapon(Weapon* _weapon);

	CharacterController* getController() const
	{
		return m_pController;
	}
};



inline NodeVisible* PlayerCharacter::getVisual() const
{
	return m_pVisual;
}
//----------------------------------------------------------------------
inline void PlayerCharacter::addPoints(uint _points)
{
	m_nPoints += _points;
	if(m_nPoints && m_nPoints % 100 == 0)
	{
		levelUp();
	}
}
//----------------------------------------------------------------------
inline uint PlayerCharacter::getPoints() const
{
	return m_nPoints;
}
//----------------------------------------------------------------------
