/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceRenderDX9.h
Version:	0.12
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DEVICERENDERDX9_H_
#define __INC_DEVICERENDERDX9_H_


#include <D3D9.h>
#include "AutoPointer.h"
#include "DeviceRender.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** A rendering device supporting DirectX 9.0c.
		@remarks
			It is obvious that this class could be used on Windows
			platform only.
	*/
	class nGENEDLL DeviceRenderDX9: public DeviceRender
	{
		EXPOSE_TYPE
	private:
		// Hide assignment operator and copy constructor to prevent
		// it from being used.
		DeviceRenderDX9(const DeviceRenderDX9& src);
		DeviceRenderDX9& operator=(const DeviceRenderDX9& rhs);

	protected:
		/// DirectX Direct3D Device
		IDirect3DDevice9* m_pD3DDev;

		/// On-screen presentation parameters
		D3DPRESENT_PARAMETERS& m_Params;
		
		/// Direct3D
		IDirect3D9* m_pD3D;

		/// Handle to the owner window
		HWND m_hWnd;

		/// Device caps.
		D3DCAPS9 m_DeviceCaps;

		/// Is PerfHUD enabled (if available)?
		bool m_bProfile;

		/// Is multi-threaded flag enabled?
		bool m_bMultiThreaded;

	public:
		/** Creates rendering device.
			@param
				d3d a pointer to the Direct3D9 object.
			@param
				params parameters of presenting the rendered image on screen.
			@param
				type the way the vertices are being processed.
		*/
		DeviceRenderDX9(IDirect3D9* _d3d, D3DPRESENT_PARAMETERS& _params,
			HWND _hwnd=NULL, PROCESSING_TYPE _type=PROCESSING_HARDWARE,
			bool _profile=false, bool _multiThreaded=false);
		virtual ~DeviceRenderDX9();

		virtual void onDeviceCreate() {}
		virtual void onDeviceLost();
		virtual void onDeviceDestroy() {}

		/** Tests graphical capabilities of the device. It helps the engine
			to determine if the application will run properly on the current
			device.
		*/
		virtual bool testCapabilities(nGENE_CAPABILITIES _caps, void* _param);

		/// Initializes the device.
		void init();
		void cleanup();
		virtual void reset(D3DPRESENT_PARAMETERS* _params=NULL);

		/// Gets the current Direct3D Device.
		virtual void* getDevicePtr() const;
		/// Returns pointer to DirectX API object.
		IDirect3D9* getD3DPtr() const;


		virtual void beginRender();
		virtual void endRender();
	};



	inline void* DeviceRenderDX9::getDevicePtr() const
	{
		return m_pD3DDev;
	}
//----------------------------------------------------------------------
	inline IDirect3D9* DeviceRenderDX9::getD3DPtr() const
	{
		return m_pD3D;
	}
//----------------------------------------------------------------------
}


#endif