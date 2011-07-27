/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		InterpolatedController.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_INTERPOLATEDCONTROLLER_H_
#define __INC_INTERPOLATEDCONTROLLER_H_


#include "AnimationCurve.h"
#include "IAnimationController.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Interpolated animation.
		@par
			Animation is done by interpolating between key frames
			where t factor is time.
	*/
	template <typename Interpolator>
	class InterpolatedController: public IAnimationController, public Interpolator
	{
	protected:
		float m_fTime;					///< Total animation time
		float m_fPassedTime;			///< Time which already passed
		bool m_bLooped;					///< Is animation looped?

		vector <AnimationCurve> m_vCurves;
		vector <Real> m_Current;
		vector <Real> m_Prev;

		list <vector <Real> > m_Buffer;

		Semaphore m_FillCount;
		Semaphore m_EmptyCount;


		/// Update AnimationCurve object.
		virtual void updateCurve()=0;

		virtual void animate();

	public:
		InterpolatedController(Real _time, bool _loop=false, uint _bufferSize=3);

		/// Adds new AnimationCurve.
		void addCurve(AnimationCurve& _curve);

		/// Returns given AnimationCurve.
		AnimationCurve& getCurve(uint _index);

		vector <Real> getCurrent();

		virtual void update();
	};



	template <typename Interpolator>
	InterpolatedController <Interpolator>::InterpolatedController(Real _time, bool _loop, uint _bufferSize):
		m_fTime(_time),
		m_fPassedTime(0.0f),
		m_bLooped(_loop),
		m_FillCount(0, _bufferSize),
		m_EmptyCount(_bufferSize, _bufferSize)
	{
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void InterpolatedController <Interpolator>::addCurve(AnimationCurve& _curve)
	{
		Real value = 0.0f;
		m_Current.push_back(value);
		m_vCurves.push_back(_curve);
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	AnimationCurve& InterpolatedController <Interpolator>::getCurve(uint _index)
	{
		return m_vCurves[_index];
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	vector <Real> InterpolatedController <Interpolator>::getCurrent()
	{
		vector <Real> result;

		if(!m_FillCount.tryDown())
			return m_Prev;

			m_Prev = m_Buffer.front();
			m_Buffer.pop_front();

		m_EmptyCount.up(1);

		return m_Prev;
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void InterpolatedController <Interpolator>::update()
	{
		if(m_bLooped)
		{
			if(!m_bStopped)
				animate();
		}
		else
		{
			if(!m_bStopped && !m_bFinished)
				animate();
		}
	}
//----------------------------------------------------------------------
	template <typename Interpolator>
	void InterpolatedController <Interpolator>::animate()
	{
		if(!m_bAnimating)
		{
			m_nPrevTime = Engine::getSingleton().getTimer().getMilliseconds();
			return;
		}

		while(m_fPassedTime >= m_fTime)
			m_fPassedTime -= m_fTime;

		m_EmptyCount.down();

		dword now = Engine::getSingleton().getTimer().getMilliseconds();
		dword diff = now - m_nPrevTime;

		m_fPassedTime += static_cast<float>(diff);

		if(m_fPassedTime >= m_fTime)
		{
			for(uint i = 0; i < m_vCurves.size(); ++i)
				m_Current[i] = m_vCurves[i].getPoint(m_vCurves[i].size() - 1);

			if(!m_bLooped)
			{
				m_bFinished = true;
				return;
			}
		}


		updateCurve();

		m_FillCount.up(1);

		m_nPrevTime = now;
	}
//----------------------------------------------------------------------
}


#endif