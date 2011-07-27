#include "FrameworkWin32.h"
 
using namespace nGENE::Application;
 
class MyFirstApp: public FrameworkWin32
{
public:
	MyFirstApp() {}
	~MyFirstApp() {}
 
    void createApplication();
};