/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Thread.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Thread.h"


#ifndef _WIN32
	#include <unistd.h>
#endif


namespace nGENE
{
#ifdef _WIN32
	typedef struct SThreadNameInfo
	{
		DWORD dwType;
		LPCSTR szName;
		DWORD dwThreadID;
		DWORD dwFlags;
	} THREADNAME_INFO;
#endif

#ifdef _WIN32
	DWORD WINAPI thread_func(LPVOID data)
	{
		Thread* pThread = static_cast<Thread*>(data);
		if(pThread->m_bRunning)
			pThread->run();

		pThread->m_bRunning = false;
		pThread->signal();


		return 0;
	}
#endif
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	Thread::Thread():
		m_bRunning(false),
		m_bWaiting(false),
		m_AffinityMask(0)
	{
		#ifdef _WIN32
			m_hThread = NULL;
			m_hEvent = CreateEvent(NULL, false, false, NULL);
			if(!m_hEvent)
			{
				Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Creating an event object failed!");
			}
		#endif
	}
//----------------------------------------------------------------------
	Thread::~Thread()
	{
		signal();
		terminate();

		#ifdef _WIN32
			CloseHandle(m_hThread);
			if(!CloseHandle(m_hEvent))
			{
				Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Closing a handle to an event failed!");
			}
		#endif
	}
//----------------------------------------------------------------------
	void Thread::start()
	{
		// Thread is already run, so exit
		if(m_bRunning)
			return;

		m_bRunning = true;

		#ifdef _WIN32
			m_hThread = CreateThread(NULL, 0, thread_func, this, 0, NULL);
			if(!m_hThread)
			{
				Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Creating a thread failed!");
			}
		#endif
	}
//----------------------------------------------------------------------
	void Thread::sleep(uint _time)
	{
		// Suspend the thread
		#ifdef _WIN32
			m_bRunning = false;

			uint infinity = FastFloat::floatToInt(Maths::Infinity);
			if(_time >= infinity)
				SuspendThread(m_hThread);
			else
				Sleep(_time);
		#endif
	}
//----------------------------------------------------------------------
	void Thread::suspend()
	{
		// Suspend the thread
		#ifdef _WIN32
			m_bRunning = false;

			SuspendThread(m_hThread);
		#endif
	}
//----------------------------------------------------------------------
	void Thread::kill()
	{
		#ifdef _WIN32
			TerminateThread(m_hThread, static_cast<DWORD>(-1));
			m_bRunning = false;
		#endif
	}
//----------------------------------------------------------------------
	void Thread::resume()
	{
		// Thread is already run, so exit
		if(m_bRunning)
			return;

		#ifdef _WIN32
			ResumeThread(m_hThread);
			m_bRunning = true;
		#endif
	}
//----------------------------------------------------------------------
	void Thread::wait()
	{
		// Wait for the object if necessary
		if(m_bWaiting || !m_bRunning)
			return;

		#ifdef _WIN32
			m_bWaiting = true;
			WaitForSingleObject(m_hEvent,
								INFINITE);
		#endif
	}
//----------------------------------------------------------------------
	void Thread::signal()
	{
		if(!m_bWaiting)
			return;


		#ifdef _WIN32
			if(!SetEvent(m_hEvent))
			{
				Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Signaling an event failed!");
			}
			m_bWaiting = false;
		#endif
	}
//----------------------------------------------------------------------
	void Thread::terminate()
	{
		// Exit the thread
		if(!m_bRunning)
			return;

		m_bWaiting = false;
		m_bRunning = false;
	}
//----------------------------------------------------------------------
	void Thread::setName(const string& _name)
	{
		#ifdef _WIN32
			#ifdef _DEBUG
				THREADNAME_INFO info;
				info.dwType = 0x1000;
				info.szName = _name.c_str();
				info.dwThreadID = -1;
				info.dwFlags = 0;

				__try
				{
					RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (DWORD*)&info);
				}
				__except(EXCEPTION_CONTINUE_EXECUTION)
				{
				}
			#endif
		#endif
	}
