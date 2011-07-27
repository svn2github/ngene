/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CatmullRomInterpolator.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "CatmullRomInterpolator.h"


namespace nGENE
{
	CatmullRomInterpolator::CatmullRomInterpolator():
		m_nCurrent(1)
	{
	}
//----------------------------------------------------------------------
	uint CatmullRomInterpolator::updateSegment(float _stage, AnimationCurve& _segments)
	{
		uint seg = m_nCurrent;
		uint i;
		for(i = m_nCurrent; i < _segments.size(); ++i)
		{
			if((i + 2) == _segments.size())
				break;

			if(_stage >= _segments.getTime(i) && _stage < _segments.getTime(i + 1))
			{
				m_nCurrent = i;
				seg = i;
				break;
			}
		}

		if(i == _segments.size() - 2)
			m_nCurrent = 1;

		return seg;
	}
//----------------------------------------------------------------------
	Real CatmullRomInterpolator::interpolate(Real _stage, AnimationCurve& _data)
	{
		int previous1 = updateSegment(_stage, _data);
		int previous2 = previous1 - 1;
		int next1 = previous1 + 1;
		int next2 = next1 + 1;

		float x1 = _data.getTime(previous1);
		float x2 = _data.getTime(next1);
		float t = (_stage - x1) / (x2 - x1);

		if(previous2 < 0 || next2 >= _data.size())
			return 0.0f;

		float P0 = _data[previous2];
		float P1 = _data[previous1];
		float P2 = _data[next1];
		float P3 = _data[next2];

		Real result = 0.5f * ((2.0f * P1) +
			(-P0 + P2) * t +
			(2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3) * t * t +
			(-P0 + 3.0f * P1 - 3.0f * P2 + P3) * t * t * t);

		return result;
	}
//----------------------------------------------------------------------
}