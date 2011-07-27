/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceRender.h
Version:	0.11
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DEVICERENDER_H_
#define __INC_DEVICERENDER_H_


#include "BaseClass.h"
#include "Listener.h"
#include "DeviceRender.h"


namespace nGENE
{
	/** Interface describing rendering adapter.
		@remarks
			DeviceRender interface lets you control rendering adapter
			behaviour and checking its capabilites to ensure its
			capable of performing specific tasks.
	*/
	class nGENEDLL DeviceRender: public BaseClass, public Listener <DeviceEvent>
	{
	protected:
		/** Is device lost?
			@remarks
				Loosing device may occur eg. when in Windows switching from
				the application full-screen window to something different
				using ALT-TAB combination.
		*/
		mutable bool m_bIsLost;

		/// How the vertices are being processed?
		PROCESSING_TYPE m_Processing;

	public:
		/** Constructor.
			@param
				_type the way the vertices will be processed by the rendering mechanisms.
		*/
		DeviceRender(PROCESSING_TYPE _type);
		virtual ~DeviceRender();

		/// Performs actions on device creation.
		virtual void onDeviceCreate()=0;

		/// Performs actions when rendering device is lost.
		virtual void onDeviceLost()=0;

		/// Performs actions on device destruction.
		virtual void onDeviceDestroy()=0;

		/** Tests the graphical capabilities of the rendering device.
			@remarks
				This is a very important function. You should use it always
				before you make an attempt to use graphical card features. Otherwise,
				if the card does not support specific feature, error might occur.
			@param
				caps the setting/capability/whatever you want to test to ensure
				that user's graphics adapter will handle it properly.
			@param
				param if any result would be produced by the function (eg. supported
				shaders' version) it would be stored here.
		*/
		virtual bool testCapabilities(nGENE_CAPABILITIES _caps, void* _param)=0;

		virtual void handleEvent(const DeviceEvent& _evt);

		virtual void init()=0;
		virtual void cleanup()=0;

		/** Resets the device.
			@remarks
				This function is especially useful, when rendering device
				becomes lost and so rendering could not be performed.
		*/
		virtual void reset() {}

		/** Returns graphical API dependent 3D device pointer.
		*/
		virtual void* getDevicePtr() const=0;


		/** Prepares device for the rendering loop.
		*/
		virtual void beginRender()=0;
		/** Executes necessary cleanup when the rendering is done.
		*/
		virtual void endRender()=0;

		/** Checks whether the device is still valid or needs resetting.
		*/
		bool isDeviceLost() const {return m_bIsLost;}
	};
}


#endif