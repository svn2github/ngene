/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Maths.h
Version:	0.13
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MATHS_H_
#define __INC_MATHS_H_


#include <limits>
#include "Log.h"
#include "FastFloat.h"
#include "Prerequisities.h"


namespace nGENE
{
	// Some typedefs
	typedef Matrix4x4 Matrix4;				///< Just simplified class name
	typedef Matrix3x3 Matrix3;				///< Just simplified class name


	/** Represents mathematical calculations and algorithms.
		@remarks
			Vast majority of functions of this class are marked
			static. The same case is with the constants.
	*/
	class nGENEDLL Maths
	{
	public:
		/** The angular units used by the engine.
		*/
		enum ANGLE_UNIT
		{
			ANGLE_UNKNOWN,						///< Unit is unknown
			ANGLE_DEGREE,						///< Degrees
			ANGLE_RADIAN						///< Radians
		};

		static const Real FloatEpsilon;			///< Epsilon to compare float values

		static const Real PI;					///< PI number
		static const Real PI_DOUBLE;			///< PI * 2
		static const Real PI_HALF;				///< PI / 2
		static const Real Infinity;				/**< Infinity. Yes - it has a numerical
													 value, and is pretty small */
		static const Real MinusInfinity;		///< Minus infinity

	private:
		static ANGLE_UNIT s_AngleUnit;			///< Degrees/Radians
		static FastFloat* s_FastFloat;			/**< Fast calculations on floating
													 point numbers */

		static bool m_bUseTables;				///< Should trigonometry tables be used?
		static bool m_bUseSSE;					///< Should SSE extensions be used?

		static const Real m_kDegToRad;
		static const Real m_kRadToDeg;

	public:
		Maths();
		~Maths();

		/** Sets if SSE should be used or not.
			@param _value if set to 1 then SSE is used. In other case - not.
		*/
		static void setUseSSE(bool _value) {m_bUseSSE=_value;}
		/// Checks if SSE is used.
		static bool isUsingSSE() {return m_bUseSSE;}

		/** Safe compare of two floating point values.
			@returns
				int value is 0 when compared values are equal; 1 - when
				first of them is greater; -1 - whenever the second one is greater.
		*/
		static int floatCompare(Real _value1, Real _value2);

		/** Sets angular units used within the engine.
		*/
		static void setAngleUnit(ANGLE_UNIT _unit) {s_AngleUnit=_unit;}
		static ANGLE_UNIT getAngleUnit() {return s_AngleUnit;}

		/** Sets the trigonometry functions mode.
			@param
				value if is set to 1, trigonometry tables will be used.
				It is default value and it speeds up sine and cosine
				"calculations" as it just becomes a mean of looking
				angle in the table up.
		*/
		static void setTrigTableEnabled(bool _value) {m_bUseTables=_value;}
		static bool getTrigTableEnabled() {return m_bUseTables;}

		/** Converts degrees to radians.
			@param
				_angle angle in degrees
			@return
				float given angle converted to radians
		*/
		static Real degToRad(Real _angle);

		/** Converts radians to degrees.
			@param
				_angle angle in radians
			@return
				float given angle converted to degrees.
		*/
		static Real radToDeg(Real _angle);

		/// Returns sine value from a table for a given angle.
		static Real fsin(Real _angle);

		/// Returns cosine value from a table for a given angle.
		static Real fcos(Real _angle);

		/// Returns tangens value for a given angle.
		static Real ftan(Real _angle);

		/// Returns arcus-tangens value for a given angle.
		static Real fatan(Real _angle);

		/// Calculates square root of the number.
		static Real fsqrt(Real _num);

		/** Calculates perlin noise function value.
			@remarks
				This function is quite often used as random
				number generator.
			@returns
				Real result of this function is always in the
				range [-1.0f, 1.0f].
		*/
		static Real perlinNoiseGenerator(int _x, int _y, int _random);


		// Below are functions marked 'friend' elsewhere


