/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Console.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CONSOLE_H_
#define __INC_CONSOLE_H_


#include "GUIWindow.h"
#include "InputListener.h"
#include "LogListener.h"
#include "Prerequisities.h"


namespace nGENE
{
namespace Application
{
	/** Console for running nGENE commands.
		@par
			Apart from running nGENE commands console serves
			purpose of logger too.
	*/
	class Console: public GUIWindow, public LogListener
	{
	private:
		GUIListBox* m_lstCommands;
		GUITextField* m_txtCommand;

		ScriptLua* m_pScript;

		/// History of previously used commands.
		vector <wstring> m_CommandsHistory;
		uint m_CommandIndex;

	public:
		Console();
		~Console();

		/// Sets visibility of the console.
		void setVisible(bool _value);
		/// Toggles console window visibility.
		void toggleVisible();

		void log(const LogEvent& _evt);

		void lstCommandsChange();
		void txtCommandKeyboardDown(const KeyboardEvent& _evt);

		/// Specifies whether the control is enabled or not.
		void setEnabled(bool _value);
		/// Toggles enabled state of this control.
		void toggleEnabled();
	};
}
}


#endif