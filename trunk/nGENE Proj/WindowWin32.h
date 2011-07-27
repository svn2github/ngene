/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		WindowWin32.h
Version:	0.15
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_WINDOW32_H_
#define __INC_WINDOW32_H_


#include "Prerequisities.h"
#include "TypeInfo.h"
#include "Window.h"


#ifdef _WIN32
	#define NOMINMAX
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0400
	#endif
	#include <windows.h>
#endif


namespace nGENE
{
	/** Window to be use in Win32 platform.
	*/
	class nGENEDLL WindowWin32: public Window
	{
		EXPOSE_TYPE
	private:
		// Hide assignment operator and copy constructor to prevent
		// it from being used.
		WindowWin32(const WindowWin32& src);
		WindowWin32& operator=(const WindowWin32& rhs);

	protected:
		/// Handle to the window
		HWND m_hWnd;

	public:
		WindowWin32();
		WindowWin32(const WINDOW_DESC& _desc);
		virtual ~WindowWin32();

		/// Creates new Win32 compatible window.
		void createWindow();
		/** Shows/hides window.
			@param
				value when true window is showed, otherwise it is
				hidden.
		*/
		void showWindow(bool _value);
		void closeWindow();

		HWND getHandle() const {return m_hWnd;}

		/// Sets the caption of the window.
		void setWindowCaption(const wstring& _caption);

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	};
}


#endif