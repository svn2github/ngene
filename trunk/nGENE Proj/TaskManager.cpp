/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TaskManager.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "TaskManager.h"
#include "Task.h"


namespace nGENE
{
	TaskManager::TaskManager():
		m_nActiveTasks(0),
		m_bRunning(true)
	{
	}
//----------------------------------------------------------------------
	TaskManager::~TaskManager()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void TaskManager::stop()
	{
		cleanup();

		while(!m_vOpenTasks.empty())
			m_vOpenTasks.pop();
		m_vNotCompleted.clear();

		m_nActiveTasks = 0;
		m_pRootTask = NULL;
	}
//----------------------------------------------------------------------
	void TaskManager::init(uint _threadsNum)
	{
		for(uint i = 0; i < _threadsNum; ++i)
		{
			stringstream buffer;
			buffer << "Worker Thread " << i;
			WorkerThread* pThread = new WorkerThread(buffer.str());
			m_vWorkers.push_back(pThread);

			pThread->start();
		}
	}
//----------------------------------------------------------------------
	void TaskManager::cleanup()
	{
		if(m_vWorkers.size())
		{
			for(uint i = 0; i < m_vWorkers.size(); ++i)
			{
				m_vWorkers[i]->setProcessing(false);
				m_vWorkers[i]->setTask(NULL);
			}

			for(uint i = 0; i < m_vWorkers.size(); ++i)
				m_vWorkers[i]->wait();

			// Release memory
			for(uint i = 0; i < m_vWorkers.size(); ++i)
				NGENE_DELETE(m_vWorkers[i]);
			m_vWorkers.clear();
		}
	}
//----------------------------------------------------------------------
	void TaskManager::update()
	{
		bool run = true;

		while(run && m_bRunning)
		{
			uint size = 0;
			{
				SharedObject sharedObj(&m_CriticalSection);
				size = m_vOpenTasks.size();
			}

			if(size)
			{
				// We have to support case when there are no worker threads
				if(m_vWorkers.size())
				{
					WorkerThread* pThread = NULL;
					for(uint i = 0; i < m_vWorkers.size(); ++i)
					{
						if(!m_vWorkers[i]->getTask())
						{
							pThread = m_vWorkers[i];
							break;
						}
					}

					if(pThread)
					{
						Task* pTask = getNextTask();
						if(pTask)
						{
							pThread->setTask(pTask);
						}
					}
				}
				else
				{
					Task* pTask = getNextTask();
					pTask->onBeginUpdate();
					pTask->update();

					// Mark task as completed
					pTask->complete();
				}
			}

			m_CriticalSection.lock();

				run = m_nActiveTasks ? true : false;
			
			m_CriticalSection.unlock();

			if(!run && m_vWorkers.size())
			{
				for(uint i = 0; i < m_vWorkers.size(); ++i)
				{
					if(m_vWorkers[i]->getTask())
					{
						run = true;
						break;
					}
				}
			}
		}
	}
//----------------------------------------------------------------------
	Task* TaskManager::getNextTask()
	{
		SharedObject sharedObj(&m_CriticalSection);

		if(!m_vOpenTasks.size())
			return NULL;

		Task* pTask = m_vOpenTasks.top();
		m_vOpenTasks.pop();

		return pTask;
	}
//----------------------------------------------------------------------
	void TaskManager::addTask(Task* _task)
	{
		// No task, so exit
		if(!_task)
			return;

		// Reset Task's data
		_task->onFrame();

		SharedObject sharedObj(&m_CriticalSection);

		++m_nActiveTasks;
		m_vOpenTasks.push(_task);
	}
//----------------------------------------------------------------------
	void TaskManager::removeTask(Task* _task)
	{
		// No task, so exit
		if(!_task)
			return;

		priority_queue <Task*, vector <Task*>, TaskComparator> openTasks;
		while(m_vOpenTasks.size())
		{
			Task* task = m_vOpenTasks.top();
			if(task != _task)
			{
				openTasks.push(task);
				m_vOpenTasks.pop();
			}
		}

		while(openTasks.size())
		{
			Task* task = openTasks.top();
			m_vOpenTasks.push(task);
			openTasks.pop();
		}

		for(uint i = 0; i < m_vWorkers.size(); ++i)
		{
			if(m_vWorkers[i]->getTask() == _task)
				m_vWorkers[i]->setTask(NULL);
		}
	}
//----------------------------------------------------------------------
	void TaskManager::decreaseActiveTasks()
	{
		SharedObject SharedObject(&m_CriticalSection);

		//if(m_vOpenTasks.empty())
		//	m_nActiveTasks = 0;
		--m_nActiveTasks;
	}
//----------------------------------------------------------------------
}
