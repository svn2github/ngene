/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Logger.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TOOLSETLOGGER_H_
#define __INC_TOOLSETLOGGER_H_


#include "FrameworkWin32.h"
#include "nGENE.h"

#include <vcclr.h>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using System::String;


	ref class frmOutput;
	ref class ToolsetLogManaged;

	/// Class deriving from Unmanaged one.
	class ToolsetLog: public LogListener
	{
	private:
		gcroot <ToolsetLogManaged^> m_pManaged;
		
	public:
		ToolsetLog(ToolsetLogManaged^ _managed)
		{
			m_pManaged = _managed;
		}
		~ToolsetLog() {}

		void log(const LogEvent& _evt);
	};

	public delegate void LogEventHandler(String^ str, int type);

	/// Managed logger class.
	public ref class ToolsetLogManaged
	{
	public:
		ToolsetLogManaged()
		{
			m_pEventHandler = nullptr;
			m_pFeedback = new ToolsetLog(this);
		}

		~ToolsetLogManaged()
		{
			delete m_pFeedback;
		}


		event LogEventHandler^ log
		{
		public:
		  void add( LogEventHandler^ d )
		  {
			  m_pEventHandler += d;
		  }

		protected:
		  void remove( LogEventHandler^ d )
		  {
			 m_pEventHandler -= d;
		  }

		public:
		  void raise(String^ str, int type)
		  {
			 if(m_pEventHandler != nullptr)
				 m_pEventHandler(str, type);
		  }

		};
	
		private:
			ToolsetLog* m_pFeedback;
			
			LogEventHandler^ m_pEventHandler;
	};
}


#endif