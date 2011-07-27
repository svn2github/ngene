/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIText.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUITEXT_H_
#define __INC_GUITEXT_H_


#include "GUIControl.h"


namespace nGENE
{
	/// Text-alignment
	enum TEXT_ALIGNMENT
	{
		TEXT_LEFT,
		TEXT_RIGHT,
		TEXT_CENTRE
	};

	/// Text wrapping method
	enum TEXT_WRAP
	{
		TW_LETTER,
		TW_WORD
	};


	/** Abstract text control interface.
	*/
	class nGENEDLL GUIText: public GUIControl
	{
	protected:
		uint m_nFontSize;					///< Font size

		TEXT_ALIGNMENT m_Alignment;			///< Text alignment
		TEXT_WRAP m_Wrapping;				///< Text wrapping method

		Colour m_TextColour;				///< Colour of the text

		wstring m_stText;					///< Caption of the element

	public:
		GUIText():
			m_nFontSize(12),
			m_TextColour(Colour::COLOUR_BLACK),
			m_Alignment(TEXT_LEFT),
			m_Wrapping(TW_WORD)
		{
		}

		virtual void render(ScreenOverlay* _overlay, Vector2& _position)=0;

		/// Sets text of this control.
		virtual void setText(const wstring& _text);
		/// Returns text of this control.
		virtual const wstring& getText() const;

		/// Sets text font size.
		virtual void setFontSize(uint _size);
		/// Returns text font size.
		virtual uint getFontSize() const;

		/// Sets text font colour.
		virtual void setTextColour(const Colour& _clr);
		/// Returns text font colour.
		virtual Colour& getTextColour();

		/// Sets text alignment for this label.
		void setTextAlignment(TEXT_ALIGNMENT _alignment);
		/// Returns text alignment for this label.
		TEXT_ALIGNMENT getTextAlignment() const;

		/** Sets text wrapping method.
			@par
				One can decide to wrap based on words or letters.
		*/
		void setTextWrapping(TEXT_WRAP _wrap);
		/// Returns text wrapping method.
		TEXT_WRAP getTextWrapping() const;
	};



	inline void GUIText::setText(const wstring& _text)
	{
		m_stText = _text;
	}
//----------------------------------------------------------------------
	inline const wstring& GUIText::getText() const
	{
		return m_stText;
	}
//----------------------------------------------------------------------
	inline void GUIText::setFontSize(uint _size)
	{
		m_nFontSize = _size;
	}
//----------------------------------------------------------------------
	inline uint GUIText::getFontSize() const
	{
		return m_nFontSize;
	}
//----------------------------------------------------------------------
	inline void GUIText::setTextColour(const Colour& _clr)
	{
		m_TextColour = _clr;
	}
//----------------------------------------------------------------------
	inline Colour& GUIText::getTextColour()
	{
		return m_TextColour;
	}
//----------------------------------------------------------------------
	inline void GUIText::setTextAlignment(TEXT_ALIGNMENT _alignment)
	{
		m_Alignment = _alignment;
	}
//----------------------------------------------------------------------
	inline TEXT_ALIGNMENT GUIText::getTextAlignment() const
	{
		return m_Alignment;
	}
//----------------------------------------------------------------------
	inline void GUIText::setTextWrapping(TEXT_WRAP _wrap)
	{
		m_Wrapping = _wrap;
	}
//----------------------------------------------------------------------
	inline TEXT_WRAP GUIText::getTextWrapping() const
	{
		return m_Wrapping;
	}
//----------------------------------------------------------------------
}


#endif