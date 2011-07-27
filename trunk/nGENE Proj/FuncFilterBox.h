/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FuncFilterBox.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FUNCFILTERBOX_H_
#define __INC_FUNCFILTERBOX_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** It is a functor object letting you apply box filtering to a 2D array.
		@remarks
			Filter box is a simple method of filtering. It averages value in
			pixel by means of arithmetic mean of the pixel itself and all it
			neighbors (ranging from 3 to 8).
		@par
			This functor allocates memory. It is client responsibility to
			free it.
		@returns
			RESULT* if this function succeeds it returs valid array containing
			filtered data. On failure it returns NULL pointer.
	*/
	template <typename T, typename RESULT=T>
	class FuncFilterBox
	{
	public:
		FuncFilterBox() {}
		~FuncFilterBox() {}

		RESULT* operator()(const T* _t, uint _width, uint _height, bool _filterCorners=false);
	};



//----------------------------------------------------------------------
	template <typename T, typename RESULT>
	RESULT* FuncFilterBox<typename T, typename RESULT>::
		operator()(const T* _t, uint _width, uint _height, bool _filterCorners)
	{
		uint startX, startZ;
		uint endX, endZ;

		startX = startZ = (_filterCorners ? 0 : 1);
		endX = (_filterCorners ? _width : _width - 1);
		endZ = (_filterCorners ? _height : _height - 1);

		// Array is to small to be filtered, so return
		if(endX < startX + 3 || endZ < startZ + 3)
			return NULL;


		dword pixelsNum = _width * _height;

		RESULT* pResult = new RESULT[pixelsNum];

		// We don't filter these pixels
		for(uint z = 0; z < startZ; ++z)
		{
			for(uint x = 0; x < _width; ++x)
				pResult[z * _width + x] = _t[z * _width + x];
		}

		for(uint z = endZ; z < _height; ++z)
		{
			for(uint x = 0; x < _width; ++x)
				pResult[z * _width + x] = _t[z * _width + x];
		}


		// Ok now let's filter the array
		float cellAverage, value;
		for(uint z = startZ; z < endZ; ++z)
		{
			// We don't filter these pixels
			for(uint x = 0; x < startX; ++x)
				pResult[z * _width + x] = _t[z * _width + x];

			for(uint x = startX; x < endX; ++x)
			{
				value = 0;
				cellAverage = 1;
      
				// Sample top row
      
				if (((x - 1) + (z - 1) * _width) >= 0 &&
					((x - 1) + (z - 1) * _width) < pixelsNum)
				{
					value += _t[((x - 1) + (z - 1) * _width)];
					++cellAverage;
				}
      
				if ((x + (z - 1) * _width) >= 0 &&
					(x + (z - 1) * _width) < pixelsNum)
				{
					value += _t[(x + (z - 1) * _width)];
					++cellAverage;
				}
			      
				if (((x + 1) + (z - 1) * _width) >= 0 &&
					((x + 1) + (z - 1) * _width) < pixelsNum)
				{
					value += _t[((x + 1) + (z - 1) * _width)];
					++cellAverage;
				}
			      
				// Sample middle row
			      
				if (((x - 1) + (z - 0) * _width) >= 0 &&
					((x - 1) + (z - 0) * _width) < pixelsNum)
				{
					value += _t[((x - 1) + (z    ) * _width)];
					++cellAverage;
				}
			    
				// Sample center point (will always be in pixelsNum)
				value += _t[(x + z * _width)];
			      
				if (((x + 1) + (z - 0) * _width) >= 0 &&
					((x + 1) + (z - 0) * _width) < pixelsNum)
				{
					value += _t[((x + 1) + z * _width)];
					++cellAverage;
				}
			      
				// Sample bottom row
			      
				if (((x - 1) + (z + 1) * _width) >= 0 &&
					((x - 1) + (z + 1) * _width) < pixelsNum)
				{
					value += _t[((x - 1) + (z + 1) * _width)];
					++cellAverage;
				}
      
				if (((x - 0) + (z + 1) * _width) >= 0 &&
					((x - 0) + (z + 1) * _width) < pixelsNum)
				{
					value += _t[(x + (z + 1) * _width)];
					++cellAverage;
				}
      
				if (((x + 1) + (z + 1) * _width) >= 0 &&
					((x + 1) + (z + 1) * _width) < pixelsNum)
				{
					value += _t[((x + 1) + (z + 1) * _width)];
					++cellAverage;
				}
      
				// Store the result
				pResult[z * _width + x] = static_cast<RESULT>(value / static_cast<float>(cellAverage));
			}

			// We don't filter these pixels
			for(uint x = endX; x < _width; ++x)
				pResult[z * _width + x] = _t[z * _width + x];
		}

		return pResult;
	}
}


#endif