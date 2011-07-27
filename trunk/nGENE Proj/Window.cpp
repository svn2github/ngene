/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Window.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Window.h"


namespace nGENE
{
	// Initialize static members
	dword Window::s_nWindowsNum = 0;
	TypeInfo Window::Type(L"Window", NULL);


	Window::Window()
	{
		m_WndDesc.width = 800;
		m_WndDesc.height= 600;
		m_WndDesc.name = L"nGENE Tech";

		++s_nWindowsNum;
	}
//----------------------------------------------------------------------
	Window::Window(const WINDOW_DESC& _desc):
		m_WndDesc(_desc)
	{
		++s_nWindowsNum;
	}
//----------------------------------------------------------------------
	Window::~Window()
	{
	}
//----------------------------------------------------------------------
}