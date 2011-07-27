/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AutoPointer.h
Version:	0.53
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_AUTOPOINTER_H_
#define __INC_AUTOPOINTER_H_


#include "HashTable.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** AutoPointer is implementation of a smart pointer with reference counting.
		@remarks
			Try to use this class instead of regular pointers as often as possible
			as it offers safer pointer usage - you do not need to worry about
			releasing memory as it will be automatically freed on destuction.
			It is possible due to the fact AutoPointer is created on the stack,
			whereas regular pointers are created on the heap.
	*/
	template <typename T>
	class AutoPointer
	{
	private:
		T* m_pPtr;
		static HashTable <T*, uint> s_RefCounter;

		/// Releases pointer.
		void finalizePointer();
		/// Initializes pointer
		void initPointer(T* ptr);

	public:
		AutoPointer(T* ptr);
		AutoPointer();
		AutoPointer(const AutoPointer<T>& src);
		AutoPointer<T>& operator=(const AutoPointer<T>& rhs);
		AutoPointer<T>& operator=(T* rhs);
		virtual ~AutoPointer();

		operator T*() const;
		const T& operator*() const;
		const T* operator->() const;

		bool operator==(const AutoPointer<T>& rhs);
		bool operator!=(const AutoPointer<T>& rhs);

		bool operator==(T* rhs);
		bool operator!=(T* rhs);

		T& operator*();
		T* operator->();

		void setPointer(T* ptr);
		T* getPointer() const {return m_pPtr;}
		T& getRef() const {return *m_pPtr;}
	};


	template <typename T>
	HashTable <T*, uint> AutoPointer <T>::s_RefCounter;


	template <typename T>
	AutoPointer <T>::AutoPointer():
		m_pPtr(NULL)
	{
	}
//----------------------------------------------------------------------	
	template <typename T>
	AutoPointer <T>::AutoPointer(T* ptr)
	{
		initPointer(ptr);
	}
//----------------------------------------------------------------------
	template <typename T>
	AutoPointer <T>::AutoPointer(const AutoPointer<T>& src)
	{
		initPointer(src.m_pPtr);
	}
//----------------------------------------------------------------------
	template <typename T>
	AutoPointer <T>& AutoPointer <T>::operator=(const AutoPointer <T>& rhs)
	{
		if(this == &rhs)
			return (*this);

		finalizePointer();
		initPointer(rhs.m_pPtr);

		return (*this);
	}
//----------------------------------------------------------------------
	template <typename T>
	AutoPointer<T>& AutoPointer <T>::operator=(T* rhs)
	{
		if(m_pPtr == rhs)
			return (*this);

		if(rhs)
		{
			if(m_pPtr)
				finalizePointer();
			initPointer(rhs);
		}
		else
			finalizePointer();

		return (*this);
	}
//----------------------------------------------------------------------
	template <typename T>
	AutoPointer <T>::~AutoPointer()
	{
		finalizePointer();
	}
//----------------------------------------------------------------------
	template <typename T>
	void AutoPointer <T>::setPointer(T* ptr)
	{
		if(m_pPtr == ptr)
			return;

		if(ptr)
		{
			if(m_pPtr)
				finalizePointer();
			initPointer(ptr);
		}
		else
			finalizePointer();
	}
//----------------------------------------------------------------------
	template <typename T>
	void AutoPointer <T>::initPointer(T* ptr)
	{
		m_pPtr = ptr;

		if(s_RefCounter.find(m_pPtr) == s_RefCounter.end())
			s_RefCounter[m_pPtr] = 1;
		else
			++s_RefCounter[m_pPtr];
	}
//----------------------------------------------------------------------
	template <typename T>
	void AutoPointer <T>::finalizePointer()
	{
		if(s_RefCounter.find(m_pPtr) == s_RefCounter.end())
			return;
		
		--s_RefCounter[m_pPtr];
		if(!s_RefCounter[m_pPtr])
		{
			// No more references, so pointer can be safely deleted
			s_RefCounter.erase(m_pPtr);
			NGENE_DELETE(m_pPtr);
		}
	}
//----------------------------------------------------------------------
	template <typename T>
	const T* AutoPointer <T>::operator->() const
	{
		return (m_pPtr);
	}
//----------------------------------------------------------------------
	template <typename T>
	AutoPointer <T>::operator T*() const
	{
		return (m_pPtr);
	}
//----------------------------------------------------------------------
	template <typename T>
	const T& AutoPointer <T>::operator*() const
	{
		return (*m_pPtr);
	}
//----------------------------------------------------------------------
	template <typename T>
	T* AutoPointer <T>::operator->()
	{
		return (m_pPtr);
	}
//----------------------------------------------------------------------
	template <typename T>
	T& AutoPointer <T>::operator*()
	{
		return (*m_pPtr);
	}
//----------------------------------------------------------------------
	template <typename T>
	bool AutoPointer <T>::operator==(T* rhs)
	{
		return (m_pPtr == rhs);
	}
//----------------------------------------------------------------------
	template <typename T>
	bool AutoPointer <T>::operator!=(T* rhs)
	{
		return (m_pPtr != rhs);
	}
//----------------------------------------------------------------------
	template <typename T>
	bool AutoPointer <T>::operator==(const AutoPointer<T>& rhs)
	{
		return (m_pPtr == rhs.m_pPtr);
	}
//----------------------------------------------------------------------
	template <typename T>
	bool AutoPointer <T>::operator!=(const AutoPointer<T>& rhs)
	{
		return (m_pPtr != rhs.m_pPtr);
	}
//----------------------------------------------------------------------
}


#endif
