/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Task.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TASK_H_
#define __INC_TASK_H_


#include "Prerequisities.h"
#include "Singleton.h"
#include "Thread.h"


namespace nGENE
{
	/** Task base object to be used in multi-threading applications.
		@par
			Task is a smallest building block of a nGENE multi-threading
			application.
	*/
	class nGENEDLL Task
	{
	private:
		vector <Task*> m_vChildren;					///< Child tasks (ones depending on it)
		vector <Task*> m_vSubtasks;					///< Subtasks
		vector <Task*> m_vDependencies;				///< Dependencies

		Task* m_pParent;							///< Parent Task (of subtasks)

		uint m_nPriority;							///< Priority of this task
		uint m_nAffinity;							///< Affinity mask for this task

		uint m_nActiveSubtasks;						///< Number of not completed subtasks
		uint m_nActiveDependencies;					///< Number of not completed dependencies
		uint m_nActiveChildren;						///< Number of not completed children

		bool m_bFinished;							///< Is task done in this frame?

		CriticalSection m_CriticalSection;

	public:
		Task();
		virtual ~Task();

		/// Called just before update.
		virtual void onBeginUpdate() {}
		/// Updates object.
		virtual void update()=0;
		/// Called just after update.
		virtual void onEndUpdate() {}

		/// Decreases active subtasks number.
		void decreaseActiveSubtasks();
		/// Decreases active dependencies number.
		void decreaseActiveDependecies();
		/// Decreases active children number.
		void decreaseActiveChildren();

		/// Once-per-frame update.
		virtual void onFrame();

		/// Adds new child task.
		void addChildTask(Task* _child);
		/// Removes given child task.
		void removeChildTask(Task* _child);
		/// Checks if this task has any children.
		bool hasChildren() const;

		/// Adds new subtask.
		void addSubtask(Task* _subtask);
		/// Removes given subtask.
		void removeSubtask(Task* _subtask);
		/// Checks if this task has any subtasks.
		bool hasSubtasks() const;

		/// Adds new dependency.
		void addDependency(Task* _dependency);
		/// Removes given dependency.
		void removeDependency(Task* _dependency);

		/// Sets parent Task.
		void setParent(Task* _parent);
		/// Returns parent Task.
		Task* getParent() const;

		/// Sets priority of this task.
		void setPriority(uint _priority);
		/// Returns priority of this task.
		uint getPriority() const;

		/// Sets affinity mask for this task.
		void setAffinityMask(uint _affinity);
		/// Returns affinity mask for this task.
		uint getAffinityMask() const;

		/// Checks if this Task can be started.
		bool isReadyToRun();

		/// Checks if the Task has finished.
		bool isFinished() const;

		/// Marks this Task as completed.
		void complete();
		/// Notifies parent Task of completion.
		void notifyParent();
		/// Notifies all dependencies of completion.
		void notifyDependencies();

		void noSubtasks();
	};



	inline void Task::setPriority(uint _priority)
	{
		m_nPriority = _priority;
	}
//----------------------------------------------------------------------
	inline uint Task::getPriority() const
	{
		return m_nPriority;
	}
//----------------------------------------------------------------------
	inline void Task::setAffinityMask(uint _affinity)
	{
		m_nAffinity = _affinity;
	}
//----------------------------------------------------------------------
	inline uint Task::getAffinityMask() const
	{
		return m_nAffinity;
	}
//----------------------------------------------------------------------
	inline bool Task::isReadyToRun()
	{
		SharedObject sharedObj(&m_CriticalSection);

		return (m_nActiveDependencies ? false : true);
	}
//----------------------------------------------------------------------
	inline bool Task::isFinished() const
	{
		return m_bFinished;
	}
//----------------------------------------------------------------------
	inline void Task::setParent(Task* _parent)
	{
		m_pParent = _parent;
	}
//----------------------------------------------------------------------
	inline Task* Task::getParent() const
	{
		return m_pParent;
	}
//----------------------------------------------------------------------


	/// Used to compare two Task objects based on their priorities.
	class TaskComparator
	{
	public:
		bool operator()(Task* _task1, Task* _task2)
		{
			return (_task1->getPriority() < _task2->getPriority());
		}
	};



	/// Dummy task used to group tasks.
	class nGENEDLL DummyTask: public Task
	{
	public:
		void update() {}
	};
}


#endif