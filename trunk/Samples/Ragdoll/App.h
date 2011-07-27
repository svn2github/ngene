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
#include "MyWindow.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;
using nGENE::CharacterController;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;
	PhysicsWorld* pWorld;
	PhysicsMaterial* physMat;
	PhysicsActor* pSelectedActor;

	vector <NodePrefab*> prefabs;
	vector <PhysicsActor*> actors;

	MyInputListener* m_pInputListener;

	CameraThirdPerson* m_pCamera;

	GUIFont* m_pFont;
	MyWindow m_Window;

	PhysicsActor* createSphere(SceneManager* sm, const wstring& name, float radius, Vector3& pos);
	PhysicsActor* createCapsule(SceneManager* sm, const wstring& name, Vector3& pos, Quaternion* rot, float height, float radius);
	PhysicsActor* createBox(SceneManager* sm, const wstring& name, Vector3& pos, Quaternion* rot, Vector3& size);

	void createSphericalJoint(PhysicsActor* actor1, PhysicsActor* actor2, Vector3& pos, const Vector3& axis);
	void createHingeJoint(PhysicsActor* actor1, PhysicsActor* actor2, Vector3& pos, const Vector3& axis);

public:
	App() {}
	~App() {}

	void createApplication();

	CameraThirdPerson* getCamera() const;
	PhysicsActor* getSelectedActor() const;

	void selectNextActor();
};