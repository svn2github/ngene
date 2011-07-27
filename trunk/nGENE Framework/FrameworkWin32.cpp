/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FrameworkWin32.cpp
Version:	0.28
---------------------------------------------------------------------------
*/

#define NOMINMAX
#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400
#endif
#include <windows.h>

#include "FrameworkWin32.h"
#include "SemanticFunctions.h"
#include "SettingsWindowWin32.h"

#include "Console.h"
#include "CrashDump.h"

#include "AABB.h"
#include "Camera.h"
#include "Camera2D.h"
#include "CameraFirstPerson.h"
#include "CameraThirdPerson.h"
#include "FileManager.h"
#include "Frustum.h"
#include "GUIManager.h"
#include "ICullable.h"
#include "IFile.h"
#include "InputSystem.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "MeshLoadPolicy3dsFile.h"
#include "MeshLoadPolicyCollada.h"
#include "MeshLoadPolicyXFile.h"
#include "Node.h"
#include "NodeLight.h"
#include "NodeMesh.h"
#include "NodeParticleSystem.h"
#include "NodeTerrain.h"
#include "NodeVisible.h"
#include "NodeMesh.h"
#include "NodeParticleSystem.h"
#include "NodeTerrain.h"
#include "NodeVisible.h"
#include "NodeVolumetric.h"
#include "ParticleEmitter.h"
#include "PrefabArrow.h"
#include "PrefabAxisRods.h"
#include "PrefabBox.h"
#include "PrefabCapsule.h"
#include "PrefabCylinder.h"
#include "PrefabPlane.h"
#include "PrefabPyramid.h"
#include "PrefabSphere.h"
#include "PrefabTeapot.h"
#include "PrefabTorus.h"
#include "Surface.h"
#include "NodeClouds.h"
#include "NodeFur.h"
#include "NodeLightning.h"
#include "NodeSky.h"
#include "NodeSound.h"
#include "NodeWater.h"
#include "RendererDX9.h"
#include "SceneManagerFactory.h"
#include "ScenePartitioner.h"
#include "ScriptLua.h"
#include "ScriptSystemLua.h"
#include "ShaderManager.h"
#include "Surface.h"
#include "Thread.h"
#include "XMLDocument.h"
#include "XMLParser.h"


#include "VertexBufferManager.h"
#include "Console.h"
#include "ISerializer.h"
#include "Node.h"
#include "XMLSerializer.h"


using nGENE::Nature::NodeClouds;
using nGENE::Nature::NodeFur;
using nGENE::Nature::NodeLightning;
using nGENE::Nature::NodeSky;
using nGENE::Nature::NodeVolumetric;
using nGENE::Nature::NodeWater;


