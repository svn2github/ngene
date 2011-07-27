#include "FrameworkWin32.h"
#include "ScenePartitionerQuadTree.h"
 
using namespace nGENE::Application;
using nGENE::ScenePartitionerQuadTree;

 
class App: public FrameworkWin32
{
private:
	ScenePartitionerQuadTree* m_pPartitioner;

public:
	App() {}
	~App() {}

	void createApplication();
};