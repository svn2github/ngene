/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		QuaternionInterpolator.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "QuaternionInterpolator.h"


namespace nGENE
{
	QuaternionInterpolator::QuaternionInterpolator():
		m_nCurrent(0)
	{
	}
//----------------------------------------------------------------------
	uint QuaternionInterpolator::updateSegment(float _stage, vector <AnimationCurve>& _segments)
	{
		uint seg = m_nCurrent;
		uint i;
		for(i = m_nCurrent; i < _segments[0].size(); ++i)
		{
			if((i + 1) == _segments[0].size())
				break;

			if(_stage >= _segments[0].getTime(i) && _stage < _segments[0].getTime(i + 1))
			{
				m_nCurrent = i;
				seg = i;
				break;
			}
		}

		if(i == _segments[0].size() - 1)
			m_nCurrent = 0;

		return seg;
	}
//----------------------------------------------------------------------
	Quaternion QuaternionInterpolator::interpolate(Real _stage, vector <AnimationCurve>& _data)
	{
		uint previous = updateSegment(_stage, _data);
		uint next = previous + 1;

		Quaternion quatPrev(_data[0].getPoint(previous),
							_data[1].getPoint(previous),
							_data[2].getPoint(previous),
							_data[3].getPoint(previous));
		Quaternion quatNext(_data[0].getPoint(next),
							_data[1].getPoint(next),
							_data[2].getPoint(next),
							_data[3].getPoint(next));

		float x0 = _data[0].getTime(previous);
		float x1 = _data[0].getTime(next);
		float x = (_stage - x0);
		float t = x / (x1 - x0);

		Quaternion result = slerp(quatPrev, quatNext, t);

		return result;
	}
//----------------------------------------------------------------------
}