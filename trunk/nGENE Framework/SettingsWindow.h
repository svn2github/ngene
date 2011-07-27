/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SettingsWindow.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SETTINGSWINDOW_H_
#define __INC_SETTINGSWINDOW_H_


#include "AutoPointer.h"
#include "Export.h"
#include "HashTable.h"
#include "Renderer.h"


namespace nGENE
{
namespace Application
{
	/** Lets you set basic display, sound and control settings
		@remarks
			This class has many pure virtual functions so if you
			want to make use of it, you have to subclass it for
			particular platform.
	*/
	class SettingsWindow
	{
	protected:
		AutoPointer <SEngineInfo> m_EngineInfo;
		HashTable <string, SDisplayMode> m_DisplayModes;

	public:
		SettingsWindow();
		virtual ~SettingsWindow();

		/// Creates window.
		virtual bool createWindow()=0;
		virtual void showWindow(bool value)=0;

		virtual SEngineInfo& getEngineInfo() {return m_EngineInfo.getRef();};
		virtual HashTable <string, SDisplayMode> getDisplayModes() const {return m_DisplayModes;};
	};
}
}


#endif