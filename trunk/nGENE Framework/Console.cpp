/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Console.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "Console.h"
#include "Engine.h"
#include "GUIListBox.h"
#include "GUITextField.h"
#include "InputSystem.h"
#include "ScriptLua.h"
#include "ScriptSystemLua.h"


namespace nGENE
{
namespace Application
{
	Console::Console():
		GUIWindow(WND_BORDER),
		m_lstCommands(NULL),
		m_txtCommand(NULL),
		m_pScript(NULL),
		m_CommandIndex(0)
	{
		this->setWidth(800);
		this->setHeight(462);
		this->setPosition(0, 0);
		this->setVisible(false);
		this->setMovable(false);

		m_lstCommands = new GUIListBox();
		m_lstCommands->setWidth(780);
		m_lstCommands->setHeight(400);
		m_lstCommands->setPosition(10, 10);
		m_lstCommands->setMultiSelection(false);
		m_lstCommands->onChange.bind(this, &Console::lstCommandsChange);

		m_txtCommand = new GUITextField();
		m_txtCommand->setWidth(780);
		m_txtCommand->setHeight(32);
		m_txtCommand->setPosition(10, 420);
		m_txtCommand->setMultiline(false);
		m_txtCommand->setFontSize(24);
		m_txtCommand->onKeyboard.bind(this, &Console::txtCommandKeyboardDown);

		this->addControl(m_lstCommands);
		this->addControl(m_txtCommand);

		// Create script
		m_pScript = Engine::getSingleton().getLuaScripting()->createScript();
	}
//----------------------------------------------------------------------
	Console::~Console()
	{
		NGENE_DELETE(m_lstCommands);
		NGENE_DELETE(m_txtCommand);

		Engine::getSingleton().getLuaScripting()->removeScript(m_pScript);
	}
//----------------------------------------------------------------------
	void Console::setVisible(bool _value)
	{
		GUIWindow::setVisible(_value);
	}
//----------------------------------------------------------------------
	void Console::toggleVisible()
	{
		GUIWindow::toggleVisible();
	}
//----------------------------------------------------------------------
	void Console::lstCommandsChange()
	{
		LIST_BOX_ITEM* pItem = (*m_lstCommands->getSelected())[0];
		wstring command = pItem->text;

		m_txtCommand->setText(command);
	}
//----------------------------------------------------------------------
	void Console::txtCommandKeyboardDown(const KeyboardEvent& _evt)
	{
		if(_evt.isKeyPressed(KC_RETURN))
		{
			wstring temp = m_txtCommand->getText();
			string command(temp.begin(), temp.end());

			m_pScript->runScriptFromString(command);
			m_lstCommands->addItem(temp);

			m_CommandsHistory.push_back(temp);
			m_CommandIndex = m_CommandsHistory.size();

			m_txtCommand->setText(L"");
		}
		else if(_evt.isKeyPressed(KC_DOWN))
		{
			if(m_CommandIndex < m_CommandsHistory.size() - 1)
			{
				++m_CommandIndex;
				m_txtCommand->setText(m_CommandsHistory[m_CommandIndex]);
			}
		}
		else if(_evt.isKeyPressed(KC_UP))
		{
			if(m_CommandIndex > 0)
			{
				--m_CommandIndex;
				m_txtCommand->setText(m_CommandsHistory[m_CommandIndex]);
			}
		}
	}
//----------------------------------------------------------------------
	void Console::log(const LogEvent& _evt)
	{
		if(!m_bEnabled)
			return;

		wstring msg = getEventTypeName(_evt.type) + L":" + _evt.stMessage;
		m_lstCommands->addItem(msg);
	}
//----------------------------------------------------------------------
	void Console::setEnabled(bool _value)
	{
		GUIControl::setEnabled(_value);
		LogListener::setEnabled(_value);
	}
//----------------------------------------------------------------------
	void Console::toggleEnabled()
	{
		bool bEnabled = (GUIControl::isEnabled() ? false : true);

		GUIControl::setEnabled(bEnabled);
		LogListener::setEnabled(bEnabled);
	}
//----------------------------------------------------------------------
}
}
