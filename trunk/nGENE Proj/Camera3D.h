/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Camera3D.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CAMERA3D_H_
#define __INC_CAMERA3D_H_


#include "Camera.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Abstract camera class used in 3D space.
	*/
	class nGENEDLL Camera3D: public Camera
	{
		EXPOSE_TYPE
	protected:
		Real m_fVelocity;					///< Camera speed

	public:
		Camera3D();
		virtual ~Camera3D();

		virtual void init();
		virtual void cleanup();

		/** Moves the camera in the z-direction.
			@param
				_direction pass 1 to make camera go forward and -1,
				to go backwards.
		*/
		virtual void move(Real _direction)=0;

		/** Moves the camera in the x-direction.
			@param
				_direction pass 1 to make camera go right and -1,
				to go left.
		*/
		virtual void strafe(Real _direction)=0;

		virtual void setVelocity(Real _velocity);
		virtual Real getVelocity() const;

		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};



	inline void Camera3D::setVelocity(Real _velocity)
	{
		m_fVelocity = _velocity;
	}
//----------------------------------------------------------------------
	inline Real Camera3D::getVelocity() const
	{
		return m_fVelocity;
	}
//----------------------------------------------------------------------
}


#endif