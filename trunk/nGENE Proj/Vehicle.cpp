/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Vehicle.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Vehicle.h"


namespace nGENE
{
	Vehicle::Vehicle(PhysicsWorld* _world, PhysicsActor* _body):
		m_pWorld(_world),
		m_pBody(_body)
	{
	}
//----------------------------------------------------------------------
	Vehicle::~Vehicle()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Vehicle::init()
	{
		m_pMotor = new VehicleMotor();
		m_pGears = new VehicleGears();

		uint wheelsNum = 4;
		for(uint i = 0; i < wheelsNum; ++i)
		{
			VehicleWheel* pWheel = new VehicleWheel();
			m_vWheels.push_back(pWheel);
		}
	}
//----------------------------------------------------------------------
	void Vehicle::cleanup()
	{
		NGENE_DELETE(m_pMotor);
	}
//----------------------------------------------------------------------
	void Vehicle::update(Real _delta)
	{
	}
//----------------------------------------------------------------------
	void Vehicle::steer(const Vector2& _movement, bool _handbrake)
	{
		if(Maths::floatCompare(_movement.x, 0.0f) ||
		   Maths::floatCompare(_movement.y, 0.0f) ||
		   _handbrake)
		{
		}
	}
//----------------------------------------------------------------------
	void Vehicle::gearUp()
	{
		//if(m_pGears)
		//	m_pGears->gearUp();
	}
//----------------------------------------------------------------------
	void Vehicle::gearDown()
	{
		//if(m_pGears)
		//	m_pGears->gearDown();
	}
//----------------------------------------------------------------------
}