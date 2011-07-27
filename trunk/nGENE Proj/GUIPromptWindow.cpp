/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIPromptWindow.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIPromptWindow.h"


namespace nGENE
{
	GUIPromptWindow::GUIPromptWindow(PROMPT_WINDOW_BUTTON _buttons,
		const wstring& _message, const wstring& _title):
		GUIWindow(WND_BORDER | WND_CAPTION),
		m_Result(PWRB_NONE),
		onButtonPressed(NULL)
	{
		this->setWidth(400);
		this->setHeight(200);
		this->onPaint.bind(this, &GUIPromptWindow::_Window_Paint);
		this->setCaption(_title);
		this->setMovable(false);

		m_lblMessage.setPosition(Vector2(10, 10));
		m_lblMessage.setHeight(100);
		m_lblMessage.setWidth(380);
		m_lblMessage.setCaption(_message);
		m_lblMessage.setFontSize(20);
		m_lblMessage.setTextWrapping(TW_WORD);
		
		this->addControl(&m_lblMessage);


		// Create buttons
		if(_buttons == PWB_OK)
		{
			GUIButton buttonOK(Vector2(64, 32), Vector2(326, 126), L"OK", true);
			
			m_vButtons.push_back(buttonOK);
			m_vButtons[0].onClick.bind(this, &GUIPromptWindow::_ButtonOK_Click);
		}
		else if(_buttons == PWB_OK_CANCEL)
		{
			GUIButton buttonOK(Vector2(64, 32), Vector2(10, 126), L"OK", true);
			GUIButton buttonCancel(Vector2(64, 32), Vector2(326, 126), L"Cancel", true);

			m_vButtons.push_back(buttonOK);
			m_vButtons.push_back(buttonCancel);

			m_vButtons[0].onClick.bind(this, &GUIPromptWindow::_ButtonOK_Click);
			m_vButtons[1].onClick.bind(this, &GUIPromptWindow::_ButtonCancel_Click);
		}
		else if(_buttons == PWB_YES_NO)
		{
			GUIButton buttonYES(Vector2(64, 32), Vector2(10, 126), L"YES", true);
			GUIButton buttonNO(Vector2(64, 32), Vector2(326, 126), L"NO", true);

			m_vButtons.push_back(buttonYES);
			m_vButtons.push_back(buttonNO);

			m_vButtons[0].onClick.bind(this, &GUIPromptWindow::_ButtonYES_Click);
			m_vButtons[1].onClick.bind(this, &GUIPromptWindow::_ButtonNO_Click);
		}

		Colour clrHighlighted = Colour::COLOUR_BLACK;
		for(uint i = 0; i < m_vButtons.size(); ++i)
		{
			GUIButton& button = m_vButtons[i];

			button.setTextAlignment(TEXT_CENTRE);
			button.setVerticalAlignment(CVA_MIDDLE);
			button.setFontSize(20);
			button.setMouseOverColour(clrHighlighted);

			addControl(&button);
		}
	}
//----------------------------------------------------------------------
	GUIPromptWindow::~GUIPromptWindow()
	{
	}
//----------------------------------------------------------------------
	void GUIPromptWindow::_ButtonOK_Click(uint _x, uint _y)
	{
		m_Result = PWRB_OK;
		
		buttonPressed();
	}
//----------------------------------------------------------------------
	void GUIPromptWindow::_ButtonCancel_Click(uint _x, uint _y)
	{
		m_Result = PWRB_CANCEL;

		buttonPressed();
	}
//----------------------------------------------------------------------
	void GUIPromptWindow::_ButtonYES_Click(uint _x, uint _y)
	{
		m_Result = PWRB_YES;

		buttonPressed();
	}
//----------------------------------------------------------------------
	void GUIPromptWindow::_ButtonNO_Click(uint _x, uint _y)
	{
		m_Result = PWRB_NO;

		buttonPressed();
	}
//----------------------------------------------------------------------
	void GUIPromptWindow::_Window_Paint()
	{
		SDisplayMode displayMode = Renderer::getSingleton().getDisplayMode();
		
		this->setPosition((displayMode.width - m_fWidth) * 0.5f,
						  (displayMode.height - m_fHeight) * 0.5f);
	}
//----------------------------------------------------------------------
	void GUIPromptWindow::buttonPressed()
	{
		setVisible(false);

		if(onButtonPressed)
			onButtonPressed(m_Result);
	}
//----------------------------------------------------------------------
}