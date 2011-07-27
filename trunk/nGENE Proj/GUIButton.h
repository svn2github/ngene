/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIButton.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUIBUTTON_H_
#define __INC_GUIBUTTON_H_


#include "GUIControl.h"
#include "Prerequisities.h"
#include "GUIText.h"


namespace nGENE
{
	/// Vertical alignment of control's content.
	enum CONTENT_VERTICAL_ALIGNMENT
	{
		CVA_TOP,
		CVA_MIDDLE,
		CVA_BOTTOM
	};

	/** Button control.
	*/
	class nGENEDLL GUIButton: public GUIControl
	{
	private:
		wstring m_stCaption;					///< Caption of the element
		
		GUIImage* m_pImage;						///< Foreground image

		/// Vertical alignment of the caption/image
		CONTENT_VERTICAL_ALIGNMENT m_VerticalAlignment;
		TEXT_ALIGNMENT m_Alignment;				///< Text alignment

		uint m_nFontSize;						///< Font size
		bool m_bBorder;							///< Has button border

		Colour m_TextColour;					///< Colour of the text
		Colour m_TextOverColour;

	public:
		GUIButton();
		GUIButton(const Vector2& _size, const Vector2& _position,
			const wstring& _caption, bool _border);
		virtual ~GUIButton();

		virtual void render(ScreenOverlay* _overlay, Vector2& _position);
		
		/// Sets caption of the button.
		void setCaption(const wstring& _caption);
		/// Returns caption of the button.
		const wstring& getCaption() const;

		/// Sets image of the button.
		void setImage(GUIImage* _image);
		/// Returns image of the button.
		GUIImage* getImage() const;

		/// Sets vertical alignment of text/image.
		void setVerticalAlignment(CONTENT_VERTICAL_ALIGNMENT _alignment);
		/// Returns vertical alignment of text/image.
		CONTENT_VERTICAL_ALIGNMENT getVerticalAlignment() const;

		/// Sets text alignment for this label.
		void setTextAlignment(TEXT_ALIGNMENT _alignment);
		/// Returns text alignment for this label.
		TEXT_ALIGNMENT getTextAlignment() const;

		/// Specifies whether border should be used.
		void setBorder(bool _value);
		/// Checks if border is displayed.
		bool getBorder() const;

		/// Sets control text colour when mouse is over control.
		void setMouseTextOverColour(const Colour& _colour);
		/// Returns control text colour when mouse is over control.
		Colour& getMouseTextOvetColour();

		/// Sets text font size.
		virtual void setFontSize(uint _size);
		/// Returns text font size.
		virtual uint getFontSize() const;

		/// Sets text font colour.
		virtual void setTextColour(const Colour& _clr);
		/// Returns text font colour.
		virtual Colour& getTextColour();
	};



	inline void GUIButton::setCaption(const wstring& _caption)
	{
		m_stCaption = _caption;
	}
//----------------------------------------------------------------------
	inline const wstring& GUIButton::getCaption() const
	{
		return m_stCaption;
	}
//----------------------------------------------------------------------
	inline void GUIButton::setImage(GUIImage* _image)
	{
		m_pImage = _image;
	}
//----------------------------------------------------------------------
	inline GUIImage* GUIButton::getImage() const
	{
		return m_pImage;
	}
//----------------------------------------------------------------------
	inline void GUIButton::setBorder(bool _value)
	{
		m_bBorder = _value;
	}
//----------------------------------------------------------------------
	inline bool GUIButton::getBorder() const
	{
		return m_bBorder;
	}
//----------------------------------------------------------------------
	inline void GUIButton::setVerticalAlignment(CONTENT_VERTICAL_ALIGNMENT _alignment)
	{
		m_VerticalAlignment = _alignment;
	}
//----------------------------------------------------------------------
	inline CONTENT_VERTICAL_ALIGNMENT GUIButton::getVerticalAlignment() const
	{
		return m_VerticalAlignment;
	}
//----------------------------------------------------------------------
	inline void GUIButton::setFontSize(uint _size)
	{
		m_nFontSize = _size;
	}
//----------------------------------------------------------------------
	inline uint GUIButton::getFontSize() const
	{
		return m_nFontSize;
	}
//----------------------------------------------------------------------
	inline void GUIButton::setTextColour(const Colour& _clr)
	{
		m_TextColour = _clr;
	}
//----------------------------------------------------------------------
	inline Colour& GUIButton::getTextColour()
	{
		return m_TextColour;
	}
//----------------------------------------------------------------------
	inline void GUIButton::setTextAlignment(TEXT_ALIGNMENT _alignment)
	{
		m_Alignment = _alignment;
	}
//----------------------------------------------------------------------
	inline TEXT_ALIGNMENT GUIButton::getTextAlignment() const
	{
		return m_Alignment;
	}
//----------------------------------------------------------------------
	inline void GUIButton::setMouseTextOverColour(const Colour& _colour)
	{
		m_TextOverColour = _colour;
	}
//----------------------------------------------------------------------
	inline Colour& GUIButton::getMouseTextOvetColour()
	{
		return m_TextOverColour;
	}
//----------------------------------------------------------------------
}


#endif