/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIStage.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUISTAGE_H_
#define __INC_GUISTAGE_H_


#include "Prerequisities.h"
#include "RenderStage.h"


namespace nGENE
{
	/// Render stage for GUI.
	class nGENEDLL GUIStage: public RenderStage
	{
	private:
		GUIManager* m_pManager;

	public:
		GUIStage();
		~GUIStage();

		void render();

		void setManager(GUIManager* _manager);
	};



	inline void GUIStage::setManager(GUIManager* _manager)
	{
		m_pManager = _manager;
	}
//----------------------------------------------------------------------
}


#endif