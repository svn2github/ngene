/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once

#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"
#include "MyInputListener.h"
 
using namespace nGENE::Application;
 
class App: public FrameworkWin32
{
private:
	MyInputListener* m_pInputListener;
	CameraThirdPerson* m_pCamera;

public:
	App() {}
	~App() {}

	void createApplication();
	// This function will load materials and stuff like that.
	void loadResources();

	CameraThirdPerson* getCamera() const;
};