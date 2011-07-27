/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LogXML.cpp
Version:	0.26
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "LogXML.h"
#include "TimeDate.h"


namespace nGENE
{
	LogXML::LogXML():
		m_pLogFile(NULL),
		m_nRingBuffer(100),
		m_nEventsCount(0)
	{
	}
//----------------------------------------------------------------------
	LogXML::~LogXML()
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
	void LogXML::init()
	{
		if(m_nRingBuffer)
			m_vLogItems.reserve(m_nRingBuffer);

		m_pLogFile = new File(L"Log.xml", OPEN_WRITE);

		// Write introduction message
		intro();
	}
//----------------------------------------------------------------------
	void LogXML::log(const LogEvent& _evt)
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
	void LogXML::intro()
	{
		m_pLogFile->write(L"<?xml version=\"1.0\" ?>\n");
		m_pLogFile->write(L"<?xml-stylesheet type=\"text/xsl\" href=\"Logger.xsl\"?>\n");
		m_pLogFile->write(L"<RunTimeLog>\n");

		m_pLogFile->write(L"<LogHeader>\n");
		m_pLogFile->write(L"<Session>\n");
		m_pLogFile->write(L"<Started>\n");
		TimeDate time;
		time = time.now();
		m_pLogFile->write(L"<Time>%02d:%02d:%02d</Time>", time.hours, time.minutes, time.seconds);
		m_pLogFile->write(L"<Date>%02d.%02d.%d</Date>", time.month_day, time.month, time.year);
		m_pLogFile->write(L"</Started>\n");

		CPUInfo cpu;
		cpu.getCPUInfo();
		string temp = cpu.getCPUName();
		wstring stCpuName(temp.begin(), temp.end());
		
		m_pLogFile->write(L"<Configuration>\n");
		m_pLogFile->write(L"<ProcessorType>%ls</ProcessorType>\n", stCpuName.c_str());
		m_pLogFile->write(L"<NumberOfProcessors>%d</NumberOfProcessors>\n", cpu.getLogicalProcessorsNum());
		m_pLogFile->write(L"</Configuration>\n");

		m_pLogFile->write(L"</Session>\n");
		m_pLogFile->write(L"</LogHeader>\n");

		outro();
	}
//----------------------------------------------------------------------
	void LogXML::outro()
	{
		wstring outro = L"</RunTimeLog>";
		m_pLogFile->write(outro.c_str());
		m_pLogFile->seek(-(int)((uint)(outro.length())), SD_CURRENT);
	}
//----------------------------------------------------------------------
	void LogXML::saveLogToFile()
	{
		if(!m_bIsEnabled)
			return;

		vector <LogEvent>::iterator it;
		for(it = m_vLogItems.begin(); it != m_vLogItems.end(); ++it)
		{
			m_pLogFile->write(L"<LogEvent id=\"%d\">\n", ++m_nEventsCount);
			m_pLogFile->write(L"<Type>%d</Type>\n", it->type);
			m_pLogFile->write(L"<TimeIndex>%d ms</TimeIndex>\n", it->nTime);
			m_pLogFile->write(L"<NameSpace>%ls</NameSpace>\n", it->stNamespace.c_str());
			m_pLogFile->write(L"<File>%ls</File>\n", it->stSourceFile.c_str());
			m_pLogFile->write(L"<Function>%ls()</Function>\n", it->stFunction.c_str());
			m_pLogFile->write(L"<LineNumber>%d</LineNumber>\n", it->nLine);
			m_pLogFile->write(L"<Message>%ls</Message>\n", it->stMessage.c_str());
			m_pLogFile->write(L"</LogEvent>\n");

			outro();
		}
	}
//----------------------------------------------------------------------
}