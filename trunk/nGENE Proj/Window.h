/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Window.h
Version:	0.12
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_WINDOW_H_
#define __INC_WINDOW_H_


#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/// Window descriptor class.
	typedef struct nGENEDLL SWindowDesc
	{
		uint width;					///< Width of the client area
		uint height;				///< Height of the client area
		wstring name;				///< Caption of the window
	} WINDOW_DESC;


	/** Interface representing application window.
	*/
	class nGENEDLL Window
	{
		EXPOSE_TYPE
	private:
		// Hide assignment operator and copy constructor to prevent
		// it from being used.
		Window(const Window& src);
		Window& operator=(const Window& rhs);

	protected:
		/// Descriptor of the window
		WINDOW_DESC m_WndDesc;

	public:
		Window();
		Window(const WINDOW_DESC& _desc);
		virtual ~Window();

		/// Creates new window.
		virtual void createWindow()=0;

		/** Shows/hides window.
			@param
				value when true window is showed, otherwise it is
				hidden.
		*/
		virtual void showWindow(bool _value)=0;
		/// Closes the window.
		virtual void closeWindow()=0;

		/// Returns the name of the window
		virtual const wstring& getWindowName() const {return m_WndDesc.name;}

		/// Sets window caption.
		virtual void setWindowCaption(const wstring& _caption)=0;

		/// How many windows are being used throughout the application
		static dword s_nWindowsNum;
	};
}


#endif