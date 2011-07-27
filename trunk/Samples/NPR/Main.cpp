/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Main.cpp
Version:	0.50
Info:		Main source for the IGK'2008 demo application.
---------------------------------------------------------------------------
*/

#define NOMINMAX
#include <windows.h>
#include "App.h"

using namespace nGENE::Application;


/*
Application is created here.
*/
int main()
{
	#ifdef _WIN32
		App app;
	#else
	#endif

	app.createApplication();
	app.run();

	app.shutdown();


	return 0;
}