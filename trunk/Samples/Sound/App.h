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
#include "IAnimationController.h"
#include "OrientationController.h"
#include "InterpolatedController.h"
#include "LinearInterpolator.h"
#include "CatmullRomInterpolator.h"
#include "QuaternionInterpolator.h"
#include "PositionController.h"
#include "ScaleController.h"
#include "AnimatedMeshLoadPolicyXFile.h"
#include "NodeDebugPath.h"

#include "MyCollisionListener.h"
#include "MyInputListener.h"
#include "MyWindow.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;
using nGENE::CharacterController;
using nGENE::IAnimationController;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

	PositionController <LinearInterpolator>* m_pController;
	PositionController <LinearInterpolator>* m_pControllerLight;

	MyInputListener* m_pInputListener;
	MyCollisionListener* m_pCollisionListener;

	CameraThirdPerson* m_pCamera;

	// Post process material
	SPriorityMaterial postSSAO;

	GUIFont* m_pFont;
	MyWindow m_Window;

public:
	App();
	~App() {}

	void createApplication();
	void closeApplication();

	CameraThirdPerson* getCamera() const;
};