		/** Normalizes given vector.
			@remarks
				The new vector has the same direction as it had, but
				its	length is equal to 1. Therefore it is usually
				called unit	vector.
		*/
		static Vector2 normalize(const Vector2& _vec);
		static Vector3 normalize(const Vector3& _vec);
		static Vector4 normalize(const Vector4& _vec);

		/** Clamps the number of type T to given boundaries.
			@param
				_value a number to clamp
			@param
				_down lower boundary
			@param
				_up upper boundary
		*/
		template <typename T> static void clamp(T& _value, const T& _down, const T& _up);
		/** Clamps the number of type T with roll.
			@remarks
				If the number exceeds the maximum value it is rolled to the
				minimum one. When is smaller than minimum value, then it is
				rolled to the greatest. Note however that this function
				in case of eg. clamp_roll(10, 0, 8) will result in 0 - not
				in 1.
		*/
		template <typename T> static void clamp_roll(T& _value, const T& _down, const T& _up);

		/// Rolls the number of type T if it exceeds its boundaries.
		template <typename T> static void roll(T& _value, const T& _down, const T& _up);
		
		/// Computes absolute value
		template <typename T> static T abs(const T& _value);
		template <> static int abs(const int& _value);

		/// Checks to see if integer is power of 2.
		static bool isPowerOfTwo(uint _value);
		/// Rounds up to the next power of 2.
		static uint roundUpToPowerOfTwo(uint _value);


		/** Linear interpolation of two values in 1D-space.
			@param
				_start start value.
			@param
				_end end value.
			@param
				_s coefficient showing at which point between bounding values
				the result is. If it is 0.0 then return value equals _start,
				whereas when it is 1.0 then it equals _end.
		*/
		template <typename T> static T lerp(const T& _start, const T& _end, const Real& _s);

		/** Returns random permutation of the given input set.
			@par
				Permutation is done in-place and this algorithm is implementation
				of one described on http://www.algoblog.com/2007/06/04/permutation/.
			@param
				_set input set of type T.
			@param
				_start index to start permutation on.
			@param
				_count number of elements to 'shuffle'.
		*/
		template <typename T> static void permutation(T* _set, uint _start, uint _count);

		/** Cosine interpolation of two values in 1D-space.
			@param
				_start start value.
			@param
				_end end value.
			@param
				_s coefficient showing at which point between bounding values
				the result is. If it is 0.0 then return value equals _start,
				whereas when it is 1.0 then it equals _end.
		*/
		template <typename T> static T coserp(const T& _start, const T& _end, const Real& _s);


		/// Swaps two values.
		template <typename T> static void swap(T& _val1, T& _val2);
	};



	inline int Maths::floatCompare(Real _value1, Real _value2)
	{
		float val = _value1 - _value2;
		if(fabs(val) <= FloatEpsilon)
			return 0;
		else
			return (val > 0.0f ? 1 : -1);
	}
//----------------------------------------------------------------------
	inline Real Maths::degToRad(Real _angle)
	{
		return (_angle * m_kDegToRad);
	}
//----------------------------------------------------------------------
	inline Real Maths::radToDeg(Real _angle)
	{
		return (_angle * m_kRadToDeg);
	}
//----------------------------------------------------------------------
	inline Real Maths::fsin(Real _angle)
	{
		if(m_bUseTables)
			return (s_AngleUnit == ANGLE_DEGREE ? FastFloat::fsin(degToRad(_angle)) : FastFloat::fsin(_angle));
		else
			return (s_AngleUnit == ANGLE_DEGREE ? sinf(degToRad(_angle)) : sinf(_angle));
	}
//----------------------------------------------------------------------
	inline Real Maths::fcos(Real _angle)
	{
		if(m_bUseTables)
			return (s_AngleUnit == ANGLE_DEGREE ? FastFloat::fcos(degToRad(_angle)) : FastFloat::fcos(_angle));
		else
			return (s_AngleUnit == ANGLE_DEGREE ? cosf(degToRad(_angle)) : cosf(_angle));
	}
//----------------------------------------------------------------------
	inline Real Maths::ftan(Real _angle)
	{
		return (s_AngleUnit == ANGLE_DEGREE ? tanf(degToRad(_angle)) : tanf(_angle));
	}
//----------------------------------------------------------------------
	inline Real Maths::fatan(Real _angle)
	{
		return (s_AngleUnit == ANGLE_DEGREE ? atanf(degToRad(_angle)) : atanf(_angle));
	}
//----------------------------------------------------------------------
	inline Real Maths::fsqrt(Real _num)
	{
		if(_num < 0.0f)
			return numeric_limits <Real>::quiet_NaN();

		__asm
		{
			fld		_num		// s0 = num
			fsqrt				// s0 = sqrtf(_num)
		}
	}
//----------------------------------------------------------------------
	inline Real Maths::perlinNoiseGenerator(int _x, int _y, int _random)
	{
		float result;

		int n = _x + (_y * 57) + (_random * 131);
		n = (n << 13) ^ n;

		result = (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) * 0.000000000931322574615478515625f);

