/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IEncoder.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_IENCODER_H_
#define __INC_IENCODER_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Abstract interface to encode/decode block of data
		or string.
		@par
			This interface provides two methods (in each two variants):
			encode() for encoding and decode() for decoding data.
	*/
	class nGENEDLL IEncoder
	{
	public:
		/// This method encodes data and returns number of bytes in the result.
		virtual uint encode(char* _result, const char* _data, uint _size)=0;
		virtual void encode(ostream& _result, const char* _data, uint _size)=0;

		/// This method decodes data and returns number of bytes in the result.
		virtual uint decode(char* _result, const char* _data, uint _size)=0;
		virtual void decode(ostream& _result, const char* _data, uint _size)=0;
	};
}


#endif