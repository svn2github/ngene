/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Renderer.h
Version:	0.21
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RENDERER_H_
#define __INC_RENDERER_H_


#include "AutoPointer.h"
#include "BaseClass.h"
#include "Colour.h"
#include "IRenderable.h"
#include "FrameListener.h"
#include "Prerequisities.h"
#include "RenderQueue.h"
#include "RendererTypes.h"
#include "SceneManager.h"
#include "Singleton.h"
#include "TypeInfo.h"
#include "UtilitiesTypes.h"
#include "FastDelegate.h"


namespace nGENE
{
	/** Renders 3D graphics on the screen.
		@par
			This class, implemented as an adapter, handles all the rendering
			done by the engine both 3D and 2D (however 2D rendering is still
			3D rendering with specific transformations applied - but hell, in
			the end - everything on the screen is 2D :) ). Although client cannot
			directly affect rendering process differently than by subclassing
			this class,	she can add FrameListener which lets her set it up.
		@par
			Note that this class has to be subclassed to provide any rendering
			functionality depending on the chosen graphical API.
		@par
			User can override default rendering scheme either by subclassing
			appropriate Renderer class or by binding onRender delegate. If it
			is set it will be called instead of regular routine.
	*/
	class nGENEDLL Renderer: public BaseClass, public IRenderable,
		public Singleton <Renderer>
	{
		EXPOSE_TYPE
	public:
		/// Some delegates
		typedef fastdelegate::FastDelegate0 <> ON_RENDER_DELEGATE;

		ON_RENDER_DELEGATE onRender;

	private:
		// Matrices used to restore 3D mode after changing to 2D
		Matrix4x4* m_matViewOld;
		Matrix4x4* m_matProjOld;
		Matrix4x4* m_matWorldOld;
		uint m_nClippingFlagOld;
		

	protected:
		/// Initial number of windows to which rendering is performed
		uint m_nRenderWindows;

		/// How many viewports are being used
		uint m_nViewportsNum;

		/** Background colour being visible due to the fact of clearing the buffers
			@remarks
				Clear colour may not work with some full-screen post-processing
				effects.
		*/
		Colour m_ClearColour;

		/** Frame listeners.
			@remarks
				Note that multiple frame listeners can be used at once.
		*/
		vector <AutoPointer <FrameListener> > m_vFrameListeners;

		/// State manager
		AutoPointer <StateManager> m_pStateManager;

		/** Singleton VertexBufferManager.
			@remarks
				You have to call its init() method after initializing the
				rendering device.
		*/
		AutoPointer <VertexBufferManager> m_pVBManager;
		
		/// ShaderManager singleton.
		AutoPointer <ShaderManager> m_pShaderManager;
		
		/// Singleton MaterialManager
		AutoPointer <MaterialManager> m_pMaterialManager;

		/// Singleton TextureManager
		AutoPointer <TextureManager> m_pTextureManager;

		/** Dynamically allocated array of windows. Notice that multiple windows
			could be used in windowed mode only.
		*/
		vector <Window*> m_pWindow;

		/// Currently set display mode
		SDisplayMode m_DisplayMode;

		/** Hash table containing all display modes available
			on the current adapter, being differentiated by
			the width of the screen.
		*/
		HashTable <uint, SDisplayMode> m_DisplayModes;

		HashTable <wstring, Material*> m_pMaterials;

		/// Vector of all render stages.
		vector <RenderStage*> m_vRenderStages;
		/// HashTable with all render stages.
		HashTable <wstring, RenderStage*> m_RenderStages;


		vector <NodeLight*> m_vLights;
		NodeLight* m_pActiveLight;

		Material* m_pActiveMaterial;								///< Material being currently rendered

		/// Is profiling enabled (if available)?
		bool m_bProfile;
		/// Is multi-threaded flag enabled?
		bool m_bMultiThreaded;


		/** Determines which window has to be used as the render
			target.
			@param
				index index of the window to be used.
		*/
		virtual void useWindow(uint index)=0;

	public:
		Renderer(bool _multiThreaded=false);
		virtual ~Renderer();


		virtual void init();
		virtual void cleanup();

		/** When the rendering device is lost, memory has to be freed and
			objects have to be reinitialized.
		*/
		virtual void cleanupDefaultResources() {}


		/// Creates new rendering window.
		virtual Window* createRenderWindow()=0;
		/** Creates new rendering window.
			@param _desc description of the window.
		*/
		virtual Window* createRenderWindow(const WINDOW_DESC& _desc)=0;


		/** In some cases there might be need to reset renderer. For instance
			changing the device or resolution could not be performed on the
			fly - it requires reinitialization of some basic objects.
		*/
		virtual void reset()=0;

		/** Prepares nGENE for rendering process by setting all necessary
			rendering parameters, sorting the materials and so on. This
			process can be extended by using FrameListener onFrameBegin()
			method.
		*/
		virtual void beginRender();
		/** Performs real rendering operations. There is no way to alter
			this operation but to subclass Renderer and overload this
			function.
		*/
		virtual void render()=0;
		/** Finishes the rendering process, resets engine variables and
			restores its state from time before call to beginRender() function.
			This process can be extended by using FrameListener onFrameEnd()
			method.
		*/
		virtual void endRender();

		/** Enumerates all devices and display modes available in the system
			so the most appropriate one could be chosen either by the engine
			or the end-user.
		*/
		virtual void enumerateDevices()=0;
		/** Creates device supporting chosen 3D API.
		*/
		virtual void createDevice()=0;

		/** Clears specified buffers. Any buffer combination can be cleared
			at the time.
		*/
		virtual void clearBuffer(dword _buffer)=0;
		/** Shows or hides hardware supported cursor.
			@param
				value if true the cursor is visible, otherwise - not.
		*/
		virtual void showCursor(bool _value)=0;


		/** Changes rendering mode to 2D mode.
			@remarks
				In 2D mode all perspective information are lost. Besides position
				of the vertices is defined in screen space.
		*/
		void set2DMode();
		/// Restores 3D mode
		void restore3DMode();

		virtual void updateBackBuffer()=0;
		virtual ITexture* getBackBuffer() const=0;
		virtual void setBackBuffer(ITexture* _texture)=0;


		/// Sets the new number of windows to which rendering is done.
		void setRenderWindowsNum(uint _num) {m_nRenderWindows=_num;}

		/** Sets the colour of the background which appears when
			the Z-buffer is being cleared.
		*/
		void setClearColour(const Colour& _colour);

		/** Sets the colour of the background which appears when
			the Z-buffer is being cleared.
		*/
		void setClearColour(dword _colour);

		/** Sets the colour of the background which appears when
			the Z-buffer is being cleared.
		*/
		void setClearColour(byte r, byte g, byte b);

		/** Returns all display modes available on the current adapter.
			@returns
				HashTable returns HashTable containing all display modes
		*/
		virtual HashTable <uint, SDisplayMode> getAvailableDisplayModes() const {return m_DisplayModes;}
		virtual void setDisplayMode(SDisplayMode& _mode);
		const SDisplayMode& getDisplayMode() const;

		/// Sets gamma value.
		virtual void setGamma(float _value);
		/// Returns the gamma aspect of the running application.
		float getGamma() const;

		/** Sets the mode in which application is running.
			@param
				value if is set to true (1) than application will run in
				windowed mode. If it is false (0) - in full-screen mode.
				Notice, however that the windowed mode is the only which
				can take advantage from rendering to multiple separate
				windows.
		*/
		virtual void setWindowed(bool _value);
		/// Checks if the app is running in a windowed mode.
		bool getWindowed() const;

		/// Enables or disables vertical synchronization (vsync).
		virtual void setVSync(bool _value);
		/// Checks if vsync is enabled.
		bool getVSync() const;

		/// Returns given window.
		virtual Window* getWindow(uint _index) const;

		/** Returns the number of viewports being used.
			@remarks
				Note that apart from using rendering to multiple windows,
				one can also render to the same window, but to its
				different areas
		*/
		uint getViewportsNum() const;

		/** Returns the rendering device used by the renderer.
			@return
				DeviceRender& reference to the DeviceRender in use.
		*/
		virtual DeviceRender& getDeviceRender() const=0;

		/** Returns FrameListener with the given index.
			@param
				_index index of a listener.
		*/
		AutoPointer <FrameListener> getFrameListener(uint _index) const;

		/** Registers new frame listener in the renderer. This way you
			can easily handle incoming frame events.
			@param
				_listener pointer to the FrameListener object. Note that this
				object should be allocated on the heap as internally smart pointers
				are used, and trying to release them might result in heap corruption.
		*/
		virtual void addFrameListener(FrameListener* listener);


		/** Sets the world matrix.
			@remarks
				In order to render a node in the desired place, it has to
				call this function providing its transformation matrix.
		*/
		virtual void setWorldMatrix(const Matrix4x4& _mat)=0;
		/// Returns world matrix.
		virtual Matrix4x4 getWorldMatrix() const=0;

		/** Sets the view matrix.
			@remarks
				This function is required to represent camera in the scene.
		*/
		virtual void setViewMatrix(const Matrix4x4& _mat)=0;
		/// Returns view matrix.
		virtual Matrix4x4 getViewMatrix() const=0;

		/** Sets the projection matrix.
			@remarks
				This function is required to represent camera in the scene.
		*/
		virtual void setProjectionMatrix(const Matrix4x4& _mat)=0;
		/// Returns projection matrix.
		virtual Matrix4x4 getProjectionMatrix() const=0;


		/// Binds the material
		virtual void setMaterial(Material& _material);
		Material* getActiveMaterial() const;


		/// Returns current render state
		RENDER_STATE& getRenderState();


		/// Sets lights on or off.
		virtual void setLightEnabled(bool _value);
		/// Are light enabled.
		bool isLightEnabled() const;

		/// Sets alpha test.
		virtual void setAlphaTestEnabled(bool _value);
		/// Checks if alpha test is enabled.
		bool isAlphaTestEnabled() const;

		/// Sets reference value for alpha testing.
		virtual void setAlphaTestReference(dword _value);
		/// Returns reference value for alpha test.
		dword getAlphaTestReference() const;

		/// Specifies if alpha blending is enabled.
		virtual void setAlphaBlendEnabled(bool _value);
		/// Checks if alpha blending is enabled.
		bool isAlphaBlendEnabled() const;

		virtual void setSeparateAlphaBlendEnabled(bool _value);
		bool isSeparateAlphaBlendEnabled() const;

		/// Sets source blending mode.
		virtual void setSourceBlending(BLEND_TYPE _mode);
		/// Sets destination blending mode.
		virtual void setDestinationBlending(BLEND_TYPE _mode);
		/// Returns source blending mode.
		BLEND_TYPE getSourceBlending() const;
		/// Returns destination blending mode.
		BLEND_TYPE getDestinationBlending() const;

		/// Sets blending operation.
		virtual void setBlendOperation(BLEND_OPERATION _operation);
		/// Returns blending operation.
		BLEND_OPERATION getBlendOperation() const;

		virtual void setSeparateAlphaSourceBlending(BLEND_TYPE _mode);
		virtual void setSeparateAlphaDestinationBlending(BLEND_TYPE _mode);
		BLEND_TYPE getSeparateAlphaSourceBlending() const;
		BLEND_TYPE getSeparateDestinationBlending() const;

		/// Enables/disables writes to the depth buffer.
		virtual void setZWriteEnabled(bool _value);
		/// Checks if writing to the depth buffer is enabled.
		bool isZWriteEnabled() const;

		/// Specifies if Z-buffer is enabled.
		virtual void setZEnabled(bool _value);
		/// Checks if Z-buffer is enabled.
		bool isZEnabled() const;

		/// Sets culling mode.
		virtual void setCullingMode(CULL_MODE _cull);
		/// Returns current culling mode.
		CULL_MODE getCullingMode() const;

		/// Sets fill mode.
		virtual void setFillMode(FILL_MODE _fill);
		/// Returns fill mode.
		FILL_MODE getFillMode() const;

		/** When two or more triangle are co-planar artifacts caused by Z-fighting
			may occur (in the worst scenario some of them will not be visible at all).
			To avoid this, additional depth value has to be added to some of them to
			ensure right rendering order.
			@param
				_bias a depth bias to apply ranging from 0 to 16 inclusive.
		*/
		virtual void setZBias(float _bias);
		/// Returns Z offset value.
		float getZBias() const;

		virtual void setAmbientLight(Colour& _ambient) {m_DisplayMode.state.ambient=_ambient;}
		Colour& getAmbientLight() {return m_DisplayMode.state.ambient;}

		/** Sets basic texture filtering.
			@remarks
				Note that you cannot set FILTER_ANISOTROPIC to the FILTER_MIP (standing
				for mip-maps) as it is not supported by DirectX.
			@par
				Setting FILTER_NONE is valid only for FILTER_MIP and in that case instead
				of using mip-maps magnification filter will be used.
		*/
		virtual void setTextureFilter(uint _sampler, dword _type, TEXTURE_FILTER _filter);
		/// Returns texture filtering mode for a given sampler.
		TEXTURE_FILTER getTextureFilter(uint _sampler, FILTER_TYPE _type) const;

		/// Sets texture addressing mode.
		virtual void setTextureAddressingMode(uint _sampler, TEXTURE_ADDRESSING_MODE _mode);
		/// Returns texture addressing mode.
		TEXTURE_ADDRESSING_MODE getTextureAddressingMode(uint _sampler) const;

		/// Sets anisotropic filtering level.
		virtual void setAnisotropyLevel(uint _sampler, uint _anisotropy);
		/// Returns anisotropic filtering level.
		uint getAnisotropyLevel(uint _sampler) const;

		/// Specifies if scissor test is enabled.
		virtual void setScissorTestEnabled(bool _value);
		/// Checks if scissor test is enabled.
		bool isScissorTestEnabled() const;

		/// Sets scissor test rectangle.
		virtual void setScissorRectangle(SRect <uint>& _rect);
		/// Returns scissor test rectangle.
		SRect <uint>& getScissorRectangle();

		/** Disables or enables specified clipping planes.
			@param
				_flag if a bit in this flag is set to 1, corresponding clipping plane is
				enabled. If 0 - disabled. For convenience you can use any combination of
				PLANE_n_ENABLED where n ranges 0 to 5. To disable all planes use PLANE_NONE value.
		*/
		virtual void setClippingPlanesFlag(uint _flag);
		/// Returns clipping planes flag.
		uint getClippingPlanesFlag();

		/** Adds surface to the appropriate render queue.
			@remarks
				Prior to rendering a surface it has to be added to the render queue
				related to its material. Otherwise surface will remain invisible.
		*/
		virtual void addToRender(Surface* _renderable, Material* _mat)=0;

		/// Adds light to the renderer.
		void addLight(NodeLight* _light);
		/// Removes given light from renderer.
		void removeLight(NodeLight* _light);
		/// Removes all lights.
		void removeLights();
		/// Returns currently processed light.
		NodeLight* getActiveLight() const;
		void setActiveLight(NodeLight* _light);
		vector <NodeLight*>* getLights() {return &m_vLights;}

		/// Resets rendering states.
		virtual void resetRenderStates();


		/// Adds render stage to the rendering chain.
		virtual void addRenderStage(const wstring& _name, RenderStage* _stage);
		/// Removes render stage from the rendering chain.
		virtual void removeRenderStage(const wstring& _name);
		/// Returns render stage with the given index.
		RenderStage* getRenderStage(uint _index) const;
		/// Returns render stage with the given name.
		RenderStage* getRenderStage(const wstring& _name);

		/** Returns index of the given stage.
			@param
				_stage RenderStage for which index has to be find.
			@returns
				int index of the given stage is greater of equal 0 if a stage exists 
				or -1 otherwise.
		*/
		int getRenderStageIndex(const RenderStage* _stage);
		/// Returns number of stages.
		uint getRenderStagesNum() const;
		/// Sorts render stages based on their priorities.
		void sortStages();
		/// Sorts lights based on their shadow resolution.
		void sortLights();


		/** Computes light scissor rectangle as described by Eric Lengyel
			and Fabio Policarpo. */
		void setLightScissor(NodeLight& _light);

		/// Sets user clipping plane.
		virtual void setUserClipPlane(uint _index, const Plane& _plane)=0;

		/// Sets if graphics profiler should be used.
		void setProfile(bool _value);

		/// Specifies if a given state is a global state.
		void setGlobalState(GLOBAL_STATE _state, bool _value);
		/// Checks if specified state is global state.
		bool isGlobalState(GLOBAL_STATE _state) const;

		/// Sets global states mask.
		void setGlobalStatesMask(uint _mask);
		/// Returns global states mask.
		uint getGlobalStatesMask() const;

		virtual API_TYPE getAPIType() const=0;
	};



