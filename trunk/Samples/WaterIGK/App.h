/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once

#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"

#include "MyInputListener.h"
#include "MyWindow.h"
#include "NodeClouds.h"
#include "NodeMeadow.h"
#include "NodeSky.h"
#include "NodeWater.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;
using nGENE::CharacterController;

using nGENE::Nature::NodeClouds;
using nGENE::Nature::NodeMeadow;
using nGENE::Nature::PLANT_DESC;
using nGENE::Nature::DECORATOR_DESC;
using nGENE::Nature::NodeSky;
using nGENE::Nature::NodeWater;


#define POST_MATERIALS_COUNT 23

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

	MyInputListener* m_pInputListener;
	MyWindow m_Window;
	SplashScreen* m_Splash;

	CameraFirstPerson* m_pCamera;
	GUIFont* m_pFont;

	NodeMeadow meadow;
	NodeMeadow forest;

	SPriorityMaterial m_Post[POST_MATERIALS_COUNT];

	TimeDate time;


	void createPostProcess();

public:
	App(bool _profile=false);
	~App() {}

	void createApplication();

	CameraFirstPerson* getCamera();
	SPriorityMaterial* getPostEffect(uint _index);

	TimeDate& getTime();

	NodeWater* getWater();
	NodeSky* getSky() const;
	NodeClouds* getClouds() const;
};