/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Colour.h
Version:	0.08
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_COLOUR_H_
#define __INC_COLOUR_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Represents a colour value.
		@remarks
			Native nGENE colour representation is in RGBA space. However,
			you can easily obtain it in CMYK or HSV colour spaces.
	*/
	class nGENEDLL Colour: public BaseClass
	{
		EXPOSE_TYPE
	private:
		byte m_cRed;								///< Red component of the colour
		byte m_cGreen;								///< Green component of the colour
		byte m_cBlue;								///< Blue component of the colour
		byte m_cAlpha;								///< Alpha component of the colour

		dword m_dwColourValue;						/**< Dword value of the colour stored
														 here to speed things up */

		void updateDword();

	public:
		/// It is just a different colour representation.
		typedef struct SCmyk
		{
			byte cBlack;
			byte cCyan;
			byte cMagenta;
			byte cYellow;
		} CMYK;

		/// It is just a different colour representation.
		typedef struct SHSV
		{
			int nHue;
			byte cSaturation;
			byte cValue;
		} HSV;

	public:
		/** Constructs a Colour object
			@remarks
				Using this construction method you have to pass
				all colour members values ranging 0 to 255.
		*/
		Colour(byte _r=255, byte _g=255, byte _b=255, byte _a=255);
		/** You can also create colour by passing dword	value
			as the constructor's argument.
		*/
		Colour(const dword _colour);
		~Colour();

		/// Prepares colour object for use.
		void init();
		void cleanup();
 
		/// Inverses the colour.
		void inverse();
		/** Saturates colour.
			@param
				value the greater the value is, the more saturated
				image becomes.
		*/
		void saturate(Real _value);

		byte getRed() const {return m_cRed;}
		byte getGreen() const {return m_cGreen;}
		byte getBlue() const {return m_cBlue;}
		byte getAlpha() const {return m_cAlpha;}

		float getRedF() const;
		float getGreenF() const;
		float getBlueF() const;
		float getAlphaF() const;

		void setRed(byte _red);
		void setGreen(byte _green);
		void setBlue(byte _blue);
		void setAlpha(byte _alpha);

		/// Converts colour to CMYK representation.
		CMYK getCMYK() const;
		/// Converts colour to HSV representation.
		HSV getHSV() const;

		/** Lets you set colour value by providing it as dword type.
			@remarks
				Alpha, Red, Green, Blue channel order is used.
		*/
		void setARGB(const dword _colour);
		/** Lets you set colour value by providing it as dword type.
			@remarks
				Red, Green, Blue, Alpha channel order is used.
		*/
		void setRGBA(const dword _colour);
		/// Returns a value of the colour as a dword type.
		dword getDwordARGB() const {return m_dwColourValue;}

		/// Lets you set colour value by directly specifying all rgba members.
		void setRGBA(byte _r, byte _g, byte _b, byte _a=255);
		/** You can also specify colour by passing float value
			as the function's arguments. Note that all of them
			has to be less-equal 1.0f and greater-equal 0.0f.
		*/
		void setRGBAFloat(Real _r, Real _g, Real _b, Real _a=1.0f);


		/** Creates colour from string.
			@param
				_string keeping data of colour. Three formats are accepted:
				"red green blue alpha", "red;green;blue;alpha" and "red,green,blue,alpha".
		*/
		void fromString(const string& _string);
		/// Writes colour to string as "red green blue alpha".
		string toString();


		// Overloaded operators

		operator dword() const {return getDwordARGB();}

		bool operator==(const Colour& rhs);
		friend bool operator==(const Colour& lhs, const Colour& rhs);

		bool operator!=(const Colour& rhs);
		friend bool operator!=(const Colour& lhs, const Colour& rhs);


		// Some important colour values
		static const Colour COLOUR_BLACK;			///< Black
		static const Colour COLOUR_GRAY;			///< Gray
		static const Colour COLOUR_SILVER;			///< Silver
		static const Colour COLOUR_RED;				///< Red
		static const Colour COLOUR_ORANGE;			///< Orange
		static const Colour COLOUR_GREEN;			///< Green
		static const Colour COLOUR_BLUE;			///< Blue
		static const Colour COLOUR_CYAN;			///< Cyan
		static const Colour COLOUR_MAGENTA;			///< Magenta
		static const Colour COLOUR_YELLOW;			///< Yellow
		static const Colour COLOUR_WHITE;			///< White
		static const Colour COLOUR_PURPLE;			///< Purple
		static const Colour COLOUR_MAROON;			///< Maroon
		static const Colour COLOUR_OLIVE;			///< Olive
		static const Colour COLOUR_TRANSPARENT;		///< Transparent

		// Members importance constants
		static const float s_kfRedImportance;
		static const float s_kfGreenImportance;
		static const float s_kfBlueImportance;
	};



	inline void Colour::setRed(byte _red)
	{
		m_cRed = _red;
		updateDword();
	}
//----------------------------------------------------------------------
	inline void Colour::setGreen(byte _green)
	{
		m_cGreen = _green;
		updateDword();
	}
//----------------------------------------------------------------------
	inline void Colour::setBlue(byte _blue)
	{
		m_cBlue = _blue;
		updateDword();
	}
//----------------------------------------------------------------------
	inline void Colour::setAlpha(byte _alpha)
	{
		m_cAlpha = _alpha;
		updateDword();
	}
//----------------------------------------------------------------------
	inline bool Colour::operator==(const Colour& rhs)
	{
		return ((m_cRed   == rhs.m_cRed) &&
				(m_cGreen == rhs.m_cGreen) &&
				(m_cBlue  == rhs.m_cBlue) &&
				(m_cAlpha == rhs.m_cAlpha));
	}
//----------------------------------------------------------------------
	inline bool operator==(const Colour& lhs, const Colour& rhs)
	{
		return ((lhs.m_cRed   == rhs.m_cRed) &&
				(lhs.m_cGreen == rhs.m_cGreen) &&
				(lhs.m_cBlue  == rhs.m_cBlue) &&
				(lhs.m_cAlpha == rhs.m_cAlpha));
	}
//----------------------------------------------------------------------
	inline bool Colour::operator!=(const Colour& rhs)
	{
		return ((m_cRed   != rhs.m_cRed) ||
				(m_cGreen != rhs.m_cGreen) ||
				(m_cBlue  != rhs.m_cBlue) ||
				(m_cAlpha != rhs.m_cAlpha));
	}
//----------------------------------------------------------------------
	inline bool operator!=(const Colour& lhs, const Colour& rhs)
	{
		return ((lhs.m_cRed   != rhs.m_cRed) ||
				(lhs.m_cGreen != rhs.m_cGreen) ||
				(lhs.m_cBlue  != rhs.m_cBlue) ||
				(lhs.m_cAlpha != rhs.m_cAlpha));
	}
//----------------------------------------------------------------------
	inline float Colour::getRedF() const
	{
		return (float)m_cRed / 255.0f;
	}
//----------------------------------------------------------------------
	inline float Colour::getGreenF() const
	{
		return (float)m_cGreen / 255.0f;
	}
//----------------------------------------------------------------------
	inline float Colour::getBlueF() const
	{
		return (float)m_cBlue / 255.0f;
	}
//----------------------------------------------------------------------
	inline float Colour::getAlphaF() const
	{
		return (float)m_cAlpha / 255.0f;
	}
//----------------------------------------------------------------------
}


#endif