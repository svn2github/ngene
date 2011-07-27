/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceRenderDX10.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DEVICERENDERDX10_H_
#define __INC_DEVICERENDERDX10_H_


#include <D3D10.h>
#include "AutoPointer.h"
#include "DeviceRender.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** A rendering device supporting DirectX 10.0.
		@remarks
			It is obvious that this class could be used on Windows
			platform only.
	*/
	class nGENEDLL DeviceRenderDX10: public DeviceRender
	{
		EXPOSE_TYPE
	private:
		ID3D10RenderTargetView* m_pRenderTargetView;

		// Hide assignment operator and copy constructor to prevent
		// it from being used.
		DeviceRenderDX10(const DeviceRenderDX10& src);
		DeviceRenderDX10& operator=(const DeviceRenderDX10& rhs);

	protected:
		/// DirectX Direct3D Device.
		ID3D10Device* m_pD3DDev;

		/// On-screen presentation parameters
		DXGI_SWAP_CHAIN_DESC& m_Params;

		/// Swap chain.
		IDXGISwapChain* m_pSwapChain;

		/// Handle to the owner window
		HWND m_hWnd;

	public:
		/** Creates rendering device.
			@param
				params parameters of presenting the rendered image on screen.
			@param
				type the way the vertices are being processed.
		*/
		DeviceRenderDX10(DXGI_SWAP_CHAIN_DESC& _params, HWND _hwnd=NULL, PROCESSING_TYPE _type=PROCESSING_HARDWARE);
		virtual ~DeviceRenderDX10();

		/** @todo
				Provide implementation.
		*/
		virtual void onDeviceCreate() {}
		virtual void onDeviceLost();
		/** @todo
				Provide implementation.
		*/
		virtual void onDeviceDestroy() {}

		/** Tests graphical capabilities of the device. It helps the engine
			to determine if the application will run properly on the current
			device.
		*/
		virtual bool testCapabilities(nGENE_CAPABILITIES _caps, void* _param);

		/** Initializes the device.
		*/
		void init();
		void cleanup();
		virtual void reset();

		/** Gets the current Direct3D Device.
		*/
		virtual void* getDevicePtr() const {return m_pD3DDev;}


		virtual void beginRender();
		virtual void endRender();
	};
}


#endif