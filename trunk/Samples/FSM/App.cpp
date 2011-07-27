/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "nGENE.h"

using namespace nGENE;
using namespace nGENE::AI;


/// Type of location
enum LOCATION_TYPE
{
	LT_UNKNOWN,
	LT_GOLDMINE,
	LT_BANK,
	LT_BAR,
	LT_HOUSE
};

/// Type of event
enum EVENT_TYPE
{
	ET_POCKETS_FULL = 1,
	ET_POCKETS_EMPTY= 2,
	ET_THIRSTY		= 3,
	ET_TIRED		= 4
};

/// Miner actor
class MinerActor: public AIActor
{
	EXPOSE_TYPE
private:
	LOCATION_TYPE m_Location;

	int m_nGoldCarried;
	int m_nMoneyInBank;

	uint m_nThirst;
	uint m_nFatigue;

public:
	MinerActor()
	{
		m_Location = LT_UNKNOWN;
		m_nGoldCarried = 0;
		m_nMoneyInBank = 0;
		m_nThirst = 0;
		m_nFatigue = 0;
	}

	MinerActor(IBrain* _brain):
		AIActor(_brain)
	{
		m_Location = LT_UNKNOWN;
		m_nGoldCarried = 0;
		m_nMoneyInBank = 0;
		m_nThirst = 0;
		m_nFatigue = 0;
	}

	void update()
	{
		++m_nThirst;
		AIActor::update();
	}

	LOCATION_TYPE getLocation() const
	{
		return m_Location;
	}

	void setLocation(LOCATION_TYPE _location)
	{
		m_Location = _location;
	}

	void increaseFatique()
	{
		++m_nFatigue;
	}

	void rest()
	{
		if(m_nFatigue > 0)
			--m_nFatigue;
	}

	void notThirsty()
	{
		m_nThirst = 0;
	}

	void addToGoldCarried(int _gold)
	{
		m_nGoldCarried += _gold;
	}

	int getGoldCarried() const
	{
		return m_nGoldCarried;
	}

	void depositGold(int _gold)
	{
		m_nGoldCarried -= _gold;
		m_nMoneyInBank += _gold;
	}

	uint getGoldTotal() const
	{
		return (m_nGoldCarried + m_nMoneyInBank);
	}

	uint getThirst() const
	{
		return m_nThirst;
	}

	uint getFatigueLevel() const
	{
		return m_nFatigue;
	}

	bool pocketsFull()
	{
		return (m_nGoldCarried == 10 ? true : false);
	}

	bool pocketsEmpty()
	{
		return (!m_nGoldCarried ? true : false);
	}
};

// Initialize static members
TypeInfo MinerActor::Type(L"MinerActor", &AIActor::Type);


/// MinerActor class factory.
class MinerFactory: public IAIActorFactory
{
public:
	virtual AIActor* createAIActor()
	{
		return new MinerActor();
	}

	virtual AIActor* createAIActor(IBrain* _brain)
	{
		return new MinerActor(_brain);
	}
};


/// This state increases actors wealth.
class EnterMineAndDigForNugget: public AIActorFSMState
{
private:
	MinerActor* m_pMiner;

public:
	void onEnter()
	{
		m_pMiner = (MinerActor*)m_pActor;
		if(m_pMiner->getLocation() != LT_GOLDMINE)
		{
			wcout << L"\n" << m_pActor->getName() << L": " << L"Walkin' to the goldmine";

			m_pMiner->setLocation(LT_GOLDMINE);
		}

		Sleep(1000);
	}

	void onExit()
	{
		wcout << L"\n" << m_pActor->getName() << L": " << L"Leavin' goldmine";
		Sleep(1000);
	}

