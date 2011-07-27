/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IFile.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_IFILE_H_
#define __INC_IFILE_H_


#include "FileManager.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Abstract interface for file classes.
	*/
	class nGENEDLL IFile
	{
	protected:
		virtual ios::seekdir getSeekDir(SEEK_DIRECTION _direction) const=0;

	public:
		IFile();
		virtual ~IFile();

		/** Opens the File.
			@param
				_fileName wide char file name.
			@param
				_openMode combination of flags of OPEN_MODE type. You
				can combine any of them.
		*/
		virtual void open(const wstring& _fileName, dword _openMode)=0;
		/** Closes the file.
			@remarks
				This function is automatically called on object's destruction.
		*/
		virtual void close()=0;

		/** Puts C-formatted string of characters to the file.
			@remarks
				When you use "%s" format identifier make sure you
				pass a char* (not string!) variable. Otherwise
				the data written will be (null).
		*/
		virtual void write(const char* _data, ...)=0;
		/** Puts C-formatted string of characters to the file.
			@remarks
				When you use "%s" format identifier make sure you
				pass a char* (not string!) variable. Otherwise
				the data written will be (null).
		*/
		virtual void write(const wchar_t* _data, ...)=0;

		/** Reads binary data from file.
			@remarks
				Reads _bytes bytes and returns them as a char pointer.
			@returns
				uint number of bytes read. 
		*/
		virtual uint read(char* _buffer, uint _bytes)=0;

		/// Puts block of data into file.
		virtual void put(const char* _data, uint _bytes)=0;

		/// Gets the name of the file.
		virtual const wstring& getFileName() const=0;
		/// Gets open mode of the file.
		virtual dword getFileOpenMode() const=0;

		/// Gets size of the file in bytes.
		virtual uint getSize() const=0;

		/// Returns the whole file as a buffer of chars.
		virtual wstring getBuffer() const=0;

		/// Checks whether End-Of-File has been reached.
		virtual bool eof() const=0;
		/// Checks whether the file is valid (no error conditions occurred).
		virtual bool isValid() const=0;
		/// Checks whether the file is opened.
		virtual bool isOpened() const=0;

		/// Sets the put/get pointer position.
		virtual void seek(int _pos, SEEK_DIRECTION _direction)=0;
		/** Tells the put/get pointer position.
			@returns
				int current position in the file. If file is not opened
				-1 is returned.
		*/
		virtual int tell()=0;

		/// Returns single line from a file.
		virtual wstring getLine() const=0;
	};
}


#endif