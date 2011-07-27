#include "GUI.h"


SimpleWindow::SimpleWindow():
	GUIWindow(WND_CAPTION | WND_BORDER | WND_CONTROLS)
{
	this->setWidth(300);
	this->setHeight(300);
	this->setPosition(50, 100);
	this->setVisible(false);
	this->setCaption(L"Simple window");

	btnCloseMe.setPosition(Vector2(10, 20));
	btnCloseMe.setHeight(32);
	btnCloseMe.setWidth(64);
	btnCloseMe.setCaption(L"close");
	btnCloseMe.setBorder(true);
	btnCloseMe.onClick.bind(this, &SimpleWindow::btnCloseMe_Click);

	this->addControl(&btnCloseMe);
}

void SimpleWindow::btnCloseMe_Click(uint _x, uint _y)
{
	this->hide();
}


MyWindow::MyWindow(App* _app):
	GUIWindow(WND_CAPTION | WND_BORDER | WND_CONTROLS),
		m_pApp(_app)
{
	this->setWidth(700);
	this->setHeight(500);
	this->setPosition(100, 200);
	this->setVisible(true);
	this->setCaption(L"Tutorial 1.4");

	addControls();
}

MyWindow::~MyWindow()
{
}

void MyWindow::addControls()
{
	lblHello.setCaption(L"Welcome to the tutorial 1.4 - GUI Creation");
	lblHello.setFontSize(18);
	lblHello.setTextColour(Colour::COLOUR_BLUE);
	lblHello.setPosition(Vector2(50, 25));

	lblLight.setCaption(L"Light enabled");
	lblLight.setFontSize(18);
	lblLight.setTextColour(Colour::COLOUR_BLACK);
	lblLight.setPosition(Vector2(50, 50));

	chkLight.setWidth(32);
	chkLight.setHeight(32);
	chkLight.setPosition(Vector2(150, 50));
	chkLight.setValue(true);
	chkLight.onChange.bind(this, &MyWindow::chkLight_Change);

	btnShowChild.setPosition(Vector2(50, 300));
	btnShowChild.setHeight(32);
	btnShowChild.setWidth(128);
	btnShowChild.setCaption(L"show child");
	btnShowChild.setBorder(true);
	btnShowChild.onClick.bind(this, &MyWindow::btnShowChild_Click);

	this->addControl(&lblHello);
	this->addControl(&lblLight);
	this->addControl(&chkLight);
	this->addControl(&btnShowChild);
}

void MyWindow::chkLight_Change()
{
	Renderer::getSingleton().setLightEnabled(chkLight.getValue());
}

void MyWindow::btnShowChild_Click(uint _x, uint _y)
{
	wndChild.setVisible(true);

	GUIManager::getSingleton().addWindow(&wndChild);
}