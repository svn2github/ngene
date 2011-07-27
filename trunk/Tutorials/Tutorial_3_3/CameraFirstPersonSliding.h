/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CameraFirstPersonSliding.h
Version:	0.01
---------------------------------------------------------------------------
*/

#ifndef __INC_CAMERAFIRSTPERSONSLIDING_H_
#define __INC_CAMERAFIRSTPERSONSLIDING_H_


#include "CameraFirstPerson.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Presents view from the first person perspective.
		@par
			It is derived from CameraFirstPerson class. The
			difference is that this camera is sliding.
	*/
	class CameraFirstPersonSliding: public CameraFirstPerson
	{
		EXPOSE_TYPE
	protected:
		Real m_fCurrentVelocity;
		Real m_fSlowDownRatio;

		Vector3 m_vecMovementDir;

	public:
		CameraFirstPersonSliding();
		virtual ~CameraFirstPersonSliding();

		void updateCamera();

		void setSlowDownSpeed(Real _value);

		/** Moves the camera in the z-direction.
			@param
				_direction pass 1 to make camera go forward and -1,
				to go backwards.
		*/
		void move(Real _direction);

		/** Moves the camera in the x-direction.
			@param
				_direction pass 1 to make camera go right and -1,
				to go left.
		*/
		void strafe(Real _direction);

		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};

	
	/** Camera factory to be used for creating CameraFirstPersonSliding
		type objects.
	*/
	class SlidingCameraFactory: public CameraFactory
	{
	public:
		SlidingCameraFactory();
		virtual ~SlidingCameraFactory();

		Camera* createCamera() {return new CameraFirstPersonSliding();}
	};
}


#endif