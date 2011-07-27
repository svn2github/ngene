/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Colour.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Colour.h"
#include "FuncSplit.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Colour::Type(L"Colour", &Colour::Type);


	const Colour Colour::COLOUR_BLACK(0, 0, 0, 255);
	const Colour Colour::COLOUR_GRAY(128, 128, 128, 255);
	const Colour Colour::COLOUR_SILVER(192, 192, 192, 255);
	const Colour Colour::COLOUR_RED(255, 0, 0, 255);
	const Colour Colour::COLOUR_ORANGE(255, 164, 0, 255);
	const Colour Colour::COLOUR_GREEN(0, 255, 0, 255);
	const Colour Colour::COLOUR_BLUE(0, 0, 255, 255);
	const Colour Colour::COLOUR_CYAN(0, 255, 255, 255);
	const Colour Colour::COLOUR_MAGENTA(255, 0, 255, 255);
	const Colour Colour::COLOUR_YELLOW(255, 255, 0, 255);
	const Colour Colour::COLOUR_WHITE(255, 255, 255, 255);
	const Colour Colour::COLOUR_PURPLE(128, 0, 128, 255);
	const Colour Colour::COLOUR_MAROON(128, 0, 0, 255);
	const Colour Colour::COLOUR_OLIVE(128, 128, 0, 255);
	const Colour Colour::COLOUR_TRANSPARENT(0, 0, 0, 0);

	const float Colour::s_kfRedImportance = 0.241f;
	const float Colour::s_kfGreenImportance = 0.691f;
	const float Colour::s_kfBlueImportance = 0.068f;


	Colour::Colour(byte _r, byte _g, byte _b, byte _a):
		m_cRed(_r),
		m_cGreen(_g),
		m_cBlue(_b),
		m_cAlpha(_a)
	{
		init();
	}
//----------------------------------------------------------------------
	Colour::Colour(const dword _colour): m_dwColourValue(_colour)
	{
		setARGB(_colour);
		init();
	}
//----------------------------------------------------------------------
	Colour::~Colour()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Colour::init()
	{
		updateDword();
	}
//----------------------------------------------------------------------
	void Colour::setARGB(const dword _colour)
	{
		m_dwColourValue = _colour;

		// Convert value to ARGB 0-255 values
		m_cAlpha = static_cast<byte>((_colour >> 24) & 0xff);
		m_cRed	 = static_cast<byte>((_colour >> 16) & 0xff);
		m_cGreen = static_cast<byte>((_colour >> 8)  & 0xff);
		m_cBlue	 = static_cast<byte>((_colour) & 0xff);
	}
//----------------------------------------------------------------------
	void Colour::setRGBA(const dword _colour)
	{
		m_dwColourValue = _colour;

		// Convert value to RGBA 0-255 values
		m_cRed   = static_cast<byte>((_colour >> 24) & 0xff);
		m_cGreen = static_cast<byte>((_colour >> 16) & 0xff);
		m_cBlue  = static_cast<byte>((_colour >> 8)  & 0xff);
		m_cAlpha = static_cast<byte>((_colour) & 0xff);
	}
//----------------------------------------------------------------------
	void Colour::setRGBAFloat(Real _r, Real _g, Real _b, Real _a)
	{
		m_cRed  = FastFloat::floatToInt(_r * 255.0f);
		m_cGreen= FastFloat::floatToInt(_g * 255.0f);
		m_cBlue = FastFloat::floatToInt(_b * 255.0f);
		m_cAlpha= FastFloat::floatToInt(_a * 255.0f);

		// Convert value to dword value
		updateDword();
	}
//----------------------------------------------------------------------
	void Colour::setRGBA(byte _r, byte _g, byte _b, byte _a)
	{
		m_cRed  = _r;
		m_cGreen= _g;
		m_cBlue = _b;
		m_cAlpha= _a;

		// Convert value to dword value
		updateDword();
	}
