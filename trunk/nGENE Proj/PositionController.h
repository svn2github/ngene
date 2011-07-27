/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PositionController.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_POSITIONCONTROLLER_H_
#define __INC_POSITIONCONTROLLER_H_


#include "AnimationCurve.h"
#include "InterpolatedController.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** PositionController allows you to change position of the
		the node.
		@par
			A node will follow the path specified. By default the node will not, however,
			face the direction of movement. To change this behaviour call
			setFaceTarget(true).
	*/
	template <typename Interpolator>
	class PositionController: public InterpolatedController <Interpolator>
	{
	protected:
		bool m_bFaceTarget;					///< Specifies if the node faces target of movement


		virtual void updateCurve();

	public:
		PositionController(float _time, bool _looped, uint _bufferSize=3);

		/** Adds new point.
			@param _point point to be added.
			@param _stage time value from range [0.0; 1.0].
		*/
		virtual void addPoint(Vector3& _point, Real _stage);

		/// Returns given key-frame.
		virtual Vector3 getPoint(uint _index);

		virtual void retrieveData();

		/// Specifies whether node should face direction of movement.
		void setFaceTarget(bool _value);

		/// Checks if node faces direction of movement.
		bool isFacingTarget() const;
	};



	template <typename Interpolator>
	PositionController <Interpolator>::PositionController(float _time, bool _looped, uint _bufferSize):
		InterpolatedController <Interpolator>(_time, _looped, _bufferSize),
		m_bFaceTarget(false)
	{
		for(uint i = 0; i < 3; ++i)
		{
			AnimationCurve curve;
			addCurve(curve);
		}
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void PositionController <Interpolator>::addPoint(Vector3& _point, Real _stage)
	{
		getCurve(0).addPoint(_point.x, _stage);
		getCurve(1).addPoint(_point.y, _stage);
		getCurve(2).addPoint(_point.z, _stage);
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	Vector3 PositionController <Interpolator>::getPoint(uint _index)
	{
		return Vector3(getCurve(0)[_index],
			getCurve(1)[_index],
			getCurve(2)[_index]);
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void PositionController <Interpolator>::retrieveData()
	{
		if(!m_bAnimating || !m_pNode)
			return;

		vector <Real>& vPos = getCurrent();
		if(!vPos.size())
			return;
		Vector3 pos(vPos[0], vPos[1], vPos[2]);


		if(m_bFaceTarget)
		{
			Quaternion quatRot;
			Matrix4x4 matRot;
			matRot.lookAt(m_pNode->getPositionLocal(), pos, Vector3::UNIT_Y);
			matRot.transpose();
			quatRot.fromMatrix(matRot);
			quatRot.normalize();
			if(m_pNode->getUpVector() == UV_Y)
				quatRot *= Quaternion(Vector3::UNIT_Y, Maths::PI);
			else
				quatRot *= Quaternion(Vector3::UNIT_Y, -Maths::PI_HALF) * Quaternion(Vector3::UNIT_X, -Maths::PI_HALF);

			quatRot.normalize();

			m_pNode->setRotation(quatRot);
		}

		m_pNode->setPosition(pos);
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void PositionController <Interpolator>::setFaceTarget(bool _value)
	{
		m_bFaceTarget = _value;
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	bool PositionController <Interpolator>::isFacingTarget() const
	{
		return m_bFaceTarget;
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void PositionController <Interpolator>::updateCurve()
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