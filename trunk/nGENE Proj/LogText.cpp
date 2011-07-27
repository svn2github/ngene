/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LogText.cpp
Version:	0.24
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "LogText.h"


namespace nGENE
{
	LogText::LogText():
		m_pLogFile(NULL),
		m_nRingBuffer(100),
		m_nEventsCount(0)
	{
	}
//----------------------------------------------------------------------
	LogText::~LogText()
	{
		if(!m_vLogItems.empty())
		{
			saveLogToFile();
			m_vLogItems.clear();
		}

		if(m_pLogFile)
		{
			m_pLogFile->close();
			NGENE_DELETE(m_pLogFile);
		}
	}
//----------------------------------------------------------------------
	void LogText::intro()
	{
		m_pLogFile->write(L"nGENE Tech logging system started...\n\n");
	}
//----------------------------------------------------------------------
	void LogText::init()
	{
		if(m_nRingBuffer)
			m_vLogItems.reserve(m_nRingBuffer);

		m_pLogFile = new File(L"Log.txt", OPEN_WRITE);

		// Write introduction message
		intro();
	}
//----------------------------------------------------------------------
	void LogText::log(const LogEvent& _evt)
	{
		m_vLogItems.push_back(_evt);

		// If vector contains more or equal items than
		// ring buffer size, then clear it and save log to file.
		// Also save when errors occur
		if((m_vLogItems.size() >= m_nRingBuffer) ||
		   (_evt.type == LET_ERROR) || 
		   (_evt.type == LET_FATAL_ERROR))
		{
			// Save results
			saveLogToFile();
			m_vLogItems.clear();
		}
	}
//----------------------------------------------------------------------
	void LogText::saveLogToFile()
	{
		if(!m_bIsEnabled)
			return;

		vector <LogEvent>::iterator it;
		for(it = m_vLogItems.begin(); it != m_vLogItems.end(); ++it)
		{
			m_pLogFile->write(L"%ls: ", getEventTypeName(it->type).c_str());
			m_pLogFile->write(L"%ls ", it->stMessage.c_str());
			m_pLogFile->write(L"in %ls ", it->stFunction.c_str());
			m_pLogFile->write(L"in %ls ", it->stSourceFile.c_str());
			m_pLogFile->write(L"at line %d\n", it->nLine);
		}
	}
//----------------------------------------------------------------------
}