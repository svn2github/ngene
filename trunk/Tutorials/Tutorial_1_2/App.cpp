#include "App.h"
#include "nGENE.h"


void App::createApplication()
{
	FrameworkWin32::createApplication();

	m_pPartitioner = new ScenePartitionerQuadTree();
	SceneManager* sm = Engine::getSingleton().getSceneManager(0);
	sm->setScenePartitioner(m_pPartitioner);

	Renderer& renderer = Renderer::getSingleton();
	renderer.setClearColour(0);
	renderer.setCullingMode(CULL_CW);
	uint anisotropy = 0;
	renderer.getDeviceRender().testCapabilities(CAPS_MAX_ANISOTROPY, &anisotropy);
	for(uint i = 0; i < 8; ++i)
		renderer.setAnisotropyLevel(i, anisotropy);

	Camera* cam;
	cam = sm->createCamera(L"CameraFirstPerson", L"Camera");
	cam->setPosition(Vector3(0.0f, 5.0f, -10.0f));
	sm->getRootNode()->addChild(L"Camera", cam);
	Engine::getSingleton().setActiveCamera(cam);


	NodeMesh <MeshLoadPolicyXFile>* pSculp = sm->createMesh <MeshLoadPolicyXFile>(L"statue.x");
	pSculp->setPosition(0.0f, 0.0f, 7.0f);
	pSculp->setScale(0.25f, 0.25f, 0.25f);
	Surface* pSurface = pSculp->getSurface(L"surface_2");
	pSurface->flipWindingOrder();
	Material* matstone = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"dotCel");
	pSurface->setMaterial(matstone);
	sm->getRootNode()->addChild(L"Sculpture", *pSculp);

	Plane pl(Vector3::UNIT_Y, Point(0.0f, 0.0f, 0.0f));
	PrefabPlane* plane = sm->createPlane(pl, Vector2(400.0f, 400.0f));
	plane->setPosition(0.0f, 1.4, 0.0f);
	pSurface = plane->getSurface(L"Base");
	Material* matGround = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
	pSurface->setMaterial(matGround);
	sm->getRootNode()->addChild(L"Plane", plane);

	NodeLight* pLight = sm->createLight(LT_DIRECTIONAL);
	pLight->setPosition(-4.0f, 5.0f, 0.0f);
	pLight->setDirection(Vector3(0.7f, -0.5f, 1.0f));
	pLight->setDrawDebug(false);
	Colour clrWorld(204, 204, 0);
	pLight->setColour(clrWorld);
	pLight->setRadius(180.0f);
	sm->getRootNode()->addChild(L"WorldLight", *pLight);
	renderer.addLight(pLight);
}

int main()
{
	App app;

	app.createApplication();
	app.run();

	app.shutdown();

	return 0;
}
