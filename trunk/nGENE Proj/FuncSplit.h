/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FuncSplit.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FUNCSPLIT_H_
#define __INC_FUNCSPLIT_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** It is a functor object letting you split the strings/wstrings/anything
		similar to them into set of strings based on the delimiter.
	*/
	template <typename T>
	class FuncSplit
	{
	public:
		FuncSplit() {}
		virtual ~FuncSplit() {}

		void operator()(const T& t, vector <T>& vec, const T& delim, uint limit=0);
	};



//----------------------------------------------------------------------
	template <typename T>
	void FuncSplit<typename T>::operator()(const T& t, vector <T>& vec, const T& delim, uint limit)
	{
		// Skip delimiters at beginning.
		T::size_type lastPos = t.find_first_not_of(delim);
		// Find first "non-delimiter".
		T::size_type pos     = t.find_first_of(delim, lastPos);

		uint count = 1;
		while(T::npos != pos || T::npos != lastPos)
		{
			vec.push_back(t.substr(lastPos, pos - lastPos));
			lastPos = t.find_first_not_of(delim, pos);
			pos = t.find_first_of(delim, lastPos);
			if(limit)
			{
				if(limit == count++)
					return;
			}
		}
	}
}


#endif