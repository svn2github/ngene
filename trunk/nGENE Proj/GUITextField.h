/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUITextField.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUITEXTFIELD_H_
#define __INC_GUITEXTFIELD_H_


#include "DeviceKeyboard.h"
#include "GUIText.h"
#include "HashTable.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Static text control.
		@remarks
			Static text, also known as label, cannot be changed
			by user directly.
	*/
	class nGENEDLL GUITextField: public GUIText
	{
	private:
		/// Critical section for multiple threads support
		CriticalSection m_CriticalSection;

		Colour m_EditTextColour;			///< Colour of the text in edit mode

		int m_nCursorPosition;				///< Position of the cursor
		int m_nLineNumber;					///< Number of the line

		uint m_nMaxCharacters;				///< Maximum number of characters

		bool m_bMultiLine;					///< Are multiple lines enabled?
		bool m_bBorder;						///< Is border enabled?
		bool m_bEdited;

		static HashTable <KEY_CONSTANT, wchar_t> s_KeysToChars;

	public:
		ON_PAINT_DELEGATE onChange;

	public:
		GUITextField();
		virtual ~GUITextField();

		virtual void render(ScreenOverlay* _overlay, Vector2& _position);

		/// Specifies if multiple lines are enabled.
		void setMultiline(bool _value);
		/// Checks if multiple lines are enabled.
		bool isMultiline() const;

		void keyboardDown(const KeyboardEvent& _evt);

		void gettingFocus();
		void loosingFocus();

		/** Sets text of the field.
			@param _text to set.
			@remarks
				If setMaxCharacters() was called the first
				max characters characters will be copied from the
				_text object.
		*/
		void setText(const wstring& _text);

		/// Sets maximum length of the text in characters.
		void setMaxCharacters(uint _length);
		/// Returns maximum length of the text in characters.
		uint getMaxCharacters() const;

		/// Sets text font colour when in edit mode.
		virtual void setEditTextColour(const Colour& _clr);
		/// Returns text font colour when in edit mode.
		virtual Colour& getEditTextColour();


		static void fillCharTable();
	};


	inline void GUITextField::setMultiline(bool _value)
	{
		m_bMultiLine = _value;
	}
//----------------------------------------------------------------------
	inline bool GUITextField::isMultiline() const
	{
		return m_bMultiLine;
	}
//----------------------------------------------------------------------
	inline void GUITextField::gettingFocus()
	{
		GUIControl::gettingFocus();
		m_bEdited = true;
	}
//----------------------------------------------------------------------
	inline void GUITextField::loosingFocus()
	{
		GUIControl::loosingFocus();
		m_bEdited = false;
	}
//----------------------------------------------------------------------
	inline void GUITextField::setText(const wstring& _text)
	{
		SharedObject sharedObj(&m_CriticalSection);


		if(m_nMaxCharacters && m_nMaxCharacters <= m_stText.length())
		{
			m_stText = _text.substr(0, m_nMaxCharacters);
			m_nCursorPosition = m_nMaxCharacters;
		}
		else
		{
			m_stText = _text;
			m_nCursorPosition = m_stText.length();
		}
	}
//----------------------------------------------------------------------
	inline void GUITextField::setEditTextColour(const Colour& _clr)
	{
		m_EditTextColour = _clr;
	}
//----------------------------------------------------------------------
	inline Colour& GUITextField::getEditTextColour()
	{
		return m_EditTextColour;
	}
//----------------------------------------------------------------------
	inline void GUITextField::setMaxCharacters(uint _length)
	{
		m_nMaxCharacters = _length;
	}
//----------------------------------------------------------------------
	inline uint GUITextField::getMaxCharacters() const
	{
		return m_nMaxCharacters;
	}
//----------------------------------------------------------------------
}


#endif