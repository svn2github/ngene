/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIStatic.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIStatic.h"
#include "GUIFont.h"
#include "GUIManager.h"
#include "FuncSplit.h"
#include "ScreenOverlay.h"
#include "StringFunc.h"


namespace nGENE
{
	GUIStatic::GUIStatic()
	{
		m_fWidth = 800;
		m_fHeight = 600;

		m_Colour.setAlpha(0);
	}
//----------------------------------------------------------------------
	GUIStatic::~GUIStatic()
	{
	}
//----------------------------------------------------------------------
	void GUIStatic::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		Colour clr = m_Colour;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		_overlay->render(_position,
						 m_fWidth,
						 m_fHeight,
						 SRect<Real>(0.0f, 0.0f, 0.125f, 0.0625f),
						 clr);

		if(!m_stText.empty())
		{
			// We need to make a copy of text as we have to alter it
			wstring text = m_stText;
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
					wstring stText = text.substr(begin, text.length() - begin);

					vector <wstring> vWords;
					FuncSplit <wstring> tokenizer;
					tokenizer(stText, vWords, L" ");

					uint nWord = 0;
					uint counter = 0;

					while(nWord < vWords.size() &&
						  width + pFont->getTextWidth(vWords[nWord], m_nFontSize) < m_fWidth)
					{
						l = stText.find_first_of(L'\n', 0);
						if(l != wstring::npos)
						{
							if(l < counter)
								break;
						}
						width += pFont->getTextWidth(vWords[nWord], m_nFontSize) +
							pFont->getCharacterWidth(' ', m_nFontSize);
						i += vWords[nWord].length() + 1;
						counter += vWords[nWord++].length() + 1;
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

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
}