/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CPUInfo.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "CPUInfo.h"


namespace nGENE
{
	CPUInfo::CPUInfo():
		m_nCores(0),
		m_bSSE(false),
		m_bSSE2(false),
		m_bSSE3(false),
		m_b3DNow(false),
		m_bMMX(false),
		m_bExt(false),
		m_bMMXEX(false),
		m_b3DNowEX(false),
		m_bHyperThreading(false),
		m_bSystemSupport(false)
	{
	}
//----------------------------------------------------------------------
	CPUInfo::~CPUInfo()
	{
	}
//----------------------------------------------------------------------
	void CPUInfo::getCPUInfo()
	{
		char* pVendor = m_acVendor;
		bool* sse3 = &m_bSSE3;
		bool* sse2 = &m_bSSE2;
		bool* sse  = &m_bSSE;
		bool* mmx  = &m_bMMX;
		bool* ext  = &m_bExt;
		bool* now3d= &m_b3DNow;


		__try
		{
			__asm
			{
				xor eax, eax			// get vendor name
				CPUID

				mov esi, pVendor
				mov [esi], ebx
				mov [esi+4], edx
				mov [esi+8], ecx

				mov eax, 1				// get processor feature-list
				CPUID

				test ecx, 00000001h		// test for SSE3 support
				jz	 _NOSSE3
				mov  esi, sse3
				mov	 [esi], 1
			
			_NOSSE3:
				test edx, 04000000h		// test for SSE2 support
				jz	 _NOSSE2
				mov	 esi, sse2
				mov	 [esi], 1

			_NOSSE2:
				test edx, 02000000h		// test for SSE support
				jz	 _NOSSE
				mov  esi, sse
				mov  [esi], 1

			_NOSSE:
				test edx, 00800000h		// test for MMX support
				jz	 _EXIT
				mov  esi, mmx
				mov	 [esi], 1

			_EXIT:						// no extensions are available
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			if(_exception_code() == STATUS_ILLEGAL_INSTRUCTION)
				return;
		}

		__asm
		{
			mov eax, 80000000h
			CPUID

			cmp eax, 80000000h
			jbe _EXIT2
			mov esi, ext
			mov [esi], 1

			mov eax, 80000001h
			CPUID
			test edx, 80000000h
			jz _EXIT2
			mov esi, now3d
			mov [esi], 1
		_EXIT2:
		}

		// Check for Hyper-Threading technology support
		bool* ht = &m_bHyperThreading;
		__asm
		{
			mov eax, 1
			CPUID
			test edx, 10000000h
			jz _NO_HT

			mov esi, ht
			mov [esi], 1
		_NO_HT:
		}

		// Check number of physical cores
		uint* cores = &m_nCores;
		__asm
		{
			xor	eax, eax
			CPUID
			cmp eax, 4
			jl _NO_MULTICORE

			mov eax, 4
			CPUID
			and eax, 0xfc000000
			shr eax, 26
			add eax, 1

			mov esi, cores
			mov [esi], eax
		_NO_MULTICORE:
		}


		pVendor[12] = '\0';
		getCPUBrandName();
	}
//----------------------------------------------------------------------
	void CPUInfo::getOSInfo()
	{
		__try
		{
			__asm xorps xmm0, xmm0
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			m_bSystemSupport = false;
		}
		
		m_bSystemSupport = true;
	}
//----------------------------------------------------------------------
	void CPUInfo::getCPUBrandName()
	{
		bool* now3dex = &m_b3DNowEX;
		bool* mmxex= &m_bMMXEX;
		int n = 1;
		int* pn = &n;

		if(m_bExt)
		{
			if(strncmp(m_acVendor, "GenuineIntel", 12) == 0)
			{
				m_acName[48] = '\0';
				char* pName = m_acName;
				__asm
				{
					mov esi, pName
					mov eax, 80000002h
					CPUID
					mov [esi], eax
					mov [esi+4], ebx
					mov [esi+8], ecx
					mov [esi+12], edx

					mov eax, 80000003h
					CPUID
					mov [esi+16], eax
					mov [esi+20], ebx
					mov [esi+24], ecx
					mov [esi+28], edx

					mov eax, 80000004h
					CPUID
					mov [esi+32], eax
					mov [esi+36], ebx
					mov [esi+40], ecx
					mov [esi+44], edx
				}
			}
			else if(strncmp(m_acVendor, "AuthenticAMD", 12) == 0)
			{
				__asm
				{
					mov eax, 1
					CPUID
					mov esi, pn
					mov [esi], eax

					mov eax, 0x80000001
					CPUID

					test edx, 0x40000000
					jz	 _AMD1
					mov  esi, now3dex
					mov  [esi], 1

				_AMD1:
					test edx, 0x00400000
					jz   _AMD2
					mov  esi, mmxex
					mov  [esi], 1
				_AMD2:
				}
			}
		}
	}
//----------------------------------------------------------------------
	bool CPUInfo::isSIMDSupported() const
	{
		return ((m_bSSE || m_b3DNow || m_bMMX) && (m_bSystemSupport));
	}
//----------------------------------------------------------------------
}