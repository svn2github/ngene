/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.h
Version:	0.01
Info:		This demo presents Screen Space Ambient Occlusion.
---------------------------------------------------------------------------
*/

#pragma once


#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"
#include "MyWindow.h"

#include "MyInputListener.h"

using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;


/** Demo application presenting some basic techniques
	available through nGENE Tech.
*/
class App: public FrameworkWin32
{
protected:
	ScenePartitionerQuadTree* m_pPartitioner;

	GUIFont* m_pFont;
	MyWindow m_Window;
	CameraThirdPerson* m_pCamera;

	// Post process material
	SPriorityMaterial postSSAO;

	MyInputListener* m_pInputListener;


	void cleanup();

public:
	App();
	virtual ~App();

	void createApplication();

	CameraThirdPerson* getCamera() const;
};