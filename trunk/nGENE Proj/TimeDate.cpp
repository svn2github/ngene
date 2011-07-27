/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TimeDate.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "TimeDate.h"
#include "FuncSplit.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo TimeDate::Type(L"TimeDate", NULL);


	TimeDate::TimeDate():
		seconds(0),
		minutes(0),
		hours(0),
		month(1),
		year(1),
		week_day(1),
		month_day(1),
		year_day(1)
	{
	}
//----------------------------------------------------------------------
	TimeDate::TimeDate(const tm& _time):
		seconds(_time.tm_sec),
		minutes(_time.tm_min),
		hours(_time.tm_hour),
		month(_time.tm_mon + 1),
		year(_time.tm_year + 1900),
		week_day(_time.tm_wday + 1),
		month_day(_time.tm_mday),
		year_day(_time.tm_yday + 1)
	{
	}
//----------------------------------------------------------------------
	TimeDate::~TimeDate()
	{
	}
//----------------------------------------------------------------------
	void TimeDate::reset()
	{
		seconds = 0;
		minutes = 0;
		hours = 0;
		month = 1;
		year = 1;
		week_day = 1;
		month_day = 1;
		year_day = 1;
	}
//----------------------------------------------------------------------
	tm TimeDate::getCurrentTime()
	{
		time_t rawTime;
		time(&rawTime);

		tm localTime;
		localTime = *localtime(&rawTime);

		return localTime;
	}
//----------------------------------------------------------------------
	TimeDate TimeDate::now()
	{
		TimeDate time = getCurrentTime();
		return time;
	}
//----------------------------------------------------------------------
	TimeDate TimeDate::today()
	{
		TimeDate time = now();
		time.seconds = time.minutes = time.hours = 0;

		return time;
	}
//----------------------------------------------------------------------
	TimeDate TimeDate::from_now(const TimeDate& _time)
	{
		TimeDate time = now();
		time += _time;

		return time;
	}
//----------------------------------------------------------------------
	TimeDate TimeDate::ago(const TimeDate& _time)
	{
		TimeDate time = now();
		time -= _time;

		return time;
	}
//----------------------------------------------------------------------
	TimeDate TimeDate::since(const TimeDate& _time)
	{
		TimeDate time = now();
		time -= _time;

		return time;
	}
//----------------------------------------------------------------------
	TimeDate& TimeDate::operator+=(const TimeDate &rhs)
	{
		seconds += rhs.seconds;
		if(seconds >= 60)
		{
			uint full = floor((float)seconds / 60);
			minutes += full;
			seconds -= full * 60;
		}

		minutes += rhs.minutes;
		if(minutes >= 60)
		{
			uint full = floor((float)minutes / 60);
			hours += full;
			minutes -= full * 60;
		}

		hours += rhs.hours;
		if(hours >= 24)
		{
			uint full = floor((float)hours / 24);
			hours -= full * 24;

			Maths::clamp_roll <uint>(week_day += full, 1, 7);
			month_day += full;
			year_day += full;
		}

		if(month_day > 31)
		{
			uint full = floor((float)month_day / 31);
			month += full;
			month_day -= full * 31;
		}
		
		month += rhs.month;
		uint year_change = 0;
		if(month > 12)
		{
			uint full = floor((float)month / 12);
			year_change = full;
			month -= full * 12;
		}
		
		if(year_day > 365)
		{
			uint full = floor((float)year_day / 365);
			if(full > year_change)
				year_change = full;
			year_day -= full * 365;
		}
		
		year += (rhs.year + year_change);

		return (*this);
	}
