/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Framework.cpp
Version:	0.15
---------------------------------------------------------------------------
*/

#include "Framework.h"
#include "Console.h"
#include "ISerializer.h"
#include "Node.h"
#include "XMLSerializer.h"


namespace nGENE
{
namespace Application
{
	Framework::Framework():
		m_bShowConfigDialog(true),
		m_pConfigDialog(NULL),
		m_pEngine(NULL),
		m_pKeyboard(NULL),
		m_pMouse(NULL),
		m_pConsoleWnd(NULL)
	{
	}
//----------------------------------------------------------------------
	Framework::~Framework()
	{
		XMLSerializer::removeFactories();
	}
//----------------------------------------------------------------------
	void Framework::initLoggers()
	{
		m_LogConsole.init();
		m_LogXML.init();

		m_Loggers[L"DefaultLogConsole"] = &m_LogConsole;
		m_Loggers[L"DefaultLogXML"] = &m_LogXML;
	}
//----------------------------------------------------------------------
	void Framework::showConsole()
	{
		if(m_pConsoleWnd)
			m_pConsoleWnd->setVisible(true);
	}
//----------------------------------------------------------------------
	void Framework::hideConsole()
	{
		if(m_pConsoleWnd)
			m_pConsoleWnd->setVisible(false);
	}
//----------------------------------------------------------------------
	void Framework::toggleConsole()
	{
		if(m_pConsoleWnd)
			m_pConsoleWnd->toggleVisible();
	}
//----------------------------------------------------------------------
	void Framework::serialize(const wstring& _fileName)
	{
		Engine& engine = Engine::getSingleton();
		for(uint i = 0; i < engine.getSceneManagersNum(); ++i)
		{
			XMLSerializer serializer;
			serializer.setFileName(_fileName);
			serializer.setSceneManager(engine.getSceneManager(i));
			engine.getSceneManager(i)->serialize(&serializer, true, true);
		}
	}
//----------------------------------------------------------------------
	void Framework::deserialize(const wstring& _fileName)
	{
		// Remove all post-process materials
		((nGENE::PostStage*)Renderer::getSingleton().getRenderStage(L"PostProcess"))->removeAllMaterials();

		Engine& engine = Engine::getSingleton();
		for(uint i = 0; i < engine.getSceneManagersNum(); ++i)
		{
			engine.getSceneManager(i)->cleanup();

			XMLSerializer serializer;
			serializer.setFileName(_fileName);
			serializer.setSceneManager(engine.getSceneManager(i));
			serializer.deserialize();
		}
	}
//----------------------------------------------------------------------
	LogListener* Framework::getLog(const wstring& _name)
	{
		if(m_Loggers.find(_name) == m_Loggers.end())
			return NULL;

		return m_Loggers[_name];
	}
//----------------------------------------------------------------------
	void Framework::addLog(const wstring& _name, LogListener* _listener)
	{
		m_Loggers[_name] = _listener;
	}
//----------------------------------------------------------------------
}
}
