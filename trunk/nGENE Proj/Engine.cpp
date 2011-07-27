/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Engine.cpp
Version:	0.19
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Engine.h"
#include "Camera.h"
#include "DeviceRender.h"
#include "Frustum.h"
#include "GUIManager.h"
#include "InputSystemDX9.h"
#include "ISoundManager.h"
#include "RendererEnumerator.h"
#include "SceneManagerFactory.h"
#include "ScriptSystemLua.h"
#include "SoundManagerEnumerator.h"
#include "Task.h"
#include "TaskManager.h"
#include "Window.h"
#include "XMLParser.h"


namespace nGENE
{
	// Initialize static members
	bool Engine::s_bUseSIMD = false;


	Engine::Engine(API_TYPE _api, uint _renderWindows, const wstring& _soundLibrary,
		const wstring& _renderer, int _threadsNum):
		m_bUpdateFinished(true),
		m_pTaskManager(new TaskManager()),
		m_pFileManager(new FileManager()),
		m_pTimer(new Timer()),
		m_pProfiler(new Profiler()),
		m_pXMLParser(new XMLParser()),
		m_pEngineInfo(new SEngineInfo()),
		m_pActiveCamera(NULL),
		m_pSoundManagerEnumerator(new SoundManagerEnumerator()),
		m_pRendererEnumerator(new RendererEnumerator()),
		onInit(NULL),
		onUpdate(NULL)
	{
		m_pEngineInfo->RenderingAPI = _api;
		m_pEngineInfo->renderWindowsNum = _renderWindows;
		m_pEngineInfo->threadsNum = _threadsNum;
		m_pEngineInfo->soundLibrary = _soundLibrary;
		m_pEngineInfo->renderer = _renderer;
	}
//----------------------------------------------------------------------
	Engine::Engine(ENGINE_DESC& _info):
		m_bUpdateFinished(true),
		m_pTaskManager(new TaskManager()),
		m_pFileManager(new FileManager()),
		m_pTimer(new Timer()),
		m_pProfiler(new Profiler()),
		m_pXMLParser(new XMLParser()),
		m_pEngineInfo(new SEngineInfo(_info)),
		m_pActiveCamera(NULL),
		m_pSoundManagerEnumerator(new SoundManagerEnumerator()),
		m_pRendererEnumerator(new RendererEnumerator()),
		onInit(NULL),
		onUpdate(NULL)
	{
	}
//----------------------------------------------------------------------
	Engine::~Engine()
	{
		Cleanup();
	}
//----------------------------------------------------------------------
	void Engine::Init()
	{
		if(onInit)
		{
			onInit();
		}
		else
		{
			// Create utilities
			InitUtilities();

			// Create renderer
			InitRenderer();

			// Create physics
			InitPhysics();

			// Create sound system
			InitSound();

			// Create user interface
			InitUI();

			// Create networking
			InitNetwork();

			// Set up task manager
			InitTaskManager();
		}
	}
//----------------------------------------------------------------------
	void Engine::InitUtilities()
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__,	__LINE__, L"Initializing utilities");

		// Check for CPU extensions
		InitCPU();

		// Init scripting
		m_pLua = new ScriptSystemLua();
		m_pLua->init();

		srand(m_pTimer->getMilliseconds());
	}
//----------------------------------------------------------------------
	void Engine::InitCPU()
	{
		m_CpuInfo.getCPUInfo();
		m_CpuInfo.getOSInfo();

		wchar_t vendor[13];
		string stTemp = m_CpuInfo.getCPUVendor();
		mbstowcs(vendor, stTemp.c_str(), 13);

		Log::log(LET_GAME_MESSAGE, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"CPU Vendor is: %ls", vendor);

		setUseSIMD(true);

		if(s_bUseSIMD)
		{
			Log::log(LET_GAME_MESSAGE, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"SIMD supported");
		}
		else
		{
			Log::log(LET_GAME_MESSAGE, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"SIMD not supported");
		}
	}
//----------------------------------------------------------------------
	void Engine::InitUI()
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__,
			__LINE__, L"Initializing input");

		// Create appropriate InputSystem
		switch(m_pEngineInfo->RenderingAPI)
		{
		case API_DIRECTX9C: m_pInputSystem.setPointer(new InputSystemDX9());
							break;
		default: break;
		}

		m_pInputSystem->init();
		m_pInputSystem->setWindowed(m_pEngineInfo->DisplayMode.windowed);

		// Initialize GUI
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__,
			__LINE__, L"Initializing user interface");

		m_pGUIManager.setPointer(new GUIManager(m_pEngineInfo->DisplayMode.width,
												m_pEngineInfo->DisplayMode.height));
		m_pGUIManager->init();
	}
