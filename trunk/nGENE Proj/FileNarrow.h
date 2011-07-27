/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileNarrow.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FILENARROW_H_
#define __INC_FILENARROW_H_


#include "FileManager.h"
#include "IFile.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** File class giving opportunity to use files in a
		higher-level way.
		@remarks
			This file class version is to be used with narrow
			character sets. Despite broad wide-character
			support in the nGENE, sometimes it is more convenient
			and yet faster to use narrow characters. The biggest
			issues are with binary mode, in which wfstream due
			to some problems of VC++ might go crazy.
	*/
	class nGENEDLL FileNarrow: public IFile
	{
	protected:
		/// Wide stream
		fstream* m_pFile;
		/// File name
		wstring m_stFileName;
		/// The mode in which the file is opened
		dword m_dwOpenMode;


		ios::seekdir getSeekDir(SEEK_DIRECTION _direction) const;

	public:
		FileNarrow();
		/** Constructs the File and opens it.
			@param
				_fileName wide char file name.
			@param
				_openMode combination of flags of OPEN_MODE type. You
				can combine any of them.
		*/
		FileNarrow(const wstring& _fileName, dword _openMode);
		/// Copy constructor.
		FileNarrow(const FileNarrow& src);
		/// Closes and than releases wfstream.
		virtual ~FileNarrow();

		/// Assignment operator.
		FileNarrow& operator=(const FileNarrow& rhs);

		/** Opens the File.
			@param
				_fileName wide char file name.
			@param
				_openMode combination of flags of OPEN_MODE type. You
				can combine any of them.
		*/
		void open(const wstring& _fileName, dword _openMode);
		/** Closes the file.
			@remarks
				This function is automatically called on object's destruction.
		*/
		virtual void close();

		/** Puts C-formatted string of characters to the file.
			@remarks
				When you use "%s" format identifier make sure you
				pass a char* (not string!) variable. Otherwise
				the data written will be (null).
		*/
		void write(const char* _data, ...);
		void write(const wchar_t* _data, ...) {}

		/** Reads binary data from file.
			@remarks
				Reads _bytes bytes and returns them as a char pointer.
			@returns
				uint number of bytes read. 
		*/
		uint read(char* _buffer, uint _bytes);

		/// Puts block of data into file.
		void put(const char* _data, uint _bytes);

		/// Gets the name of the file.
		const wstring& getFileName() const {return m_stFileName;}
		/// Gets open mode of the file.
		dword getFileOpenMode() const {return m_dwOpenMode;}

		/// Reads one line of the file.
		void getLine(char* _line) const;
		/// Gets size of the file in bytes.
		uint getSize() const;

		/// Returns the whole file as a buffer of chars.
		wstring getBuffer() const;

		/// Checks whether End-Of-File has been reached.
		bool eof() const;
		/// Checks whether the file is valid (no error conditions occurred).
		bool isValid() const;
		/// Checks whether the file is opened.
		bool isOpened() const;

		/// Sets the put/get pointer position.
		void seek(int _pos, SEEK_DIRECTION _direction);
		/// Tells the put/get pointer position.
		int tell();

		/// Returns single line from a file.
		wstring getLine() const;
	};
}


#endif