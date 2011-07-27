/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrecompiledHeaders.h
Version:	0.10
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PRECOMPILEDHEADERS_
#define __INC_PRECOMPILEDHEADERS_


#ifndef _DEBUG
	// Disable iterators debugging
	#undef _SECURE_SCL
	#define _SECURE_SCL 0

	#undef _HAS_ITERATOR_DEBUGGING
	#define _HAS_ITERATOR_DEBUGGING 0

	// Disable exceptions
	#undef _HAS_EXCEPTIONS
	#define _HAS_EXCEPTIONS 0
#endif


// disable: "non dll-interface class <type> used as base for dll-interface struct <type2>"
#pragma warning (disable: 4275)


// External headers
#include <stdlib.h>
#include <sstream>
#include <time.h>


// nGENE Tech headers
#include "Engine.h"
#include "File.h"
#include "FileManager.h"
#include "Listener.h"
#include "ListenerRegistry.h"
#include "Log.h"
#include "Maths.h"
#include "Matrix4x4.h"
#include "Profiler.h"
#include "Singleton.h"
#include "Timer.h"
#include "Vector3.h"


#endif