/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Task.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Task.h"
#include "TaskManager.h"


namespace nGENE
{
	Task::Task():
		m_pParent(NULL),
		m_nPriority(0),
		m_nAffinity(0),
		m_nActiveSubtasks(0),
		m_nActiveDependencies(0),
		m_nActiveChildren(0),
		m_bFinished(false)
	{
	}
//----------------------------------------------------------------------
	Task::~Task()
	{
	}
//----------------------------------------------------------------------
	void Task::addChildTask(Task* _child)
	{
		m_vChildren.push_back(_child);

		m_nActiveChildren = m_vChildren.size();
	}
//----------------------------------------------------------------------
	void Task::removeDependency(Task* _dependency)
	{
		uint i = 0;
		for(i = 0; i < m_vDependencies.size(); ++i)
		{
			if(m_vDependencies[i] == _dependency)
				break;
		}

		// Dependency found - remove it
		if(i != m_vDependencies.size())
			m_vDependencies.erase(m_vDependencies.begin() + i);
	}
//----------------------------------------------------------------------
	void Task::removeChildTask(Task* _child)
	{
		uint i = 0;
		for(i = 0; i < m_vChildren.size(); ++i)
		{
			if(m_vChildren[i] == _child)
				break;
		}

		// Child task found - remove it
		if(i != m_vChildren.size())
		{
			m_vChildren[i]->removeDependency(this);
			m_vChildren.erase(m_vChildren.begin() + i);
		}
	}
//----------------------------------------------------------------------
	void Task::addSubtask(Task* _subtask)
	{
		m_vSubtasks.push_back(_subtask);

		m_nActiveSubtasks = m_vSubtasks.size();

		_subtask->setParent(this);
	}
//----------------------------------------------------------------------
	void Task::removeSubtask(Task* _subtask)
	{
		uint i = 0;
		for(i = 0; i < m_vSubtasks.size(); ++i)
		{
			if(m_vSubtasks[i] == _subtask)
				break;
		}

		// Subtask found - remove it
		if(i != m_vSubtasks.size())
		{
			m_vSubtasks[i]->setParent(NULL);
			m_vSubtasks.erase(m_vSubtasks.begin() + i);
		}
	}
//----------------------------------------------------------------------
	void Task::addDependency(Task* _dependency)
	{
		_dependency->addChildTask(this);

		m_vDependencies.push_back(_dependency);

		m_nActiveDependencies = m_vDependencies.size();
	}
//----------------------------------------------------------------------
	void Task::decreaseActiveSubtasks()
	{
		SharedObject sharedObj(&m_CriticalSection);

		--m_nActiveSubtasks;
	}
//----------------------------------------------------------------------
	void Task::decreaseActiveDependecies()
	{
		SharedObject sharedObj(&m_CriticalSection);

		--m_nActiveDependencies;
	}
//----------------------------------------------------------------------
	void Task::decreaseActiveChildren()
	{
		SharedObject sharedObj(&m_CriticalSection);

		--m_nActiveChildren;
	}
//----------------------------------------------------------------------
	void Task::notifyParent()
	{
		if(m_pParent)
		{
			m_pParent->decreaseActiveSubtasks();

			m_pParent->noSubtasks();
		}
	}
//----------------------------------------------------------------------
	void Task::notifyDependencies()
	{
		for(uint i = 0; i < m_vDependencies.size(); ++i)
		{
			Task* pTask = m_vDependencies[i];

			pTask->decreaseActiveChildren();
			pTask->notifyParent();
		}
	}
//----------------------------------------------------------------------
	void Task::noSubtasks()
	{
		if(!m_nActiveSubtasks)
		{
			onEndUpdate();

			if(!m_nActiveChildren)
			{
				notifyParent();
			}

			for(uint i = 0; i < m_vChildren.size(); ++i)
			{
				Task* pChild = m_vChildren[i];
				pChild->decreaseActiveDependecies();
				// Task is ready to be run (all dependencies resolved)
				if(pChild->isReadyToRun())
				{
					TaskManager::getSingleton().addTask(pChild);
				}
			}

			// That is the last task
			// @todo not really true as almost each subtask will be the last one
			// according to this condition ;)
			//if(!m_pParent && !hasChildren() && !hasSubtasks())
			TaskManager::getSingleton().decreaseActiveTasks();
		}
	}
//----------------------------------------------------------------------
	bool Task::hasChildren() const
	{
		return !(m_vChildren.empty());
	}
//----------------------------------------------------------------------
	bool Task::hasSubtasks() const
	{
		return !(m_vSubtasks.empty());
	}
//----------------------------------------------------------------------
	void Task::complete()
	{
		// All subtasks completed, this task is done too.
		// Therefore notify parent
		if(!m_nActiveSubtasks)
		{
			noSubtasks();

			// Notify all dependencies
			notifyDependencies();
		}
		else
		{
			for(uint i = 0; i < m_vSubtasks.size(); ++i)
			{
				Task* pSubtask = m_vSubtasks[i];
				TaskManager::getSingleton().addTask(pSubtask);
			}
		}
	}
//----------------------------------------------------------------------
	void Task::onFrame()
	{
		m_bFinished = false;

		m_nActiveSubtasks = m_vSubtasks.size();
		m_nActiveDependencies = m_vDependencies.size();
		m_nActiveChildren = m_vChildren.size();
	}
//----------------------------------------------------------------------
}
