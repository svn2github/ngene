/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileNarrow.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FileNarrow.h"


namespace nGENE
{
	FileNarrow::FileNarrow():
		m_pFile(new fstream()),
		m_dwOpenMode(0)
	{
	}
//----------------------------------------------------------------------
	FileNarrow::FileNarrow(const wstring& _fileName, dword _openMode):
		m_pFile(new fstream())
	{
		open(_fileName, _openMode);
	}
//----------------------------------------------------------------------
	FileNarrow::FileNarrow(const FileNarrow& src):
		IFile(src),
		m_pFile(new fstream())
	{
		open(src.m_stFileName, src.m_dwOpenMode);
	}
//----------------------------------------------------------------------
	FileNarrow::~FileNarrow()
	{
		close();
		NGENE_DELETE(m_pFile);
	}
//----------------------------------------------------------------------
	FileNarrow& FileNarrow::operator=(const FileNarrow& rhs)
	{
		if(this == &rhs)
			return (*this);

		IFile::operator=(rhs);

		open(rhs.m_stFileName, rhs.m_dwOpenMode);

		return (*this);
	}
//----------------------------------------------------------------------
	void FileNarrow::open(const wstring& _fileName, dword _openMode)
	{
		m_stFileName = _fileName;
		m_dwOpenMode = _openMode;

		// Set opening flags
		ios_base::openmode mode = 0;
		if(m_dwOpenMode & OPEN_READ)
			mode |= ios::in;
		if(m_dwOpenMode & OPEN_WRITE)
			mode |= ios::out;
		if(m_dwOpenMode & OPEN_BINARY)
			mode |= ios::binary;

		// Open stream
		string fileName(m_stFileName.begin(), m_stFileName.end());
		m_pFile->open(fileName.c_str(), mode);
		if(!isOpened())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					 L"File: %ls could not be opened", m_stFileName.c_str());
			return;
		}

		m_pFile->clear();
	}
//----------------------------------------------------------------------
	void FileNarrow::close()
	{
		m_dwOpenMode = 0;

		if(!m_pFile)
			return;
		if(isOpened())
			m_pFile->close();
	}
//----------------------------------------------------------------------
	void FileNarrow::write(const char* _data, ...)
	{
		// Improper open mode
		if(!(m_dwOpenMode & OPEN_WRITE))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"File is not opened for writing!");
			return;
		}

		// File is not valid
		if(!m_pFile->is_open())
			return;

		char* tempMsg;
		int len = 0;
		va_list arg_ptr;

		va_start(arg_ptr, _data);
			len = _vscprintf(_data, arg_ptr) + 1;
			tempMsg = new char[len];
			vsprintf(tempMsg, _data, arg_ptr);
		va_end(arg_ptr);

		(*m_pFile) << tempMsg;

		NGENE_DELETE_ARRAY(tempMsg);
	}
//----------------------------------------------------------------------
	void FileNarrow::getLine(char* _line) const
	{
		// Improper open mode
		if(!(m_dwOpenMode & OPEN_READ))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"File is not opened for reading!");
			return;
		}

		m_pFile->getline(_line, 255);
	}
//----------------------------------------------------------------------
	wstring FileNarrow::getLine() const
	{
		// Improper open mode
		if(!(m_dwOpenMode & OPEN_READ))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"File is not opened for reading!");
			return L"";
		}

		char line[255];
		m_pFile->getline(line, 255);

		string stTemp = line;

		return wstring(stTemp.begin(), stTemp.end());
	}
//----------------------------------------------------------------------
	bool FileNarrow::eof() const
	{
		return m_pFile->eof();
	}
//----------------------------------------------------------------------
	bool FileNarrow::isOpened() const
	{
		return m_pFile->is_open();
	}
//----------------------------------------------------------------------
	bool FileNarrow::isValid() const
	{
		return (!m_pFile->eof() && m_pFile->good() && m_pFile->is_open());
	}
//----------------------------------------------------------------------
	uint FileNarrow::getSize() const
	{
		// If the file is not opened, exit
		if(!isOpened())
			return 0;

		int curPos = 0;
		if(m_dwOpenMode & OPEN_READ)
		{
			int oldPos = m_pFile->tellg();
			m_pFile->seekg(0, ios::end);
			curPos = m_pFile->tellg();
			m_pFile->seekg(oldPos, ios::beg);
		}
		else if(m_dwOpenMode & OPEN_WRITE)
		{
			int oldPos = m_pFile->tellp();
			m_pFile->seekp(0, ios::end);
			curPos = m_pFile->tellp();
			m_pFile->seekp(oldPos, ios::beg);
		}
		
		return curPos;
	}
//----------------------------------------------------------------------
	wstring FileNarrow::getBuffer() const
	{
		// Improper open mode
		if(!(m_dwOpenMode & OPEN_READ))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"File is not opened for reading!");
			return L"";
		}

		// Get file size in bytes
		uint size = getSize();

		// If the file is not opened, exit
		if(!size)
			return L"";

		// Read file as a null-terminated string
		char* pData = new char[size + 1];
		m_pFile->read((char*)pData, size);
		if(!(m_dwOpenMode & OPEN_BINARY))
			pData[m_pFile->gcount()] = '\0';
		else
			pData[size] = '\0';
		string temp = pData;
		wstring result(temp.begin(), temp.end());

		NGENE_DELETE_ARRAY(pData);

		m_pFile->clear();

		return result;
	}
//----------------------------------------------------------------------
	ios::seekdir FileNarrow::getSeekDir(SEEK_DIRECTION _direction) const
	{
		ios::seekdir dir = ios::beg;
		switch(_direction)
		{
		case SD_CURRENT: dir = ios::cur; break;
		case SD_BEGIN  : dir = ios::beg; break;
		case SD_END	   : dir = ios::end; break;
		}

		return dir;
	}
//----------------------------------------------------------------------
	void FileNarrow::seek(int _pos, SEEK_DIRECTION _direction)
	{
		// If the file is not opened, exit
		if(!isOpened())
			return;

		// Find iostream seek direction
		ios::seekdir dir = getSeekDir(_direction);
		if(m_dwOpenMode & OPEN_WRITE)
			m_pFile->seekp(_pos, dir);
		else if(m_dwOpenMode & OPEN_READ)
			m_pFile->seekg(_pos, dir);
	}
//----------------------------------------------------------------------
	int FileNarrow::tell()
	{
		// If the file is not opened, exit
		if(!isOpened())
			return -1;

		// Find iostream position
		if(m_dwOpenMode & OPEN_WRITE)
			return m_pFile->tellp();
		else if(m_dwOpenMode & OPEN_READ)
			return m_pFile->tellg();
		else
			return -1;
	}
//----------------------------------------------------------------------
	uint FileNarrow::read(char* _buffer, uint _bytes)
	{
		if(!isOpened() || !(m_dwOpenMode & OPEN_READ))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"File is not opened for reading!");
			return 0;
		}

		m_pFile->read(_buffer, _bytes);

		return _bytes;
	}
//----------------------------------------------------------------------
	void FileNarrow::put(const char* _data, uint _bytes)
	{
		if(!isOpened() || !(m_dwOpenMode & OPEN_WRITE))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"File is not opened for writing!");
			return;
		}

		m_pFile->write(_data, _bytes);
	}
//----------------------------------------------------------------------
}