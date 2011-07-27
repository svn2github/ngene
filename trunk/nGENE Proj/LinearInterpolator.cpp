/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LinearInterpolator.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "LinearInterpolator.h"


namespace nGENE
{
	LinearInterpolator::LinearInterpolator():
		m_nCurrent(0)
	{
	}
//----------------------------------------------------------------------
	uint LinearInterpolator::updateSegment(float _stage, AnimationCurve& _segments)
	{
		uint seg = m_nCurrent;
		uint i;
		for(i = m_nCurrent; i < _segments.size(); ++i)
		{
			if((i + 1) == _segments.size())
				break;

			if(_stage >= _segments.getTime(i) && _stage < _segments.getTime(i + 1))
			{
				m_nCurrent = i;
				seg = i;
				break;
			}
		}

		if(i == _segments.size() - 1)
			m_nCurrent = 0;

		return seg;
	}
//----------------------------------------------------------------------
	Real LinearInterpolator::interpolate(Real _stage, AnimationCurve& _data)
	{
		uint previous = updateSegment(_stage, _data);
		uint next = previous + 1;


		float x  = _stage;
		float x0 = _data.getTime(previous);
		float x1 = _data.getTime(next);

		Real result = _data[previous] +
			(x - x0) * (_data[next] -
			_data[previous]) / (x1 - x0);

		return result;
	}
//----------------------------------------------------------------------
}