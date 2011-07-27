/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileManager.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FILEMANAGER_H_
#define __INC_FILEMANAGER_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "Singleton.h"


namespace nGENE
{
	/// In which mode open a file
	enum OPEN_MODE
	{
		OPEN_UNKNOWN = (1<<0),
		OPEN_READ	 = (1<<1),				///< Open for reading
		OPEN_WRITE   = (1<<2),				///< Open for writing
		OPEN_BINARY	 = (1<<3)				///< Open as a binary
	};

	/// File seeking direction
	enum SEEK_DIRECTION
	{
		SD_UNKNOWN,
		SD_CURRENT,							///< From current position
		SD_BEGIN,							///< From the beginning of the file
		SD_END								///< From the end of the file
	};

	/// Type of the file
	enum FILE_TYPE
	{
		FT_NORMAL,							///< Contains narrow characters
		FT_UNICODE							///< Supports Unicode wide-characters
	};

	/** Manages all the files opened by the engine.
		@par
			If possible you should open all files via this
			manager as it keeps track of them.
		@par
			Also archives are opened through this class. If you want then
			to access files from them using FileManager::openFile() method
			you have to set m_bReadFromArchives flag to true using setReadFromArchives()
			method (true by default).
	*/
	class nGENEDLL FileManager: public Singleton <FileManager>
	{
	private:
		/// Maximum number of files opened simultaneously
		uint m_nMaxFilesNum;
		
		/// HashTable holding all currently opened files
		HashTable <wstring, IFile*> m_pFiles;

		/// HashTable holding all currently opened archives
		HashTable <wstring, FileArchive*> m_pArchives;

		/// Enumerator for file archives.
		FileArchiveEnumerator* m_pFileArchiveEnumerator;

		/** Specifies if openFile() method should support archives or only OS
			file system.
		*/
		bool m_bReadFromArchives;

		/// Use this if is inside archive
		string m_stArchiveDir;

	public:
		/** Constructs the FileManager.
			@param
				_maxFiles maximum number of files that can be opened
				at the same time. If you reach this limit, in order
				to open new files you have to close some of
				already opened ones.
		*/
		FileManager(uint _maxFiles=20);
		virtual ~FileManager();

		/** Opens new file.
			@remarks
				File opened using this method has to be closed using
				FileManager::closeFile() method.
		*/
		IFile* openFile(const wstring& _fileName, dword _openMode, FILE_TYPE _type=FT_UNICODE);

		/** Opens new archive file.
			@remarks
				File opened using this method has to be closed using
				FileManager::closeFile() method.
		*/
		FileArchive* openArchive(const wstring& _type, const wstring& _fileName, dword _openMode);

		/// Closes the file.
		void closeFile(const wstring& _fileName);

		/** Close all files.
			@remarks
				This function is called automatically on FileManager
				destruction.
			@return
				Returns the pointer to the just opened file.
		*/
		void closeAll();

		/** Changes current working directory.
			@param
				_dir path to the directory to be opened.
			@remarks
				If the directory specified doesn't exist or you
				have no privileges to access it this method will do
				nothing but report an error.
		*/
		void changeDir(const string& _dir);
		/// Returns current directory.
		string getCurrentDir();

		/** Specifies if openFile() method should support archives or only OS
			file system.
		*/
		void setReadFromArchives(bool _value);
		/// Checks if openFile() method supports archives.
		bool isReadingFromArchives() const;
	};



	inline void FileManager::setReadFromArchives(bool _value)
	{
		m_bReadFromArchives = _value;
	}
//----------------------------------------------------------------------
	inline bool FileManager::isReadingFromArchives() const
	{
		return m_bReadFromArchives;
	}
//----------------------------------------------------------------------
}


#endif
