/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MTFEncoder.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MTFENCODER_H_
#define __INC_MTFENCODER_H_


#include "IEncoder.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Simple MTF (Move-to-Front) encoder.
		@par
			The aim of MTF is to reduce entropy what would improve
			result of algorithms based on it.
	*/
	class nGENEDLL MTFEncoder
	{
	public:
		uint encode(char* _result, const char* _data, uint _size);
		void encode(ostream& _result, const char* _data, uint _size);

		uint decode(char* _result, const char* _data, uint _size);
		void decode(ostream& _result, const char* _data, uint _size);
	};
}


#endif