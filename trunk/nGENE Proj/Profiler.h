/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Profiler.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PROFILER_H_
#define __INC_PROFILER_H_


#include "AutoPointer.h"
#include "BaseClass.h"
#include "HashTable.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Lets you track application performance.
		@remarks
			If you want to profile some block of code you have to
			place beginSample() and endSample() calls before and
			after that block. To show the results, showResults()
			method has to be called.
	*/
	class nGENEDLL Profiler: public BaseClass
	{
		EXPOSE_TYPE
	public:
		/** Represents a single sample being profiled.
		*/
		typedef struct SSample
		{
			uint nLevel;						///< Sample's nesting level
			unsigned __int64 nStartTime;		///< When execution started
			unsigned __int64 nEndTime;			///< When executed finished
			uint nLastPerformance;				///< Last execution time
			uint nAvaragePerformance;			///< Avarage execution time
			uint nMinPerformance;				///< Minimum execution time
			uint nMaxPerformance;				///< Maximum execution time
			bool bInit;
			wstring stDescription;				///< Description
			SSample* pChildren;
			SSample* pParent;

			SSample():
				nLevel(0),
				nStartTime(0),
				nEndTime(0),
				nLastPerformance(0),
				nAvaragePerformance(0),
				nMinPerformance(Maths::Infinity),
				nMaxPerformance(Maths::MinusInfinity),
				bInit(0),
				stDescription(L""),
				pChildren(NULL),
				pParent(NULL)
			{
			}
		} SAMPLE_DESC;

	private:
		/// How many samples are stored
		uint m_nSamplesNum;
		/// Current sample nesting level
		uint m_nCurNestLevel;
		/// Is profiler enabled?
		bool m_bEnabled;

		/// Hash table containing all samples
		HashTable <wstring, SSample> m_Samples;

		/// LIFO queue used to control samples tracking
		stack <wstring> m_SamplesStack;

		/// Profiler timer
		AutoPointer <Timer> m_pTimer;

	public:
		Profiler();
		virtual ~Profiler();

		virtual void init();
		virtual void cleanup();

		/** Begins profiling of code block.
			@param
				sampleName name of the sample
		*/
		virtual void beginSample(const wstring& sampleName);
		/**	Ends profiling of code block.
		*/
		virtual void endSample();
		/** Shows results to the client.
		*/
		virtual void showResults();

		bool isEnabled() const {return m_bEnabled;}
		void setIsEnabled(bool value) {m_bEnabled=value;}

		uint getNestingLevel() const {return m_nCurNestLevel;}
		uint getSamplesNum() const {return m_nSamplesNum;}
	};
}


#endif