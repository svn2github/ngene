#pragma once

#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"
#include "NodeMeadow.h"
#include "NodeSky.h"
#include "NodeClouds.h"

#include "MyInputListener.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;
using nGENE::CharacterController;
using nGENE::Nature::NodeClouds;
using nGENE::Nature::NodeMeadow;
using nGENE::Nature::PLANT_DESC;
using nGENE::Nature::NodeSky;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

	MyInputListener* m_pInputListener;

	CharacterController* m_pController;

public:
	App() {}
	~App() {}

	void createApplication();

	CharacterController* getController() const;
};