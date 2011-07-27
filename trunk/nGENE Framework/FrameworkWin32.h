/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FrameworkWin32.h
Version:	0.19
Info:		Framework header for 32-bit Windows platform.
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRAMEWORKWIN32_H_
#define __INC_FRAMEWORKWIN32_H_


#define NOMINMAX
#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400
#endif
#include <windows.h>

#include "Framework.h"
#include "ShaderSemanticFunctionLua.h"


namespace nGENE
{
namespace Application
{
	/** Windows32 example application.
	*/
	class FrameworkWin32: public Framework
	{
	private:
		void loadScript(XMLDocument* _node);
		void loadSemantic(XMLDocument* _node);
		void loadDataType(XMLDocument* _node);
		void applyFunction(SEMANTIC_DESC* _semantic);

		void createApp(HWND* _hwnd, ENGINE_DESC* _desc=NULL);


		bool m_bProfile;
		bool m_bMultiThreadedRendering;

	protected:
		/// Application instance
		HINSTANCE m_hInstance;

		/// Application window
		/// As for now only one is used but in the future
		/// more windows per application will be available
		HWND hwnd;


		ScriptLua* m_pScript;
		vector <ShaderSemanticFunction*> m_vFunctions;


		virtual void createSettingsWindow();
		virtual bool showSettingsWindow();
		virtual void cleanup();
		virtual void loadShaderConfig(const wstring& _fileName);

		virtual Node* getDebugAABB();
		virtual Node* getDebugAxisRods();

	public:
		FrameworkWin32(bool _profiler=false, bool _multiThreadedRendering=false);
		virtual ~FrameworkWin32();

		virtual void createApplication();
		virtual void createApplication(HWND* _hwnd);
		virtual void createApplication(HWND* _hwnd, ENGINE_DESC* _desc);

		virtual void loadResources();

		virtual void toggleWindowed();

		virtual void hideCursor();
		virtual void showCursor();
		virtual void toggleCursor();

		virtual void shutdown();
		virtual void processInput();
		virtual bool run();
		virtual void runOneFrame();

		ICullable* pick(float _x, float _y, bool _perTriangle=false);
		Ray constructPickRay(float _x, float _y);

		virtual void drawAABB(ICullable* _node);
		virtual void drawAABB(NodeVisible* _node);

		virtual void drawAxisRods(ICullable* _node);
		virtual void drawAxisRods(NodeVisible* _node);

		virtual void closeApplication();

		virtual void suppressConfigDialog();

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	};
}
}


#endif