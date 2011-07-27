/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIScrollbar.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUISCROLLBAR_H_
#define __INC_GUISCROLLBAR_H_


#include "GUIButton.h"
#include "GUIControl.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Scrollbar.
	*/
	class nGENEDLL GUIScrollbar: public GUIControl
	{
	private:
		float m_fMin;								///< Minimum value
		float m_fMax;								///< Maximum value
		float m_fValue;								///< Current value
		float m_fSmallStep;							///< Small step (buttons pressed)
		float m_fBigStep;							///< Big step (track pressed)

		GUIButton m_btnUp;							///< Up button
		GUIButton m_btnDown;						///< Down button

		bool m_bHorizontal;							///< Is scrollbar horizontal?
		bool m_bDragged;							///< Is trackbar dragged?


		void _ButtonUp_Click(uint _x, uint _y);
		void _ButtonDown_Click(uint _x, uint _y);

	public:
		ON_PAINT_DELEGATE onChange;
	
	public:
		GUIScrollbar();
		virtual ~GUIScrollbar();

		virtual void render(ScreenOverlay* _overlay, Vector2& _position);

		void setParent(GUIControl* _parent);

		void setVisible(bool _value);
		void toggleVisible();
		void show();
		void hide();

		void setMin(Real _min);
		Real getMin() const;

		void setMax(Real _max);
		Real getMax() const;

		void setValue(Real _value);
		Real getValue() const;

		void setSmallStep(Real _value);
		Real getSmallStep() const;

		void setBigStep(Real _value);
		Real getBigStep() const;

		void mouseClick(uint _x, uint _y);
		void mouseDown(uint _x, uint _y);
		void mouseRelease(uint _x, uint _y);
		void mouseOver(uint _x, uint _y);
	};



	inline void GUIScrollbar::setMin(Real _min)
	{
		m_fMin = _min;
	}
//----------------------------------------------------------------------
	inline Real GUIScrollbar::getMin() const
	{
		return m_fMin;
	}
//----------------------------------------------------------------------
	inline void GUIScrollbar::setMax(Real _max)
	{
		m_fMax = _max;
	}
//----------------------------------------------------------------------
	inline Real GUIScrollbar::getMax() const
	{
		return m_fMax;
	}
//----------------------------------------------------------------------
	inline void GUIScrollbar::setValue(Real _value)
	{
		m_fValue = _value;
		Maths::clamp(m_fValue, m_fMin, m_fMax);
	}
//----------------------------------------------------------------------
	inline Real GUIScrollbar::getValue() const
	{
		return m_fValue;
	}
//----------------------------------------------------------------------
	inline void GUIScrollbar::setSmallStep(Real _value)
	{
		m_fSmallStep = _value;
	}
//----------------------------------------------------------------------
	inline Real GUIScrollbar::getSmallStep() const
	{
		return m_fSmallStep;
	}
//----------------------------------------------------------------------
	inline void GUIScrollbar::setBigStep(Real _value)
	{
		m_fBigStep = _value;
	}
//----------------------------------------------------------------------
	inline Real GUIScrollbar::getBigStep() const
	{
		return m_fBigStep;
	}
//----------------------------------------------------------------------
}


#endif