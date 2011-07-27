/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileArchive.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FileArchive.h"


namespace nGENE
{
	FileArchive::FileArchive()
	{
	}
//----------------------------------------------------------------------
	FileArchive::FileArchive(const wstring& _fileName, dword _openMode):
		FileNarrow(_fileName, _openMode)
	{
	}
//----------------------------------------------------------------------
	FileArchive::FileArchive(const FileArchive& src):
		FileNarrow(src)
	{
	}
//----------------------------------------------------------------------
	FileArchive::~FileArchive()
	{
	}
//----------------------------------------------------------------------
	FileArchive& FileArchive::operator=(const FileArchive& rhs)
	{
		if(this == &rhs)
			return (*this);

		FileNarrow::operator=(rhs);
		
		return (*this);
	}
//----------------------------------------------------------------------
}