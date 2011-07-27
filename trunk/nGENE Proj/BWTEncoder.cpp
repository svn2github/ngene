/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		BWTEncoder.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "BWTEncoder.h"

#include <set>

namespace nGENE
{
	BWTEncoder::BoundedCompare::BoundedCompare(char* _buffer, uint _count):
		m_pBuffer(_buffer),
		m_nCount(_count)
	{
	}
//----------------------------------------------------------------------
	bool BWTEncoder::BoundedCompare::operator()(const char *p1, const char *p2) const
	{
		unsigned int l1 = (unsigned int)((m_pBuffer - p1) + m_nCount);
		unsigned int l2 = (unsigned int)((m_pBuffer - p2) + m_nCount);
		int result = memcmp(p1, p2, min(l1, l2));
		if(result < 0)
			return true;
		if(result > 0)
			return false;
		return l1 > l2;
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	BWTEncoder::BWTEncoder(uint _blockSize):
		m_pBuffer(NULL),
		m_nCount(0),
		m_kBlockSize(_blockSize)
	{
	}
//----------------------------------------------------------------------
	uint BWTEncoder::encode(char* _result, const char* _data, uint _size)
	{
		if(!_size)
			return 0;

		const char* pointer = _data;
		uint index = 0;
		uint size = _size;
		uint totalSize = _size;
		uint prevSize = 0;

		while(pointer < _data + _size)
		{
			m_nCount = m_kBlockSize < size ? m_kBlockSize : size;
			if(m_nCount != prevSize)
			{
				NGENE_DELETE_ARRAY(m_pBuffer);
				m_pBuffer = new char[m_nCount];
				prevSize = m_nCount;
			}
			memcpy(m_pBuffer, pointer, m_nCount);

			// Save block size
			uint temp = m_nCount + 1;
			memcpy(&_result[index], &temp, sizeof(uint));
			index += sizeof(uint);

			uint i = 0;
			std::set <char*, BoundedCompare> p(BoundedCompare(m_pBuffer, m_nCount));
			for(i = 0; i < m_nCount; ++i)
				p.insert(m_pBuffer + i);

			std::set <char*, BoundedCompare>::iterator iter;

			dword first = 0;
			dword last = 0;
			for(i = 0, iter = p.begin(); iter != p.end(); i++, iter++)
			{
				if(*iter == (m_pBuffer + 1))
					first = i;
				if(*iter == m_pBuffer)
				{
					last = i;
					_result[index++] = '?';
				}
				else
					_result[index++] = (*iter)[-1];
			}
			p.erase(p.begin(), p.end());

			memcpy(&_result[index], &first, sizeof(dword));
			index += sizeof(dword);
			memcpy(&_result[index], &last, sizeof(dword));
			index += sizeof(dword);

			totalSize += 2 * sizeof(dword) + sizeof(uint);


			pointer += m_nCount;
			size -= m_nCount;
		}

		NGENE_DELETE_ARRAY(m_pBuffer);

		return totalSize;
	}
//----------------------------------------------------------------------
	void BWTEncoder::encode(ostream& _result, const char* _data, uint _size)
	{
		if(!_size)
			return;

		const char* pointer = _data;
		uint size = _size;
		char prevFill = _result.fill(0);
		uint prevSize = 0;

		while(pointer < _data + _size)
		{
			m_nCount = m_kBlockSize < size ? m_kBlockSize : size;
			if(m_nCount != prevSize)
			{
				NGENE_DELETE_ARRAY(m_pBuffer);
				m_pBuffer = new char[m_nCount];
				prevSize = m_nCount;
			}
			memcpy(m_pBuffer, pointer, m_nCount);

			// Save block size
			_result.width(sizeof(uint));
			uint temp = m_nCount + 1;
			_result.write((char*)&temp, sizeof(uint));
			_result.width(sizeof(char));

			uint i = 0;
			std::set <char*, BoundedCompare> p(BoundedCompare(m_pBuffer, m_nCount));
			for(i = 0; i <= m_nCount; ++i)
				p.insert(m_pBuffer + i);

			std::set <char*, BoundedCompare>::iterator iter;

			dword first = 0;
			dword last = 0;
			for(i = 0, iter = p.begin(); iter != p.end(); i++, iter++)
			{
				if(*iter == (m_pBuffer + 1))
					first = i;
				if(*iter == m_pBuffer)
				{
					last = i;
					_result << '?';
				}
				else
					_result << (*iter)[-1];
			}
			p.erase(p.begin(), p.end());


			_result.width(sizeof(dword));
			_result.write((char*)&first, sizeof(dword));
			_result.width(sizeof(dword));
			_result.write((char*)&last, sizeof(dword));


			pointer += m_nCount;
			size -= m_nCount;
		}

		NGENE_DELETE_ARRAY(m_pBuffer);

		_result.fill(prevFill);
	}
//----------------------------------------------------------------------
	uint BWTEncoder::decode(char* _result, const char* _data, uint _size)
	{
		const char* pointer = _data;
		uint* T = NULL;
		uint begin = 0;

		uint num[257];
		uint runningTotal[257];
		uint prevSize = 0;

		while(pointer < _data + _size)
		{
			memcpy(&m_nCount, pointer, sizeof(uint));
			pointer += sizeof(uint);

			if(m_nCount != prevSize)
			{
				NGENE_DELETE_ARRAY(m_pBuffer);
				NGENE_DELETE_ARRAY(T);

				m_pBuffer = new char[m_nCount];
				T = new uint[m_nCount];

				prevSize = m_nCount;
			}

			memcpy(m_pBuffer, pointer, m_nCount);
			pointer += m_nCount;

			dword first;
			dword last;
			memcpy(&first, pointer, sizeof(dword));
			pointer += sizeof(dword);
			memcpy(&last, pointer, sizeof(dword));
			pointer += sizeof(dword);

			memset(num, 0, sizeof(uint) * 257);
			for(uint i = 0; i < m_nCount; ++i)
			{
				if(i == last)
					++num[256];
				else
					++num[(byte)m_pBuffer[i]];
			}

			uint sum = 0;
			for(uint i = 0; i < 257; ++i)
			{
				runningTotal[i] = sum;
				sum += num[i];
				num[i] = 0;
			}

			memset(T, 0, sizeof(uint) * m_nCount);
			for(uint i = 0; i < m_nCount; ++i)
			{
				uint index = 0;
				if(i == last)
					index = 256;
				else
					index = (byte)m_pBuffer[i];
				T[num[index] + runningTotal[index]] = i;
				num[index]++;
			}

			uint index = first;
			uint j = 0;
			for(j = 0; j < m_nCount - 1; ++j)
			{
				_result[j + begin] = m_pBuffer[index];
				index = T[index];
			}

			begin = j;
		}

		NGENE_DELETE_ARRAY(m_pBuffer);
		NGENE_DELETE_ARRAY(T);


		return _size;
	}
//----------------------------------------------------------------------
	void BWTEncoder::decode(ostream& _result, const char* _data, uint _size)
	{
		const char* pointer = _data;
		uint* T = NULL;

		uint num[257];
		uint runningTotal[257];
		uint prevSize = 0;

		while(pointer < _data + _size)
		{
			memcpy(&m_nCount, pointer, sizeof(uint));
			pointer += sizeof(uint);

			if(m_nCount != prevSize)
			{
				NGENE_DELETE_ARRAY(m_pBuffer);
				NGENE_DELETE_ARRAY(T);

				m_pBuffer = new char[m_nCount];
				T = new uint[m_nCount];

				prevSize = m_nCount;
			}

			memcpy(m_pBuffer, pointer, m_nCount);
			pointer += m_nCount;

			dword first;
			dword last;
			memcpy(&first, pointer, sizeof(dword));
			pointer += sizeof(dword);
			memcpy(&last, pointer, sizeof(dword));
			pointer += sizeof(dword);

			
			memset(num, 0, sizeof(uint) * 257);
			for(uint i = 0; i < m_nCount; ++i)
			{
				if(i == last)
					++num[256];
				else
					++num[(byte)m_pBuffer[i]];
			}


			uint sum = 0;
			for(uint i = 0; i < 257; ++i)
			{
				runningTotal[i] = sum;
				sum += num[i];
				num[i] = 0;
			}

			memset(T, 0, sizeof(uint) * m_nCount);
			for(uint i = 0; i < m_nCount; ++i)
			{
				uint index = 0;
				if(i == last)
					index = 256;
				else
					index = (byte)m_pBuffer[i];
				T[num[index] + runningTotal[index]] = i;
				num[index]++;
			}

			uint index = first;
			for(uint j = 0; j < m_nCount - 1; ++j)
			{
				_result << m_pBuffer[index];
				index = T[index];
			}
		}

		NGENE_DELETE_ARRAY(m_pBuffer);
		NGENE_DELETE_ARRAY(T);
	}
//----------------------------------------------------------------------
}