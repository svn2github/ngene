#ifdef _WIN32
	#define NOMINMAX
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0400
	#endif
	#include <windows.h>
	#include "FrameworkWin32.h"
#endif

using namespace nGENE::Application;


/*
Application is created here.
*/
int main()
{
	#ifdef _WIN32
		FrameworkWin32 app;
	#else
	#endif

	app.createApplication();
	app.run();

	app.shutdown();


	return 0;
}