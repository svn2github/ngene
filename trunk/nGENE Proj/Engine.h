/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Engine.h
Version:	0.20
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ENGINE_H_
#define __INC_ENGINE_H_


#ifdef _WIN32
	#define NOMINMAX
    #ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0400
	#endif
	#include <windows.h>
#endif


#include "AutoPointer.h"
#include "CPUInfo.h"
#include "EngineUpdateListener.h"
#include "FastDelegate.h"
#include "Physics.h"
#include "Prerequisities.h"
#include "Renderer.h"
#include "Singleton.h"
#include "Task.h"


namespace nGENE
{
	/// Camera update task.
	class nGENEDLL UpdateCameraTask: public Task
	{
	public:
		void update();
	};


	/** Configuration of the engine.
	*/
	typedef struct nGENEDLL SEngineInfo
	{
		API_TYPE RenderingAPI;				///< Which API is used for rendering
		uint renderWindowsNum;				///< How many windows are used in rendering
		int threadsNum;						///< Number of threads used

		SDisplayMode DisplayMode;			///< Engine's DisplayMode

		wstring soundLibrary;				///< Name of the sound library
		wstring renderer;					///< Name of the renderer

		bool multiThreadedRendering;		/**< Is multi-threading rendering enabled?
												 It can result in performance penalty. */

		SEngineInfo():
			RenderingAPI(API_DIRECTX9C),
			renderWindowsNum(1),
			threadsNum(-1),
			soundLibrary(L"FMOD"),
			renderer(L"DirectX 9.0"),
			multiThreadedRendering(false)
		{
		}

		~SEngineInfo() {}
	} ENGINE_DESC;


	/** It is a key nGENE Tech class.
		@par
			It holds responsibility for initializing, updating the engine and releasing
			resources at exit. Client can also access all of the engine objects via this
			interface and change their settings and behaviour.
		@remarks
			Engine is defined as a Singleton and therefore any attempts to create more than
			one	object of this class will result in assertion arising.
		@par
			Init() function has to be called immediately after creating the Engine class
			object to start the Engine properly. One can also specify custom initialization
			behaviour by binding onInit delegate. If it is set it will be called instead of
			regular nGENE Tech Engine class Init() routine.
	*/
	class nGENEDLL Engine: public Singleton <Engine>
	{
	public:
		/// Some delegates
		typedef fastdelegate::FastDelegate0 <> ON_INIT_DELEGATE;

		ON_INIT_DELEGATE onInit;
		ON_INIT_DELEGATE onUpdate;

	private:

#ifdef _WIN32
		HWND m_TopLevelWindow;						///< Handle to the top-level window
#endif

		CPUInfo m_CpuInfo;

		static bool s_bUseSIMD;						///< SIMD extensions are used

		bool m_bUpdateFinished;

		/// TaskManager object
		AutoPointer <TaskManager> m_pTaskManager;

		/// FileManager object
		AutoPointer <FileManager> m_pFileManager;
		/// Timer - notice that it starts counting from the application start
		AutoPointer <Timer> m_pTimer;
		/// Profiler class object
		AutoPointer <Profiler> m_pProfiler;
		/// XML Parser
		AutoPointer <XMLParser> m_pXMLParser;
		
		/// Renderer class object
		AutoPointer <Renderer> m_pRenderer;

		/// Physics class object
		AutoPointer <Physics> m_pPhysics;

		/// GUIManager object
		AutoPointer <GUIManager> m_pGUIManager;

		/// Factory for scene managers.
		AutoPointer <SceneManagerFactory> m_pSceneManagerFactory;

		/// InputSystem class object
		AutoPointer <InputSystem> m_pInputSystem;

		/// Lua script system
		AutoPointer <ScriptSystemLua> m_pLua;

		/// Sound system enumerator
		SoundManagerEnumerator* m_pSoundManagerEnumerator;
		/// Renderer enumerator
		RendererEnumerator* m_pRendererEnumerator;

		/// Sound system
		AutoPointer <ISoundManager> m_pSoundManager;



		/// Engine settings
		AutoPointer <ENGINE_DESC> m_pEngineInfo;

		/** Engine update listeners.
			@remarks
				Note that multiple engine update listeners can be used at once.
		*/
		vector <AutoPointer <EngineUpdateListener> > m_vUpdateListeners;

		/** Scene managers vector. Multiple ones can be used.
		*/
		vector <AutoPointer <SceneManager> > m_pSceneManagers;

		/** Currently active camera.
			@remarks
				Although there could be as many cameras as you want in
				each scene manager (as they are scene-nodes) only one
				per view can be 
		*/
		Camera* m_pActiveCamera;

		/// Task updating camera.
		UpdateCameraTask m_UpdateCameraTask;
		DummyTask m_DummyTask;

	private:
		/// Retrieves info about CPU.
		virtual void InitCPU();

		/// Initializes built-in tools like eg. profiler or log.
		virtual void InitUtilities();

		/// Creates all objects related to rendering via Renderer creation.
		virtual void InitRenderer();

		/// Initializes Physics module.
		virtual void InitPhysics();

