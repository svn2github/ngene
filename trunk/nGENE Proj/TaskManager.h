/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TaskManager.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TASKMANAGER_H_
#define __INC_TASKMANAGER_H_


#include "Prerequisities.h"
#include "Singleton.h"
#include "WorkerThread.h"


namespace nGENE
{
	/** Manages all the tasks.
		@todo remember to synchronize access to the vectors in this class!
	*/
	class nGENEDLL TaskManager: public Singleton <TaskManager>
	{
	private:
		vector <Task*> m_vNotCompleted;				///< List of task to do this frame
		vector <WorkerThread*> m_vWorkers;			///< Worker threads

		///< Tasks currently possible to perform (ready to run).
		priority_queue <Task*, vector <Task*>, TaskComparator> m_vOpenTasks;

		Task* m_pRootTask;							///< Root task

		uint m_nActiveTasks;						///< Number of not completed tasks

		CriticalSection m_CriticalSection;			///< Critical section

		bool m_bRunning;

	public:
		TaskManager();
		~TaskManager();

		/// Stops task manager.
		void stop();

		/// Adds Task to open list.
		void addTask(Task* _task);
		/// Removes Task from the manager.
		void removeTask(Task* _task);

		/// Returns next Task to perform.
		Task* getNextTask();

		/// Sets root task.
		void setRootTask(Task* _task);
		/// Returns root task.
		Task* getRootTask() const;

		/// Decreases number of active tasks.
		void decreaseActiveTasks();

		void init(uint _threadsNum);
		void cleanup();

		void setRunning(bool _run);

		void update();
	};



	inline void TaskManager::setRootTask(Task* _task)
	{
		m_pRootTask = _task;
	}
//----------------------------------------------------------------------
	inline Task* TaskManager::getRootTask() const
	{
		return m_pRootTask;
	}
//----------------------------------------------------------------------
	inline void TaskManager::setRunning(bool _run)
	{
		m_bRunning = _run;
	}
//----------------------------------------------------------------------
}


#endif