	inline void Renderer::setZWriteEnabled(bool _value)
	{
		if(!isGlobalState(GS_ZWRITE))
			m_DisplayMode.state.zWrite = _value;
	}
//----------------------------------------------------------------------
	inline bool Renderer::isZWriteEnabled() const
	{
		return m_DisplayMode.state.zWrite;
	}
//----------------------------------------------------------------------
	inline void Renderer::setZEnabled(bool _value)
	{
		if(!isGlobalState(GS_ZBUFFER))
			m_DisplayMode.state.zBuffer = _value;
	}
//----------------------------------------------------------------------
	inline bool Renderer::isZEnabled() const
	{
		return m_DisplayMode.state.zBuffer;
	}
//----------------------------------------------------------------------
	inline void Renderer::setCullingMode(CULL_MODE _cull)
	{
		if(!isGlobalState(GS_CULLING))
			m_DisplayMode.state.culling = _cull;
	}
//----------------------------------------------------------------------
	inline CULL_MODE Renderer::getCullingMode() const
	{
		return m_DisplayMode.state.culling;
	}
//----------------------------------------------------------------------
	inline uint Renderer::getAnisotropyLevel(uint _sampler) const
	{
		return m_DisplayMode.anisotropyLevel[_sampler];
	}
//----------------------------------------------------------------------
	inline NodeLight* Renderer::getActiveLight() const
	{
		return m_pActiveLight;
	}
//----------------------------------------------------------------------
	inline void Renderer::setActiveLight(NodeLight* _light)
	{
		m_pActiveLight = _light;
	}
//----------------------------------------------------------------------
	inline uint Renderer::getClippingPlanesFlag()
	{
		return m_DisplayMode.clippingPlanesFlag;
	}
//----------------------------------------------------------------------
	inline SRect <uint>& Renderer::getScissorRectangle()
	{
		return m_DisplayMode.scissorRectangle;
	}
//----------------------------------------------------------------------
	inline bool Renderer::isScissorTestEnabled() const
	{
		return m_DisplayMode.state.scissorTest;
	}
//----------------------------------------------------------------------
	inline void Renderer::setZBias(float _bias)
	{
		if(!isGlobalState(GS_ZBIAS))
			m_DisplayMode.state.zBias = _bias;
	}
//----------------------------------------------------------------------
	inline float Renderer::getZBias() const
	{
		return m_DisplayMode.state.zBias;
	}
//----------------------------------------------------------------------
	inline void Renderer::setFillMode(FILL_MODE _fill)
	{
		if(!isGlobalState(GS_FILLMODE))
			m_DisplayMode.state.fillMode = _fill;
	}
//----------------------------------------------------------------------
	inline FILL_MODE Renderer::getFillMode() const
	{
		return m_DisplayMode.state.fillMode;
	}
//----------------------------------------------------------------------
	inline void Renderer::setSeparateAlphaSourceBlending(BLEND_TYPE _mode)
	{
		if(!isGlobalState(GS_ALPHABLENDSOURCE))
			m_DisplayMode.state.alphaBlendSource = _mode;
	}
//----------------------------------------------------------------------
	inline void Renderer::setSeparateAlphaDestinationBlending(BLEND_TYPE _mode)
	{
		if(!isGlobalState(GS_ALPHABLENDDESTINATION))
			m_DisplayMode.state.alphaBlendDestination = _mode;
	}
//----------------------------------------------------------------------
	inline BLEND_TYPE Renderer::getSeparateAlphaSourceBlending() const
	{
		return m_DisplayMode.state.alphaBlendSource;
	}
//----------------------------------------------------------------------
	inline BLEND_TYPE Renderer::getSeparateDestinationBlending() const
	{
		return m_DisplayMode.state.alphaBlendDestination;
	}
//----------------------------------------------------------------------
	inline bool Renderer::isLightEnabled() const
	{
		return m_DisplayMode.state.light;
	}
//----------------------------------------------------------------------
	inline void Renderer::setAlphaTestEnabled(bool _value)
	{
		if(!isGlobalState(GS_ALPHATEST))
			m_DisplayMode.state.isAlphaTest = _value;
	}
//----------------------------------------------------------------------
	inline bool Renderer::isAlphaTestEnabled() const
	{
		return m_DisplayMode.state.isAlphaTest;
	}
//----------------------------------------------------------------------
	inline void Renderer::setAlphaTestReference(dword _value)
	{
		if(!isGlobalState(GS_ALPHATESTREF))
			m_DisplayMode.state.alphaTestRef = _value;
	}
//----------------------------------------------------------------------
	inline dword Renderer::getAlphaTestReference() const
	{
		return m_DisplayMode.state.alphaTestRef;
	}
//----------------------------------------------------------------------
	inline void Renderer::setAlphaBlendEnabled(bool _value)
	{
		if(!isGlobalState(GS_ALPHABLEND))
			m_DisplayMode.state.alphaBlend = _value;
	}
//----------------------------------------------------------------------
	inline bool Renderer::isAlphaBlendEnabled() const
	{
		return m_DisplayMode.state.alphaBlend;
	}
//----------------------------------------------------------------------
	inline void Renderer::setSeparateAlphaBlendEnabled(bool _value)
	{
		if(!isGlobalState(GS_SEPARATEALPHABLEND))
			m_DisplayMode.state.separateAlphaBlend = _value;
	}
//----------------------------------------------------------------------
	inline bool Renderer::isSeparateAlphaBlendEnabled() const
	{
		return m_DisplayMode.state.separateAlphaBlend;
	}
//----------------------------------------------------------------------
	inline void Renderer::setSourceBlending(BLEND_TYPE _mode)
	{
		if(!isGlobalState(GS_SOURCE))
			m_DisplayMode.state.source = _mode;
	}
//----------------------------------------------------------------------
	inline void Renderer::setDestinationBlending(BLEND_TYPE _mode)
	{
		if(!isGlobalState(GS_DESTINATION))
			m_DisplayMode.state.destination = _mode;
	}
//----------------------------------------------------------------------
	inline BLEND_TYPE Renderer::getSourceBlending() const
	{
		return m_DisplayMode.state.source;
	}
//----------------------------------------------------------------------
	inline BLEND_TYPE Renderer::getDestinationBlending() const
	{
		return m_DisplayMode.state.destination;
	}
//----------------------------------------------------------------------
	inline void Renderer::setBlendOperation(BLEND_OPERATION _operation)
	{
		if(!isGlobalState(GS_BLENDOP))
			m_DisplayMode.state.blendOP = _operation;
	}
//----------------------------------------------------------------------
	inline BLEND_OPERATION Renderer::getBlendOperation() const
	{
		return m_DisplayMode.state.blendOP;
	}
//----------------------------------------------------------------------
	inline Material* Renderer::getActiveMaterial() const
	{
		return m_pActiveMaterial;
	}
//----------------------------------------------------------------------
	inline RENDER_STATE& Renderer::getRenderState()
	{
		return m_DisplayMode.state;
	}
//----------------------------------------------------------------------
	inline uint Renderer::getViewportsNum() const
	{
		return m_nViewportsNum;
	}
//----------------------------------------------------------------------
	inline void Renderer::setWindowed(bool _value)
	{
		m_DisplayMode.windowed = _value;
	}
//----------------------------------------------------------------------
	inline bool Renderer::getWindowed() const
	{
		return m_DisplayMode.windowed;
	}
//----------------------------------------------------------------------
	inline void Renderer::setVSync(bool _value)
	{
		m_DisplayMode.vsync = _value;
	}
//----------------------------------------------------------------------
	inline bool Renderer::getVSync() const
	{
		return m_DisplayMode.vsync;
	}
//----------------------------------------------------------------------
	inline void Renderer::setDisplayMode(SDisplayMode& _mode)
	{
		m_DisplayMode = _mode;
	}
//----------------------------------------------------------------------
	inline const SDisplayMode& Renderer::getDisplayMode() const
	{
		return m_DisplayMode;
	}
//----------------------------------------------------------------------
	inline void Renderer::setGamma(float _value)
	{
		if(!isGlobalState(GS_GAMMA))
			m_DisplayMode.state.gamma = _value;
	}
//----------------------------------------------------------------------
	inline float Renderer::getGamma() const
	{
		return m_DisplayMode.state.gamma;
	}
//----------------------------------------------------------------------
	inline void Renderer::setGlobalState(GLOBAL_STATE _state, bool _value)
	{
		if(_value)
			m_DisplayMode.state.globalStates |= _state;
		else
			m_DisplayMode.state.globalStates &= ~_state;
	}
//----------------------------------------------------------------------
	inline void Renderer::setGlobalStatesMask(uint _mask)
	{
		m_DisplayMode.state.globalStates = _mask;
	}
//----------------------------------------------------------------------
	inline bool Renderer::isGlobalState(GLOBAL_STATE _state) const
	{
		return ((m_DisplayMode.state.globalStates & _state) ? true : false);
	}
//----------------------------------------------------------------------
	inline uint Renderer::getGlobalStatesMask() const
	{
		return m_DisplayMode.state.globalStates;
	}
//----------------------------------------------------------------------



	/// Factory to be used for Renderer class objects creation.
	class nGENEDLL RendererFactory
	{
	public:
		virtual Renderer* createRenderer(bool _windowed, uint _renderWndsNum=1,
			bool _multiThreading=false)=0;
	};
}


#endif