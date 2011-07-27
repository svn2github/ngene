#include "JNIApp.h"
#include "App.h"

App app;

JNIEXPORT void JNICALL Java_JNIApp_initCppSide(JNIEnv*, jobject, jint hwnd)
{
	HWND handle = (HWND)hwnd;
	app.createApplication(&handle);
	//app.run();

	//app.shutdown();
}

JNIEXPORT void JNICALL Java_JNIApp_oneFrame(JNIEnv *, jobject)
{
	app.runOneFrame();
}