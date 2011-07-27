/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SettingsWindow.cpp
Version:	0.03
---------------------------------------------------------------------------
*/


#include "PrecompiledHeaders.h"
#include "SettingsWindow.h"


namespace nGENE
{
namespace Application
{
	SettingsWindow::SettingsWindow():
		m_EngineInfo(new SEngineInfo())
	{
	}
//----------------------------------------------------------------------
	SettingsWindow::~SettingsWindow()
	{
	}
//----------------------------------------------------------------------
}
}