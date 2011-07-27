#pragma once

#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"

#include "MyInputListener.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

	MyInputListener* m_pInputListener;

public:
	App() {}
	~App() {}

	void createApplication();
};