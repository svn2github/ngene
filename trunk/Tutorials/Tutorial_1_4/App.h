#pragma once

#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"

#include "MyInputListener.h"
#include "GUI.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

	MyInputListener* m_pInputListener;
	MyWindow* m_pWindow;

public:
	App() {}
	~App() {}

	void createApplication();
};