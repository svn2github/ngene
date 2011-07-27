/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once

#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"

#include "MyInputListener.h"
#include "MyWindow.h"
#include "NodeFur.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;
using nGENE::CharacterController;
using nGENE::Nature::NodeFur;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

	MyInputListener* m_pInputListener;
	MyWindow m_Window;

	CameraThirdPerson* m_pCamera;
	GUIFont* m_pFont;

	NodeFur m_Fur;

public:
	App() {}
	~App() {}

	void createApplication();
	void loadResources();

	CameraThirdPerson* getCamera() const;
};