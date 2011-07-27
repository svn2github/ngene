#include "MyInputListener.h"
#include "nGENE.h"

#include "App.h"

using namespace nGENE;

MyInputListener::MyInputListener():
	m_pApp(NULL)
{
}

MyInputListener::~MyInputListener()
{
}

void MyInputListener::handleEvent(const MouseEvent &_evt)
{
}

void MyInputListener::handleEvent(const KeyboardEvent& _evt)
{
	if(_evt.isKeyPressed(KC_ESCAPE))
	{
		m_pApp->closeApplication();
		return;
	}

	return;
}