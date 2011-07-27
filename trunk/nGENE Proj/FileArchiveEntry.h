/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileArchiveEntry.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FILEARCHIVEENTRY_H_
#define __INC_FILEARCHIVEENTRY_H_


#include "IFile.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Class describing a single entry in the archive.
	*/
	typedef class nGENEDLL FileArchiveEntry: public IFile
	{
	protected:
		char* m_pData;					///< Pointer to entry data
		char* m_pPointer;

		uint m_nLength;					///< Size of the data in bytes
		uint m_nPosition;				///< Current position inside file in bytes

		wstring m_stFileName;			///< File name in the archive


		virtual ios::seekdir getSeekDir(SEEK_DIRECTION _direction) const;

	public:
		FileArchiveEntry();
		FileArchiveEntry(void* _data, uint _length);
		FileArchiveEntry(const FileArchiveEntry& src);
		FileArchiveEntry& operator=(const FileArchiveEntry& rhs);
		~FileArchiveEntry();

		virtual void open(const wstring& _fileName, dword _openMode);
		virtual void close();

		virtual void write(const char* _data, ...);
		virtual void write(const wchar_t* _data, ...);

		virtual uint read(char* _buffer, uint _bytes);
		virtual void put(const char* _data, uint _bytes);

		virtual void setFileName(const string& _name);
		virtual const wstring& getFileName() const;
		virtual dword getFileOpenMode() const;

		virtual uint getSize() const;
		virtual wstring getBuffer() const;

		char* getData();

		bool eof() const;
		bool isValid() const;
		bool isOpened() const;

		virtual void seek(int _pos, SEEK_DIRECTION _direction);

		int tell();

		wstring getLine() const;
	} VFS_ENTRY;



	inline int FileArchiveEntry::tell()
	{
		return m_nPosition;
	}
//----------------------------------------------------------------------
	inline bool FileArchiveEntry::eof() const
	{
		if(m_nPosition >= m_nLength)
			return true;
		else
			return false;
	}
//----------------------------------------------------------------------
	inline bool FileArchiveEntry::isValid() const
	{
		return true;
	}
//----------------------------------------------------------------------
	inline bool FileArchiveEntry::isOpened() const
	{
		return true;
	}
//----------------------------------------------------------------------
	inline uint FileArchiveEntry::getSize() const
	{
		return m_nLength;
	}
//----------------------------------------------------------------------
	inline dword FileArchiveEntry::getFileOpenMode() const
	{
		return (OPEN_WRITE | OPEN_READ | OPEN_BINARY);
	}
//----------------------------------------------------------------------
	inline ios::seekdir FileArchiveEntry::getSeekDir(SEEK_DIRECTION _direction) const
	{
		return ios::end;
	}
//----------------------------------------------------------------------
	inline char* FileArchiveEntry::getData()
	{
		return m_pData;
	}
//----------------------------------------------------------------------
}


#endif