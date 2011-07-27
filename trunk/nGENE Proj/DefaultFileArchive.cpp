/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DefaultFileArchive.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DefaultFileArchive.h"
#include "BWTEncoder.h"
#include "FileArchiveEntry.h"
#include "FileArchiveEnumerator.h"
#include "HuffmanEncoder.h"
#include "MTFEncoder.h"
#include "RLEEncoder.h"


#if _MSC_VER < 1500
	#include "omp.h"
#endif


namespace nGENE
{
	// Initialize static members
	const uint DefaultFileArchive::s_kArchiverVersion = 1;


	DefaultFileArchive::DefaultFileArchive(const wstring& _fileName, dword _openMode):
		FileArchive(_fileName, _openMode),
		m_nVersion(s_kArchiverVersion),
		m_nFlags(0),
		m_nFilesCount(0),
		m_nDataOffset(0)
	{
	}
//----------------------------------------------------------------------
	DefaultFileArchive::~DefaultFileArchive()
	{
	}
//----------------------------------------------------------------------
	VFS_ENTRY* DefaultFileArchive::getEntry(const string& _fileName)
	{
		// No file exists in the archive, so exit
		if(m_FileHeaders.find(_fileName) == m_FileHeaders.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"File does not exist in the archive");
			return NULL;
		}

		SFileHeader& header = m_FileHeaders[_fileName];

		// Read archive entry
		seek(header.offset, SD_BEGIN);
		char* data = new char[header.length];
		read((char*)data, header.length);


		VFS_ENTRY* pEntry = new VFS_ENTRY(data, header.length);
		pEntry->setFileName(_fileName);
		NGENE_DELETE_ARRAY(data);


		// Decompress data
		if((m_nFlags & DFAF_COMPRESS) && (header.flags & DFAF_COMPRESS))
		{
			// Perform inverse Huffman to really decompress data...
			HuffmanEncoder huffman;
			stringstream output1;
			huffman.decode(output1, pEntry->getData(), pEntry->getSize());

			// Perform inverse Run-Length Encoding
			RLEEncoder rle;
			stringstream output2;
			rle.decode(output2, output1.str().c_str(), output1.str().length());

			// Now perform inverse Move-to-Front
			MTFEncoder mtf;
			stringstream output3;
			mtf.decode(output3, output2.str().c_str(), output2.str().length());

			// Finally Run Burrows-Wheeler inverse transform
			BWTEncoder bwt;
			stringstream output4;
			bwt.decode(output4, output3.str().c_str(), output3.str().length());


			// Set entry data
			pEntry->put(output4.str().c_str(), output4.str().length());
		}

		return pEntry;
	}
//----------------------------------------------------------------------
	bool DefaultFileArchive::hasEntry(const string& _fileName)
	{
		return (m_FileHeaders.find(_fileName) != m_FileHeaders.end());
	}
//----------------------------------------------------------------------
	void DefaultFileArchive::init()
	{
		if(m_dwOpenMode & OPEN_READ)
		{
			// Read archive file header
			read((char*)&m_nVersion, sizeof(uint));
			read((char*)&m_nFlags, sizeof(uint));
			read((char*)&m_nFilesCount, sizeof(uint));

			// Check if version is the same as current one
			if(m_nVersion != s_kArchiverVersion)
			{
				Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Archive version: %d is different than currently supported by nGENE: %d",
					m_nVersion, s_kArchiverVersion);
			}

			// Read files' headers
			for(uint i = 0; i < m_nFilesCount; ++i)
			{
				short fileNameLength = 0;
				read((char*)&fileNameLength, sizeof(short));
				char* name = new char[fileNameLength + 1];
				read(name, fileNameLength);
				name[fileNameLength] = '\0';

				SFileHeader header;
				read((char*)&header.offset, sizeof(uint));
				read((char*)&header.length, sizeof(uint));
				read((char*)&header.flags, sizeof(uint));

				m_FileHeaders[name] = header;
			}
		}
		else
			m_nDataOffset = 3 * sizeof(uint);
	}
//----------------------------------------------------------------------
	uint DefaultFileArchive::getEntriesCount()
	{
		return m_nFilesCount;
	}
