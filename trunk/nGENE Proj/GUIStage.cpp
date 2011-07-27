/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIStage.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIStage.h"
#include "GUIManager.h"


namespace nGENE
{
	GUIStage::GUIStage():
		m_pManager(NULL)
	{
	}
//----------------------------------------------------------------------
	GUIStage::~GUIStage()
	{
	}
//----------------------------------------------------------------------
	void GUIStage::render()
	{
		if(m_pManager && m_bEnabled)
			m_pManager->render();
	}
//----------------------------------------------------------------------
}
