/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LinearInterpolator.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_LINEARINTERPOLATOR_H_
#define __INC_LINEARINTERPOLATOR_H_


#include "AnimationCurve.h"
#include "IInterpolator.h"
#include "Prerequisities.h"


namespace nGENE
{
	/// Interpolates the values linearly.
	class nGENEDLL LinearInterpolator: public IInterpolator
	{
	private:
		uint m_nCurrent;			///< Current segment


		uint updateSegment(float _stage, AnimationCurve& _segments);

	public:
		LinearInterpolator();

		Real interpolate(Real _stage, AnimationCurve& _data);
	};
}


#endif