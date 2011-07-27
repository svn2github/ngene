/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MyCollisionListener.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "MyCollisionListener.h"
#include "nGENE.h"

#include "App.h"

#include <sstream>

using namespace nGENE;


MyCollisionListener::MyCollisionListener():
	counter(0),
	m_pApp(NULL)
{
}
//----------------------------------------------------------------------
MyCollisionListener::~MyCollisionListener()
{
}
//----------------------------------------------------------------------
void MyCollisionListener::handleEvent(const CollisionEvent& _evt)
{
	if(_evt.type & CT_START)
	{
		if(!_evt.actor1 || !_evt.actor2)
		{
			return;
		}
		Node* pNode1 = _evt.actor2->getNode();
		Node* pNode2 = _evt.actor1->getNode();

		Node* pEnemy;
		Node* pHitter;

		if(pNode1->getName().length() >= 5 && pNode1->getName().substr(0, 5) == L"Enemy")
		{
			pEnemy = pNode1;
			if(pNode2->getName().substr(0, 5) == L"Proje")
				pHitter = pNode2;
			else
				return;
		}
		else if(pNode2->getName().length() >= 5 && pNode2->getName().substr(0, 5) == L"Enemy")
		{
			pEnemy = pNode2;
			if(pNode1->getName().substr(0, 5) == L"Proje")
				pHitter = pNode1;
			else
				return;
		}
		else
		{
			if(pNode1->getName().substr(0, 5) == L"Proje")
			{
				wstring name = pNode1->getName();
				Projectile* pProjectile = m_pApp->getProjectile(name);
				if(pProjectile)
				{
					pProjectile->dispose();
				}
			}
			if(pNode2->getName().substr(0, 5) == L"Proje")
			{
				wstring name = pNode2->getName();
				Projectile* pProjectile = m_pApp->getProjectile(name);
				if(pProjectile)
				{
					pProjectile->dispose();
				}
			}
			return;
		}

		//wcout << pHitter->getName() << L"   " << pEnemy->getName() << endl;

		// An enemy?
		CPUCharacter* pCPU = m_pApp->getEnemy(pEnemy->getName());
		if(pCPU)
		{
			pCPU->onHit(10);

			wstring name = pHitter->getName();
			Projectile* pProjectile = m_pApp->getProjectile(name);
			if(pProjectile)
			{
				pProjectile->dispose();
			}
		}
	}
}
//----------------------------------------------------------------------
void MyCollisionListener::handleEvent(const ControllerCollisionEvent& _evt)
{
	Node* pHit = _evt.actor->getNode();

	// Pickup Item
	if(pHit->getName().substr(0, 5) == L"Item_")
	{
		PickupItem* pItem = m_pApp->getItem(pHit->getName());
		if(pHit)
		{
			pItem->pick();
			wstringstream msg;
			msg << L"You picked a " << pItem->getType() << " item";
			m_pApp->getHUD()->setMessage(msg.str());
		}
	}
	// Enemy
	else if(pHit->getName().substr(0, 5) == L"Enemy")
	{
		CPUCharacter* pCPU = m_pApp->getEnemy(pHit->getName());
		if(pCPU)
		{
			pCPU->onHit(10);
			PlayerCharacter::getSingleton().onHit(10);
		}
	}
	// Enemy projectile
	else if(pHit->getName().substr(0, 5) == L"E_Pro")
	{
		Projectile* pProjectile = m_pApp->getProjectile(pHit->getName());
		if(pProjectile)
		{
			pProjectile->dispose();
			PlayerCharacter::getSingleton().onHit(10);
		}
	}
	else
		return;
}
//----------------------------------------------------------------------