/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RendererDX10.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RENDERERDX10_H_
#define __INC_RENDERERDX10_H_


#include "DeviceRenderDX10.h"
#include "Listener.h"
#include "Prerequisities.h"
#include "Renderer.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Renderer using DirectX 10.0 SDK.
	*/
	class nGENEDLL RendererDX10: public Renderer
	{
		EXPOSE_TYPE
	private:
		/// Matrices used to store transforms of world, view and projection to be used
		/// by shaders programs.
		Matrix4x4* m_matWorld;
		Matrix4x4* m_matView;
		Matrix4x4* m_matProj;

		/// DirectX 10.0 rendering device
		AutoPointer <DeviceRenderDX10> m_pDeviceRender;

		/// Display parameters
		DXGI_SWAP_CHAIN_DESC m_D3DParams;

		/// Pointer to the Direct3D Device
		ID3D10Device* m_pD3DDev;

		/** Alpha rendering stage (necessary as we are using 
			deferred shading).
		*/
		AlphaStage* m_pAlphaStage;

		/** Geometry rendering stage (necessary as we are using
			deferred shading).
		*/
		GeometryStage* m_pGeometryStage;

		/** Light rendering stage (necessary as we are using
			deferred shading).
		*/
		LightStage* m_pLightStage;


		virtual void renderStages();

		virtual void initRendering();
		virtual void useWindow(uint _index);
//		virtual void setRenderState(D3DRENDERSTATETYPE _rs, int _value) const;

		/** This method is used for geometry stage of deferred shading.
			@remarks
				The standard material plays role of diffuse buffer, but
				several other rendering passes are applied (eg. normals,
				specular) to enhance visual quality of the final image.
		*/
		virtual void renderGeometry();
		/// This method is used for lightning stage of deferred shading.
		virtual void renderLight();

	protected:
		virtual void renderPostProcessing();
	
	public:
		/** Creates DirectX 10.0 renderer and demanded number
			of windows to which rendering is done.
			@param
				renderWndsNum how many render target window are to
				be created
		*/
		RendererDX10(bool _windowed, uint _renderWndsNum=1, bool _multiThreaded=false);
		virtual ~RendererDX10();

		virtual void init();
		virtual void cleanup();


		virtual Window* createRenderWindow();
		virtual Window* createRenderWindow(const WINDOW_DESC& _desc);

		/** When the rendering device is lost, memory has to be freed and
			objects have to be reinitialized.
		*/
		virtual void cleanupManagedResources();

		virtual void reset();

		virtual DeviceRender& getDeviceRender() const {return m_pDeviceRender.getRef();}

		virtual void addToRender(Surface* _renderable, Material* _mat);

		virtual void beginRender();
		virtual void render();
		virtual void endRender();

		virtual void enumerateDevices();
		virtual void createDevice();
		virtual void createDevice(HWND* _hwnd);

		virtual void clearBuffer(dword _buffer);

		
		void showCursor(bool _value);
		void setWindowed(bool _value);


		void setWorldMatrix(const Matrix4x4& _mat);
		void setViewMatrix(const Matrix4x4& _mat);
		void setProjectionMatrix(const Matrix4x4& _mat);

		Matrix4x4 getWorldMatrix() const;
		Matrix4x4 getViewMatrix() const;
		Matrix4x4 getProjectionMatrix() const;

		void updateBackBuffer() {}
		ITexture* getBackBuffer() const {return NULL;}
		void setBackBuffer(ITexture* _texture) {}

		void setMaterial(Material& _material);

		void setLightEnabled(bool _value);
		void setAlphaTestEnabled(bool _value);
		void setAlphaBlendEnabled(bool _value);
		void setZWriteEnabled(bool _value);
		void setSourceBlending(BLEND_TYPE _mode);
		void setDestinationBlending(BLEND_TYPE _mode);
		void setCullingMode(CULL_MODE _cull);
		void setFillMode(FILL_MODE _fill);
		void setZBias(float _bias);
		void setAmbientLight(Colour& _ambient);
		void setTextureFilter(uint _sampler, dword _type, TEXTURE_FILTER _filter);
		void setAnisotropyLevel(uint _sampler, uint _anisotropy);

		void setUserClipPlane(uint _index, const Plane& _plane) {}

		API_TYPE getAPIType() const;
	};



	inline API_TYPE RendererDX10::getAPIType() const
	{
		return API_DIRECTX10;
	}
//----------------------------------------------------------------------



	/// Factory to be used for RendererDX10 class objects creation.
	class nGENEDLL RendererDX10Factory: public RendererFactory
	{
	public:
		Renderer* createRenderer(bool _windowed, uint _renderWndsNum=1,
			bool _multiThreading=false);
	};
}


#endif