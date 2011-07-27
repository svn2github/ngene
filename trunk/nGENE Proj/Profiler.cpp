/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Profiler.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Profiler.h"


namespace nGENE
{
	TypeInfo Profiler::Type(L"Profiler", &BaseClass::Type);


	Profiler::Profiler():
		m_nSamplesNum(0),
		m_nCurNestLevel(0),
		m_bEnabled(true)
	{
		init();
	}
//----------------------------------------------------------------------
	Profiler::~Profiler()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Profiler::init()
	{
		// Create timer
		m_pTimer.setPointer(new Timer(false));
	}
//----------------------------------------------------------------------
	void Profiler::cleanup()
	{
		while(!m_SamplesStack.empty())
			m_SamplesStack.pop();

		if(!m_Samples.empty())
			m_Samples.clear();

		m_nCurNestLevel = 0;
		m_nSamplesNum = 0;
	}
//----------------------------------------------------------------------
	void Profiler::beginSample(const wstring& sampleName)
	{
		// Exit, if profiler is disabled
		if(!m_bEnabled)
			return;

		// If sample has not yet been added, add it and increase
		// samples counter
		if(m_Samples.find(sampleName) == m_Samples.end())
			++m_nSamplesNum;

		SSample& temp = m_Samples[sampleName];

		temp.nStartTime = m_pTimer->getMicroseconds();
		temp.stDescription = sampleName;
		temp.nLevel = (++m_nCurNestLevel);

		// Put the item on the stack
		m_SamplesStack.push(sampleName);
	}
//----------------------------------------------------------------------
	void Profiler::endSample()
	{
		// Exit if profiler is disabled
		if(!m_bEnabled)
			return;

		// Remove item from the stack
		wstring sampleName = m_SamplesStack.top();
		m_SamplesStack.pop();

		// Update performance statistics
		SSample& temp = m_Samples[sampleName];
		temp.nEndTime = m_pTimer->getMicroseconds();
		temp.nLastPerformance = temp.nEndTime - temp.nStartTime;
		if(temp.bInit)
		{
			temp.nAvaragePerformance = ((temp.nAvaragePerformance +
				temp.nLastPerformance) >> 1);
		}
		else
		{
			temp.nAvaragePerformance = temp.nLastPerformance;
			temp.bInit = 1;
		}

		if(temp.nLastPerformance > temp.nMaxPerformance)
			temp.nMaxPerformance = temp.nLastPerformance;
		else if(temp.nLastPerformance < temp.nMinPerformance)
			temp.nMinPerformance = temp.nLastPerformance;

		// Decrease nesting level
		--m_nCurNestLevel;
	}
//----------------------------------------------------------------------
	void Profiler::showResults()
	{
	}
//----------------------------------------------------------------------
}
