/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LogText.h
Version:	0.24
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_LOGTEXT_H_
#define __INC_LOGTEXT_H_


#include "LogListener.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Plain text output for logged messages.
	*/
	class nGENEDLL LogText: public LogListener
	{
	private:
		/// Output stream to store log data if necessary (xml mode)
		IFile* m_pLogFile;

		/// How many elements have to be logged before displaying them
		uint m_nRingBuffer;
		/// How many events have been logged
		uint m_nEventsCount;

		/// Vector containing logged items
		vector <LogEvent> m_vLogItems;

	public:
		LogText();
		~LogText();

		void init();
		void intro();

		void log(const LogEvent& _evt);

		/// Saves messages to file.
		void saveLogToFile();

		/// Sets the size of ring buffer.
		void setRingBuffer(uint _value);
		/// Gets the size of ring buffer.
		uint getRingBuffer() const;
	};



	inline uint LogText::getRingBuffer() const
	{
		return m_nRingBuffer;
	}
//----------------------------------------------------------------------
	inline void LogText::setRingBuffer(uint _value)
	{
		m_nRingBuffer = _value;
	}
//----------------------------------------------------------------------
}


#endif
