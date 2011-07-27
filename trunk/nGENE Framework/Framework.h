/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Framework.h
Version:	0.17
---------------------------------------------------------------------------
*/


#pragma once
#ifndef __INC_FRAMEWORK_H_
#define __INC_FRAMEWORK_H_


#include "Prerequisities.h"
#include "SettingsWindow.h"
#include "LogConsole.h"
#include "LogText.h"
#include "LogXML.h"

#include "GUIStage.h"
#include "PostStage.h"
#include "Ray.h"
#include "ReflectionStage.h"
#include "ShadowStage.h"


using namespace nGENE;
using nGENE::Nature::ShadowStage;
using nGENE::Nature::ReflectionStage;


namespace nGENE
{
namespace Application
{
	class Console;


	/** Base example application
		@par
			Framework is not implemented as a pure interface, rather in a similar way
			to Java adapter.
		@par
			Must be derived before using.
		@par
			First step of creating new application using this framework has to be calling
			createApplication() method and then run(). Calling shutdown is optional but
			strongly recommended.
	*/
	class Framework
	{
	protected:
		/// Is run in windowed mode?
		bool m_bWindowed;
		/// Is cursor visible?
		bool m_bShowCursor;
		/// Is configuration dialog to be shown?
		bool m_bShowConfigDialog;

		float m_fMouseSensitivity;
		/// Configuration dialog instantiated by subclass
		SettingsWindow* m_pConfigDialog;

		/// Engine singleton object
		Engine* m_pEngine;

		/// Keyboard device
		DeviceKeyboard* m_pKeyboard;
		/// Mouse device
		DeviceMouse* m_pMouse;

		/// All loggers.
		HashTable <wstring, LogListener*> m_Loggers;

		LogConsole m_LogConsole;
		LogXML m_LogXML;

		/// Console object.
		Console* m_pConsoleWnd;

		/// Stage for rendering shadows
		ShadowStage m_ShadowStage;
		/// Stage for rendering GUI
		GUIStage m_GUIStage;
		/// Stage for rendering reflections
		ReflectionStage m_ReflectionStage;

	protected:
		virtual void createSettingsWindow()=0;
		virtual bool showSettingsWindow()=0;
		virtual void cleanup()=0;
		virtual void loadShaderConfig(const wstring& _fileName)=0;

	public:
		Framework();
		virtual ~Framework();

		/// Creates application.
		virtual void createApplication()=0;
		/// Sets up logger objects.
		virtual void initLoggers();

		/// Loads default resources (textures, materials etc.).
		virtual void loadResources()=0;

		/** Run the engine.
			@remarks
				The function runs until exit message arise. Then false value
				is returned and application can safely terminate.
		*/
		virtual bool run()=0;
		/** Runs a single frame of simulation.
		*/
		virtual void runOneFrame()=0;
		/** Toggles between windowed and full-screen mode.
		*/
		virtual void toggleWindowed()=0;
		/// Hides cursor.
		virtual void hideCursor()=0;
		/// Shows cursor.
		virtual void showCursor()=0;
		/// Toggles cursor visibility.
		virtual void toggleCursor()=0;


		virtual void processInput()=0;
		virtual void shutdown()=0;

		/// Shows console window.
		virtual void showConsole();
		/// Hides console window.
		virtual void hideConsole();
		/// Toggles console window.
		virtual void toggleConsole();

		/// Config dialog is not shown at the beginning of the application.
		virtual void suppressConfigDialog()=0;

		/// Lets you pick a 3D object belonging to the scene.
		virtual ICullable* pick(float _x, float _y, bool _perTriangle=false)=0;
		/// Constructs picking ray.
		virtual Ray constructPickRay(float _x, float _y)=0;

		/// Lets you display Axis Aligned Bounding Box for a given ICullable.
		virtual void drawAABB(ICullable* _node)=0;
		/// Lets you display Axis Aligned Bounding Box for a given NodeVisible.
		virtual void drawAABB(NodeVisible* _node)=0;

		/// Lets you display axis rods for a given ICullable.
		virtual void drawAxisRods(ICullable* _node)=0;
		/// Lets you display axis rods for a given NodeVisible.
		virtual void drawAxisRods(NodeVisible* _node)=0;


		/// Serializes world.
		virtual void serialize(const wstring& _fileName);
		/// Deserializes world
		virtual void deserialize(const wstring& _fileName);


		/// Returns pointer to the shadow stage object.
		ShadowStage* getShadowStage();


		/// Adds new log.
		void addLog(const wstring& _name, LogListener* _listener);
		/// Returns log with the given name.
		LogListener* getLog(const wstring& _name);
	};



	inline ShadowStage* Framework::getShadowStage()
	{
		return &m_ShadowStage;
	}
//----------------------------------------------------------------------
}
}


#endif