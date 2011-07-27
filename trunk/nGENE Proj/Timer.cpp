/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Timer.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Timer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Timer::Type(L"Timer", &BaseClass::Type);


	Timer::Timer(bool _startFromZero):
		m_bEnabled(true),
		m_bFromZero(_startFromZero)
	{
		init();

		if(m_bFromZero)
		{
			m_nStartMiliseconds = timeGetTime();
			QueryPerformanceCounter(&m_nStartMicroseconds);
		}
	}
//----------------------------------------------------------------------
	Timer::~Timer()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Timer::init()
	{
		#ifdef _WIN32
			QueryPerformanceFrequency(&m_nFrequency);
		#endif
	}
//----------------------------------------------------------------------
	void Timer::cleanup()
	{
	}
//----------------------------------------------------------------------
	void Timer::reset()
	{
		if(m_bFromZero)
		{
			m_nStartMiliseconds = timeGetTime();
			QueryPerformanceCounter(&m_nStartMicroseconds);
		}
	}
//----------------------------------------------------------------------
	dword Timer::getMilliseconds()
	{
		// Exit if timer is disabled
		if(!m_bEnabled)
			return 0;

		#ifdef _WIN32
			return timeGetTime() - m_nStartMiliseconds;
		#else
		#endif
	}
//----------------------------------------------------------------------
	unsigned __int64 Timer::getMicroseconds()
	{
		// Exit if timer is disabled
		if(!m_bEnabled)
			return 0;

		#ifdef _WIN32
			LARGE_INTEGER time;
			QueryPerformanceCounter(&time);
			return (__int64)(time.QuadPart - m_nStartMicroseconds.QuadPart) * 1000000 /
				m_nFrequency.QuadPart;
		#else
		#endif
	}
//----------------------------------------------------------------------
}