		return result;
	}
//----------------------------------------------------------------------
	template <typename T>
	inline void Maths::clamp(T& _value, const T& _down, const T& _up)
	{
		if(_up <= _down)
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Illegal boundaries. Upper limit has to be greater.");
			return;
		}

		// Clamp number
		if(_value > _up)
			_value = _up;
		else if(_value < _down)
			_value = _down;
	}
//----------------------------------------------------------------------
	template <typename T>
	inline void Maths::clamp_roll(T& _value, const T& _down, const T& _up)
	{
		if(_up <= _down)
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Illegal boundaries. Upper limit has to be greater.");
			return;
		}

		if(_value > _up)
			_value = _down;
		else if(_value < _down)
			_value = _up;
	}
//----------------------------------------------------------------------
	template <typename T>
	inline void Maths::roll(T& _value, const T& _down, const T& _up)
	{
		if(_up <= _down)
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Illegal boundaries. Upper limit has to be greater.");
			return;
		}

		if(_value > _up)
		{
			uint full = (int)floor((float)_value / (_up - _down + 1));
			_value -= full * (_up - _down + 1);
		}
		else if(_value < _down)
		{
			int full = -floor((float)_value / (_up - _down + 1));
			_value += full * (_up - _down + 1);
		}
	}
//----------------------------------------------------------------------
	template <typename T>
	inline T Maths::abs(const T& _value)
	{
		return (_value > 0 ? _value : -_value);
	}
//----------------------------------------------------------------------
	template <>
	inline int Maths::abs(const int& _value)
	{
		int const mask = _value >> (sizeof(int) * CHAR_BIT - 1);
		return ((_value + mask) ^ mask);
	}
//----------------------------------------------------------------------
	inline bool Maths::isPowerOfTwo(uint _value)
	{
		return !(_value & (_value - 1)) && _value;
	}
//----------------------------------------------------------------------
	inline uint Maths::roundUpToPowerOfTwo(uint _value)
	{
		uint result = _value;

		result--;
		result |= result >> 1;
		result |= result >> 2;
		result |= result >> 4;
		result |= result >> 8;
		result |= result >> 16;

		return (++result);
	}
//----------------------------------------------------------------------
	template <typename T>
	inline T Maths::lerp(const T& _start, const T& _end, const Real& _s)
	{
		return (_start + _s * (_end - _start));
	}
//----------------------------------------------------------------------
	template <typename T>
	inline void Maths::permutation(T* _set, uint _start, uint _count)
	{
		for(uint i = _start + 1; i < _count; ++i)
		{
			uint j = rand() % _count;
			Maths::swap <T> (_set[i], _set[j]);
		}
	}
//----------------------------------------------------------------------
	template <typename T>
	inline T Maths::coserp(const T& _start, const T& _end, const Real& _s)
	{
		Real temp = _s * Maths::PI;
		Real s = (1.0f - Maths::fcos(temp)) * 0.5f;

		return (_start + s * (_end - _start));
	}
//----------------------------------------------------------------------
	template <typename T>
	inline void Maths::swap(T& _val1, T& _val2)
	{
		T temp = _val1;

		_val1 = _val2;
		_val2 = temp;
	}
//----------------------------------------------------------------------
}


#endif