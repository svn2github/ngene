/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RLEEncoder.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "RLEEncoder.h"


namespace nGENE
{
	uint RLEEncoder::encode(char* _result, const char* _data, uint _size)
	{
		if(!_size)
			return 0;

		char prev = 0;
		char curr = 0;
		uint index = 0;
		const char* pointer = _data;

		uint count = 0;
		while(pointer < _data + _size)
		{
			curr = (*pointer++);
			_result[index++] = curr;
			if(curr == prev)
			{
				count = 0;
				while(count < 255 && pointer < _data + _size)
				{
					if((curr = *(pointer++)) == prev)
						++count;
					else
						break;
				}
				_result[index++] = (byte)count;
				if(count != 255 && curr != prev)
					_result[index++] = curr;
			}
			prev = curr;
		}

		return index;
	}
//----------------------------------------------------------------------
	void RLEEncoder::encode(ostream& _result, const char* _data, uint _size)
	{
		if(!_size)
			return;

		char prev = 0;
		char curr = 0;
		const char* pointer = _data;

		uint count = 0;
		while(pointer < _data + _size)
		{
			curr = (*pointer++);
			_result << curr;
			if(curr == prev)
			{
				count = 0;
				while(count < 255 && pointer < _data + _size)
				{
					if((curr = *(pointer++)) == prev)
						++count;
					else
						break;
				}
				_result << (byte)count;
				if(count != 255 && curr != prev)
					_result << curr;
			}
			prev = curr;
		}
	}
//----------------------------------------------------------------------
	uint RLEEncoder::decode(char* _result, const char* _data, uint _size)
	{
		char curr = 0;
		char prev = 0;
		byte count = 0;
		uint index = 0;
		const char* pointer = _data;

		while(pointer < _data + _size)
		{
			curr = *(pointer++);
			_result[index++] = curr;
			if(curr == prev)
			{
				count = *(pointer++);
				while(count-- > 0)
					_result[index++] = curr;
			}
			prev = curr;
		}

		return index;
	}
//----------------------------------------------------------------------
	void RLEEncoder::decode(ostream& _result, const char* _data, uint _size)
	{
		char curr = 0;
		char prev = 0;
		byte count = 0;
		const char* pointer = _data;

		while(pointer < _data + _size)
		{
			curr = *(pointer++);
			_result << curr;
			if(curr == prev)
			{
				count = *(pointer++);
				while(count-- > 0)
					_result << curr;
			}
			prev = curr;
		}
	}
//----------------------------------------------------------------------
}