//----------------------------------------------------------------------
	void DefaultFileArchive::insertFile(IFile* _file)
	{
		SFileHeader header;
		header.length = _file->getSize();
		header.offset = 0;

		char* data = new char[header.length];
		_file->read(data, header.length);

		VFS_ENTRY entry(data, header.length);
		m_vEntries.push_back(entry);

		header.entry = m_nFilesCount;
		header.flags = 0;
		
		string stName(_file->getFileName().begin(), _file->getFileName().end());
		m_FileHeaders[stName] = header;


		m_nDataOffset += sizeof(short) + stName.length() * sizeof(char) + 3 * sizeof(uint);

		++m_nFilesCount;

		NGENE_DELETE_ARRAY(data);
	}
//----------------------------------------------------------------------
	void DefaultFileArchive::close()
	{
		if(m_dwOpenMode & OPEN_WRITE)
		{
			// Write archive file header
			put((char*)&m_nVersion, sizeof(uint));
			put((char*)&m_nFlags, sizeof(uint));
			put((char*)&m_nFilesCount, sizeof(uint));

			// If we have to compress, lets do it here
			HashTable <string, SFileHeader>::iterator iter;
			if(m_nFlags & DFAF_COMPRESS)
			{
				#pragma omp parallel for schedule( dynamic )
				for(iter = m_FileHeaders.begin(); iter != m_FileHeaders.end(); ++iter)
				{
					VFS_ENTRY& entry = m_vEntries[iter->second.entry];

					// First run Burrows-Wheeler transform on the data to make
					// it more compression friendly
					BWTEncoder bwt;
					stringstream output1;
					bwt.encode(output1, entry.getData(), entry.getSize());

					// Now perform Move-to-Front to reduce data entropy
					MTFEncoder mtf;
					stringstream output2;
					mtf.encode(output2, output1.str().c_str(), output1.str().length());

					// Perform Run-Length Encoding to remove runs of identical characters
					RLEEncoder rle;
					stringstream output3;
					rle.encode(output3, output2.str().c_str(), output2.str().length());

					// Now perform Huffman to really compress data...
					HuffmanEncoder huffman;
					stringstream output4;
					huffman.encode(output4, output3.str().c_str(), output3.str().length());


					// Now set it as the entry data if it is smaller than originally was
					if(output4.str().length() < entry.getSize())
					{
						iter->second.flags |= DFAF_COMPRESS;
						entry.put(output4.str().c_str(), output4.str().length());
					}
					else
					{
						iter->second.flags |= 0;
					}
				}
			}

			// Write files' headers
			uint nPrevSize = 0;
			for(iter = m_FileHeaders.begin(); iter != m_FileHeaders.end(); ++iter)
			{
				short fileNameLength = (short)iter->first.length();
				put((char*)&fileNameLength, sizeof(short));
				put(iter->first.c_str(), fileNameLength);

				SFileHeader& header = iter->second;
				header.offset = m_nDataOffset + nPrevSize;
				header.length = m_vEntries[iter->second.entry].getSize();
				put((char*)&header.offset, sizeof(uint));
				put((char*)&header.length, sizeof(uint));

				// Write file compression flag
				put((char*)&header.flags, sizeof(uint));

				nPrevSize += header.length;
			}

			// Write files' entries
			for(iter = m_FileHeaders.begin(); iter != m_FileHeaders.end(); ++iter)
			{
				VFS_ENTRY& entry = m_vEntries[iter->second.entry];
				put((char*)entry.getData(), entry.getSize());
			}
		}


		FileArchive::close();
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	DefaultFileArchiveFactory::DefaultFileArchiveFactory()
	{
	}
//----------------------------------------------------------------------
	DefaultFileArchiveFactory::~DefaultFileArchiveFactory()
	{
	}
//----------------------------------------------------------------------
	FileArchive* DefaultFileArchiveFactory::createFileArchive(const wstring& _fileName, dword _openMode)
	{
		return new DefaultFileArchive(_fileName, _openMode);
	}
//----------------------------------------------------------------------
}