//----------------------------------------------------------------------
	void Engine::InitTaskManager()
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__,
			__LINE__, L"Initializing task manager");

		//m_pLua->addDependency(m_pInputSystem);
		m_pPhysics->addDependency(m_pLua);
		m_UpdateCameraTask.addDependency(m_pPhysics);
		if(m_pSoundManager)
			m_pSoundManager->addDependency(m_pPhysics);

		// Init task manager
		/*if(m_pEngineInfo->threadsNum == -1)
			m_pTaskManager->init(m_CpuInfo.getLogicalProcessorsNum());
		else
			m_pTaskManager->init(m_pEngineInfo->threadsNum);*/
		// @todo remove this
		m_pTaskManager->init(0);
		
		// Set root task
		m_pTaskManager->setRootTask(m_pLua);
	}
//----------------------------------------------------------------------
	void Engine::InitRenderer()
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__,
			__LINE__, L"Initializing renderer");

		// As renderer is a Singleton we have to release current renderer before
		// creating new one.
		if(m_pRenderer)
			m_pRenderer.~AutoPointer();

		m_pRenderer.setPointer(m_pRendererEnumerator->create(m_pEngineInfo->renderer,
															 m_pEngineInfo->DisplayMode.windowed,
															 m_pEngineInfo->renderWindowsNum,
															 m_pEngineInfo->multiThreadedRendering));
		if(!m_pRenderer.getPointer())
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Initializing renderer failed");
			return;
		}
		m_pEngineInfo->RenderingAPI = m_pRenderer->getAPIType();

		m_pRenderer->setDisplayMode(m_pEngineInfo->DisplayMode);

		if(!m_pSceneManagerFactory)
		{
			Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Initializing scene manager");
			m_pSceneManagerFactory.setPointer(new SceneManagerFactory());
		}
	}
//----------------------------------------------------------------------
	void Engine::InitPhysics()
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__,
			__LINE__, L"Initializing physics: Ageia PhysX engine");

		m_pPhysics.setPointer(new Physics());
	}
//----------------------------------------------------------------------
	void Engine::Update()
	{
		if(onUpdate)
		{
			onUpdate();
		}
		else
		{
			EngineUpdateEvent evt;
			ListenerRegistry <EngineUpdateEvent>::handleEvent(evt);

			UpdateMT();
		}
	}
//----------------------------------------------------------------------
	void Engine::UpdateST()
	{
		// Get input data
		if(m_pInputSystem)
			m_pInputSystem->processData();

		// Scripting
		if(m_pLua)
			m_pLua->update();

		// Update physics
		m_pProfiler->beginSample(L"Physics");
			if(m_pPhysics)
				m_pPhysics->update();
		m_pProfiler->endSample();

		// Update the camera prior to rendering, so some tests
		// could be performed later on
		m_UpdateCameraTask.update();

		// Update all scene managers
		for(uint i = 0; i < m_pSceneManagers.size(); ++i)
			m_pSceneManagers[i]->update();

		// Render
		m_pProfiler->beginSample(L"Rendering");
			if(m_pRenderer)
				m_pRenderer->render();
		m_pProfiler->endSample();

		// Update sound system
		if(m_pSoundManager)
			m_pSoundManager->update();


		// Present profiling results
		m_pProfiler->showResults();
	}
//----------------------------------------------------------------------
	void Engine::UpdateMT()
	{
		m_bUpdateFinished = false;

		// Get input data
		if(m_pInputSystem)
			m_pInputSystem->processData();

		// Add root task to start processing
		m_pTaskManager->addTask(m_pTaskManager->getRootTask());

		// Update tasks chain
		m_pTaskManager->update();

		// Finally render everything
		// @todo should be task too :)
		if(m_pRenderer)
			m_pRenderer->render();


		m_bUpdateFinished = true;
	}
//----------------------------------------------------------------------
	void Engine::Cleanup()
	{
		m_pTaskManager->stop();
	}
//----------------------------------------------------------------------
	void Engine::setRenderingAPI(const wstring& _api)
	{
		// If there is no API change, return
		if(m_pEngineInfo->renderer == _api)
			return;

		// Set new rendering API
		m_pEngineInfo->renderer = _api;
		InitRenderer();
	}
