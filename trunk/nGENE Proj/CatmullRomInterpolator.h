/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CatmullRomInterpolator.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CATMULLROMINTERPOLATOR_H_
#define __INC_CATMULLROMINTERPOLATOR_H_


#include "AnimationCurve.h"
#include "IInterpolator.h"
#include "Prerequisities.h"


namespace nGENE
{
	/// Interpolates between points in accordance to Catmull-Rom spline.
	class nGENEDLL CatmullRomInterpolator: public IInterpolator
	{
	private:
		uint m_nCurrent;			///< Current segment


		uint updateSegment(float _stage, AnimationCurve& _segments);

	public:
		CatmullRomInterpolator();

		Real interpolate(Real _stage, AnimationCurve& _data);
	};
}


#endif