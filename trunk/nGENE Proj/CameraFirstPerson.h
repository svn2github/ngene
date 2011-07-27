/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CameraFirstPerson.h
Version:	0.08
---------------------------------------------------------------------------
*/

#ifndef __INC_CAMERAFIRSTPERSON_H_
#define __INC_CAMERAFIRSTPERSON_H_


#include "Camera3D.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Presents view from the first person perspective.
		@remarks
			It is especially useful in FPS games.
		@par
			Although position of the camera is influenced by the
			position of parental node, rotation is not. It is due to
			the fact FPS camera is quite independent and rarely fixed,
			so there is no such need.
	*/
	class nGENEDLL CameraFirstPerson: public Camera3D
	{
		EXPOSE_TYPE
	protected:
		virtual void updateAxes();

	public:
		CameraFirstPerson();
		virtual ~CameraFirstPerson();

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

		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};

	
	/** Camera factory to be used for creating CameraFirstPerson
		type objects.
	*/
	class nGENEDLL FPPCameraFactory: public CameraFactory
	{
	public:
		FPPCameraFactory();
		virtual ~FPPCameraFactory();

		Camera* createCamera() {return new CameraFirstPerson();}
	};
}


#endif