//----------------------------------------------------------------------
	void Thread::setAffinityMask(uint _affinity)
	{
		m_AffinityMask = _affinity;

		#ifdef _WIN32
			if(_affinity)
				SetThreadAffinityMask(m_hThread, m_AffinityMask);
		#endif
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	CriticalSection::CriticalSection()
	{
		#ifdef _WIN32
			InitializeCriticalSection(&m_CriticalSection);
		#endif
	}
//----------------------------------------------------------------------
	CriticalSection::~CriticalSection()
	{
		#ifdef _WIN32
			DeleteCriticalSection(&m_CriticalSection);
		#endif
	}
//----------------------------------------------------------------------
	void CriticalSection::lock()
	{
		#ifdef _WIN32
			EnterCriticalSection(&m_CriticalSection);
		#endif
	}
//----------------------------------------------------------------------
	bool CriticalSection::tryLock()
	{
		#ifdef _WIN32
			return (TryEnterCriticalSection(&m_CriticalSection) != 0);
		#endif
	}
//----------------------------------------------------------------------
	void CriticalSection::unlock()
	{
		#ifdef _WIN32
			LeaveCriticalSection(&m_CriticalSection);
		#endif
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	Mutex::Mutex(bool _owned)
	{
		#ifdef _WIN32
			m_Mutex = CreateMutex(NULL,
								  _owned,
								  NULL);
		#endif
	}
//----------------------------------------------------------------------
	Mutex::~Mutex()
	{
		#ifdef _WIN32
			CloseHandle(m_Mutex);
		#endif
	}
//----------------------------------------------------------------------
	void Mutex::lock()
	{
		#ifdef _WIN32
			WaitForSingleObject(m_Mutex,
							    INFINITE);
		#endif
	}
//----------------------------------------------------------------------
	bool Mutex::tryLock()
	{
		#ifdef _WIN32
			dword result = WaitForSingleObject(m_Mutex, 0);

			if(result == WAIT_OBJECT_0)
				return true;
			else
				return false;
		#endif

		return true;
	}
//----------------------------------------------------------------------
	void Mutex::unlock()
	{
		#ifdef _WIN32
			ReleaseMutex(m_Mutex);
		#endif
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	Semaphore::Semaphore(long _initial, long _maximum)
	{
		#ifdef _WIN32
			m_Semaphore = CreateSemaphore(NULL,
										  _initial,
										  _maximum,
										  NULL);

			if(!m_Semaphore)
				return;
		#endif
	}
//----------------------------------------------------------------------
	Semaphore::~Semaphore()
	{
		#ifdef _WIN32
			CloseHandle(m_Semaphore);
		#endif
	}
//----------------------------------------------------------------------
	void Semaphore::lock()
	{
		#ifdef _WIN32
			WaitForSingleObject(m_Semaphore,
								INFINITE);
		#endif
	}
//----------------------------------------------------------------------
	bool Semaphore::tryLock()
	{
		#ifdef _WIN32
			dword result = WaitForSingleObject(m_Semaphore, 0);

			if(result == WAIT_OBJECT_0)
				return true;
			else
				return false;
		#endif

		return true;
	}
//----------------------------------------------------------------------
	void Semaphore::release(long _count)
	{
		#ifdef _WIN32
			if(!ReleaseSemaphore(m_Semaphore,
								 _count,
								 NULL))
			{
				return;
			}
		#endif
	}
//----------------------------------------------------------------------
	void Semaphore::up(long _count)
	{
		release(_count);
	}
//----------------------------------------------------------------------
	void Semaphore::down()
	{
		lock();
	}
//----------------------------------------------------------------------
	bool Semaphore::tryDown()
	{
		return tryLock();
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	SharedObject::SharedObject(CriticalSection* _cs)
	{
		Assert(_cs, "Cannot pass null critical section to shared object");

		m_pCS = _cs;
		m_pCS->lock();
	}
//----------------------------------------------------------------------
	SharedObject::~SharedObject()
	{
		m_pCS->unlock();
	}
//----------------------------------------------------------------------
}