namespace nGENE
{
namespace Application
{
	FrameworkWin32::FrameworkWin32(bool _profile, bool _multiThreadedRendering):
		m_pScript(NULL),
		m_bProfile(_profile),
		m_bMultiThreadedRendering(_multiThreadedRendering)
	{
	}
//----------------------------------------------------------------------
	FrameworkWin32::~FrameworkWin32()
	{
	}
//----------------------------------------------------------------------
	void FrameworkWin32::createApplication()
	{
		createApp(NULL);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::createApplication(HWND* _hwnd)
	{
		createApp(_hwnd);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::createApplication(HWND* _hwnd, ENGINE_DESC* _desc)
	{
		createApp(_hwnd, _desc);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::createApp(HWND* _hwnd, ENGINE_DESC* _desc)
	{
		initLoggers();


		// Add factories to the XML serializer
		XMLSerializer::addFactory(Camera::Type.getTypeName(), new SerializableFactory <Camera>());
		XMLSerializer::addFactory(Camera2D::Type.getTypeName(), new SerializableFactory <Camera2D>());
		XMLSerializer::addFactory(CameraFirstPerson::Type.getTypeName(), new SerializableFactory <CameraFirstPerson>());
		XMLSerializer::addFactory(CameraThirdPerson::Type.getTypeName(), new SerializableFactory <CameraThirdPerson>());
		XMLSerializer::addFactory(Node::Type.getTypeName(), new SerializableFactory <Node>());
		XMLSerializer::addFactory(NodeLight::Type.getTypeName(), new SerializableFactory <NodeLight>());
		XMLSerializer::addFactory(NodeMesh <MeshLoadPolicy3dsFile>::Type.getTypeName(), new SerializableFactory <NodeMesh <MeshLoadPolicy3dsFile> >());
		XMLSerializer::addFactory(NodeMesh <MeshLoadPolicyCollada>::Type.getTypeName(), new SerializableFactory <NodeMesh <MeshLoadPolicyCollada> >());
		XMLSerializer::addFactory(NodeMesh <MeshLoadPolicyXFile>::Type.getTypeName(), new SerializableFactory <NodeMesh <MeshLoadPolicyXFile> >());
		XMLSerializer::addFactory(NodeParticleSystem::Type.getTypeName(), new SerializableFactory <NodeParticleSystem>);
		XMLSerializer::addFactory(NodeTerrain::Type.getTypeName(), new SerializableFactory <NodeTerrain>());
		XMLSerializer::addFactory(NodeVisible::Type.getTypeName(), new SerializableFactory <NodeVisible>());
		XMLSerializer::addFactory(ParticleEmitter::Type.getTypeName(), new SerializableFactory <ParticleEmitter>());
		XMLSerializer::addFactory(PrefabArrow::Type.getTypeName(), new SerializableFactory <PrefabArrow>());
		XMLSerializer::addFactory(PrefabAxisRods::Type.getTypeName(), new SerializableFactory <PrefabAxisRods>());
		XMLSerializer::addFactory(PrefabBox::Type.getTypeName(), new SerializableFactory <PrefabBox>());
		XMLSerializer::addFactory(PrefabCapsule::Type.getTypeName(), new SerializableFactory <PrefabCapsule>());
		XMLSerializer::addFactory(PrefabCylinder::Type.getTypeName(), new SerializableFactory <PrefabCylinder>());
		XMLSerializer::addFactory(PrefabPlane::Type.getTypeName(), new SerializableFactory <PrefabPlane>());
		XMLSerializer::addFactory(PrefabPyramid::Type.getTypeName(), new SerializableFactory <PrefabPyramid>());
		XMLSerializer::addFactory(PrefabSphere::Type.getTypeName(), new SerializableFactory <PrefabSphere>());
		XMLSerializer::addFactory(PrefabTeapot::Type.getTypeName(), new SerializableFactory <PrefabTeapot>());
		XMLSerializer::addFactory(PrefabTorus::Type.getTypeName(), new SerializableFactory <PrefabTorus>());
		XMLSerializer::addFactory(Surface::Type.getTypeName(), new SerializableFactory <Surface>());
		XMLSerializer::addFactory(NodeClouds::Type.getTypeName(), new SerializableFactory <NodeClouds>());
		XMLSerializer::addFactory(NodeLightning::Type.getTypeName(), new SerializableFactory <NodeLightning>());
		XMLSerializer::addFactory(NodeFur::Type.getTypeName(), new SerializableFactory <NodeFur>());
		XMLSerializer::addFactory(NodeSky::Type.getTypeName(), new SerializableFactory <NodeSky>());
		XMLSerializer::addFactory(NodeSound::Type.getTypeName(), new SerializableFactory <NodeSound>());
		XMLSerializer::addFactory(NodeVolumetric::Type.getTypeName(), new SerializableFactory <NodeVolumetric>());
		XMLSerializer::addFactory(NodeWater::Type.getTypeName(), new SerializableFactory <NodeWater>());


		// Set top-level exception filter
		SetUnhandledExceptionFilter(createCrashDump);

		// Display config dialog
		if(m_bShowConfigDialog)
		{
			m_pEngine = new Engine();
			if(!showSettingsWindow())
			{
				shutdown();
				closeApplication();
				exit(0);
			}

			m_pConfigDialog->getEngineInfo().multiThreadedRendering = m_bMultiThreadedRendering;
			m_pEngine->setEngineInfo(m_pConfigDialog->getEngineInfo());
		}
		else
		{
			if(_desc)
				m_pEngine = new Engine(*_desc);
			else
				m_pEngine = new Engine();
		}

		// Create the engine
		if(_hwnd)
			m_pEngine->setTopLevelWndHandle(*_hwnd);
		m_pEngine->Init();

		// Enable/disable profiler
		Renderer::getSingleton().setProfile(m_bProfile);

		// Create rendering device
		if(_hwnd)
			(static_cast<RendererDX9*>(Renderer::getSingletonPtr()))->createDevice(_hwnd);
		else
			(static_cast<RendererDX9*>(Renderer::getSingletonPtr()))->createDevice();

		// Create default scene manager
		SceneManager* pSceneManager = SceneManagerFactory::getSingleton().createSceneManager(SceneManagerFactory::SMT_GENERIC);
		m_pEngine->addSceneManager(pSceneManager);

		// Create keyboard device
		InputSystem& input = InputSystem::getSingleton();
		m_pKeyboard = static_cast<DeviceKeyboard*>(input.createDevice(L"Keyboard1", L"KeyboardDX9"));

		// Create mouse device
		m_pMouse = static_cast<DeviceMouse*>(input.createDevice(L"Mouse1", L"MouseDX9"));

		// Load default resources
		loadResources();


		// Create console window
		m_pConsoleWnd = new Console();
		GUIManager::getSingleton().addWindow(m_pConsoleWnd);


		// Ensure main thread is run on the specified processor for all the time
		SetThreadAffinityMask(GetCurrentThread(), 1);
		// Name main thread
		Thread::setName("Main Thread");


		NGENE_DELETE(m_pConfigDialog);


		// Add render stages
		Renderer& renderer = Renderer::getSingleton();
		for(uint i = 0; i < renderer.getRenderStagesNum(); ++i)
			m_ReflectionStage.addRenderStage(renderer.getRenderStage(i));
		renderer.addRenderStage(L"Reflection", &m_ReflectionStage);
		m_ReflectionStage.init();
		m_ReflectionStage.setPriority(0);

		renderer.addRenderStage(L"Shadows", &m_ShadowStage);
		m_ShadowStage.init();
		m_ShadowStage.setPriority(11);
		m_ShadowStage.setShadowMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"shadow_pcf"));

		renderer.addRenderStage(L"GUI", &m_GUIStage);
		m_GUIStage.init();
		m_GUIStage.setManager(&GUIManager::getSingleton());
		m_GUIStage.setPriority(1000);
	}
//----------------------------------------------------------------------
	LRESULT FrameworkWin32::WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(nMsg)
		{
		case WM_SETCURSOR:
			return true;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(hwnd, nMsg, wParam, lParam);
	}
//----------------------------------------------------------------------
	bool FrameworkWin32::showSettingsWindow()
	{
		m_pConfigDialog = new SettingsWindowWin32();

		return m_pConfigDialog->createWindow();
	}
//----------------------------------------------------------------------
	void FrameworkWin32::createSettingsWindow()
	{
	}
//----------------------------------------------------------------------
	bool FrameworkWin32::run()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));


