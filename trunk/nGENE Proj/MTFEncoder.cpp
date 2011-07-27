/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MTFEncoder.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "MTFEncoder.h"


namespace nGENE
{
	uint MTFEncoder::encode(char* _result, const char* _data, uint _size)
	{
		if(!_size)
			return 0;

		uint index = 0;
		byte order[256];
		for(uint i = 0; i < 256 ; ++i)
			order[i] = (byte)i;
		char curr = 0;
		const char* pointer = _data;
		while(pointer < _data + _size)
		{
			curr = *(pointer++);
			uint i;
			for(i = 0; i < 256; ++i)
			{
				if(order[i] == (byte)curr)
					break;
			}
			_result[index++] = (byte)i;

			for(uint j = i; j > 0; j--)
				order[j] = order[j - 1];
			order[0] = (byte)curr;
		}

		return index;
	}
//----------------------------------------------------------------------
	void MTFEncoder::encode(ostream& _result, const char* _data, uint _size)
	{
		if(!_size)
			return;

		byte order[256];
		for(uint i = 0; i < 256 ; ++i)
			order[i] = (byte)i;
		char curr = 0;
		const char* pointer = _data;
		while(pointer < _data + _size)
		{
			curr = *(pointer++);
			uint i;
			for(i = 0; i < 256; ++i)
			{
				if(order[i] == (curr & 0xff))
					break;
			}
			_result << (byte)i;

			for(uint j = i; j > 0; j--)
				order[j] = order[j - 1];
			order[0] = (byte)curr;
		}
	}
//----------------------------------------------------------------------
	uint MTFEncoder::decode(char* _result, const char* _data, uint _size)
	{
		char curr = 0;
		uint index = 0;
		const char* pointer = _data;

		byte order[256];
		for(uint i = 0; i < 256; ++i)
			order[i] = (byte)i;
		byte c = 0;
		while(pointer < _data + _size)
		{
			c = *(pointer++);
			curr = (char)order[c];
			_result[index++] = curr;
			for(uint j = c; j > 0; j--)
				order[j] = order[j - 1];
			order[0] = (byte)curr;
		}

		return index;
	}
//----------------------------------------------------------------------
	void MTFEncoder::decode(ostream& _result, const char* _data, uint _size)
	{
		char curr = 0;
		const char* pointer = _data;

		byte order[256];
		for(uint i = 0; i < 256; ++i)
			order[i] = (byte)i;
		byte c = 0;
		while(pointer < _data + _size)
		{
			c = *(pointer++);
			curr = (char)order[c];
			_result << curr;
			for(uint j = c; j > 0; j--)
				order[j] = order[j - 1];
			order[0] = (byte)curr;
		}
	}
//----------------------------------------------------------------------
}