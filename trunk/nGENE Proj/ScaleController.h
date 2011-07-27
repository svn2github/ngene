/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScaleController.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCALECONTROLLER_H_
#define __INC_SCALECONTROLLER_H_


#include "AnimationCurve.h"
#include "InterpolatedController.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** ScaleController allows you to change scale of the node.
	*/
	template <typename Interpolator>
	class ScaleController: public InterpolatedController <Interpolator>
	{
	protected:
		virtual void updateCurve();

	public:
		ScaleController(float _time, bool _looped, uint _bufferSize=3);

		/** Adds new point.
			@param _point point to be added.
			@param _stage time value from range [0.0; 1.0].
		*/
		virtual void addPoint(Vector3& _point, Real _stage);

		/// Returns given key-frame.
		virtual Vector3 getPoint(uint _index);

		virtual void retrieveData();
	};



	template <typename Interpolator>
	ScaleController <Interpolator>::ScaleController(float _time, bool _looped, uint _bufferSize):
		InterpolatedController <Interpolator>(_time, _looped, _bufferSize)
	{
		for(uint i = 0; i < 3; ++i)
		{
			AnimationCurve curve;
			addCurve(curve);
		}
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void ScaleController <Interpolator>::addPoint(Vector3& _point, Real _stage)
	{
		getCurve(0).addPoint(_point.x, _stage);
		getCurve(1).addPoint(_point.y, _stage);
		getCurve(2).addPoint(_point.z, _stage);
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	Vector3 ScaleController <Interpolator>::getPoint(uint _index)
	{
		return Vector3(getCurve(0)[_index],
			getCurve(1)[_index],
			getCurve(2)[_index]);
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void ScaleController <Interpolator>::retrieveData()
	{
		if(!m_bAnimating || !m_pNode)
			return;

		vector <Real>& vScale = getCurrent();
		if(!vScale.size())
			return;
		Vector3 scale(vScale[0], vScale[1], vScale[2]);

		m_pNode->setScale(scale);
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void ScaleController <Interpolator>::updateCurve()
	{
		float t = m_fPassedTime / m_fTime;
		if(t > 1.0f)
			t = 1.0f;

		for(uint i = 0; i < m_vCurves.size(); ++i)
		{
			m_Current[i] = Interpolator::interpolate(t, m_vCurves[i]);
		}


		m_Buffer.push_back(m_Current);
	}
//----------------------------------------------------------------------
}


#endif