/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		WindowWin32.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "WindowWin32.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo WindowWin32::Type(L"WindowWin32", &Window::Type);


	WindowWin32::WindowWin32(): 
		Window()
	{
		createWindow();
	}
//----------------------------------------------------------------------
	WindowWin32::WindowWin32(const WINDOW_DESC& _desc):
		Window(_desc)
	{
		createWindow();
	}
//----------------------------------------------------------------------
	WindowWin32::~WindowWin32()
	{
	}
//----------------------------------------------------------------------
	void WindowWin32::createWindow()
	{
		HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandleW(NULL));
		WNDCLASSEXW wnd;

		// Set properties of the application main window
		wnd.cbSize = sizeof(WNDCLASSEXW);
		wnd.style = CS_HREDRAW | CS_VREDRAW;
		wnd.lpfnWndProc = WndProc;
		wnd.cbClsExtra = 0;
		wnd.cbWndExtra = 0;
		wnd.hInstance = hInstance;
		wnd.hIcon = NULL;
		wnd.hCursor = LoadCursor(hInstance, IDC_ARROW);
		wnd.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		wnd.lpszMenuName = NULL;
		wnd.lpszClassName = L"nGENE Tech";
		wnd.hIconSm = NULL;

		RegisterClassExW(&wnd);

		dword wndStyle = WS_OVERLAPPEDWINDOW;

		// Adjust client region size
		RECT rect = {0, 0, m_WndDesc.width, m_WndDesc.height};
		AdjustWindowRectEx(&rect, wndStyle, false, WS_EX_LEFT);


		m_hWnd = CreateWindowExW(WS_EX_LEFT, L"nGENE Tech", m_WndDesc.name.c_str(),
								 wndStyle, rect.left + 100, rect.top + 100,
								 rect.right - rect.left, rect.bottom - rect.top,
								 NULL, NULL, hInstance, NULL);


		setWindowCaption(m_WndDesc.name.c_str());
	}
//----------------------------------------------------------------------
	void WindowWin32::showWindow(bool _value)
	{
		dword wndShow = (_value ? SW_SHOW : SW_HIDE);
		ShowWindow(m_hWnd, wndShow);
		
		UpdateWindow(m_hWnd);
	}
//----------------------------------------------------------------------
	void WindowWin32::closeWindow()
	{
		DestroyWindow(m_hWnd);
		HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandleW(NULL));
		UnregisterClassW(m_WndDesc.name.c_str(), hInstance);
	}
//----------------------------------------------------------------------
	void WindowWin32::setWindowCaption(const wstring& _caption)
	{
		m_WndDesc.name = _caption;
		SetWindowTextW(m_hWnd, m_WndDesc.name.c_str());
	}
//----------------------------------------------------------------------
	LRESULT WindowWin32::WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(nMsg)
		{
		case WM_DESTROY:
			if(!(--s_nWindowsNum))
				PostQuitMessage(0);
			return 0;
		}

		return DefWindowProcW(hwnd, nMsg, wParam, lParam);
	}
//----------------------------------------------------------------------
}