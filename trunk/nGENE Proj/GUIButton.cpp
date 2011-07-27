/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIButton.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIButton.h"
#include "Colour.h"
#include "GUIFont.h"
#include "GUIImage.h"
#include "GUIManager.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUIButton::GUIButton():
		m_pImage(NULL),
		m_VerticalAlignment(CVA_TOP),
		m_Alignment(TEXT_LEFT),
		m_nFontSize(24),
		m_bBorder(false),
		m_TextColour(Colour::COLOUR_BLACK)
	{
		setMouseOverColour(Colour(Colour::COLOUR_RED));
	}
//----------------------------------------------------------------------
	GUIButton::GUIButton(const Vector2& _size, const Vector2& _position,
		const wstring& _caption, bool _border):
		m_stCaption(_caption),
		m_pImage(NULL),
		m_VerticalAlignment(CVA_TOP),
		m_Alignment(TEXT_LEFT),
		m_nFontSize(24),
		m_bBorder(_border),
		m_TextColour(Colour::COLOUR_BLACK)
	{
		setMouseOverColour(Colour(Colour::COLOUR_RED));

		this->setSize(_size);
		this->setPosition(_position);
	}
//----------------------------------------------------------------------
	GUIButton::~GUIButton()
	{
	}
//----------------------------------------------------------------------
	void GUIButton::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		Colour col = m_Colour;
		if(m_State == STATE_MOUSE_OVER)
			col = m_MouseOverColour;

		col.setAlpha(col.getAlpha() * m_fOpacity);
		_overlay->render(_position, m_fWidth, m_fHeight,
			SRect<Real>(0.0f, 0.0f, 0.125f, 0.0625f), col);

		if(m_pImage)
			m_pImage->render(_overlay, _position);

		// Apply vertical and horizontal alignment
		if(!m_stCaption.empty())
		{
			Vector2 vecPos = _position;

			GUIFont* pFont = GUIManager::getSingleton().getFont();
			if(m_Alignment == TEXT_CENTRE)
				vecPos.x += (m_fWidth - pFont->getTextWidth(m_stCaption, m_nFontSize)) / 2;
			else if(m_Alignment == TEXT_RIGHT)
				vecPos.x += (m_fWidth - pFont->getTextWidth(m_stCaption, m_nFontSize));

			if(m_VerticalAlignment == CVA_MIDDLE)
				vecPos.y += (m_fHeight - m_nFontSize) / 2;
			else if(m_VerticalAlignment == CVA_BOTTOM)
				vecPos.y += m_fHeight - m_nFontSize;
			
			col = m_TextColour;
			if(m_State == STATE_MOUSE_OVER)
				col = m_TextOverColour;
			col.setAlpha(col.getAlpha() * m_fOpacity);
			pFont->drawText(m_stCaption,
							vecPos.x,
							vecPos.y,
							m_nFontSize,
							col);
		}

		if(m_bBorder)
		{
			col = Colour::COLOUR_BLACK;
			col.setAlpha(col.getAlpha() * m_fOpacity);
			_overlay->render(Vector2(_position.x, _position.y), 2.0f, m_fHeight, SRect<Real>(0.0f, 0.125f, 0.00390625f, 0.1875f), col);
			_overlay->render(Vector2(_position.x + m_fWidth - 2.0f, _position.y), 2.0f, m_fHeight, SRect<Real>(0.0f, 0.125f, 0.00390625f, 0.1875f), col);
			_overlay->render(Vector2(_position.x + 2.0f, _position.y), m_fWidth - 4.0f, 2.0f, SRect<Real>(0.0f, 0.1875f, 0.0625f, 0.19140625f), col);
			_overlay->render(Vector2(_position.x + 2.0f, _position.y + m_fHeight - 2.0f), m_fWidth - 4.0f, 2.0f, SRect<Real>(0.0f, 0.1875f, 0.0625f, 0.19140625f), col);
		}

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
}