		while(msg.message != WM_QUIT)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

			processInput();
			m_pEngine->Update();
		}

		return false;
	}
//----------------------------------------------------------------------
	void FrameworkWin32::runOneFrame()
	{
		processInput();
		m_pEngine->Update();
	}
//----------------------------------------------------------------------
	ICullable* FrameworkWin32::pick(float _x, float _y, bool _perTriangle)
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		Ray ray = constructPickRay(_x, _y);
		sm->getScenePartitioner()->intersect(ray, _perTriangle);

		return sm->getScenePartitioner()->getClosest();
	}
//----------------------------------------------------------------------
	Ray FrameworkWin32::constructPickRay(float _x, float _y)
	{
		const SDisplayMode& mode = Renderer::getSingleton().getDisplayMode();
		Vector2 vecNormalized(_x / static_cast<float>(mode.width - 1),
							 ((static_cast<float>(mode.height) - 1.0f - _y)) / static_cast<float>(mode.height - 1));

		Frustum& frustum = Engine::getSingleton().getActiveCamera()->getFrustum();
		float yval = frustum.getNear() * Maths::ftan(frustum.getFOV() * 0.5f);
		float xval = frustum.getAspect() * yval;

		Point ptPoint((1.0f - vecNormalized.x) * -xval + vecNormalized.x * xval,
					  (1.0f - vecNormalized.y) * -yval + vecNormalized.y * yval,
					  0.0f);

		Camera* pCam = Engine::getSingleton().getActiveCamera();
		Vector3 vecDir(pCam->getForward() * frustum.getNear() +
					   ptPoint.x * pCam->getRight() +
					   ptPoint.y * pCam->getUp());

		Point pos = pCam->getPositionLocal();
		vecDir.normalize();
		Ray pickRay(pos, vecDir);


		return pickRay;
	}
