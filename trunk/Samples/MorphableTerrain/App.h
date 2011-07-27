/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.h
Version:	0.01

Info:		This sample demonstrates how to morph between two meshes using
			vertex tweening. Morphing is not supported in nGENE out of the
			box due to the fact it's almost completely a GPU-side effect.
---------------------------------------------------------------------------
*/

#pragma once

#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"
#include "NodeMeadow.h"
#include "NodeSky.h"
#include "NodeClouds.h"

#include "MyInputListener.h"
#include "MyWindow.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;
using nGENE::CharacterController;
using nGENE::Nature::NodeMeadow;
using nGENE::Nature::PLANT_DESC;
using nGENE::Nature::NodeSky;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

	MyInputListener* m_pInputListener;

	CameraThirdPerson* m_pCamera;

	GUIFont* m_pFont;
	MyWindow m_Window;

	NodeSky sky;

public:
	App(): sky(1000.0f)
	{}
	~App() {}

	void createApplication();

	CameraThirdPerson* getCamera() const;
};