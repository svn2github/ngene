/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIProgressBar.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIProgressBar.h"
#include "Colour.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUIProgressBar::GUIProgressBar():
		m_fMin(0.0f),
		m_fMax(1.0f),
		m_fValue(0.0f),
		m_bBorder(true),
		onChange(NULL)
	{
	}
//----------------------------------------------------------------------
	GUIProgressBar::GUIProgressBar(const Vector2& _size, const Vector2& _position,
		bool _border, float _min, float _max, float _current):
		m_fMin(_min),
		m_fMax(_max),
		m_fValue(_current),
		m_bBorder(_border),
		onChange(NULL)
	{
		this->setSize(_size);
		this->setPosition(_position);
	}
//----------------------------------------------------------------------
	GUIProgressBar::~GUIProgressBar()
	{
	}
//----------------------------------------------------------------------
	void GUIProgressBar::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		Colour clr = m_Colour;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		float fWidth = ((m_fValue - m_fMin) / (m_fMax - m_fMin)) * m_fWidth;
		_overlay->render(_position,
						 fWidth,
						 m_fHeight,
						 SRect<Real>(0.3125f, 0.0f, 0.375f, 0.0625f),
						 clr);

		if(m_bBorder)
		{
			clr = Colour::COLOUR_BLACK;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);

			_overlay->render(Vector2(_position.x, _position.y),
							 2.0f,
							 m_fHeight,
							 SRect<Real>(0.0f, 0.125f, 0.00390625f, 0.1875f),
							 clr);
			_overlay->render(Vector2(_position.x + m_fWidth - 2.0f, _position.y),
							 2.0f,
							 m_fHeight,
							 SRect<Real>(0.0f, 0.125f, 0.00390625f, 0.1875f),
							 clr);
			_overlay->render(Vector2(_position.x + 2.0f, _position.y),
							 m_fWidth - 4.0f,
							 2.0f,
							 SRect<Real>(0.0f, 0.1875f, 0.0625f, 0.19140625f),
							 clr);
			_overlay->render(Vector2(_position.x + 2.0f, _position.y + m_fHeight - 2.0f),
							 m_fWidth - 4.0f,
							 2.0f,
							 SRect<Real>(0.0f, 0.1875f, 0.0625f, 0.19140625f),
							 clr);
		}

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
	void GUIProgressBar::setMin(Real _min)
	{
		if(Maths::floatCompare(_min, m_fMin))
		{
			if(onChange)
				onChange();
		}

		m_fMin = _min;
	}
//----------------------------------------------------------------------
	void GUIProgressBar::setMax(Real _max)
	{
		if(Maths::floatCompare(_max, m_fMax))
		{
			if(onChange)
				onChange();
		}

		m_fMax = _max;
	}
//----------------------------------------------------------------------
	void GUIProgressBar::setValue(Real _value)
	{
		if(Maths::floatCompare(_value, m_fValue))
		{
			if(onChange)
				onChange();
		}

		m_fValue = _value;
		Maths::clamp(m_fValue, m_fMin, m_fMax);

		if(onChange)
			onChange();
	}
//----------------------------------------------------------------------
}