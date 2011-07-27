#pragma once

#include "InputListener.h"
#include "Vector3.h"
#include "Quaternion.h"

using nGENE::InputListener;
using nGENE::KeyboardEvent;
using nGENE::MouseEvent;
using nGENE::Vector3;
using nGENE::Real;
using nGENE::Quaternion;

class App;

class MyInputListener: public InputListener
{
private:
	App* m_pApp;

	Real m_Angle1;
	Real m_Angle2;

	long m_prev;

	bool m_bMenuMode;

public:
	MyInputListener();
	~MyInputListener();

	void handleEvent(const MouseEvent& _evt);
	void handleEvent(const KeyboardEvent& _evt);

	void setApp(App* _app) {m_pApp = _app;}
};
