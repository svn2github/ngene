/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FastFloat.h
Version:	0.52
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FASTFLOAT_H_
#define __INC_FASTFLOAT_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** This class speed up some operations on floating
		point numbers.
	*/
	class nGENEDLL FastFloat
	{
	protected:
		/// Represents either float or integer
		typedef union
		{
			int i;
			float f;
		} INT_OR_FLOAT;

		static uint s_knSinTableSize;			///< Size of the sine values table
		static float* s_pSinTable;				///< Lookup table for sine and cosine
		static float s_fModifier;
		static float s_fQuarter;
		static int s_nMask;

		static const float FTOIBIAS;			///< Bias used in operations on float


		/** Creates sine look-up table which is also used for cosine
			calculation.
			@remarks
				Using sine/cosine table version is faster than calling
				std sinf and cosf functions but results are less accurate.
		*/
		void initSinTable();
		void cleanup();

	public:
		FastFloat(uint _sinTableSize=512);
		~FastFloat();

		/** Converts float to integer.
			@remarks
				This method is a bit faster than regular C/C++
				float to int typecasting. It uses a few tricks
				on bits to achieve this effect.
		*/
		static int floatToInt(const float& _value);

		/// Returns a sine value for a given angle.
		static float fsin(const float& _phi);

		/// Returns a cosine value for a given angle.
		static float fcos(const float& _phi);

		/// Returns an absolute value for a given number.
		static float fabs(const float& _value);

		/// Returns number of entries in the sine lookup table.
		uint getSinTableSize();
	};



	inline int FastFloat::floatToInt(const float& _value)
	{
		INT_OR_FLOAT n;
		INT_OR_FLOAT bias;

		if(_value > 0.0f)
			bias.i = (23 + 127) << 23;
		else
			bias.i = ((23 + 127) << 23) + (1 << 22);

		n.f = _value;

		n.f += bias.f;
		n.i -= bias.i;

		return n.i;
	}
//----------------------------------------------------------------------
	inline float FastFloat::fsin(const float& _phi)
	{
		int i;
		INT_OR_FLOAT temp;

		temp.f = _phi * (s_fModifier) + FTOIBIAS;
		i = temp.i & s_nMask;

		return s_pSinTable[i];
	}
//----------------------------------------------------------------------
	inline float FastFloat::fcos(const float& _phi)
	{
		int i;
		INT_OR_FLOAT temp; 
		temp.f = _phi*(s_fModifier) + FTOIBIAS + s_fQuarter;
		i = temp.i & s_nMask;

		return s_pSinTable[i];
	}
//----------------------------------------------------------------------
	inline float FastFloat::fabs(const float& _value)
	{
		INT_OR_FLOAT ftmp;
		ftmp.f = _value;
		ftmp.i &= 0x7fffffff;
		
		return ftmp.f;
	}
//----------------------------------------------------------------------
}


#endif
