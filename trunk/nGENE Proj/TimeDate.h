/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TimeDate.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TIMEDATE_H_
#define __INC_TIMEDATE_H_


#include "Prerequisities.h"
#include "TypeInfo.h"

#include <iomanip>


namespace nGENE
{
	/** It is a helper class.
		@remarks
			Using this class you can easily obtain time/date now,
			in the past and in the future. In the concept it is
			class providing similar functionality to helper
			classes from Ruby on Rails -just adapted to C++.
	*/
	class nGENEDLL TimeDate
	{
		EXPOSE_TYPE
	private:
		tm getCurrentTime();

	public:
		uint seconds;
		uint minutes;
		uint hours;
		uint month;
		uint year;
		uint week_day;
		uint month_day;
		uint year_day;

	public:
		TimeDate();
		TimeDate(const tm& _time);
		virtual ~TimeDate();

		void reset();

		TimeDate now();
		TimeDate today();
		TimeDate from_now(const TimeDate& _time);
		TimeDate ago(const TimeDate& _time);
		TimeDate since(const TimeDate& _time);

		// Underneath are overloaded operators
		TimeDate& operator+=(const TimeDate& rhs);
		TimeDate& operator-=(const TimeDate& rhs);

		friend const TimeDate operator+(const TimeDate& lhs, const TimeDate& rhs);
		friend const TimeDate operator-(const TimeDate& lhs, const TimeDate& rhs);

		friend ostream& operator<<(ostream& ostr, const TimeDate& _time);
		friend wostream& operator<<(wostream& ostr, const TimeDate& _time);

		operator string() const;
		operator wstring() const;

		/// Creates TimeDate from string.
		void fromString(const string& _string);
		/// Writes TimeDate to string as "x y".
		string toString();
	};


	typedef TimeDate Time, Date;



	inline const TimeDate operator+(const TimeDate& lhs, const TimeDate& rhs)
	{
		TimeDate result = lhs;
		result += rhs;

		return result;
	}
//----------------------------------------------------------------------
	inline const TimeDate operator-(const TimeDate& lhs, const TimeDate& rhs)
	{
		TimeDate result = lhs;
		result -= rhs;

		return result;
	}
//----------------------------------------------------------------------
	inline ostream& operator<<(ostream& ostr, const TimeDate& _time)
	{
		ostr << setw(2) << setfill('0') << _time.hours << ":" <<
				setw(2) << setfill('0') << _time.minutes << ":" <<
				setw(2) << setfill('0') << _time.seconds << " " <<
				setw(2) << setfill('0') << _time.month_day << "-" <<
				setw(2) << setfill('0') << _time.month << "-" <<
				setw(4) << setfill('0') << _time.year;

		return (ostr);
	}
//----------------------------------------------------------------------
	inline wostream& operator<<(wostream& ostr, const TimeDate& _time)
	{
		ostr << setw(2) << setfill(L'0') << _time.hours << ":" <<
				setw(2) << setfill(L'0') << _time.minutes << ":" <<
				setw(2) << setfill(L'0') << _time.seconds << " " <<
				setw(2) << setfill(L'0') << _time.month_day << "-" <<
				setw(2) << setfill(L'0') << _time.month << "-" <<
				setw(4) << setfill(L'0') << _time.year;

		return (ostr);
	}
//----------------------------------------------------------------------
}


#endif