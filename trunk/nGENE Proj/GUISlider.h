/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUISlider.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUISLIDER_H_
#define __INC_GUISLIDER_H_


#include "GUIControl.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Slider/trackbar control.
	*/
	class nGENEDLL GUISlider: public GUIControl
	{
	private:
		float m_fMin;						///< Minimum value
		float m_fMax;						///< Maximum value
		float m_fValue;						///< Current value

		float m_fStep;						/**< Step by which value of the slider will 
												 be changed when user clicks on it */

		bool m_bDragged;					///< Is slider value being changed?

	public:
		ON_PAINT_DELEGATE onChange;
	
	public:
		GUISlider();
		~GUISlider();

		void render(ScreenOverlay* _overlay, Vector2& _position);

		/// Sets minimum value.
		void setMin(Real _min);
		/// Returns maximum value.
		Real getMin() const;

		/// Sets maximum value.
		void setMax(Real _max);
		/// Returns maximum value.
		Real getMax() const;

		/// Sets current value.
		void setValue(Real _value);
		/// Returns current value.
		Real getValue() const;

		void mouseClick(uint _x, uint _y);
		void mouseDown(uint _x, uint _y);
		void mouseRelease(uint _x, uint _y);
		void mouseOver(uint _x, uint _y);

		void keyboardDown(const KeyboardEvent& _evt);
	};



	inline void GUISlider::setMin(Real _min)
	{
		m_fMin = _min;
	}
//----------------------------------------------------------------------
	inline Real GUISlider::getMin() const
	{
		return m_fMin;
	}
//----------------------------------------------------------------------
	inline void GUISlider::setMax(Real _max)
	{
		m_fMax = _max;
	}
//----------------------------------------------------------------------
	inline Real GUISlider::getMax() const
	{
		return m_fMax;
	}
//----------------------------------------------------------------------
	inline void GUISlider::setValue(Real _value)
	{
		m_fValue = _value;
		Maths::clamp(m_fValue, m_fMin, m_fMax);

		if(onChange)
			onChange();
	}
//----------------------------------------------------------------------
	inline Real GUISlider::getValue() const
	{
		return m_fValue;
	}
//----------------------------------------------------------------------
}


#endif