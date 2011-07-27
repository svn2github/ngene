/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CameraEnumerator.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CAMERAENUMERATOR_H_
#define __INC_CAMERAENUMERATOR_H_


#include "Camera.h"
#include "Camera2D.h"
#include "CameraFirstPerson.h"
#include "CameraThirdPerson.h"
#include "HashTable.h"
#include "Prerequisities.h"
#include "Singleton.h"


namespace nGENE
{
	/** Camera Enumerator class.
		@remarks
			Enumerators lets you easily extend nGENE Tech by just adding
			factories of new type to the enumerator. Then by specifying
			the type name you can create objects of any available type
			in the enumerator.
	*/
	class nGENEDLL CameraEnumerator: public Singleton <CameraEnumerator>
	{
	protected:
		// Hash table containing all factories
		HashTable <wstring, CameraFactory*> m_pFactories;
		
		// Default factories
		DefaultCameraFactory m_DefaultFactory;
		FPPCameraFactory m_FPPFactory;
		TPPCameraFactory m_TPPFactory;
		Camera2DFactory m_Factory2D;

	public:
		CameraEnumerator();
		virtual ~CameraEnumerator();

		/** Adds new factory to the enumerator.
			@remarks
				Factory has to be added to the enumerator prior to
				using it.
		*/
		void addFactory(CameraFactory& _factory, const wstring& _typeName);
		void removeFactory(const wstring& _typeName);

		/// Creates new camera object.
		Camera* create(const wstring& _typeName);
		/// Destroys camera object.
		void destroy(Camera* _camera);
	};
}


#endif