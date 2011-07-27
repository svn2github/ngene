/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		App.h
Version:	0.07
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

#include "MyInputListener.h"
#include "MyWindow.h"
 
using namespace nGENE::Application;


using nGENE::AnimatedMeshLoadPolicyXFile;
using nGENE::ScenePartitionerQuadTree;
using nGENE::CharacterController;
using nGENE::IAnimationController;
using nGENE::CatmullRomInterpolator;
using nGENE::SkeletalAnimationController;
using nGENE::KeyFrameAnimation;
using nGENE::Skeleton;
using nGENE::QuaternionInterpolator;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

	PositionController <CatmullRomInterpolator>* m_pController;
	ScaleController <LinearInterpolator>* pScaleCont;
	OrientationController <QuaternionInterpolator>* pRotCont;
	SkeletalAnimationController <AnimatedMeshLoadPolicyXFile>* m_pSkeletal;

	NodeMesh <AnimatedMeshLoadPolicyXFile>* pTiny;

	MyInputListener* m_pInputListener;

	CameraThirdPerson* m_pCamera;

	GUIFont* m_pFont;
	MyWindow m_Window;

public:
	App();
	~App() {}

	void createApplication();
	void closeApplication();

	void loadResources();

	NodeMesh <AnimatedMeshLoadPolicyXFile>* getModel() const;

	SkeletalAnimationController <AnimatedMeshLoadPolicyXFile>* getSkeletalAnimation() const;
	PositionController <CatmullRomInterpolator>* getPositionController() const;
	CameraThirdPerson* getCamera() const;
};