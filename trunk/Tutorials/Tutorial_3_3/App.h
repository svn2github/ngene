#pragma once

#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"

#include "MyInputListener.h"
#include "CameraFirstPersonSliding.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;

using nGENE::CameraFirstPersonSliding;
using nGENE::SlidingCameraFactory;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

	MyInputListener* m_pInputListener;

	CameraFirstPersonSliding* m_pCamera;
	SlidingCameraFactory* m_pFactory;

public:
	App() {}
	~App();

	void createApplication();

	CameraFirstPersonSliding* getCamera() const;
};