//----------------------------------------------------------------------
	TimeDate& TimeDate::operator-=(const TimeDate &rhs)
	{
		int tseconds = (int)seconds - rhs.seconds;
		int tminutes = (int)minutes - rhs.minutes;
		int thours = (int)hours - rhs.hours;
		int tmonth = (int)month - rhs.month;
		int tyear = (int)year - rhs.year;
		int tmonth_day = month_day;
		int tyear_day = year_day;


		if(tseconds < 0)
		{
			uint full = ceil((float)Maths::abs <int>(tseconds) / 60);
			seconds = full * 60 - Maths::abs <int>(tseconds);
			tminutes -= full;
		}
		else
			seconds = (uint)tseconds;

		if(tminutes < 0)
		{
			uint full = ceil((float)Maths::abs <int>(tminutes) / 60);
			minutes = full * 60 - Maths::abs <int>(tminutes);
			thours -= full;
		}
		else
			minutes = (uint)tminutes;

		if(thours < 0)
		{
			uint full = ceil((float)Maths::abs <int>(thours) / 24);
			hours = full * 24 - Maths::abs <int>(thours);

			int tweek_day = (int)week_day - full;
			Maths::clamp_roll <int>(tweek_day, 1, 7);
			week_day = tweek_day;
			tmonth_day -= full;
			tyear_day -= full;
		}
		else
			hours = (uint)thours;

		if(tmonth_day < 1)
		{
			uint full = ceil((float)Maths::abs <int>(tmonth_day) / 31);
			month_day = full * 31 - Maths::abs <int>(tmonth_day);
			if(!month_day)
			{
				month_day = 31;
				full = 1;
			}
			tmonth -= full;
		}
		else
			month_day = (uint)tmonth_day;

		uint year_change = 0;
		if(tmonth < 1)
		{
			uint full = ceil((float)Maths::abs <int>(tmonth) / 12);
			month = full * 12 - Maths::abs <int>(tmonth);
			if(!month)
			{
				month = 12;
				full = 1;
			}
			year_change = full;
		}
		else
			month = (uint)tmonth;

		if(tyear_day < 1)
		{
			uint full = ceil((float)Maths::abs <int>(tyear_day) / 365);
			year_day = full * 31 - Maths::abs <int>(tyear_day);
			if(!year_day)
			{
				year_day = 365;
				full = 1;
			}
			if(full > year_change)
				year_change = full;
		}
		else
			year_day = (uint)tyear_day;

		tyear -= (rhs.year + year_change);
		if(tyear < 1)
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Invalid date. Year is less than 1");
			reset();
		}
		else
			year = (uint)tyear;


		return (*this);
	}
//----------------------------------------------------------------------
	TimeDate::operator string() const
	{
		ostringstream buffer;
		buffer << setw(2) << setfill('0') << hours << ":" <<
				  setw(2) << setfill('0') << minutes << ":" <<
				  setw(2) << setfill('0') << seconds << " " <<
				  setw(2) << setfill('0') << month_day << "-" <<
				  setw(2) << setfill('0') << month << "-" <<
				  setw(4) << setfill('0') << year;

		return buffer.str();
	}
//----------------------------------------------------------------------
	TimeDate::operator wstring() const
	{
		wstringstream buffer;
		buffer << setw(2) << setfill(L'0') << hours << ":" <<
				  setw(2) << setfill(L'0') << minutes << ":" <<
				  setw(2) << setfill(L'0') << seconds << " " <<
				  setw(2) << setfill(L'0') << month_day << "-" <<
				  setw(2) << setfill(L'0') << month << "-" <<
				  setw(4) << setfill(L'0') << year;

		return buffer.str();
	}
//----------------------------------------------------------------------
	void TimeDate::fromString(const string& _string)
	{
		vector <string> vecValues;
		FuncSplit <string> tokenizer;
		tokenizer(_string, vecValues, " ,;");

		if(vecValues.size() == 6)
		{
			vector <string>::iterator iter = vecValues.begin();
			hours = atoi((iter++)->c_str());
			minutes = atoi((iter++)->c_str());
			seconds = atoi((iter++)->c_str());
			month_day = atoi((iter++)->c_str());
			month = atoi((iter++)->c_str());
			year = atoi((iter++)->c_str());
		}
	}
//----------------------------------------------------------------------
	string TimeDate::toString()
	{
		stringstream buffer;
		buffer << hours << " " << minutes << " " <<
				  seconds << " " << month_day << " " <<
				  month << " " << year;


		return buffer.str();
	}
//----------------------------------------------------------------------
}