/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIProgressBar.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUIPROGRESSBAR_H_
#define __INC_GUIPROGRESSBAR_H_


#include "GUIControl.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Progress bar control.
	*/
	class nGENEDLL GUIProgressBar: public GUIControl
	{
	private:
		bool m_bBorder;							///< Has button border

		float m_fMin;							///< Minimum value
		float m_fMax;							///< Maximum value
		float m_fValue;							///< Current value

	public:
		ON_PAINT_DELEGATE onChange;

	public:
		GUIProgressBar();
		GUIProgressBar(const Vector2& _size, const Vector2& _position,
			bool _border, float _min, float _max, float _current);
		virtual ~GUIProgressBar();

		virtual void render(ScreenOverlay* _overlay, Vector2& _position);
		
		/// Specifies whether border should be used.
		void setBorder(bool _value);
		/// Checks if border is displayed.
		bool getBorder() const;

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
	};



	inline void GUIProgressBar::setBorder(bool _value)
	{
		m_bBorder = _value;
	}
//----------------------------------------------------------------------
	inline bool GUIProgressBar::getBorder() const
	{
		return m_bBorder;
	}
//----------------------------------------------------------------------
	inline Real GUIProgressBar::getMin() const
	{
		return m_fMin;
	}
//----------------------------------------------------------------------
	inline Real GUIProgressBar::getMax() const
	{
		return m_fMax;
	}
//----------------------------------------------------------------------
	inline Real GUIProgressBar::getValue() const
	{
		return m_fValue;
	}
//----------------------------------------------------------------------
}


#endif