/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		UtilitiesTypes.h
Version:	0.01
Info:		This file contains various types definitions which
			are used by the engine.
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_UTILITIESTYPES_H_
#define __INC_UTILITIESTYPES_H_


#include "Prerequisities.h"


namespace nGENE
{
	/// Simple rectangle described with its corners
	template <typename T=Real>
	struct SRect
	{
		T left;
		T top;
		T right;
		T bottom;

		SRect(T _left, T _top, T _right, T _bottom);
		SRect();
	};


	typedef SRect <Real> RECT_FLOAT;



	template <typename T>
	SRect <T>::SRect(T _left, T _top, T _right, T _bottom):
		left(_left),
		top(_top),
		right(_right),
		bottom(_bottom)
	{
		Assert(left <= right, "Invalid boundaries!");
		Assert(top <= bottom, "Invalid boundaries!");
	}
//----------------------------------------------------------------------
	template <typename T>
	SRect <T>::SRect():
		left(static_cast<T>(0)),
		top(static_cast<T>(0)),
		right(static_cast<T>(0)),
		bottom(static_cast<T>(0))
	{
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
}


#endif