//----------------------------------------------------------------------
	void FrameworkWin32::hideCursor()
	{
		Renderer::getSingleton().showCursor(false);
		m_bShowCursor = false;
	}
//----------------------------------------------------------------------
	void FrameworkWin32::showCursor()
	{
		Renderer::getSingleton().showCursor(true);
		m_bShowCursor = true;
	}
//----------------------------------------------------------------------
	void FrameworkWin32::toggleCursor()
	{
		m_bShowCursor = (m_bShowCursor ? false : true);
		Renderer::getSingleton().showCursor(m_bShowCursor);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::toggleWindowed()
	{
		m_bWindowed = (m_bWindowed ? false : true);
		Renderer::getSingleton().setWindowed(m_bWindowed);
		InputSystem::getSingleton().setWindowed(m_bWindowed);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::processInput()
	{
	}
//----------------------------------------------------------------------
	void FrameworkWin32::cleanup()
	{
	}
//----------------------------------------------------------------------
	void FrameworkWin32::shutdown()
	{
		if(m_vFunctions.size())
		{
			for(uint i = 0; i < m_vFunctions.size(); ++i)
				NGENE_DELETE(m_vFunctions[i]);
			m_vFunctions.clear();
		}

		NGENE_DELETE(m_pConsoleWnd);

		if(m_pEngine)
		{
			// Clean is called internally
			NGENE_DELETE(m_pEngine);
		}
		NGENE_DELETE(m_pConfigDialog);

		UnregisterClass("nGENE Tech", m_hInstance);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::closeApplication()
	{
		PostQuitMessage(0);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::suppressConfigDialog()
	{
		m_bShowConfigDialog = false;
	}
//----------------------------------------------------------------------
	void FrameworkWin32::loadShaderConfig(const wstring& _fileName)
	{
		// Open file
		IFile* file = FileManager::getSingleton().openFile(_fileName.c_str(), OPEN_READ);
		if(!file)
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"%ls file not found! This file is critical for nGENE", _fileName.c_str());
			return;
		}

		wstring temp = file->getBuffer();
		FileManager::getSingleton().closeFile(_fileName.c_str());
		if(temp.compare(L"") == 0)
		{
			Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"No valid shader configuration found");
			return;
		}

		XMLDocument* pDocument = XMLParser::getSingleton().parse(temp);
		XMLDocument* pChild = pDocument->getFirstChild();
		XMLDocument* pTemp = NULL;
		while(pChild)
		{
			string tagName = pChild->getTagName();

			// Load semantic
			if(tagName == "semantic")
				loadSemantic(pChild);
			// Load data-type
			else if(tagName == "data_type")
				loadDataType(pChild);
			// Load script file
			else if(tagName == "script")
				loadScript(pChild);

			pTemp = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(pTemp);
		}

		NGENE_DELETE(pDocument);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::loadScript(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;

		// Parse data
		string key = pNode->getAttribute("language");
		if(key == "lua")
		{
			key = pNode->getAttribute("file");
			ScriptSystemLua* pLua = Engine::getSingleton().getLuaScripting();
			m_pScript = pLua->createScript();
			m_pScript->loadScriptFromFile(key);
		}
		else
		{
			wstring lang(key.begin(), key.end());
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Language: %ls is not supported by nGENE Tech", lang.c_str());
		}
	}
//----------------------------------------------------------------------
	void FrameworkWin32::loadDataType(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;

		ShaderManager& manager = ShaderManager::getSingleton();
		DATATYPE_DESC* pResult = manager.createDataType();

		// Parse data
		string key = pNode->getAttribute("name");
		pResult->stName.assign(key.begin(), key.end());

		key = pNode->getAttribute("values_count");
		pResult->nValuesCount = atoi(key.c_str());

		key = pNode->getAttribute("base_type");
		if(key != "")
		{
			if(key == "FLOAT")
				pResult->baseType = SShaderConstantType::BT_FLOAT;
			else if(key == "INTEGER")
				pResult->baseType = SShaderConstantType::BT_INT;
			else if(key == "BOOLEAN")
				pResult->baseType = SShaderConstantType::BT_BOOL;
		}
		else
			pResult->baseType = SShaderConstantType::BT_FLOAT;

		manager.addDataType(pResult);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::loadSemantic(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;

		ShaderManager& manager = ShaderManager::getSingleton();
		SEMANTIC_DESC* pResult = manager.createSemantic();

		// Parse data
		string key = pNode->getAttribute("name");
		pResult->stName.assign(key.begin(), key.end());

		key = pNode->getAttribute("data_type");
		pResult->stDataType.assign(key.begin(), key.end());

		key = pNode->getAttribute("function");
		if(!key.empty())
		{
			ShaderSemanticFunction* pFunction = new ShaderSemanticFunctionLua(m_pScript, key);
			m_vFunctions.push_back(pFunction);

			pResult->function = m_vFunctions[m_vFunctions.size() - 1];
		}
		else
		{
			// Find appropriate function
			applyFunction(pResult);
		}


		manager.addSemantic(pResult);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::applyFunction(SEMANTIC_DESC* _semantic)
	{
		// Choose semantic function
		if(_semantic->stName.compare(L"WorldViewProj") == 0)
			_semantic->pFunction = worldViewProj;
		else if(_semantic->stName.compare(L"ViewProj") == 0)
			_semantic->pFunction = viewProj;
		else if(_semantic->stName.compare(L"WorldView") == 0)
			_semantic->pFunction = worldView;
		else if(_semantic->stName.compare(L"ViewInverse") == 0)
			_semantic->pFunction = viewInverse;
		else if(_semantic->stName.compare(L"ViewInverseTranspose") == 0)
			_semantic->pFunction = viewInverseTranspose;
		else if(_semantic->stName.compare(L"World") == 0)
			_semantic->pFunction = world;
		else if(_semantic->stName.compare(L"View") == 0)
			_semantic->pFunction = view;
		else if(_semantic->stName.compare(L"Proj") == 0)
			_semantic->pFunction = proj;
		else if(_semantic->stName.compare(L"ProjInverse") == 0)
			_semantic->pFunction = projInverse;
		else if(_semantic->stName.compare(L"Camera") == 0)
			_semantic->pFunction = camera;
		else if(_semantic->stName.compare(L"CameraInverse") == 0)
			_semantic->pFunction = cameraInverse;
		else if(_semantic->stName.compare(L"EyePosition") == 0)
			_semantic->pFunction = eyePosition;
		else if(_semantic->stName.compare(L"NearPlane") == 0)
			_semantic->pFunction = nearPlane;
		else if(_semantic->stName.compare(L"FarPlane") == 0)
			_semantic->pFunction = farPlane;
		else if(_semantic->stName.compare(L"LightDirection") == 0)
			_semantic->pFunction = lightDirection;
		else if(_semantic->stName.compare(L"LightColour") == 0)
			_semantic->pFunction = lightColour;
		else if(_semantic->stName.compare(L"LightPosition") == 0)
			_semantic->pFunction = lightPosition;
		else if(_semantic->stName.compare(L"LightIntensity") == 0)
			_semantic->pFunction = lightIntensity;
		else if(_semantic->stName.compare(L"LightRadius") == 0)
			_semantic->pFunction = lightRadius;
		else if(_semantic->stName.compare(L"LightFov") == 0)
			_semantic->pFunction = lightFov;
		else if(_semantic->stName.compare(L"LightSpotExponent") == 0)
			_semantic->pFunction = lightSpotExponent;
		else if(_semantic->stName.compare(L"LightConstantAttenuation") == 0)
			_semantic->pFunction = lightConstantAttenuation;
		else if(_semantic->stName.compare(L"LightLinearAttenuation") == 0)
			_semantic->pFunction = lightLinearAttenuation;
		else if(_semantic->stName.compare(L"LightQuadraticAttenuation") == 0)
			_semantic->pFunction = lightQuadraticAttenuation;
		else if(_semantic->stName.compare(L"ShadowDensity") == 0)
			_semantic->pFunction = shadowDensity;
		else if(_semantic->stName.compare(L"InvTanHalfFov") == 0)
			_semantic->pFunction = invTanHalfFov;
		else if(_semantic->stName.compare(L"Aspect") == 0)
			_semantic->pFunction = aspect;
		else if(_semantic->stName.compare(L"PixelSize") == 0)
			_semantic->pFunction = pixelSize;
		else if(_semantic->stName.compare(L"Ambient") == 0)
			_semantic->pFunction = ambient;
		else if(_semantic->stName.compare(L"Diffuse") == 0)
			_semantic->pFunction = diffuse;
		else if(_semantic->stName.compare(L"Emissive") == 0)
			_semantic->pFunction = emissive;
		else if(_semantic->stName.compare(L"Time") == 0)
			_semantic->pFunction = time;
	}
//----------------------------------------------------------------------
	void FrameworkWin32::drawAABB(ICullable* _node)
	{
		Node* pNode = getDebugAABB();
		
		if(_node)
		{
			Vector3 min = _node->getBoundingBox()->getMinimum();
			Vector3 max = _node->getBoundingBox()->getMaximum();

			pNode->setScale(Maths::abs(max.x - min.x) + 0.01f,
							Maths::abs(max.y - min.y) + 0.01f,
							Maths::abs(max.z - min.z) + 0.01f);
			pNode->setPosition(_node->getBoundingBox()->getCentre());
			pNode->setEnabled(true);
		}
		else
			pNode->setEnabled(false);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::drawAABB(NodeVisible* _node)
	{
		Node* pNode = getDebugAABB();
		
		if(_node)
		{
			uint count = _node->getSurfacesNum();
			uint surfacesNum = 0;
			AABB aabb;

			for(uint i = 0; i < count; ++i)
			{
				Surface* pSurf = _node->getSurface(i);
				if(pSurf->getMaterialPtr() && pSurf->isEnabled())
				{
					if(surfacesNum++)
						aabb += *pSurf->getBoundingBox();
					else
						aabb = *pSurf->getBoundingBox();
				}
			}

			Vector3 min = aabb.getMinimum();
			Vector3 max = aabb.getMaximum();

			pNode->setScale(Maths::abs(max.x - min.x) + 0.01f,
							Maths::abs(max.y - min.y) + 0.01f,
							Maths::abs(max.z - min.z) + 0.01f);
			pNode->setPosition(aabb.getCentre());
			pNode->setEnabled(true);
		}
		else
			pNode->setEnabled(false);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::drawAxisRods(ICullable* _node)
	{
		Node* pNode = getDebugAxisRods();

		if(_node)
		{
			pNode->setPosition(_node->getBoundingBox()->getCentre());
			pNode->setEnabled(true);

			if(dynamic_cast<Surface*>(_node))
			{
				Surface* pSurface = static_cast<Surface*>(_node);
				pNode->setRotation(pSurface->getNode()->getRotation());
			}
		}
		else
			pNode->setEnabled(false);
	}
//----------------------------------------------------------------------
	void FrameworkWin32::drawAxisRods(NodeVisible* _node)
	{
		Node* pNode = getDebugAxisRods();

		if(_node)
		{
			uint count = _node->getSurfacesNum();
			uint surfacesNum = 0;
			AABB aabb;

			for(uint i = 0; i < count; ++i)
			{
				Surface* pSurf = _node->getSurface(i);
				if(pSurf->getMaterialPtr() && pSurf->isEnabled())
				{
					if(surfacesNum++)
						aabb += *pSurf->getBoundingBox();
					else
						aabb = *pSurf->getBoundingBox();
				}
			}

			pNode->setRotation(_node->getRotation());

			pNode->setPosition(aabb.getCentre());
			pNode->setEnabled(true);
		}
		else
			pNode->setEnabled(false);
	}
//----------------------------------------------------------------------
	Node* FrameworkWin32::getDebugAABB()
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		Node* pNode = sm->getNode(L"_debug_aabb");

		if(!pNode)
		{
			PrefabBox* pBox = sm->createBox(1.0f, 1.0f, 1.0f);
			pBox->setSerializable(false);
			pBox->setPickable(false);

			Surface* pSurf = pBox->getSurface(L"Base");
			pSurf->setMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"debug_aabb"));
			sm->getRootNode()->addChild(L"_debug_aabb", pBox);

			pNode = pBox;
		}

		return pNode;
	}
//----------------------------------------------------------------------
	Node* FrameworkWin32::getDebugAxisRods()
	{
		SceneManager* sm = Engine::getSingleton().getSceneManager(0);
		Node* pNode = sm->getNode(L"_debug_axis_rods");

		if(!pNode)
		{
			PrefabAxisRods* pRods = sm->createAxisRods(1.0f, 0.1f);
			pRods->setSerializable(false);

			MaterialLibrary* pLib = MaterialManager::getSingleton().getLibrary(L"default");
			pRods->getAxisX()->setMaterial(pLib->getMaterial(L"debug_axis_red"));
			pRods->getAxisY()->setMaterial(pLib->getMaterial(L"debug_axis_blue"));
			pRods->getAxisZ()->setMaterial(pLib->getMaterial(L"debug_axis_green"));

			pRods->getAxisX()->getLine()->getSurface(L"Base")->setPickable(false);
			pRods->getAxisX()->getEnd()->getSurface(L"Base")->setPickable(false);
			pRods->getAxisY()->getLine()->getSurface(L"Base")->setPickable(false);
			pRods->getAxisY()->getEnd()->getSurface(L"Base")->setPickable(false);
			pRods->getAxisZ()->getLine()->getSurface(L"Base")->setPickable(false);
			pRods->getAxisZ()->getEnd()->getSurface(L"Base")->setPickable(false);

			sm->getRootNode()->addChild(L"_debug_axis_rods", pRods);

			pNode = pRods;
		}

		return pNode;
	}
//----------------------------------------------------------------------
	void FrameworkWin32::loadResources()
	{
		// Mount '../media' path as current directory
		FileManager::getSingleton().changeDir("../media");

		loadShaderConfig(L"ShaderConfig.xml");
		MaterialManager::getSingleton().createLibraryFromFile(L"core.lib", L"core");
		MaterialManager::getSingleton().createLibraryFromFile(L"default.lib", L"default");
	}
//----------------------------------------------------------------------
}
}