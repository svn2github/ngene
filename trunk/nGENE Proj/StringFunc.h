/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		StringFunc.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_STRINGFUNC_H_
#define __INC_STRINGFUNC_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** This class aggregates some of the useful string operations.
	*/
	class nGENEDLL StringFunc
	{
	public:
		/** Trims the provided function from leading and trailing whitespace
			character.
			@param
				_src source string to trim.
			@param
				_whitespaces list of characters considered whitespace.
		*/
		static wstring trim(const wstring& _src, const wstring& _whitespaces=L" \r\n");
		
		/** Trims the provided function from leading and trailing whitespace
			character.
			@param
				_src source string to trim.
			@param
				_whitespaces list of characters considered whitespace.
		*/
		static string trim(const string& _src, const string& _whitespaces=" \r\n");
	};



//----------------------------------------------------------------------
	inline string StringFunc::trim(const string& _src, const string& _whitespaces)
	{
		uint p2 = _src.find_last_not_of(_whitespaces);
		if(p2 == string::npos)
			return string();
		uint p1 = _src.find_first_not_of(_whitespaces);
		if(p1 == string::npos)
			p1 = 0;
		
		return _src.substr(p1, (p2 - p1) + 1);
	}
//----------------------------------------------------------------------
	inline wstring StringFunc::trim(const wstring& _src, const wstring& _whitespaces)
	{
		uint p2 = _src.find_last_not_of(_whitespaces);
		if(p2 == wstring::npos)
			return wstring();
		uint p1 = _src.find_first_not_of(_whitespaces);
		if(p1 == wstring::npos)
			p1 = 0;

		return _src.substr(p1, (p2 - p1) + 1);
	}
//----------------------------------------------------------------------
}


#endif