/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CrashDump.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "CrashDump.h"

#include "Log.h"

#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <iostream>
#include <strsafe.h>

using namespace std;


namespace nGENE
{
namespace Application
{
	LONG WINAPI createCrashDump(_EXCEPTION_POINTERS* pExceptionPointers)  
	{
		Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				 L"Unhandled exception!!! Writing crash dump file.");

		BOOL bResult;
		WCHAR stFileName[MAX_PATH]; 
		WCHAR* stAppName = L"nGENETech";
		HANDLE hDumpFile;
		SYSTEMTIME stLocalTime;
		MINIDUMP_EXCEPTION_INFORMATION ExpParam;

		GetLocalTime(&stLocalTime);

		// Create file
		StringCchPrintfW(stFileName, MAX_PATH, L"%s", stAppName);
		CreateDirectoryW(L"CrashDumps", NULL);

		StringCchPrintfW(stFileName, MAX_PATH, L"CrashDumps\\%sv%f-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", 
				   stAppName, NGENE_SDK_VERSION, 
				   stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
				   stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
				   GetCurrentProcessId(), GetCurrentThreadId());
		hDumpFile = CreateFileW(stFileName,
								GENERIC_READ | GENERIC_WRITE, 
								FILE_SHARE_WRITE | FILE_SHARE_READ,
								0,
								CREATE_ALWAYS,
								0,
								0);

		ExpParam.ThreadId = GetCurrentThreadId();
		ExpParam.ExceptionPointers = pExceptionPointers;
		ExpParam.ClientPointers = TRUE;

		// Write minidump
		bResult = MiniDumpWriteDump(GetCurrentProcess(),
									GetCurrentProcessId(), 
									hDumpFile,
									MiniDumpWithDataSegs,
									&ExpParam,
									NULL,
									NULL);

		return EXCEPTION_EXECUTE_HANDLER;  
	} 
//----------------------------------------------------------------------
}
}