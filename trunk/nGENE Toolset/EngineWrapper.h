/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		EngineWrapper.h
Version:	0.09
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ENGINEWRAPPER_H_
#define __INC_ENGINEWRAPPER_H_


#include "FrameworkWin32.h"
#include "nGENE.h"

#include "NodeWrapper.h"


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::Threading;

	using namespace nGENE;
	using namespace nGENE::Application;


	class ToolsetListener: public InputListener
	{
	private:
		Real m_Angle1;
		Real m_Angle2;

		nGENE::Quaternion quatRot1;
		nGENE::Quaternion quatRot2;
		nGENE::Quaternion quatFinal;

		FrameworkWin32* m_pApp;

		float m_XMin;
		float m_YMin;
		float m_XMax;
		float m_YMax;

		bool m_LBMDown;
		bool m_LAltDown;
		bool m_LShiftDown;


		long m_prev;

	public:
		ToolsetListener(FrameworkWin32* _app):
			m_LBMDown(false),
			m_LAltDown(false),
			m_LShiftDown(false)
		{
			m_prev = Engine::getSingleton().getTimer().getMilliseconds();
			m_pApp = _app;
		}

		~ToolsetListener()
		{
		}

		void setCoordsZero(float xMin, float yMin, float xMax, float yMax)
		{
			m_XMin = xMin;
			m_XMax = xMax;
			m_YMin = yMin;
			m_YMax = yMax;
		}

		void handleEvent(const MouseEvent& _evt);

		void handleEvent(const KeyboardEvent& _evt)
		{
			float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_prev) * 0.001f;

			CameraFirstPerson* pCamera = (CameraFirstPerson*)Engine::getSingleton().getActiveCamera();
			if(!pCamera)
				return;

			if(_evt.isKeyDown(KC_UP))
				pCamera->move(1.0f);
			if(_evt.isKeyDown(KC_DOWN))
				pCamera->move(-1.0f);
			if(_evt.isKeyDown(KC_LEFT))
				pCamera->strafe(-1.0f);
			if(_evt.isKeyDown(KC_RIGHT))
				pCamera->strafe(1.0f);
			if(_evt.isKeyDown(KC_Q))
				m_Angle1 -= 0.9f * fDiff;
			if(_evt.isKeyDown(KC_W))
				m_Angle1 += 0.9f * fDiff;
			if(_evt.isKeyDown(KC_PGUP))
				m_Angle2 -= 0.9f * fDiff;
			if(_evt.isKeyDown(KC_PGDOWN))
				m_Angle2 += 0.9f * fDiff;

			quatRot1.set(nGENE::Vector3(0.0f, 1.0f, 0.0f), m_Angle1);
			quatRot2.set(nGENE::Vector3(1.0f, 0.0f, 0.0f), m_Angle2);

			quatFinal = quatRot1 * quatRot2;
			quatFinal.normalize();
			pCamera->setRotation(quatFinal);


			m_prev = Engine::getSingleton().getTimer().getMilliseconds();


			if(_evt.isKeyDown(KC_LMENU))
				m_LAltDown = true;
			else if(_evt.isKeyUp(KC_LMENU))
				m_LAltDown = false;

			if(_evt.isKeyDown(KC_LSHIFT))
				m_LShiftDown = true;
			else if(_evt.isKeyUp(KC_LSHIFT))
				m_LShiftDown = false;
		}
	};


	ref class Serialized: public System::Object
	{
	private:
		Node* m_pNode;

	public:
		Serialized(Node* _node) {m_pNode = _node;}
		System::Void deserialize();
	};


	class ToolsetApplication: public FrameworkWin32
	{
	public:
		ToolsetApplication() {}

		void initLoggers();
		void loadResources();
	};
	

	/** Wrapper for the nGENE Renderer class.
	*/
	public ref class EngineWrapper: public System::Object
	{
	private:
		ToolsetApplication* m_pApp;

		ToolsetListener* listener;				///< Input listener
		Renderer* m_pRenderer;					///< Pointer to the active renderer

		PhysicsWorld* pWorld;					///< Pointer to physics world

		Color m_BackColor;						///< Clear colour for the renderer.

		System::Object^ lock;

		bool m_bRun;
		bool m_bDebugAABB;

		bool m_XLock;
		bool m_YLock;
		bool m_ZLock;

		bool m_bLoading;

		nGENEToolset::Vector2^ m_Resolution;	///< Screen resolution
		nGENEToolset::Vector3^ m_Gravity;		///< World gravity

	public:
		EngineWrapper()
		{
			m_bRun = true;
			m_bDebugAABB = true;
			m_bLoading = false;

			lock = gcnew System::Object();

			m_XLock = true;
			m_YLock = true;
			m_ZLock = true;

			m_pApp = new ToolsetApplication();
			m_pApp->suppressConfigDialog();

			// Set startup resolution
			m_Resolution = gcnew nGENEToolset::Vector2();
		}

		~EngineWrapper()
		{
			NGENE_DELETE(listener);
		}

		System::Void setWindowZero(float xMin, float yMin, float xMax, float yMax)
		{
			if(listener)
				listener->setCoordsZero(xMin, yMin, xMax, yMax);
		}

		System::Void init(HWND hwnd)
		{
			ENGINE_DESC desc;
			desc.threadsNum = 0;

			m_pApp->createApplication(&hwnd, &desc);


			// Turn lights on
			m_pRenderer = Renderer::getSingletonPtr();
			
			m_pRenderer->setVSync(false);
			m_pRenderer->setLightEnabled(true);
			m_pRenderer->setClearColour(0);
			m_BackColor.FromArgb(0);

			m_Resolution->x = Renderer::getSingleton().getDisplayMode().width;
			m_Resolution->y = Renderer::getSingleton().getDisplayMode().height;

			Resolution = m_Resolution;

			// Create physics
			/*Physics& physics = nGENE::Physics::getSingleton();
			if(!physics.getPhysicsWorldCount())
				physics.createWorld(L"PhysicsWorld");
			uint index = 0;
			pWorld = physics.getWorld(index);
			pWorld->setGravity(nGENE::Vector3(0.0f, -9.8f, 0.0f));
			pWorld->setSkinWidth(0.01f);

			PhysicsMaterial* physMat = pWorld->createPhysicsMaterial(L"testPhysMat");
			/*physMat->setRestitution(0.5f);
			physMat->setDynamicFriction(0.5f);
			physMat->setStaticFriction(0.5f);*/

			//m_Gravity = gcnew nGENEToolset::Vector3(&pWorld->getGravity());

			// Add input listener
			listener = new ToolsetListener(m_pApp);
			InputSystem::getSingleton().addInputListener(listener);
			InputSystem::getSingleton().setWindowed(false);


			this->Anisotropy = true;

			enableInput(false);
		}

		System::Void enableInput(bool value)
		{
			if(listener)
				listener->setEnabled(value);
		}

		System::Void update()
		{
			Monitor::Enter(lock);
				bool run = m_bRun;

			if(run)
				Engine::getSingleton().Update();

			Monitor::Exit(lock);
		}

		System::Void getLock()
		{
			if(!m_bLoading)
			{
				//TaskManager::getSingleton().setRunning(false);
				//while(!Engine::getSingleton().hasUpdateFinished()) {}

				Monitor::Enter(lock);
			}
		}

		bool tryLock()
		{
			if(!m_bLoading)
			{
				//TaskManager::getSingleton().setRunning(false);
				//while(!Engine::getSingleton().hasUpdateFinished()) {}

				bool run = Monitor::TryEnter(lock);
				
				return run;
			}

			return false;
		}

		System::Void getLockLoading()
		{
			TaskManager::getSingleton().setRunning(false);
			while(!Engine::getSingleton().hasUpdateFinished()) {}

			Monitor::Enter(lock);

			m_bLoading = true;
			m_bRun = false;
		}

		System::Void releaseLockLoading()
		{
			m_bRun = true;
			m_bLoading = false;

			Monitor::Exit(lock);
			TaskManager::getSingleton().setRunning(true);
		}

		System::Void releaseLock()
		{
			if(!m_bLoading)
			{
				Monitor::Exit(lock);
				TaskManager::getSingleton().setRunning(true);
			}
		}

		System::Void removeSelected();

		System::Void reset(uint _width, uint _height);
		System::Void save();
		bool load();

		System::Void drawAABB(SceneNode^ sceneNode)
		{
			if(!m_bDebugAABB)
			{
				m_pApp->drawAABB((NodeVisible*)NULL);
				m_pApp->drawAxisRods((NodeVisible*)NULL);
				return;
			}

			NodeWrapper^ node = nullptr;
			SurfaceWrapper^ surface = nullptr;
			try {
				node = safe_cast<NodeWrapper^>(sceneNode);
				if(node != nullptr)
				{
					NodeVisible* pNode = dynamic_cast<NodeVisible*>(node->getnGENENode());
					if(pNode)
					{
						m_pApp->drawAABB(pNode);
						m_pApp->drawAxisRods(pNode);
					}
					else
					{
						ICullable* pCullable = dynamic_cast<ICullable*>(node->getnGENENode());
						if(pCullable)
						{
							m_pApp->drawAABB(pCullable);
							m_pApp->drawAxisRods(pCullable);
						}
					}
				}

				return;
			} catch(InvalidCastException^)
			{
			} catch(Exception^)
			{
			}

			try {
				surface = safe_cast<SurfaceWrapper^>(sceneNode);
				if(surface != nullptr)
				{
					m_pApp->drawAABB(surface->getSurface());
					m_pApp->drawAxisRods(surface->getSurface());
				}
			} catch(InvalidCastException^)
			{
			} catch(Exception^)
			{
			}
		}

		[Browsable(true), CategoryAttribute("Renderer"),
		DescriptionAttribute("Background colour")]
		property System::Drawing::Color BackgroundColour
		{
			Color get()
			{
				return m_BackColor;
			}

			void set(Color value)
			{
				m_BackColor = value;
				m_pRenderer->setClearColour(m_BackColor.ToArgb());
			}
		}

		[Browsable(true), CategoryAttribute("Utilities"),
		DescriptionAttribute("Specifies if debug AABB is enabled")]
		property bool DebugAABB
		{
			bool get()
			{
				return m_bDebugAABB;
			}

			void set(bool value)
			{
				m_bDebugAABB = value;
				if(!value && m_pApp)
				{
					m_pApp->drawAABB((NodeVisible*)NULL);
					m_pApp->drawAxisRods((NodeVisible*)NULL);
				}
			}
		}

		[Browsable(true), CategoryAttribute("Renderer"),
		DescriptionAttribute("Specifies if lighting is enabled")]
		property bool Lighting
		{
			bool get()
			{
				return m_pRenderer->isLightEnabled();
			}

			void set(bool value)
			{
				m_pRenderer->setLightEnabled(value);
			}
		}

		[Browsable(true), CategoryAttribute("Renderer"),
		DescriptionAttribute("Specifies screen resolution")]
		property nGENEToolset::Vector2^ Resolution
		{
			nGENEToolset::Vector2^ get()
			{
				Monitor::Enter(lock);
					SDisplayMode mode = m_pRenderer->getDisplayMode();
					mode.width = m_Resolution->x;
					mode.height = m_Resolution->y;
				
					m_pRenderer->setDisplayMode(mode);
				Monitor::Exit(lock);

				return m_Resolution;
			}

			void set(nGENEToolset::Vector2^ value)
			{
				m_Resolution->x = value->x;
				m_Resolution->y = value->y;
			}
		}

		[Browsable(true), CategoryAttribute("Renderer"),
		DescriptionAttribute("Specifies if HDR lighting is enabled")]
		property bool HDR
		{
			bool get()
			{
				return ((LightStage*)m_pRenderer->getRenderStage(L"Light"))->isUsingHDR();
			}

			void set(bool value)
			{
				((LightStage*)m_pRenderer->getRenderStage(L"Light"))->setUseHDR(value);
			}
		}

		[Browsable(true), CategoryAttribute("Renderer"),
		DescriptionAttribute("Specifies if shadows are enabled")]
		property bool Shadows
		{
			bool get()
			{
				return m_pRenderer->getRenderStage(L"Shadows")->isEnabled();
			}

			void set(bool value)
			{
				m_pRenderer->getRenderStage(L"Shadows")->setEnabled(value);
			}
		}

		[Browsable(true), CategoryAttribute("Renderer"),
		DescriptionAttribute("Specifies if reflections are enabled")]
		property bool Reflections
		{
			bool get()
			{
				return m_pRenderer->getRenderStage(L"Reflection")->isEnabled();
			}

			void set(bool value)
			{
				m_pRenderer->getRenderStage(L"Reflection")->setEnabled(value);
			}
		}

		[Browsable(true), CategoryAttribute("Renderer"),
		DescriptionAttribute("Specifies if wire frame mode is enabled")]
		property bool Wireframe
		{
			bool get()
			{
				return (m_pRenderer->getFillMode() == FILL_WIREFRAME);
			}

			void set(bool value)
			{
				if(value)
				{
					m_pRenderer->setFillMode(FILL_WIREFRAME);
					m_pRenderer->setGlobalState(GS_FILLMODE, true);
				}
				else
				{
					m_pRenderer->setGlobalState(GS_FILLMODE, false);
					m_pRenderer->setFillMode(FILL_SOLID);
				}
			}
		}

		[Browsable(true), CategoryAttribute("Renderer"),
		DescriptionAttribute("Specifies if culling is enabled")]
		property bool Culling
		{
			bool get()
			{
				return (m_pRenderer->getCullingMode() != CULL_NONE);
			}

			void set(bool value)
			{
				if(value)
				{
					m_pRenderer->setCullingMode(CULL_CW);
					m_pRenderer->setGlobalState(GS_CULLING, true);
				}
				else
				{
					m_pRenderer->setGlobalState(GS_CULLING, false);
					m_pRenderer->setCullingMode(CULL_NONE);
				}
			}
		}

		[Browsable(true), CategoryAttribute("Renderer"),
		DescriptionAttribute("Specifies if anisotropy is enabled")]
		property bool Anisotropy
		{
			bool get()
			{
				if(m_pRenderer->getAnisotropyLevel(0))
					return true;
				return false;
			}

			void set(bool value)
			{
				int anisotropy = 0;
				if(value)
					m_pRenderer->getDeviceRender().testCapabilities(CAPS_MAX_ANISOTROPY, &anisotropy);

				if(value)
				{
					for(int i = 0; i < 8; ++i)
					{
						m_pRenderer->setAnisotropyLevel(i, anisotropy);
						m_pRenderer->setTextureFilter(i, FILTER_MIN | FILTER_MAG | FILTER_MIP,
							FILTER_ANISOTROPIC);
					}
				}
				else
				{
					for(int i = 0; i < 8; ++i)
					{
						m_pRenderer->setTextureFilter(i, FILTER_MIN | FILTER_MAG | FILTER_MIP,
							FILTER_NEAREST);
						m_pRenderer->setAnisotropyLevel(i, 0);
					}
				}
			}
		}

		[Browsable(true), CategoryAttribute("Editing"),
		DescriptionAttribute("Is x axis locked for movement and rotation")]
		property bool LockX
		{
			bool get()
			{
				return m_XLock;
			}

			void set(bool value)
			{
				m_XLock = value;
			}
		}

		[Browsable(true), CategoryAttribute("Editing"),
		DescriptionAttribute("Is y axis locked for movement and rotation")]
		property bool LockY
		{
			bool get()
			{
				return m_YLock;
			}

			void set(bool value)
			{
				m_YLock = value;
			}
		}

		[Browsable(true), CategoryAttribute("Editing"),
		DescriptionAttribute("Is z axis locked for movement and rotation")]
		property bool LockZ
		{
			bool get()
			{
				return m_ZLock;
			}

			void set(bool value)
			{
				m_ZLock = value;
			}
		}


		[Browsable(true), CategoryAttribute("Other")]
		property bool ShowGrid
		{
			bool get()
			{
				return false;
			}

			void set(bool value)
			{
//				SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			}
		}

		[Browsable(true), CategoryAttribute("Physics"),
		DescriptionAttribute("Specifies if physics simulation is enabled")]
		property bool PhysicsEnabled
		{
			bool get()
			{
				return Physics::getSingleton().isEnabled();
			}

			void set(bool value)
			{
				Physics::getSingleton().setEnabled(value);
			}
		}

		[Browsable(true), CategoryAttribute("Physics"),
		DescriptionAttribute("Specifies if physics simulation is enabled")]
		property Real SkinWidth
		{
			Real get()
			{
				//return pWorld->getSkinWidth();
				return 0.0f;
			}

			void set(Real value)
			{
				//pWorld->setSkinWidth(value);
			}
		}

		[Browsable(true), CategoryAttribute("Physics"),
		DescriptionAttribute("Gravity vector")]
		property nGENEToolset::Vector3^ Gravity
		{
			nGENEToolset::Vector3^ get()
			{
				/*pWorld->setGravity(m_Gravity->x,
								   m_Gravity->y,
								   m_Gravity->z);*/

				return m_Gravity;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Gravity->x = value->x;
				m_Gravity->y = value->y;
				m_Gravity->z = value->z;
			}
		}
	};
}


#endif