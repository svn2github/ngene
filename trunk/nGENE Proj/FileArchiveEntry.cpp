/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileArchiveEntry.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FileArchiveEntry.h"


namespace nGENE
{
	FileArchiveEntry::FileArchiveEntry():
		m_pData(NULL),
		m_pPointer(NULL),
		m_nLength(0),
		m_nPosition(0)
	{
	}
//----------------------------------------------------------------------
	FileArchiveEntry::FileArchiveEntry(void* _data, uint _length):
		m_nLength(_length),
		m_nPosition(0)
	{	
		m_pData = new char[m_nLength];
		m_pPointer = m_pData;
		memcpy(m_pData, _data, m_nLength);
	}
//----------------------------------------------------------------------
	FileArchiveEntry::FileArchiveEntry(const FileArchiveEntry& src)
	{
		m_nLength = src.m_nLength;
		m_pData = new char[m_nLength];
		memcpy(m_pData, src.m_pData, m_nLength);

		m_pPointer = src.m_pPointer;
		m_nPosition = src.m_nPosition;
	}
//----------------------------------------------------------------------
	FileArchiveEntry& FileArchiveEntry::operator=(const FileArchiveEntry& rhs)
	{
		if(this == &rhs)
			return (*this);

		m_nLength = rhs.m_nLength;
		NGENE_DELETE_ARRAY(m_pData);
		m_pData = new char[m_nLength];
		memcpy(m_pData, rhs.m_pData, m_nLength);

		m_pPointer = rhs.m_pPointer;
		m_nPosition = rhs.m_nPosition;

		return (*this);
	}
//----------------------------------------------------------------------
	FileArchiveEntry::~FileArchiveEntry()
	{
		NGENE_DELETE_ARRAY(m_pData);
	}
//----------------------------------------------------------------------
	void FileArchiveEntry::open(const wstring& _fileName, dword _openMode)
	{
		m_pPointer = m_pData;
	}
//----------------------------------------------------------------------
	void FileArchiveEntry::close()
	{
		NGENE_DELETE_ARRAY(m_pData);
		m_nLength = 0;
		m_pPointer = NULL;
		m_nPosition = 0;
	}
//----------------------------------------------------------------------
	void FileArchiveEntry::write(const char* _data, ...)
	{
	}
//----------------------------------------------------------------------
	void FileArchiveEntry::write(const wchar_t* _data, ...)
	{
	}
//----------------------------------------------------------------------
	uint FileArchiveEntry::read(char* _buffer, uint _bytes)
	{
		memcpy(_buffer, m_pPointer, _bytes);
		m_pPointer += _bytes;
		m_nPosition += _bytes;

		return _bytes;
	}
//----------------------------------------------------------------------
	void FileArchiveEntry::put(const char* _data, uint _bytes)
	{
		if(_bytes != m_nLength)
		{
			NGENE_DELETE_ARRAY(m_pData);
			m_nLength = _bytes;
			m_pData = new char[m_nLength];
			m_pPointer = m_pData;
			memcpy(m_pPointer, _data, _bytes);
		}
		else
		{
			memcpy(m_pPointer, _data, _bytes);
			m_pPointer += _bytes;
			m_nPosition += _bytes;
		}
	}
//----------------------------------------------------------------------
	void FileArchiveEntry::setFileName(const string& _name)
	{
		m_stFileName.assign(_name.begin(), _name.end());
	}
//----------------------------------------------------------------------
	const wstring& FileArchiveEntry::getFileName() const
	{
		return m_stFileName;
	}
//----------------------------------------------------------------------
	wstring FileArchiveEntry::getBuffer() const
	{
		wchar_t* temp = new wchar_t[m_nLength + 1];
		mbstowcs(temp, (char*)m_pData, m_nLength);
		temp[m_nLength] = L'\0';
		wstring result(temp);

		NGENE_DELETE_ARRAY(temp);

		return result;
	}
//----------------------------------------------------------------------
	void FileArchiveEntry::seek(int _pos, SEEK_DIRECTION _direction)
	{
		switch(_direction)
		{
		case SD_BEGIN: m_pPointer = m_pData + m_nLength - _pos;
			m_nPosition = m_nLength - m_nPosition;
			break;
		case SD_CURRENT: m_pPointer += _pos;
			m_nPosition += _pos;
			break;
		case SD_END: m_pPointer = m_pData + _pos;
			m_nPosition = _pos;
			break;
		}
	}
//----------------------------------------------------------------------
	wstring FileArchiveEntry::getLine() const
	{
		return L"";
	}
//----------------------------------------------------------------------
}