//----------------------------------------------------------------------
	void Engine::setRenderWindows(uint _count)
	{
		// If demanded number of render windows equals
		// current count, return
		if(m_pEngineInfo->renderWindowsNum == _count)
			return;

		m_pEngineInfo->renderWindowsNum = _count;
		InitRenderer();
	}
//----------------------------------------------------------------------
	void Engine::setEngineInfo(ENGINE_DESC& _info)
	{
		if(m_pRenderer.getPointer())
		{
			setRenderingAPI(_info.renderer);
			m_pRenderer->setDisplayMode(_info.DisplayMode);
			m_pRenderer->setRenderWindowsNum(_info.renderWindowsNum);
		}

		if(m_pInputSystem.getPointer())
			m_pInputSystem->setWindowed(_info.DisplayMode.windowed);

		(*m_pEngineInfo) = _info;
	}
//----------------------------------------------------------------------
	void Engine::addSceneManager(SceneManager* _manager)
	{
		m_pSceneManagers.push_back(AutoPointer <SceneManager>(_manager));

		_manager->addDependency(&m_UpdateCameraTask);
		if(m_pSoundManager)
			_manager->addDependency(m_pSoundManager);

		m_DummyTask.addDependency(_manager);
	}
//----------------------------------------------------------------------
	SceneManager* Engine::getSceneManager(uint _index) const
	{
		if(_index < m_pSceneManagers.size())
		{
			return m_pSceneManagers[_index].getPointer();
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Index: %d out of bound when getting scene manager", _index);
			return NULL;
		}
	}
//----------------------------------------------------------------------
	void Engine::setUseSIMD(bool _value)
	{
		s_bUseSIMD = _value && m_CpuInfo.isSIMDSupported();
		Maths::setUseSSE(s_bUseSIMD);
	}
//----------------------------------------------------------------------
	void Engine::setActiveCamera(Camera* _camera)
	{
		if(_camera)
			m_pActiveCamera = _camera;
		else
		{
			m_pActiveCamera = _camera;
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Camera does not exist");
		}
	}
//----------------------------------------------------------------------
	void Engine::resetCameraAndFrustum()
	{
		if(m_pActiveCamera)
		{
			m_pActiveCamera->getFrustum().reset();
			m_pActiveCamera->getFrustum().update();
		}
	}
//----------------------------------------------------------------------
	Camera* Engine::getActiveCamera() const
	{
		if(!m_pActiveCamera)
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"No active camera found");
		}

		return m_pActiveCamera;
	}
//----------------------------------------------------------------------
	ScriptSystemLua* Engine::getLuaScripting() const
	{
		return m_pLua;
	}
//----------------------------------------------------------------------
	void Engine::InitSound()
	{
		// No sound is used
		if(m_pEngineInfo->soundLibrary == L"")
			return;

		ISoundManager* mgr = m_pSoundManagerEnumerator->create(m_pEngineInfo->soundLibrary);

		if(mgr)
		{
			Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"Initializing sound system: %ls library",
				m_pEngineInfo->soundLibrary.c_str());

			m_pSoundManager.setPointer(mgr);
		}
	}
//----------------------------------------------------------------------
	void Engine::addUpdateListener(EngineUpdateListener* _listener)
	{
		// Add listener to the registry so it
		// could handle upcoming messages.
		AutoPointer <EngineUpdateListener> Listener(_listener);
		ListenerRegistry <EngineUpdateEvent>::addListener(Listener.getPointer());

		m_vUpdateListeners.push_back(Listener);
	}
//----------------------------------------------------------------------
	void Engine::removeUpdateListener(uint _index)
	{
		if(_index >= m_vUpdateListeners.size())
			return;

		ListenerRegistry <EngineUpdateEvent>::removeListener(m_vUpdateListeners[_index].getPointer());
		m_vUpdateListeners.erase(m_vUpdateListeners.begin() + _index);
	}
//----------------------------------------------------------------------
	void Engine::removeAllUpdateListeners()
	{
		for(uint i = 0; i < m_vUpdateListeners.size(); ++i)
		{
			ListenerRegistry <EngineUpdateEvent>::removeListener(m_vUpdateListeners[i].getPointer());
		}

		m_vUpdateListeners.clear();
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	void UpdateCameraTask::update()
	{
		Camera* pActiveCamera = Engine::getSingleton().getActiveCamera();

		if(pActiveCamera)
			pActiveCamera->updateCamera();
	}
//----------------------------------------------------------------------
}