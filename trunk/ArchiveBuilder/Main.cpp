/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Main.cpp
Version:	0.02
Info:		Simple tool for building nGENE archive files.
---------------------------------------------------------------------------
*/

#define NOMINMAX
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

#include "nGENE.h"
using namespace nGENE;

DefaultFileArchive* pArchive;
FileManager manager;
uint nCount;

void traverse(STRSAFE_LPCSTR dir)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];

	_tprintf(TEXT("<VISITING DIR>   %s\n"), dir);

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, dir);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if(hFind == INVALID_HANDLE_VALUE)
		return;

	// List all the files in the directory with some info about them.

	do
	{
		if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(strcmp(ffd.cFileName, ".") && strcmp(ffd.cFileName, ".."))
			{
				TCHAR szDir[MAX_PATH];

				StringCchCopy(szDir, MAX_PATH, dir);
				StringCchCat(szDir, MAX_PATH, "/");
				StringCchCat(szDir, MAX_PATH, ffd.cFileName);
				traverse(szDir);
			}
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;

			char szDir[MAX_PATH];
			char tempDir[MAX_PATH];

			StringCchCopy(tempDir, MAX_PATH, dir);
			StringCchCat(tempDir, MAX_PATH, "/");
			StringCchCat(tempDir, MAX_PATH, ffd.cFileName);
			uint i = 0;
			char* pointer = tempDir;
			char c = *pointer;
			if(c == '.')
			{
				++pointer;
				c = *pointer;
				if(c == '/' || c == '\\')
				{
					++pointer;
					i += 2;
				}
				else
					--pointer;
			}

			StringCchCopy(szDir, MAX_PATH - i, pointer);

			string temp(szDir);
			wstring stName(temp.begin(), temp.end());

			wcout << L"Adding file:" << stName << endl;
			IFile* pFile = manager.openFile(stName, OPEN_READ | OPEN_BINARY, FT_NORMAL);
			if(!pFile)
			{
				_tprintf(TEXT("\nFile could not be opened for reading.\n"));
				return;
			}
			pArchive->insertFile(pFile);
			manager.closeFile(stName);

			++nCount;
		}
	}
	while(FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if(dwError != ERROR_NO_MORE_FILES) 
		return;

	FindClose(hFind);
}

int main(int argc, char* argv[])
{
	_tprintf(TEXT("\nnGENE Archive Builder\n"));
	_tprintf(TEXT("Wojciech Toman 2009\n\n"));

	size_t length_of_arg;
	

	// If the directory is not specified as a command-line argument,
	// print usage.

	if(argc != 3 && argc != 4)
	{
		_tprintf(TEXT("\nUsage: %s <directory name> <archive name> [-c OPTIONAL compression]\n"), argv[0]);
		return (-1);
	}

	// Read compression flag
	bool bCompress = false;
	if(argc == 4)
	{
		bCompress = strcmp(argv[3], "-c") ? false : true;
	}

	string temp(argv[2]);
	wstring stName(temp.begin(), temp.end());
	pArchive = (DefaultFileArchive*)manager.openArchive(L"Default", stName.c_str(), OPEN_WRITE | OPEN_BINARY);
	if(!pArchive)
	{
		_tprintf(TEXT("\nArchive file could not be created.\n"));
		return (-1);
	}

	// Set compression flag
	if(bCompress)
		pArchive->setFlags(DFAF_COMPRESS);

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(argv[1], MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		return (-1);
	}

	_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);


	traverse(argv[1]);

	manager.closeFile(stName);

	_tprintf(TEXT("\nTotal: %d files added to the archive file\n"), nCount);
}