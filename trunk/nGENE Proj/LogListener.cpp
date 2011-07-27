/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LogListener.cpp
Version:	0.24
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "LogListener.h"


namespace nGENE
{
	LogListener::LogListener():
		m_Level(LOG_ALL),
		m_bIsEnabled(true)
	{
		ListenerRegistry <LogEvent>::addListener(this);
	}
//----------------------------------------------------------------------
	LogListener::~LogListener()
	{
	}
//----------------------------------------------------------------------
	void LogListener::init()
	{
	}
//----------------------------------------------------------------------
	void LogListener::intro()
	{
	}
//----------------------------------------------------------------------
	void LogListener::outro()
	{
	}
//----------------------------------------------------------------------
	void LogListener::handleEvent(const LogEvent& _evt)
	{
		uint type = static_cast<uint>(_evt.type);

		// Too low importance to be logged
		if(((uint)m_Level == 1 && type < 5) ||
		   ((uint)m_Level == 2 && type < 6) ||
		   ((uint)m_Level == 3 && type < 7))
			return;

		if(!m_bIsEnabled)
			return;

		log(_evt);
	}
//----------------------------------------------------------------------
	wstring LogListener::getEventTypeName(LOG_EVENT_TYPE _type)
	{
		switch(_type)
		{
		case LET_GAME_MESSAGE: return L"GAME MESSAGE";
		case LET_COMMENT: return L"COMMENT";
		case LET_EVENT: return L"EVENT";
		case LET_WARNING: return L"WARNING";
		case LET_ERROR: return L"ERROR";
		case LET_FATAL_ERROR: return L"FATAL ERROR";
		case LET_DEBUG: return L"DEBUG";
		default: return L"Unknown";
		}
	}
//----------------------------------------------------------------------
}