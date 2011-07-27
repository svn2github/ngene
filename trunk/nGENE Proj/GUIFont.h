/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIFont.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUIFONT_H_
#define __INC_GUIFONT_H_


#include "Prerequisities.h"


namespace nGENE
{
	/// Structure describing single character of the font.
	typedef struct SCharacterDesc
	{
		ushort x;
		ushort y;
		ushort width;
		ushort height;
		short xOffset;
		short yOffset;
		short xAdvance;
		ushort page;

		SCharacterDesc():
			x(0),
			y(0),
			width(0),
			height(0),
			xOffset(0),
			yOffset(0),
			xAdvance(0),
			page(0)
		{
		}
	} CHAR_DESC;


	/** Font class.
	*/
	class nGENEDLL GUIFont
	{
	private:
		typedef struct SFontDesc
		{
			ushort lineHeight;
			ushort base;
			ushort width;
			ushort height;
			ushort pages;
			CHAR_DESC* chars;
		} FONT_DESC;

		struct STextDesc
		{
			uint x;
			uint y;
			uint size;
			dword colour;
			wstring text;
		};

		vector <STextDesc> m_vTexts;

		FONT_DESC m_Desc;

		ITexture* m_pTexture;					///< Pointer to texture holding font characters
		const uint m_kBatchSize;				/**< What is the batch size? */

		uint m_nCurrentIndex;					///< What is the index of current element?

		VertexBuffer* m_pVB;
		IndexedBuffer* m_pIVB;

		SVertex2D* m_pVertices;					///< Pointer to vertex data
		ushort* m_pIndices;						///< Pointer to index data


		void render(const STextDesc& _text);

	private:
		void parse(const wstring& _fontDefFile, const wstring& _textureFile);
		void parseFont(XMLDocument* _node);
		void parsePage(XMLDocument* _node);
		void parseChars(XMLDocument* _node);

		void createBuffers();

		void lock();
		void unlock();
		void flush();


		// Prevent copying and assignment
		GUIFont(const GUIFont& _src);
		GUIFont& operator=(const GUIFont& _rhs);

	public:
		/** GUIFont class constructor.
			@param
				_fontDefFile name of the file containing font definition matching Bitmap
				Font Generator (http://www.angelcode.com/products/bmfont/) format.
			@param
				_textureFile name of the texture. If it is "", the default texture specified
				in the font definition file will be used.
			@param
				_maxLength maximum length this font will be capable of rendering.
		*/
		GUIFont(const wstring& _fontDefFile, const wstring& _textureFile=L"", uint _batchSize=300);
		virtual ~GUIFont();

		void begin();
		void end();

		void cleanup();

		/// Prints text on the screen.
		void drawText(const wstring& _text, uint _x=0, uint _y=0, uint _size=32,
			const Colour& _colour=Colour::COLOUR_BLACK);

		const CHAR_DESC* getCharacterDesc(uint _index) const;
		/// Returns width of a given character.
		float getCharacterWidth(uint _index, uint _size) const;
		/// Returns width of a text.
		float getTextWidth(const wstring& _text, uint _size) const;
	};



	inline const CHAR_DESC* GUIFont::getCharacterDesc(uint _index) const
	{
		if(_index > 255)
			return NULL;

		return &m_Desc.chars[_index];
	}
//----------------------------------------------------------------------
	inline float GUIFont::getCharacterWidth(uint _index, uint _size) const
	{
		if(_index > 255)
			return 0.0f;

		float size = (float)_size / (float)m_Desc.lineHeight;
		float width = ((float)m_Desc.chars[_index].xAdvance * size);

		return width;
	}
//----------------------------------------------------------------------
}


#endif