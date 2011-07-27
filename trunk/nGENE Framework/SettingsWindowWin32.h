/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SettingsWindowWin32.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SETTINGSWINDOWWIN32_H_
#define __INC_SETTINGSWINDOWWIN32_H_


#include "SettingsWindow.h"
#include "Singleton.h"


namespace nGENE
{
namespace Application
{
	/** Config dialog for Win32 platform.
	*/
	class SettingsWindowWin32: public SettingsWindow, public Singleton <SettingsWindowWin32>
	{
	public:
		SettingsWindowWin32();
		virtual ~SettingsWindowWin32();

		/** Creates window.
			@returns
				bool is true iff user proceeds and everything is fine and false
				if either she clicked cancel or there was en error.
		*/
		virtual bool createWindow();
		virtual void showWindow(bool value);

		static BOOL CALLBACK DlgProc(HWND hDlg, UINT nMsg, WPARAM wParam, LPARAM lParam);
	};
}
}


#endif
