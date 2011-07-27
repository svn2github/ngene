/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RLEEncoder.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RLEENCODER_H_
#define __INC_RLEENCODER_H_


#include "IEncoder.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Simple RLE (run-length) encoder.
		@par
			RLE works as follows. It runs through the block of data and finds
			neighbouring characters which are the same and for each of such
			chains store its length.
	*/
	class nGENEDLL RLEEncoder
	{
	public:
		uint encode(char* _result, const char* _data, uint _size);
		void encode(ostream& _result, const char* _data, uint _size);

		uint decode(char* _result, const char* _data, uint _size);
		void decode(ostream& _result, const char* _data, uint _size);
	};
}


#endif