	void update()
	{
		m_pMiner->increaseFatique();

		if(m_pMiner->pocketsFull())
		{
			AIEvent evt;
			evt.receiver = m_pMiner;
			evt.messageID = ET_POCKETS_FULL;

			AIManager::getSingleton().handleEvent(evt);

			wcout << L"\n" << m_pActor->getName() << L": " << L"My pockets are full of golde";
		}
		else
		{
			int size = rand() % 3 + 1;
			if(m_pMiner->getGoldCarried() + size > 10)
				wcout << L"\n" << m_pActor->getName() << L": " << L"Doh! Nugget too big";
			else
			{
				wcout << L"\n" << m_pActor->getName() << L": " << L"Pickin' up a nugget of size " << size;
				m_pMiner->addToGoldCarried(size);
			}
		}

		if(m_pMiner->getThirst() >= 8 && !m_pMiner->pocketsEmpty())
		{
			AIEvent evt;
			evt.receiver = m_pMiner;
			evt.messageID = ET_THIRSTY;

			AIManager::getSingleton().handleEvent(evt);
		}

		if(m_pMiner->getFatigueLevel() >= 17)
		{
			AIEvent evt;
			evt.receiver = m_pMiner;
			evt.messageID = ET_TIRED;

			AIManager::getSingleton().handleEvent(evt);
		}

		Sleep(1000);
	}
};

/// In this state actor deposits some of its gold in bank.
class Deposit: public AIActorFSMState
{
private:
	MinerActor* m_pMiner;

public:
	void onEnter()
	{
		m_pMiner = (MinerActor*)m_pActor;
		if(m_pMiner->getLocation() != LT_BANK)
		{
			wcout << L"\n" << m_pActor->getName() << L": " << L"Walkin' to the bank";

			m_pMiner->setLocation(LT_BANK);
		}

		Sleep(1000);
	}

	void update()
	{
		m_pMiner->increaseFatique();

		if(m_pMiner->pocketsEmpty())
		{
			AIEvent evt;
			evt.receiver = m_pMiner;
			evt.messageID = ET_POCKETS_EMPTY;

			AIManager::getSingleton().handleEvent(evt);
		}
		else
		{
			wcout << L"\n" << m_pActor->getName() << L": " << L"Removin' a nugget from a pocket";
			m_pMiner->depositGold(1);
		}

		if(m_pMiner->getThirst() >= 8)
		{
			AIEvent evt;
			evt.receiver = m_pMiner;
			evt.messageID = ET_THIRSTY;

			AIManager::getSingleton().handleEvent(evt);
		}

		if(m_pMiner->getFatigueLevel() >= 17)
		{
			AIEvent evt;
			evt.receiver = m_pMiner;
			evt.messageID = ET_TIRED;

			AIManager::getSingleton().handleEvent(evt);
		}

		Sleep(1000);
	}

	void onExit()
	{
		wcout << L"\n" << m_pActor->getName() << L": " << L"Leavin' bank";
		wcout << L"\n" << m_pActor->getName() << L": " << L"Havin' " << m_pMiner->getGoldTotal() << " nuggets";

		Sleep(1000);
	}
};

/// In this state actor is drinking beer in a bar.
class Drinking: public AIActorFSMState
{
private:
	MinerActor* m_pMiner;

public:
	void onEnter()
	{
		m_pMiner = (MinerActor*)m_pActor;
		if(m_pMiner->getLocation() != LT_BAR)
		{
			wcout << L"\n" << m_pActor->getName() << L": " << L"I'm thirsty";
			wcout << L"\n" << m_pActor->getName() << L": " << L"Walkin' to the bar";

			m_pMiner->setLocation(LT_BAR);
		}

		Sleep(1000);
	}

	void update()
	{
		m_pMiner->increaseFatique();

		if(!m_pMiner->pocketsEmpty())
		{
			wcout << L"\n" << m_pActor->getName() << L": " << L"Payin' for ale";
			m_pMiner->addToGoldCarried(-1);
			wcout << L"\n" << m_pActor->getName() << L": " << L"Drinkin' ale";
		}
		else
		{
			wcout << L"\n" << m_pActor->getName() << L": " << L"Damn! Not enough golde";
			m_pFSM->revertToPreviousState();
		}

		if(m_pMiner->getThirst() > 0)
		{
			m_pMiner->notThirsty();
		}
		
		if(!m_pMiner->getThirst())
			m_pFSM->revertToPreviousState();

		if(m_pMiner->getFatigueLevel() >= 17)
		{
			AIEvent evt;
			evt.receiver = m_pMiner;
			evt.messageID = ET_TIRED;

			AIManager::getSingleton().handleEvent(evt);
		}

		Sleep(1000);
	}

