/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IInterpolator.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_IINTERPOLATOR_H_
#define __INC_IINTERPOLATOR_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Interpolates values between frames of AnimationCurve.
	*/
	class nGENEDLL IInterpolator
	{
	private:
		virtual uint updateSegment(float _stage, AnimationCurve& _segments)=0;

	public:
		/// Interpolates between values.
		virtual Real interpolate(Real _stage, AnimationCurve& _data)=0;
	};
}


#endif