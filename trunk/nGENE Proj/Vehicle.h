/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Vehicle.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VEHICLE_H_
#define __INC_VEHICLE_H_


#include "Prerequisities.h"


namespace nGENE
{
	class nGENEDLL VehicleWheel
	{
	};

	class nGENEDLL VehicleMotor
	{
	private:
		Real m_fRPM;
		Real m_fMaxTorque;
		Real m_fMaxTorquePos;
		Real m_fMaxRPMToGearUp;
		Real m_fMinRPMToGearDown;
		Real m_fMaxRPM;
		Real m_fMinRPM;
	public:
		VehicleMotor() {}
	};

	class nGENEDLL VehicleGears
	{
	private:
		uint m_nGearsNum;
		uint m_nCurrentGear;
	};

	/** This class lets you create new vehicles.
	*/
	class nGENEDLL Vehicle
	{
	private:
		vector <VehicleWheel*> m_vWheels;

		VehicleMotor* m_pMotor;
		VehicleGears* m_pGears;

		PhysicsWorld* m_pWorld;					///< Pointer to the physics world
		PhysicsActor* m_pBody;						///< Pointer to body actor

		Real m_fMass;
		Real m_fMotorForce;
		Real m_fMaxVelocity;

		Real m_fWheelRadius;

	public:
		Vehicle(PhysicsWorld* _world, PhysicsActor* _body);
		virtual ~Vehicle();

		void init();
		void update(Real _delta);
		void cleanup();

		/** Lets you 'drive' a vehicle.
			@param
				_movement 2-dimensional vector specifying movement i x, y
				axes, where x - stands for left/right and y for acceleration.
			@param
				_handbrake specifies whether handbrake was used.
		*/
		void steer(const Vector2& _movement, bool _handbrake);
		void gearUp();
		void gearDown();
	};
}


#endif