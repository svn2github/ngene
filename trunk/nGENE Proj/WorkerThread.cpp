/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		WorkerThread.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "WorkerThread.h"
#include "Task.h"


namespace nGENE
{
	WorkerThread::WorkerThread(const string& _name):
		m_pTask(NULL),
		m_bProcessing(true),
		m_Fill(0, 1),
		m_Empty(1, 1),
		m_stName(_name)
	{
	}
//----------------------------------------------------------------------
	void WorkerThread::run()
	{
		Thread::setName(m_stName);

		while(m_bProcessing)
		{
			// Suspend thread until there is task to perform
			m_Fill.lock();
			m_CriticalSection.lock();

			if(m_pTask)
			{
				// Perform Task itself
				m_pTask->update();

				// Mark task as completed
				m_pTask->complete();
				m_pTask = NULL;
			}

			m_CriticalSection.unlock();
			m_Empty.release(1);
		}

		signal();
	}
//----------------------------------------------------------------------
	void WorkerThread::setProcessing(bool _value)
	{
		m_bProcessing = _value;
	}
//----------------------------------------------------------------------
	Task* WorkerThread::getTask()
	{
		SharedObject sharedObj(&m_CriticalSection);

		return m_pTask;
	}
//----------------------------------------------------------------------
	void WorkerThread::setTask(Task* _task)
	{
		m_Empty.lock();
		m_CriticalSection.lock();

		m_pTask = _task;
		if(m_pTask->getAffinityMask())
			setAffinityMask(m_pTask->getAffinityMask());

		m_CriticalSection.unlock();
		m_Fill.release(1);
	}
//----------------------------------------------------------------------
}