		/// Initializes sound system.
		virtual void InitSound();

		/// Initializes input system.
		virtual void InitUI();

		/** @todo
				Implement it. Networking is required.
		*/
		virtual void InitNetwork() {}

		/// Initializes task manager.
		virtual void InitTaskManager();

		/// Updates all engine modules using single-threading approach.
		virtual void UpdateST();

		/// Updates all engine modules using task management.
		virtual void UpdateMT();

	public:
		/** Constructs the engine.
			@param
				_api rendering API to be set on engine startup. As for now
				it cannot be changed on run-time.
			@param
				_renderWindows how many windows are used.
			@param
				_soundLibrary name of the sound library to use. -1 means that
				their number is found automatically.
		*/
		Engine(API_TYPE _api=API_DIRECTX9C,
			   uint _renderWindows=1,
			   const wstring& _soundLibrary=L"FMOD",
			   const wstring& _renderer=L"DirectX 9.0",
			   int _threadsNum=-1);
		/** The engine can also be constructed by direct passing of
			the EngineInfo.
		*/
		explicit Engine(ENGINE_DESC& _info);
		virtual ~Engine();

		/** Initializes all engine modules.
			@remarks
				This function should be called just after engine creation.
		*/
		virtual void Init();

		/** Updates all engine modules.
		*/
		virtual void Update();

		/** Releases resources.
		*/
		virtual void Cleanup();


		/// Returns reference to the current profiler.
		Profiler& getProfiler() const {return *(m_pProfiler.getPointer());}
		/** Returns reference to the timer.
			@remarks
				Engine's timer is an application level timer, as you
				can access it from any class of the engine.
		*/
		Timer& getTimer() const {return *(m_pTimer.getPointer());}
		Timer* getTimerPtr() const {return m_pTimer;}

		/// Returns reference to the current input system.
		InputSystem& getInputSystem() const {return *(m_pInputSystem.getPointer());}

		/// Returns reference to the current renderer.
		Renderer& getRenderer() const {return *(m_pRenderer.getPointer());}

		/** Lets you set new Engine's configuration.
		*/
		virtual void setEngineInfo(ENGINE_DESC& _info);

		API_TYPE getRenderingAPI() const {return m_pEngineInfo->RenderingAPI;}
		/** Changes current rendering API.
			@remarks
				API alteration results in Engine being resetted
				as all rendering settings needs to be changed.
			@param
				API new rendering API to be set.
		*/
		virtual void setRenderingAPI(const wstring& _api);

		/** Returns the number of windows to which rendering
			is performed.
			@remarks
				This feature could be used only in a windowed mode
				as there is no window defined in a full-screen mode.
		*/
		uint getRenderWindowsNum() const {return m_pEngineInfo->renderWindowsNum;}
		/** Lets you change the number of render windows.
		*/
		virtual void setRenderWindows(uint _count);

		/** Checks whether SIMD extensions are used.
			@return
				bool specifies whether SIMD extensions are used. Note that
				this value is true only if both processor and OS support
				them.
		*/
		static bool getUseSIMD();
		/** Turns on/off SIMD extensions.
			@remarks
				Sometimes it may be useful to disable SIMD extensions,
				even if they are supported by the platform. However
				turning them off on the platform not supporting them
				will fail.
		*/
		void setUseSIMD(bool _value);


		/// Gets information about CPU.
		const CPUInfo& getCPUInfo() const {return m_CpuInfo;}

		/// Adds new SceneManager.
		void addSceneManager(SceneManager* _manager);
		/// Returns scene manager with the given index.
		SceneManager* getSceneManager(uint _index) const;
		/// Returns number of SceneManager objects.
		uint getSceneManagersNum() const;

		/// Sets active camera.
		void setActiveCamera(Camera* _camera);
		void resetCameraAndFrustum();
		/// Checks if renderer has active camera set.
		bool hasActiveCamera();
		/// Returns active camera.
		Camera* getActiveCamera() const;

		bool hasUpdateFinished() const;

		/// Returns Lua scripting system.
		ScriptSystemLua* getLuaScripting() const;

		/// Adds new engine update listener.
		virtual void addUpdateListener(EngineUpdateListener* _listener);
		/// Remove given update listener.
		virtual void removeUpdateListener(uint _index);
		/// Remove all update listeners.
		virtual void removeAllUpdateListeners();

#ifdef _WIN32
		void setTopLevelWndHandle(HWND _window) {m_TopLevelWindow=_window;}
		HWND& getTopLevelWndHandle() {return m_TopLevelWindow;}
#endif
	};



	inline uint Engine::getSceneManagersNum() const
	{
		return m_pSceneManagers.size();
	}
//----------------------------------------------------------------------
	inline bool Engine::hasActiveCamera()
	{
		return (m_pActiveCamera ? true : false);
	}
//----------------------------------------------------------------------
	inline bool Engine::getUseSIMD()
	{
		return s_bUseSIMD;
	}
//----------------------------------------------------------------------
	inline bool Engine::hasUpdateFinished() const
	{
		return m_bUpdateFinished;
	}
//----------------------------------------------------------------------
}


#endif