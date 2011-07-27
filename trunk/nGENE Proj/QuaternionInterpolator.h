/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		QuaternionInterpolator.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_QUATERNIONINTERPOLATOR_H_
#define __INC_QUATERNIONINTERPOLATOR_H_


#include "AnimationCurve.h"
#include "Prerequisities.h"
#include "Quaternion.h"


namespace nGENE
{
	/// Interpolates between points treating them as quaternions.
	class nGENEDLL QuaternionInterpolator
	{
	private:
		uint m_nCurrent;			///< Current segment


		uint updateSegment(float _stage, vector <AnimationCurve>& _segments);

	public:
		QuaternionInterpolator();

		Quaternion interpolate(Real _stage, vector <AnimationCurve>& _data);
	};
}


#endif