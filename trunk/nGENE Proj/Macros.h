/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Macros.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MACROS_H_
#define __INC_MACROS_H_


// Some types definitions
namespace nGENE
{
	typedef unsigned char  byte;
	typedef unsigned short ushort;
	typedef unsigned int   uint;
	typedef unsigned long  dword;
	typedef float		   Real;			/**< Let floats be interpreted as
												 real numbers */
}

// Some macros converting values to wide char
#define WIDEN2(x)		L ## x
#define WIDEN(x)		WIDEN2(x)

#define __WFILE__		WIDEN(__FILE__)
#define __WFUNCTION__	WIDEN(__FUNCTION__)

// Often there is need to safely release pointer or dynamically
// allocated memory. Macros below provide this kind of functionality
#define NGENE_RELEASE(x)	 {if(x) {(x)->Release();  (x)=NULL;}}
#define NGENE_DELETE(x)		 {if(x) {delete (x);	  (x)=NULL;}}
#define NGENE_DELETE_ARRAY(x){if(x) {delete[] (x);    (x)=NULL;}}


// We have to force inline sometimes
#define NGENE_INLINE __forceinline


// Current nGENE Tech version
#define NGENE_SDK_VERSION		0.1f


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


#endif