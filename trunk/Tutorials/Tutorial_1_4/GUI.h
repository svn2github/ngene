#include "nGENE.h"

using namespace nGENE;


class App;


class SimpleWindow: public GUIWindow
{
private:
	GUIButton btnCloseMe;

	void btnCloseMe_Click(uint _x, uint _y);

public:
	SimpleWindow();
};


class MyWindow: public GUIWindow
{
private:
	App* m_pApp;

	SimpleWindow wndChild;

	GUIStatic lblHello;
	GUIStatic lblLight;

	GUICheckBox chkLight;

	GUIButton btnShowChild;

	
	void addControls();


	void btnShowChild_Click(uint _x, uint _y);
	void chkLight_Change();

public:
	MyWindow(App* _app);
	~MyWindow();
};