	void onExit()
	{
		wcout << L"\n" << m_pActor->getName() << L": " << L"Leavin' bar";

		Sleep(1000);
	}
};

/// In this state actor sleeps and thus rests.
class Sleeping: public AIActorFSMState
{
private:
	MinerActor* m_pMiner;

public:
	void onEnter()
	{
		m_pMiner = (MinerActor*)m_pActor;
		if(m_pMiner->getLocation() != LT_HOUSE)
		{
			wcout << L"\n" << m_pActor->getName() << L": " << L"I'm tired";
			wcout << L"\n" << m_pActor->getName() << L": " << L"Goin' home";

			m_pMiner->setLocation(LT_HOUSE);
		}

		Sleep(1000);
	}

	void update()
	{
		if(!m_pMiner->getFatigueLevel())
			m_pFSM->revertToPreviousState();
		else
		{
			wcout << L"\n" << m_pActor->getName() << L": " << L"zzzz...";
			m_pMiner->rest();
		}

		Sleep(1000);
	}

	void onExit()
	{
		wcout << L"\n" << m_pActor->getName() << L": " << L"Leavin' house";

		Sleep(1000);
	}
};


int main()
{
	Timer timer(false);
	srand(timer.getMicroseconds());

	AIManager mgr;
	MinerFactory factory;
	AIActorEnumerator::getSingleton().addFactory(factory, MinerActor::Type.Type());

	// Create actors
	MinerActor* minerBob = mgr.createActor <MinerActor>(L"BOB the Miner");
	MinerActor* minerJohn = mgr.createActor <MinerActor>(L"JOHN the Miner");

	// Create state and transitions
	FiniteStateMachine minerBobFSM;
	FSMBrain minerBobBrain;

	EnterMineAndDigForNugget mineStateBob;
	Deposit depositStateBob;
	Drinking drinkingStateBob;
	Sleeping sleepingStateBob;

	mineStateBob.setActor(minerBob);
	depositStateBob.setActor(minerBob);
	drinkingStateBob.setActor(minerBob);
	sleepingStateBob.setActor(minerBob);

	mineStateBob.addTransition(&depositStateBob, ET_POCKETS_FULL);
	mineStateBob.addTransition(&drinkingStateBob, ET_THIRSTY);
	mineStateBob.addTransition(&sleepingStateBob, ET_TIRED);

	depositStateBob.addTransition(&mineStateBob, ET_POCKETS_EMPTY);
	depositStateBob.addTransition(&drinkingStateBob, ET_THIRSTY);
	depositStateBob.addTransition(&sleepingStateBob, ET_TIRED);

	drinkingStateBob.addTransition(&sleepingStateBob, ET_TIRED);

	minerBobFSM.setActiveState(&mineStateBob);
	minerBobBrain.setFSM(&minerBobFSM);

	minerBob->setBrain(&minerBobBrain);

	FiniteStateMachine minerJohnFSM;
	FSMBrain minerJohnBrain;

	EnterMineAndDigForNugget mineStateJohn;
	Deposit depositStateJohn;
	Drinking drinkingStateJohn;
	Sleeping sleepingStateJohn;

	mineStateJohn.setActor(minerJohn);
	depositStateJohn.setActor(minerJohn);
	drinkingStateJohn.setActor(minerJohn);
	sleepingStateJohn.setActor(minerJohn);

	mineStateJohn.addTransition(&depositStateJohn, ET_POCKETS_FULL);
	mineStateJohn.addTransition(&drinkingStateJohn, ET_THIRSTY);
	mineStateJohn.addTransition(&sleepingStateJohn, ET_TIRED);

	depositStateJohn.addTransition(&mineStateJohn, ET_POCKETS_EMPTY);
	depositStateJohn.addTransition(&drinkingStateJohn, ET_THIRSTY);
	depositStateJohn.addTransition(&sleepingStateJohn, ET_TIRED);

	drinkingStateJohn.addTransition(&sleepingStateJohn, ET_TIRED);

	minerJohnFSM.setActiveState(&mineStateJohn);
	minerJohnBrain.setFSM(&minerJohnFSM);

	minerJohn->setBrain(&minerJohnBrain);


	// Run...
	while(true)
	{
		mgr.update();
	}

	return 0;
}
//----------------------------------------------------------------------