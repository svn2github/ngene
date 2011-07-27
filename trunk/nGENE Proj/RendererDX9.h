/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RendererDX9.h
Version:	0.25
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RENDERERDX9_H_
#define __INC_RENDERERDX9_H_


#include "DeviceRenderDX9.h"
#include "Listener.h"
#include "Prerequisities.h"
#include "Renderer.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Renderer using DirectX 9.0c SDK.
		@remarks
			This renderer uses deferred rendering chain and as such
			it used deferred shading specific render stages (Geometry-,
			Light- and Alpha- stages to be more specific).
	*/
	class nGENEDLL RendererDX9: public Renderer
	{
		EXPOSE_TYPE
	private:
		/// BackBuffer copy
		ITexture* m_pBackBuffer;

		/// Deferred shading material
		Material* m_pDeferred;

		/// Pointer to the Direct3D
		IDirect3D9* m_pD3D;

		/// Pointer to the Direct3D Device
		IDirect3DDevice9* m_pD3DDev;

		/// DirectX 9.0c rendering device
		AutoPointer <DeviceRenderDX9> m_pDeviceRender;

		/// Display parameters
		D3DPRESENT_PARAMETERS m_D3DParams;

		/// Swap chains - they are used if multiple
		/// windows are to be used in a windowed mode
		vector <IDirect3DSwapChain9*> m_pSwapChains;

		/** Alpha rendering stage (necessary as we are using 
			deferred shading).
		*/
		AlphaStage* m_pAlphaStage;

		/// Stage for rendering post process effects
		PostStage* m_pPostStage;

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
		virtual void setRenderState(D3DRENDERSTATETYPE _rs, int _value) const;


		virtual void setCursor(HCURSOR _cursor);
	
	public:
		/** Creates DirectX 9.0c renderer and demanded number
			of windows to which rendering is done.
			@param
				renderWndsNum how many render target window are to
				be created
		*/
		RendererDX9(bool _windowed, uint _renderWndsNum=1, bool _multiThreaded=false);
		virtual ~RendererDX9();

		virtual void init();
		virtual void cleanup();


		virtual Window* createRenderWindow();
		virtual Window* createRenderWindow(const WINDOW_DESC& _desc);

		/** When the rendering device is lost, memory has to be freed and
			objects have to be reinitialized.
		*/
		virtual void cleanupDefaultResources();

		virtual void reset();

		virtual DeviceRender& getDeviceRender() const {return m_pDeviceRender.getRef();}

		virtual void beginRender();
		virtual void render();
		virtual void endRender();

		virtual void enumerateDevices();
		virtual void createDevice();
		virtual void createDevice(HWND* _hwnd);

		virtual void clearBuffer(dword _buffer);


		void addToRender(Surface* _renderable, Material* _mat);

		void updateBackBuffer();
		ITexture* getBackBuffer() const;
		void setBackBuffer(ITexture* _texture);

		
		void showCursor(bool _value);
		void setWindowed(bool _value);

		void setVSync(bool _value);


		void setWorldMatrix(const Matrix4x4& _mat);
		void setViewMatrix(const Matrix4x4& _mat);
		void setProjectionMatrix(const Matrix4x4& _mat);

		Matrix4x4 getWorldMatrix() const;
		Matrix4x4 getViewMatrix() const;
		Matrix4x4 getProjectionMatrix() const;

		void setMaterial(Material& _material);

		void setDisplayMode(SDisplayMode& _mode);

		void setLightEnabled(bool _value);
		void setAlphaTestReference(dword _value);
		void setAlphaTestEnabled(bool _value);
		void setSeparateAlphaBlendEnabled(bool _value);
		void setAlphaBlendEnabled(bool _value);
		void setZWriteEnabled(bool _value);
		void setZEnabled(bool _value);
		void setSourceBlending(BLEND_TYPE _mode);
		void setBlendOperation(BLEND_OPERATION _operation);
		void setDestinationBlending(BLEND_TYPE _mode);
		void setSeparateAlphaSourceBlending(BLEND_TYPE _mode);
		void setSeparateAlphaDestinationBlending(BLEND_TYPE _mode);
		void setCullingMode(CULL_MODE _cull);
		void setFillMode(FILL_MODE _fill);
		void setZBias(float _bias);
		void setAmbientLight(Colour& _ambient);
		void setTextureFilter(uint _sampler, dword _type, TEXTURE_FILTER _filter);
		void setTextureAddressingMode(uint _sampler, TEXTURE_ADDRESSING_MODE _mode);
		void setAnisotropyLevel(uint _sampler, uint _anisotropy);
		void setScissorTestEnabled(bool _value);
		void setScissorRectangle(SRect <uint>& _rect);
		void setClippingPlanesFlag(uint _flag);

		/** Sets user clipping plane.
			@param _index index of the clipping plane to set.
			@param _plane clipping plane. Has to be transformed to clipping
				   space, i.e. transformed by inverse transpose of combined
				   view and projection matrix.
		*/
		void setUserClipPlane(uint _index, const Plane& _plane);

		API_TYPE getAPIType() const;
	};



	inline API_TYPE RendererDX9::getAPIType() const
	{
		return API_DIRECTX9C;
	}
//----------------------------------------------------------------------



	/// Factory to be used for RendererDX9 class objects creation.
	class nGENEDLL RendererDX9Factory: public RendererFactory
	{
	public:
		Renderer* createRenderer(bool _windowed, uint _renderWndsNum=1,
			bool _multiThreading=false);
	};
}


#endif