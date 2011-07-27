/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CPUInfo.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CPUINFO_H_
#define __INC_CPUINFO_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Processor features list.
		@par
			Functions getCPUInfo() and getOSInfo() should be called
			before any initialization is performed as their results determine
			whether faster calculations using IA-32 Intel and AMD extensions
			could be used.
		@todo
			Add AMD support as now only Intel is supported.
	*/
	class nGENEDLL CPUInfo
	{
	private:
		/// Number of cores.
		uint m_nCores;

		bool m_bSSE;					///< Specifies if SSE is supported
		bool m_bSSE2;					///< Specifies if SSE2 is supported
		bool m_bSSE3;					///< Specifies if SSE3 is supported
		bool m_b3DNow;					///< Specifies if 3DNow! is supported
		bool m_bMMX;					///< Specifies if MMX is supported

		bool m_bExt;
		bool m_bMMXEX;
		bool m_b3DNowEX;

		/// Has Hyper-Threading?
		bool m_bHyperThreading;

		/// Does OS support SIMD extensions?
		bool m_bSystemSupport;

		/// Processor's name
		char m_acName[49];

		/// Processor vendor name
		char m_acVendor[13];


		void getCPUBrandName();
	
	public:
		/// Initializes all members to 0.
		CPUInfo();
		~CPUInfo();

		/** It is very important function as it checks whether
			SIMD extensions of IA-32 are available on the processor.
			If yes, mathematical operations (eg. vectors/matrices
			addition or multiplication) can be heavily speeded up.
		*/
		void getCPUInfo();

		/** This function checks whether operating system itself
			supports extension of the CPU (provided they are available).
			@remarks
				It could seem weird at first but apart from processor's
				support for SIMD extensions, the OS on which engine is
				being run also have to support them. Otherwise application
				cannot use these extension.
		*/
		void getOSInfo();

		/// Returns true iff both CPU and OS support SIMD extensions.
		bool isSIMDSupported() const;

		/// Gets CPU brand name.
		string getCPUName() const;
		/// Gets CPU vendor name.
		string getCPUVendor() const;

		/// Returns number of logical processors.
		uint getLogicalProcessorsNum() const;

		/// Returns number of cores.
		uint getCoresNum() const;
	};



	inline string CPUInfo::getCPUName() const
	{
		return m_acName;
	}
//----------------------------------------------------------------------
	inline string CPUInfo::getCPUVendor() const
	{
		return m_acVendor;
	}
//----------------------------------------------------------------------
	inline uint CPUInfo::getLogicalProcessorsNum() const
	{
		if(m_bHyperThreading)
			return (m_nCores << 1);
		else
			return m_nCores;
	}
//----------------------------------------------------------------------
	inline uint CPUInfo::getCoresNum() const
	{
		return m_nCores;
	}
//----------------------------------------------------------------------
}


#endif