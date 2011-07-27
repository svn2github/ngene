/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		WorkerThread.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_WORKERTHREAD_H_
#define __INC_WORKERTHREAD_H_


#include "Prerequisities.h"
#include "Singleton.h"
#include "Thread.h"


namespace nGENE
{
	/** WorkerThread is a Thread performing a Task and when done it waits
		for another Task.
	*/
	class nGENEDLL WorkerThread: public Thread
	{
	private:
		Task* m_pTask;						///< Task currently being performed

		bool m_bProcessing;					///< Is worker thread still processing?

		CriticalSection m_CriticalSection;
		Semaphore m_Fill;
		Semaphore m_Empty;

		string m_stName;					///< Name of this worker thread

	public:
		WorkerThread(const string& _name="Worker Thread");

		void run();

		/// Sets currently running Task.
		void setTask(Task* _task);
		/// Returns currently running Task.
		Task* getTask();

		/// Specifies if this thread has finished processing.
		void setProcessing(bool _value);
	};
}


#endif