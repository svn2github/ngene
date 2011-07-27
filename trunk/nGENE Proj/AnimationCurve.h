/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AnimationCurve.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ANIMATIONCURVE_H_
#define __INC_ANIMATIONCURVE_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** Single animation dimension.
		@par
			Animation curve describes changes of the single
			floating-point value over time using key frames. This
			way to achieve changes of position in 3D space one have
			to use 3 separate AnimationCurve objects, each of them
			describing changes in a single axis.
		@par
			Values between two key frames can be found using interpolator
			objects.
	*/
	class nGENEDLL AnimationCurve
	{
	protected:
		vector <Real> m_vCurve;					///< Data point values
		vector <Real> m_vTimes;					///< Time values

	public:
		AnimationCurve();
		AnimationCurve(vector <Real>& _points, vector <Real>& _times);

		/** Adds new key frame.
			@param _point value of the variable at this key frame.
			@param _time time of the key frame on the time line.
		*/
		void addPoint(Real& _point, Real& _time);

		/// Returns value of a variable.
		Real& getPoint(uint i);

		/// Returns given time value.
		Real& getTime(uint i);

		Real operator[](uint i);

		/// Returns total number of key frames.
		uint size();

		/// Returns array of variable values.
		vector <Real>& getPoints();

		/// Returns array of time values.
		vector <Real>& getTimes();
	};
}


#endif