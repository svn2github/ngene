/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUITextField.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUITextField.h"
#include "FuncSplit.h"
#include "GUIFont.h"
#include "GUIManager.h"
#include "ScreenOverlay.h"
#include "StringFunc.h"


namespace nGENE
{
	// Initialize static members
	HashTable <KEY_CONSTANT, wchar_t> GUITextField::s_KeysToChars;


	GUITextField::GUITextField():
		m_EditTextColour(Colour::COLOUR_WHITE),
		m_nCursorPosition(0),
		m_nLineNumber(0),
		m_nMaxCharacters(0),
		m_bMultiLine(false),
		m_bBorder(true),
		m_bEdited(false),
		onChange(false)
	{
	}
//----------------------------------------------------------------------
	GUITextField::~GUITextField()
	{
	}
//----------------------------------------------------------------------
	void GUITextField::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		Colour clr = m_Colour;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		_overlay->render(_position,
						 m_fWidth,
						 m_fHeight,
						 SRect<Real>(0.0f, 0.0f, 0.125f, 0.0625f),
						 clr);


		GUIFont* pFont = GUIManager::getSingleton().getFont();
		wstring text;
		text.assign(m_stText.begin(), m_stText.end());
		if(m_bEdited)
			text.insert(text.begin() + m_nCursorPosition, L'|');

		vector <int> begins, ends;

