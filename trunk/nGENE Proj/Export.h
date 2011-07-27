/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Export.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_EXPORT_H_
#define __INC_EXPORT_H_


	#ifdef _WIN32
		#ifdef _ENGINE_BUILD
			#define nGENEDLL __declspec(dllexport)
			#define nGENEDLL_ALIGNED_16 __declspec(dllexport, align(16))
		#else
			#define nGENEDLL __declspec(dllimport)
			#define nGENEDLL_ALIGNED_16 __declspec(dllimport, align(16))
		#endif
	#else
		#define nGENEDLL
	#endif


#endif