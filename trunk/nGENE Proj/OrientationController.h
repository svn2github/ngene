/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InterpolatedController.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ORIENTATIONCONTROLLER_H_
#define __INC_ORIENTATIONCONTROLLER_H_


#include "AnimationCurve.h"
#include "InterpolatedController.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** OrientationController allows you to change orientation of the node.
	*/
	template <typename Interpolator>
	class OrientationController: public InterpolatedController <Interpolator>
	{
	protected:
		virtual void updateCurve();

	public:
		OrientationController(float _time, bool _looped, uint _bufferSize=3);

		/** Adds new point.
			@param _point point to be added.
			@param _stage time value from range [0.0; 1.0].
		*/
		virtual void addPoint(Quaternion& _point, Real _stage);

		/// Returns given key-frame.
		virtual Quaternion getPoint(uint _index);

		virtual void retrieveData();
	};



	template <typename Interpolator>
	OrientationController <Interpolator>::OrientationController(float _time, bool _looped, uint _bufferSize):
		InterpolatedController <Interpolator>(_time, _looped, _bufferSize)
	{
		for(uint i = 0; i < 4; ++i)
		{
			AnimationCurve curve;
			addCurve(curve);
		}
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void OrientationController <Interpolator>::addPoint(Quaternion& _point, Real _stage)
	{
		getCurve(0).addPoint(_point.x, _stage);
		getCurve(1).addPoint(_point.y, _stage);
		getCurve(2).addPoint(_point.z, _stage);
		getCurve(3).addPoint(_point.w, _stage);
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	Quaternion OrientationController <Interpolator>::getPoint(uint _index)
	{
		return Quaternion(getCurve(0)[_index],
						  getCurve(1)[_index],
						  getCurve(2)[_index],
						  getCurve(3)[_index]);
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void OrientationController <Interpolator>::retrieveData()
	{
		if(!m_bAnimating || !m_pNode)
			return;

		vector <Real>& vOrientation = getCurrent();
		if(!vOrientation.size())
			return;

		Quaternion orientation(vOrientation[0],
							   vOrientation[1],
							   vOrientation[2],
							   vOrientation[3]);

		m_pNode->setRotation(orientation);
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void OrientationController <Interpolator>::updateCurve()
	{
		float t = m_fPassedTime / m_fTime;
		if(t > 1.0f)
			t = 1.0f;

		Quaternion quatCurrent = Interpolator::interpolate(t, m_vCurves);

		m_Current[0] = quatCurrent.x;
		m_Current[1] = quatCurrent.y;
		m_Current[2] = quatCurrent.z;
		m_Current[3] = quatCurrent.w;


		m_Buffer.push_back(m_Current);
	}
//----------------------------------------------------------------------
}


#endif