//----------------------------------------------------------------------
	void Colour::updateDword()
	{
		m_dwColourValue = (((m_cAlpha) & 0xff) << 24)|
						  (((m_cRed) & 0xff) << 16)  |
						  (((m_cGreen) & 0xff) << 8) |
						  ((m_cBlue) & 0xff);
	}
//----------------------------------------------------------------------
	void Colour::saturate(Real _value)
	{
		float change = Maths::fsqrt((m_cRed * m_cRed * s_kfRedImportance) +
									(m_cGreen * m_cGreen * s_kfGreenImportance) +
									(m_cBlue * m_cBlue * s_kfBlueImportance));

		setRGBA((change + (m_cRed - change) * _value) > 255 ? 255 : (change + (m_cRed - change) * _value),
				(change + (m_cGreen - change) * _value) > 255 ? 255 : (change + (m_cGreen - change) * _value),
				(change + (m_cBlue - change) * _value) > 255 ? 255 : (change + (m_cBlue - change) * _value),
				m_cAlpha);
	}
//----------------------------------------------------------------------
	void Colour::inverse()
	{
		setRGBA(255 - m_cRed,
				255 - m_cGreen,
				255 - m_cBlue,
				m_cAlpha);
	}
//----------------------------------------------------------------------
	Colour::CMYK Colour::getCMYK() const
	{
		CMYK result;

		// Calculate converted CMYK members
		result.cBlack	= min(255 - m_cRed, min(255 - m_cGreen, 255 - m_cBlue));
		result.cCyan	= (255 - m_cRed - result.cBlack) / (255 - result.cBlack);
		result.cMagenta	= (255 - m_cGreen - result.cBlack) / (255 - result.cBlack);
		result.cYellow	= (255 - m_cBlue - result.cBlack) / (255 - result.cBlack);

		return result;
	}
//----------------------------------------------------------------------
	Colour::HSV Colour::getHSV() const
	{
		HSV result;

		// Calculate converted HSV members
		byte cMax = max(m_cRed, max(m_cGreen, m_cBlue));
		byte cMin = min(m_cRed, min(m_cGreen, m_cBlue));

		result.nHue = 0;
		if((cMax == m_cRed) && (m_cGreen >= m_cBlue))
			result.nHue = 60 * (m_cGreen - m_cBlue) / (cMax - cMin);
		else if(cMax == m_cRed)
			result.nHue = 60 * (m_cGreen - m_cBlue) / (cMax - cMin) + 360;
		else if(cMax == m_cGreen)
			result.nHue = 60 * (m_cBlue - m_cRed) / (cMax - cMin) + 120;
		else if(cMax == m_cBlue)
			result.nHue = 60 * (m_cRed - m_cGreen) / (cMax - cMin) + 240;

		if(cMax == 0)
			result.cSaturation = 0;
		else
			result.cSaturation = static_cast<byte>(1.0f - (float)cMin / (float)cMax);

		result.cValue = cMax;

		return result;
	}
//----------------------------------------------------------------------
	void Colour::cleanup()
	{
	}
//----------------------------------------------------------------------
	void Colour::fromString(const string& _string)
	{
		vector <string> vecValues;
		FuncSplit <string> tokenizer;
		tokenizer(_string, vecValues, " ,;");

		if(vecValues.size() == 4)
		{
			vector <string>::iterator iter = vecValues.begin();
			m_cRed = atoi((iter++)->c_str());
			m_cGreen = atoi((iter++)->c_str());
			m_cBlue = atoi((iter++)->c_str());
			m_cAlpha = atoi(iter->c_str());
		}
	}
//----------------------------------------------------------------------
	string Colour::toString()
	{
		stringstream buffer;
		buffer << (int)m_cRed << " " << (int)m_cGreen << " " << (int)m_cBlue << " " << (int)m_cAlpha;

		return buffer.str();
	}
//----------------------------------------------------------------------
}
