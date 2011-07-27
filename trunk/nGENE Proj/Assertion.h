/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Assert.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ASSERT_H_
#define __INC_ASSERT_H_


#include "Prerequisities.h"


/// Custom assert macro, where a is condition and b error description
#define Assert(a, b) assert(a && b)


#endif