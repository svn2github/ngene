/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Time.h
Version:	0.11
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TIMER_H_
#define __INC_TIMER_H_


#ifdef _WIN32
	#define NOMINMAX
    #ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0400
	#endif
	#include <windows.h>
#endif
#include "BaseClass.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Lets you control time during application execution.
	*/
	class nGENEDLL Timer: public BaseClass
	{
		EXPOSE_TYPE
	private:
		#ifdef _WIN32
			LARGE_INTEGER m_nFrequency;
			unsigned long m_nStartMiliseconds;
			LARGE_INTEGER m_nStartMicroseconds;
		#endif

		/// Is timer enabled?
		bool m_bEnabled;
		/// Is timer counting from 0?
		bool m_bFromZero;

	public:
		/** Timer class constructor.
			@param
				startFromZero determines whether timer should count time passed
				since timer's start or the one passed since OS startup.
		*/
		Timer(bool _startFromZero=true);
		virtual ~Timer();

		void init();
		void cleanup();
		void reset();

		/// Returns time passed in milliseconds.
		dword getMilliseconds();
		/// Returns time passed in microseconds.
		unsigned __int64 getMicroseconds();

		/// Sets whether timer is enabled or not.
		void setEnabled(bool _value);
		/// Checks if this timer is enabled.
		bool isEnabled() const;
	};



	inline void Timer::setEnabled(bool _value)
	{
		m_bEnabled = _value;
	}
//----------------------------------------------------------------------
	inline bool Timer::isEnabled() const
	{
		return m_bEnabled;
	}
//----------------------------------------------------------------------
}


#endif