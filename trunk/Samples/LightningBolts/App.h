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
#include "MyFrameListener.h"
#include "MyWindow.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;
using nGENE::CharacterController;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

	MyInputListener* m_pInputListener;
	MyFrameListener* m_pFrameListener;

	CameraThirdPerson* m_pCamera;
	GUIFont* m_pFont;

	MyWindow m_Window;

public:
	App() {}
	~App() {}

	void createApplication();

	CameraThirdPerson* getCamera() const;
};