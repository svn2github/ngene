/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CameraThirdPerson.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CAMERATHIRDPERSON_H_
#define __INC_CAMERATHIRDPERSON_H_


#include "Camera3D.h"
#include "TypeInfo.h"
#include "Vector3.h"


namespace nGENE
{
	/** Presents view from the third person perspective.
		@par
			It is especially useful in TPS/RPG and racing games.
		@par
			Although position of the camera is influenced by the
			position of parental node, rotation is not. It is due to
			the fact TPS camera is quite independent and rarely fixed,
			so there is no such need.
	*/
	class nGENEDLL CameraThirdPerson: public Camera3D
	{
		EXPOSE_TYPE
	private:
		Real m_fDistanceFromTarget;					///< Preferred distance from target point

		Vector3 m_vecTarget;						///< Target

	protected:
		virtual void updateAxes();

	public:
		CameraThirdPerson();
		virtual ~CameraThirdPerson();

		virtual void init();
		virtual void cleanup();

		/** Moves the camera in the z-direction.
			@param
				_direction pass 1 to make camera go forward and -1,
				to go backwards.
		*/
		virtual void move(Real _direction);

		/** Moves the camera in the x-direction.
			@param
				_direction pass 1 to make camera go right and -1,
				to go left.
		*/
		virtual void strafe(Real _direction);

		/// Sets target point for the camera.
		virtual void setTarget(Vector3& _target);
		virtual Vector3& getTarget();

		/// Sets distance between camera and the target point.
		virtual void setDistanceFromTarget(Real _distance);
		virtual Real getDistanceFromTarget() const;

		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};

	
	/** Camera factory to be used for creating CameraThirdPerson
		type objects.
	*/
	class nGENEDLL TPPCameraFactory: public CameraFactory
	{
	public:
		TPPCameraFactory();
		~TPPCameraFactory();

		Camera* createCamera() {return new CameraThirdPerson();}
	};



	inline void CameraThirdPerson::setTarget(Vector3& _target)
	{
		m_vecTarget = _target;

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	inline Vector3& CameraThirdPerson::getTarget()
	{
		return m_vecTarget;
	}
//----------------------------------------------------------------------
	inline void CameraThirdPerson::setDistanceFromTarget(Real _distance)
	{
		m_fDistanceFromTarget = _distance;

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	inline Real CameraThirdPerson::getDistanceFromTarget() const
	{
		return m_fDistanceFromTarget;
	}
//----------------------------------------------------------------------
}


#endif