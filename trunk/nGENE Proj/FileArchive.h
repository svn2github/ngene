/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileArchive.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FILEARCHIVE_H_
#define __INC_FILEARCHIVE_H_


#include "FileManager.h"
#include "FileNarrow.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** This file class provides VFS functionality.
		@remarks
			It is base class.
	*/
	class nGENEDLL FileArchive: public FileNarrow
	{
	public:
		FileArchive();
		/** Constructs the File and opens it.
			@param
				_fileName wide char file name.
			@param
				_openMode combination of flags of OPEN_MODE type. You
				can combine any of them.
		*/
		FileArchive(const wstring& _fileName, dword _openMode);
		/// Copy constructor.
		FileArchive(const FileArchive& src);
		/// Closes and than releases wfstream.
		virtual ~FileArchive();

		/// Assignment operator.
		FileArchive& operator=(const FileArchive& rhs);

		/// Returns entry with the given name.
		virtual VFS_ENTRY* getEntry(const string& _fileName)=0;
		/// Checks if a specified entry exists in the archive.
		virtual bool hasEntry(const string& _fileName)=0;

		/// Inserts file to the archive when opened in OPEN_WRITE mode.
		virtual void insertFile(IFile* _file)=0;

		/// Initializes archive file.
		virtual void init()=0;

		/// Returns number of files in the archive
		virtual uint getEntriesCount()=0;
	};
}


#endif