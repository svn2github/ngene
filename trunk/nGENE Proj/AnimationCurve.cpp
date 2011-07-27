/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AnimationCurve.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "AnimationCurve.h"


namespace nGENE
{
	AnimationCurve::AnimationCurve()
	{
	}
//----------------------------------------------------------------------
	AnimationCurve::AnimationCurve(vector <Real>& _points, vector <Real>& _times):
		m_vCurve(_points),
		m_vTimes(_times)
	{
	}
//----------------------------------------------------------------------
	void AnimationCurve::addPoint(Real& _point, Real& _time)
	{
		m_vCurve.push_back(_point);
		m_vTimes.push_back(_time);
	}
//----------------------------------------------------------------------
	Real& AnimationCurve::getPoint(uint i)
	{
		return m_vCurve[i];
	}
//----------------------------------------------------------------------
	Real& AnimationCurve::getTime(uint i)
	{
		return m_vTimes[i];
	}
//----------------------------------------------------------------------
	Real AnimationCurve::operator[](uint i)
	{
		return m_vCurve[i];
	}
//----------------------------------------------------------------------
	uint AnimationCurve::size()
	{
		return m_vCurve.size();
	}
//----------------------------------------------------------------------
	vector <Real>& AnimationCurve::getPoints()
	{
		return m_vCurve;
	}
//----------------------------------------------------------------------
	vector <Real>& AnimationCurve::getTimes()
	{
		return m_vTimes;
	}
//----------------------------------------------------------------------
}