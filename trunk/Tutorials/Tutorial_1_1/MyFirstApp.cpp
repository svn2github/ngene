#include "MyFirstApp.h"
#include "nGENE.h"

void MyFirstApp::createApplication()
{
	FrameworkWin32::createApplication();

	Renderer::getSingleton().setClearColour(0, 40, 160);

	SceneManager* sm = Engine::getSingleton().getSceneManager(0);

	Camera* cam;
	cam = sm->createCamera(L"CameraFirstPerson", L"Camera");
	cam->setPosition(Vector3(0.0f, 2.0f, -5.0f));
	sm->getRootNode()->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);

	PrefabBox* box = sm->createBox(1.0f, 2.0f, 1.0f);
	box->setPosition(0.0f, 3.0f, 7.0f);
	Surface* surf = box->getSurface(L"Base");
	Material* mat = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"crate");
	surf->setMaterial(mat);
	sm->getRootNode()->addChild(L"Box", box);

	Engine::getSingleton().getRenderer().setLightEnabled(false);
}

int main()
{
	MyFirstApp app;

	app.createApplication();
	app.run();

	app.shutdown();

	return 0;
}
