/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIPromptWindow.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUIPROMPTWINDOW_H_
#define __INC_GUIPROMPTWINDOW_H_


#include "GUIWindow.h"
#include "GUIStatic.h"
#include "Prerequisities.h"


namespace nGENE
{
	/// Buttons for the prompt box.
	enum PROMPT_WINDOW_BUTTON
	{
		PWB_OK,
		PWB_OK_CANCEL,
		PWB_YES_NO
	};

	/// Result of the prompt window.
	enum PROMPT_WINDOW_RESULT_BUTTON
	{
		PWRB_NONE,
		PWRB_OK,
		PWRB_CANCEL,
		PWRB_YES,
		PWRB_NO
	};


	/** Prompt Window control.
		@par
			Prompt Window gives possibility of asking user a question.
			It is similar to the Message Box from many GUI frameworks
	*/
	class nGENEDLL GUIPromptWindow: public GUIWindow
	{
	private:
		GUIStatic m_lblMessage;						///< Message for the user
		PROMPT_WINDOW_RESULT_BUTTON m_Result;		///< Which button was pressed?

		vector <GUIButton> m_vButtons;				///< Vector with buttons


		void _ButtonOK_Click(uint _x, uint _y);
		void _ButtonCancel_Click(uint _x, uint _y);
		void _ButtonYES_Click(uint _x, uint _y);
		void _ButtonNO_Click(uint _x, uint _y);

		void _Window_Paint();

		void buttonPressed();

	public:
		// Here go delegates
		typedef fastdelegate::FastDelegate1 <PROMPT_WINDOW_RESULT_BUTTON> ON_BUTTON_PRESSED_DELEGATE;

		// Mouse events
		ON_BUTTON_PRESSED_DELEGATE onButtonPressed;

	public:
		GUIPromptWindow(PROMPT_WINDOW_BUTTON _buttons, const wstring& _message,
			const wstring& _title);
		virtual ~GUIPromptWindow();

		/// Sets message.
		void setMessage(const wstring& _message);
		/// Returns message.
		const wstring& getMessage() const;

		/// Sets visibility of the control.
		virtual void setVisible(bool _value);
		/// Toggles visibility of the control.
		virtual void toggleVisible();
		/// Shows control.
		virtual void show();

		/// Returns which button was pressed.
		PROMPT_WINDOW_RESULT_BUTTON getResult() const;
	};



	inline void GUIPromptWindow::setMessage(const wstring& _message)
	{
		m_lblMessage.setText(_message);
	}
//----------------------------------------------------------------------
	inline const wstring& GUIPromptWindow::getMessage() const
	{
		return m_lblMessage.getText();
	}
//----------------------------------------------------------------------
	inline PROMPT_WINDOW_RESULT_BUTTON GUIPromptWindow::getResult() const
	{
		return m_Result;
	}
//----------------------------------------------------------------------
	inline void GUIPromptWindow::setVisible(bool _value)
	{
		GUIWindow::setVisible(_value);

		if(_value)
			activate();
	}
//----------------------------------------------------------------------
	inline void GUIPromptWindow::toggleVisible()
	{
		GUIWindow::toggleVisible();

		if(m_bVisible)
			activate();
	}
//----------------------------------------------------------------------
	inline void GUIPromptWindow::show()
	{
		GUIWindow::show();

		activate();
	}
//----------------------------------------------------------------------
}


#endif