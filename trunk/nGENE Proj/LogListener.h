/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LogListener.h
Version:	0.24
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_LOGLISTENER_H_
#define __INC_LOGLISTENER_H_


#include "Log.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Listens to the logging events.
		@remarks
			In order to log an event in nGENE you have to:
			1. Derive from LogListener class.
			2. Create object of new class.
			3. Call its init() method.
			4. Call static log() method of Log class.
	*/
	class nGENEDLL LogListener: public Listener <LogEvent>
	{
	public:
		/// Level of logging
		enum LOG_LEVEL
		{
			LOG_ALL,						///< Logs all incoming messages
			LOG_WARNING,					///< Logs only warning and errors
			LOG_ERRORS,						///< Logs only errors and fatal errors
			LOG_FATAL_ERRORS				///< Logs only fatal errors
		};

	protected:
		/// Level of logging
		LOG_LEVEL m_Level;

		/// Is log enabled?
		bool m_bIsEnabled;


		/// Get log event type name
		wstring getEventTypeName(LOG_EVENT_TYPE _type);
	
	public:
		LogListener();
		virtual ~LogListener();

		/// Initializes listener
		virtual void init();

		void handleEvent(const LogEvent& _evt);

		/// Logs messages.
		virtual void log(const LogEvent& _evt)=0;

		/// Sets which messages will be logged.
		void setLogLevel(LOG_LEVEL _level);

		/// Enables/disables log
		void setEnabled(bool _value);
		/// Checks if Log is enabled.
		bool isEnabled() const;

		/// Message printed at the logging start.
		virtual void intro();
		/// Message printed at the logging end.
		virtual void outro();
	};



	inline void LogListener::setLogLevel(LogListener::LOG_LEVEL _level)
	{
		m_Level = _level;
	}
//----------------------------------------------------------------------
	inline void LogListener::setEnabled(bool _value)
	{
		m_bIsEnabled = _value;
	}
//----------------------------------------------------------------------
	inline bool LogListener::isEnabled() const
	{
		 return m_bIsEnabled;
	}
//----------------------------------------------------------------------
}


#endif
