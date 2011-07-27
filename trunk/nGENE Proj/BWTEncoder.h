/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		BWTEncoder.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_BWTENCODER_H_
#define __INC_BWTENCODER_H_


#include "IEncoder.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Burrows-Wheeler transform encoder.
		@par
			There is a nice description of BW transform on
			http://marknelson.us/1996/09/01/bwt/ website. What
			it basically does is input reordering in such a way
			that it becomes more compression friendly.
	*/
	class nGENEDLL BWTEncoder
	{
	private:
		char* m_pBuffer;					///< Internal buffer
		uint m_nCount;						///< Internal size

		const uint m_kBlockSize;

		class BoundedCompare
		{
		private:
			char* m_pBuffer;
			uint m_nCount;

		public:
			BoundedCompare(char* _buffer, uint _count);
			bool operator()(const char *p1, const char *p2) const;
		};


		// Prevent copying and assignment
		BWTEncoder(const BWTEncoder& _src);
		BWTEncoder& operator=(const BWTEncoder& _encoder);

	public:
		BWTEncoder(uint _blockSize=200000);

		uint encode(char* _result, const char* _data, uint _size);
		void encode(ostream& _result, const char* _data, uint _size);

		uint decode(char* _result, const char* _data, uint _size);
		void decode(ostream& _result, const char* _data, uint _size);
	};
}


#endif