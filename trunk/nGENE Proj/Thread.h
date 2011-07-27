/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Thread.h
Version:	0.10
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_THREAD_H_
#define __INC_THREAD_H_


#ifdef _WIN32
	#define NOMINMAX
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0400
	#endif
	#include <windows.h>
#endif
#include "Prerequisities.h"


namespace nGENE
{
	/** Thread base object to be used in multi-threading applications.
		@remarks
			nGENE Tech is by definition meant to use multi-threading to as
			high extent as possible making engine run faster on multi-processor
			platforms (or at least multi-core processors).
		@par
			To create new thread you have to derive from this class and overload
			the run() method where you define actual thread behaviour.
		@par
			To really make use of the thread, after defining it you have to
			call its start() member function.
	*/
	class nGENEDLL Thread
	{
	private:
		bool m_bRunning;
		bool m_bWaiting;

		uint m_AffinityMask;

		#ifdef _WIN32
			HANDLE m_hThread;
			HANDLE m_hEvent;

			friend DWORD WINAPI thread_func(LPVOID data);
		#endif

	protected:
		virtual void run()=0;

	public:
		Thread();
		virtual ~Thread();

		/// Creates new thread.
		void start();
		/// Safely exits the thread.
		void terminate();
		
		/** Kills the thread immediately.
			@remarks
				Use only in the worse possible scenario.
		*/
		void kill();

		/** Suspends the thread for a specified time.
			@param
				_time time in milliseconds for thread to be put
				in sleep mode. If this value is greater or equal
				1e9 the thread is suspended infinitely and to
				awake it, resume() function has to be called.
		*/
		void sleep(uint _time);
		/// Suspends the thread.
		void suspend();
		/// Awakes the thread.
		void resume();

		/// Waits for the object to be released.
		void wait();
		/// Signals this thread.
		void signal();

		//// Sets name of calling thread.
		static void setName(const string& _name);

		/// Checks if this thread is in running state.
		bool isRunning() const {return m_bRunning;}
		/// Checks if this thread is in waiting state.
		bool isWaiting() const {return m_bWaiting;}

		/// Sets affinity mask for this thread.
		void setAffinityMask(uint _affinity);
		/// Returns affinity mask for this thread.
		uint getAffinityMask() const {return m_AffinityMask;}
	};


	/** CriticalSection is a synchronization primitive protecting shared resource from
		simultaneous access from multiple threads or processes.
		@par
			Only one thread at a time can be in the code critical
			section to prevent conflicts.
	*/
	class nGENEDLL CriticalSection
	{
	private:
		#ifdef _WIN32
			CRITICAL_SECTION m_CriticalSection;
		#endif

	public:
		CriticalSection();
		virtual ~CriticalSection();

		/// Enters the critical section.
		void lock();

		/** Tries to enter critical section.
			@remarks
				If succeeds in entering critical section, it locks
				CriticalSection and returns true. Otherwise it returns false
				without blocking.
		*/
		bool tryLock();

		/// Leaves the critical section.
		void unlock();
	};


	/** Mutex is a synchronization primitive protecting shared resource from
		simultaneous access from multiple threads or processes.
		@par
			Only one thread at a time can be in the code critical
			section to prevent conflicts.
		@par
			Mutex is often called binary semaphore as its behaviour is very
			similar but only two values (signalled/not-signalled) are possible.
	*/
	class nGENEDLL Mutex
	{
	private:
		#ifdef _WIN32
			HANDLE m_Mutex;
		#endif

	public:
		Mutex(bool _owned=false);
		virtual ~Mutex();

		/// Gets ownership of mutex.
		void lock();

		/** Tries to get ownership of mutex.
			@remarks
				If succeeds in entering critical section, it locks
				mutex and returns true. Otherwise it returns false
				without blocking.
		*/
		bool tryLock();

		/// Gives back ownership of mutex.
		void unlock();
	};


	/** Semaphore object.
		@par
			This is general (not binary) semaphore i.e. it can have any non-negative
			integer value.
	*/
	class nGENEDLL Semaphore
	{
	private:
		#ifdef _WIN32
			HANDLE m_Semaphore;
		#endif

	public:
		Semaphore(long _initial=0, long _maximum=1);
		virtual ~Semaphore();

		/// Raises semaphore.
		void lock();
		/** Tries to lock a semaphore.
			@returns
				bool true if semaphore was locked and false if it was not.
		*/
		bool tryLock();
		/// Raises semaphore.
		void down();
		/** Tries to lock a semaphore.
			@returns
				bool true if semaphore was locked and false if it was not.
		*/
		bool tryDown();

		/// Releases semaphore.
		void release(long _count);
		/// Releases semaphore.
		void up(long _count);
	};

	/** This object enters critical section on construction and leaves it on
		destruction.
	*/
	class nGENEDLL SharedObject
	{
	private:
		CriticalSection* m_pCS;					///< Pointer to the critical section object

	public:
		SharedObject(CriticalSection* _cs);
		~SharedObject();
	};
}


#endif