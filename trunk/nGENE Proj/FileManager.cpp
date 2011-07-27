/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FileManager.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "FileManager.h"
#include "FileArchive.h"
#include "FileArchiveEntry.h"
#include "FileArchiveEnumerator.h"
#include "FileNarrow.h"
#include "FuncSplit.h"

#include <direct.h>


namespace nGENE
{
	FileManager::FileManager(uint _maxFiles):
		m_nMaxFilesNum(_maxFiles),
		m_pFileArchiveEnumerator(new FileArchiveEnumerator()),
		m_bReadFromArchives(true)
	{
	}
//----------------------------------------------------------------------
	FileManager::~FileManager()
	{
		closeAll();
	}
//----------------------------------------------------------------------
	IFile* FileManager::openFile(const wstring& _fileName, dword _openMode, FILE_TYPE _type)
	{
		// If is reading from archives
		if(m_bReadFromArchives)
		{
			vector <wstring> vecPath;
			// Check if path is already set within the archive
			if(m_stArchiveDir != "")
			{
				wstring dir(m_stArchiveDir.begin(), m_stArchiveDir.end());
				vecPath.push_back(dir);
				vecPath.push_back(_fileName);
			}
			else
			{
				FuncSplit <wstring> tokenizer;
				tokenizer(_fileName, vecPath, L"\\", 2);
			}

			if(vecPath.size() > 1)
			{
				if(m_pArchives.find(vecPath[0]) != m_pArchives.end())
				{
					FileArchive* pArchive = m_pArchives[vecPath[0]];
					// If file is present in the archive, get it
					string stName(vecPath[1].begin(), vecPath[1].end());
					if(pArchive->hasEntry(stName))
						return pArchive->getEntry(stName);
				}
			}
		}

		// Check if more files could be opened
		if(m_nMaxFilesNum == m_pFiles.size() + 1)
			return NULL;

		if(m_pFiles.find(_fileName) != m_pFiles.end())
			return (m_pFiles[_fileName]);

		switch(_type)
		{
		case FT_UNICODE: m_pFiles[_fileName] = new File(_fileName, _openMode);
			break;
		case FT_NORMAL: m_pFiles[_fileName] = new FileNarrow(_fileName, _openMode);
			break;
		default:
			break;
		}

		return (m_pFiles[_fileName]);
	}
//----------------------------------------------------------------------
	FileArchive* FileManager::openArchive(const wstring& _type, const wstring& _fileName, dword _openMode)
	{
		// Check if more files could be opened
		if(m_nMaxFilesNum == m_pFiles.size() + 1)
			return NULL;

		if(m_pArchives.find(_fileName) != m_pArchives.end())
			return (m_pArchives[_fileName]);


		FileArchive* pArchive = m_pFileArchiveEnumerator->create(_type, _fileName, _openMode);
		m_pArchives[_fileName] = pArchive;
		m_pFiles[_fileName] = pArchive;

		return pArchive;
	}
//----------------------------------------------------------------------
	void FileManager::closeFile(const wstring& _fileName)
	{
		// No file found, so exit
		if(m_pFiles.find(_fileName) == m_pFiles.end())
			return;

		wstring stName = _fileName;

		// Close and remove file
		m_pFiles[stName]->close();
		HashTable <wstring, IFile*>::iterator iter = m_pFiles.find(stName);
		NGENE_DELETE(iter->second);

		m_pFiles.erase(iter);
		if(m_pArchives.find(stName) != m_pArchives.end())
			m_pArchives.erase(m_pArchives.find(stName));
	}
//----------------------------------------------------------------------
	void FileManager::closeAll()
	{
		// Close the files and then remove them
		if(!m_pFiles.empty())
		{
			HashTable <wstring, IFile*>::iterator iter;
			for(iter = m_pFiles.begin(); iter != m_pFiles.end(); ++iter)
			{
				iter->second->close();
				NGENE_DELETE(iter->second);
			}
			m_pFiles.clear();
			m_pArchives.clear();
		}
	}
//----------------------------------------------------------------------
	void FileManager::changeDir(const string& _dir)
	{
		if(chdir(_dir.c_str()))
		{
			wstring dir(_dir.begin(), _dir.end());
			if(m_pArchives.find(dir) != m_pArchives.end())
			{
				m_stArchiveDir = _dir;
			}
			else
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Changing directory failed. Make sure: %ls exists and you can access it",
					dir.c_str());
			}
		}
		else
			m_stArchiveDir = "";
	}
//----------------------------------------------------------------------
	string FileManager::getCurrentDir()
	{
		char dir[_MAX_PATH];
		getcwd(dir, _MAX_PATH);

		if(m_stArchiveDir == "")
			return string(dir);
		else
		{
			string result = dir;
			result += "\\" + m_stArchiveDir;

			return result;
		}
	}
//----------------------------------------------------------------------
}