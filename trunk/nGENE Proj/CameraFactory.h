/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CameraFactory.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CAMERAFACTORY_H_
#define __INC_CAMERAFACTORY_H_


namespace nGENE
{
	/** Base CameraFactory class.
		@remarks
			Each Camera class should be accompanied by its own (derived)
			CameraFactory class.
	*/
	class nGENEDLL CameraFactory
	{
	public:
		CameraFactory() {}
		virtual ~CameraFactory() {}

		virtual Camera* createCamera()=0;
	};
}


#endif