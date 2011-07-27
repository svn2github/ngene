/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileArchiveFactory.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FILEARCHIVEFACTORY_H_
#define __INC_FILEARCHIVEFACTORY_H_


namespace nGENE
{
	/** Base FileArchiveFactory class.
		@remarks
			Each FileArchive class should be accompanied by its own (derived)
			FileArchiveFactory class.
		@see
			FileArchive for more details on archiving files.
	*/
	class nGENEDLL FileArchiveFactory
	{
	public:
		virtual FileArchive* createFileArchive(const wstring& _fileName, dword _openMode)=0;
	};
}


#endif