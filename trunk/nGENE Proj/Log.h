/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Log.h
Version:	0.25
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_LOG_H_
#define __INC_LOG_H_


#include "Prerequisities.h"


namespace nGENE
{
	/// Logged message type
	enum LOG_EVENT_TYPE
	{
		LET_UNKNOWN,
		LET_DEBUG,						///< Debug information
		LET_GAME_MESSAGE,				///< Regular game message
		LET_COMMENT,					///< A comment
		LET_EVENT,						///< Event
		LET_WARNING,					///< Warning - pay attention to it
		LET_ERROR,						///< Error - something went wrong
		LET_FATAL_ERROR 				/**< Fatal error - application
											 will probably crash */
	};

	/// Logged message
	struct LogEvent
	{
		LOG_EVENT_TYPE type;			///< Type of the message
		wstring stNamespace;			///< Namespace where message occurred
		wstring stSourceFile;			///< Source file in which message occurred
		wstring stFunction;				///< Function in which message occured
		wstring stMessage;				///< Message description
		uint nLine;						///< Line at which message occured
		uint nTime;						/**< Time from engine start at which
											 message occurred */
	};

	/** Log for all important events arising during application lifetime.
		@remarks
			Log could be presented in several different ways to the developer/
			tester: in console window or in file (either xml or plain text).
		@remarks
			The more events you add to a log, the better for your application
			and yourself as you give yourself opportunity of finding the inevitable
			bugs more easily.
	*/
	class nGENEDLL Log
	{
	protected:
		/// Output stream to store log data if necessary (txt mode)
		IFile* m_pLogTxtFile;
		

		/// Is log enabled?
		static bool s_bIsEnabled;

	public:
		/** Adds a new entry to the log.
			@remarks
				Note that entry actually will be put to the log
				either on ring buffer filling or at the
				application's exit.
			@param
				_type type of a message being added.
			@param
				_space namespace where message occurred.
			@param
				_source source file where message occurred.
			@param
				_function function where message occurred.
			@param
				_line line of code at which message occurred.
			@param
				_message a C-style formatted string containing message description.
		*/
		static void log(LOG_EVENT_TYPE _type, const wstring& _space,
						const wstring& _source, const wstring& _function,
						int _line, const wchar_t* _message, ...);

		/** Lets you control the working status of the log.
			@param
				value if is set to true - log is enabled. If it is false
				log is disabled.
		*/
		static void setEnabled(bool _value) {s_bIsEnabled=_value;}
		/// Checks if Log is enabled.
		static bool isEnabled() {return s_bIsEnabled;}
	};
}


#endif