		if(!text.empty())
		{
			if(m_bMultiLine)
			{
				GUIFont* pFont = GUIManager::getSingleton().getFont();
				vector <wstring> lines;
				
				uint lineNumber = 0;
				float width = 0.0f;
				uint begin = 0;
				uint i = 0;
				uint line = 0;
				while(i < text.length())
				{
					uint l = 0;
					if(m_Wrapping == TW_WORD)
					{
						vector <wstring> vWords;
						FuncSplit <wstring> tokenizer;
						tokenizer(text, vWords, L" ");

						uint nWord = 0;

						while(nWord < vWords.size() &&
							width + pFont->getTextWidth(vWords[nWord], m_nFontSize) < m_fWidth)
						{
							l = text.find_first_of(L'\n', begin);
							if(l != wstring::npos)
							{
								if(l < i)
									break;
							}
							width += pFont->getTextWidth(vWords[nWord], m_nFontSize) +
								pFont->getCharacterWidth(' ', m_nFontSize);
							i += vWords[nWord++].length() + 1;
						}
					}
					else if(m_Wrapping == TW_LETTER)
					{
						while(width + pFont->getCharacterWidth(text[i], m_nFontSize) < m_fWidth &&
							i < text.length())
						{
							l = text.find_first_of(L'\n', begin);
							if(l != wstring::npos)
							{
								if(l < i)
									break;
							}
							width += pFont->getCharacterWidth(text[i++], m_nFontSize);
						}
					}

					uint found = 0;
					if((found = text.find_first_of(L"|", begin)) != wstring::npos)
					{
						if(found < i)
							lineNumber = line;
					}

					++line;

					float fCharWidth = pFont->getCharacterWidth(L' ', m_nFontSize);
					wstring stLine = StringFunc::trim(text.substr(begin, i - begin));
					if(m_Alignment == TEXT_RIGHT)
					{
						width = pFont->getTextWidth(stLine, m_nFontSize);
						while(fCharWidth + width < m_fWidth)
						{
							stLine = L' ' + stLine;
							width += fCharWidth;
						}
					}
					else if(m_Alignment == TEXT_CENTRE)
					{
						width = pFont->getTextWidth(stLine, m_nFontSize);
						uint i = 0;
						while(fCharWidth + width < m_fWidth)
						{
							if(i % 2)
								stLine = L' ' + stLine;
							else
								stLine += L' ';
							++i;
							width += fCharWidth;
						}
					}

					lines.push_back(stLine);

					begin = i;
					width = 0.0f;
				}

				float y = 0.0f;
				float lineHeight = m_nFontSize + 4.0f;
				if((lineNumber + 1) * lineHeight > m_fHeight)
				{
					y -= ((float)(lineNumber + 1) * lineHeight - m_fHeight);
				}

				for(uint i = 0; i < lines.size(); ++i)
				{
					if(y >= 0.0f && (y + lineHeight) <= m_fHeight)
					{
						Colour clr = m_TextColour;
						if(m_bEdited)
							clr = m_EditTextColour;
						clr.setAlpha(clr.getAlpha() * m_fOpacity);

						pFont->drawText(lines[i],
							_position.x,
							_position.y + y,
							m_nFontSize,
							clr);
					}

					y += lineHeight;
				}
			}
			else
			{
				while(pFont->getTextWidth(text, m_nFontSize) > m_fWidth)
				{
					if(text[0] == L'|' || text[text.length() - 1] == L'|')
						m_bBorder = true;
					else
						m_bBorder = false;

					int length = static_cast<int>(m_stText.length());
					if(m_nCursorPosition < length - m_nCursorPosition)
					{
						if(m_bBorder && m_nCursorPosition)
							text.erase(text.begin());
						else
							text.erase(text.begin() + text.length() - 1);
					}
					else
					{
						if(m_bBorder && text[text.length() - 1] != L'|')
							text.erase(text.begin() + text.length() - 1);
						else
							text.erase(text.begin());
					}
				}

				Colour clr = m_TextColour;
				if(m_bEdited)
					clr = m_EditTextColour;
				clr.setAlpha(clr.getAlpha() * m_fOpacity);

				pFont->drawText(text,
								_position.x,
								_position.y,
								m_nFontSize,clr);
			}
		}


		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
	void GUITextField::keyboardDown(const KeyboardEvent& _evt)
	{
		GUIControl::keyboardDown(_evt);

		for(uint i = 0; i < 256; ++i)
		{
			if(_evt.isKeyPressed(i))
			{
				if(s_KeysToChars.count(static_cast<KEY_CONSTANT>(i)))
				{
					wchar_t c = s_KeysToChars[static_cast<KEY_CONSTANT>(i)];
					if(_evt.isKeyDown(KC_LSHIFT))
					{
						if(c == 57)
							c = 40;
						else if(c == 48)
							c = 41;
						else
							c -= 32;
					}


					// Maximum length reached
					if(m_stText.length() >= m_nMaxCharacters)
						continue;

					m_stText.insert(m_stText.begin() + m_nCursorPosition++, c);

					if(onChange)
						onChange();
				}
				else
				{
					switch(static_cast<KEY_CONSTANT>(i))
					{
					case KC_LEFT: --m_nCursorPosition;
						break;
					case KC_RIGHT: ++m_nCursorPosition;
						break;
					case KC_BACK:
						{
							if(m_nCursorPosition > 0)
								m_stText.erase(m_stText.begin() + --m_nCursorPosition);
						}
						break;
					case KC_DELETE:
						{
							if(m_nCursorPosition < (int)m_stText.length())
								m_stText.erase(m_stText.begin() + m_nCursorPosition);
						}
						break;
					case KC_RETURN:
						{
							if(m_bMultiLine)
								m_stText.insert(m_stText.begin() + m_nCursorPosition++, L'\n');
						}
						break;
					}

					Maths::clamp <int>(m_nCursorPosition, 0, m_stText.length());

					if(onChange)
						onChange();
				}
			}
		}
	}
//----------------------------------------------------------------------
	void GUITextField::fillCharTable()
	{
		s_KeysToChars[KC_1] = L'1';
		s_KeysToChars[KC_2] = L'2';
		s_KeysToChars[KC_3] = L'3';
		s_KeysToChars[KC_4] = L'4';
		s_KeysToChars[KC_5] = L'5';
		s_KeysToChars[KC_6] = L'6';
		s_KeysToChars[KC_7] = L'7';
		s_KeysToChars[KC_8] = L'8';
		s_KeysToChars[KC_9] = L'9';
		s_KeysToChars[KC_0] = L'0';

		s_KeysToChars[KC_MINUS] = L'-';
		s_KeysToChars[KC_EQUALS] = L'=';

		s_KeysToChars[KC_COMMA] = L',';
		s_KeysToChars[KC_PERIOD] = L'.';
		s_KeysToChars[KC_SEMICOLON] = L';';

		//s_KeysToChars[KC_RETURN] = L'\n';
		s_KeysToChars[KC_TAB] = L'\t';

		s_KeysToChars[KC_SPACE] = L' ';

		s_KeysToChars[KC_A] = L'a';
		s_KeysToChars[KC_B] = L'b';
        s_KeysToChars[KC_C] = L'c';
		s_KeysToChars[KC_D] = L'd';
		s_KeysToChars[KC_E] = L'e';
		s_KeysToChars[KC_F] = L'f';
		s_KeysToChars[KC_G] = L'g';
        s_KeysToChars[KC_H] = L'h';
		s_KeysToChars[KC_I] = L'i';
		s_KeysToChars[KC_J] = L'j';
		s_KeysToChars[KC_K] = L'k';
		s_KeysToChars[KC_L] = L'l';
        s_KeysToChars[KC_M] = L'm';
		s_KeysToChars[KC_N] = L'n';
		s_KeysToChars[KC_O] = L'o';
		s_KeysToChars[KC_P] = L'p';
		s_KeysToChars[KC_Q] = L'q';
		s_KeysToChars[KC_R] = L'r';
		s_KeysToChars[KC_S] = L's';
		s_KeysToChars[KC_T] = L't';
		s_KeysToChars[KC_U] = L'u';
		s_KeysToChars[KC_V] = L'v';
		s_KeysToChars[KC_W] = L'w';
        s_KeysToChars[KC_X] = L'x';
		s_KeysToChars[KC_Y] = L'y';
		s_KeysToChars[KC_Z] = L'z';
	}
//----------------------------------------------------------------------
}