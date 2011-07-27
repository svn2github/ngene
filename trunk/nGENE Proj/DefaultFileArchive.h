/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DefaultFileArchive.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DEFAULTFILEARCHIVE_H_
#define __INC_DEFAULTFILEARCHIVE_H_


#include "FileArchive.h"
#include "FileArchiveEntry.h"
#include "FileArchiveFactory.h"


namespace nGENE
{
	enum nGENEDLL DEFAULT_FILE_ARCHIVE_FLAGS
	{
		DFAF_COMPRESS = (1 << 0)
	};

	/** DefaultFileArchive class.
		@par
			Default file archive is pretty easy and use following format:
			[File header section]
			version ID - 4 bytes
			flags - 4 bytes
			total number of files - 4 bytes

			[File table section]
			File1 name length - 2 bytes
			File1 name - File1 name length bytes
			File1 data offset - 4 bytes
			File1 size - 4 bytes
			File1 specific flags - 4 bytes

			File2 specific flags - 4 bytes
			File2 name length - 2 bytes
			File2 name - File2 name length bytes
			File2 data offset - 4 bytes
			File2 size - 4 bytes
			File2 specific flags - 4 bytes
			...
			FileN name length - 2 bytes
			FileN name - FileN name length bytes
			FileN data offset - 4 bytes
			FileN size - 4 bytes
			FileN specific flags - 4 bytes

			[File data section]
			File1 data - File1 size bytes
			File2 data - File2 size bytes
			...
			FileN data - FileN size bytes
	*/
	class nGENEDLL DefaultFileArchive: public FileArchive
	{
	protected:
		/// This structure describes file header.
		struct SFileHeader
		{
			uint offset;
			uint length;
			uint flags;

			uint entry;
		};

		uint m_nVersion;					///< Version of the archive
		uint m_nFlags;						///< Flags of the archive
		uint m_nFilesCount;					///< Number of files in archive

		uint m_nDataOffset;

		vector <VFS_ENTRY> m_vEntries;


		/// File offsets withing the archive
		HashTable <string, SFileHeader> m_FileHeaders;

		/// Current version of the default file archiver
		static const uint s_kArchiverVersion;

	public:
		DefaultFileArchive(const wstring& _fileName, dword _openMode);
		virtual ~DefaultFileArchive();

		VFS_ENTRY* getEntry(const string& _fileName);
		bool hasEntry(const string& _fileName);

		void init();
		void close();

		uint getEntriesCount();

		void insertFile(IFile* _file);
		
		void setVersion(uint _version);
		uint getVersion() const;

		/** Sets combination of DEFAULT_FILE_ARCHIVE_FLAGS flags.
			@remarks
				This would influence only files opened in OPEN_WRITE mode.
				For ones opened OPEN_READ flags are obtained automatically
				fro the file.
		*/
		void setFlags(uint _flags);
	};

	/// Factory to be used for DefaultFileArchive class objects creation.
	class nGENEDLL DefaultFileArchiveFactory: public FileArchiveFactory
	{
	public:
		DefaultFileArchiveFactory();
		virtual ~DefaultFileArchiveFactory();

		FileArchive* createFileArchive(const wstring& _fileName, dword _openMode);
	};



	inline void DefaultFileArchive::setVersion(uint _version)
	{
		m_nVersion = _version;
	}
//----------------------------------------------------------------------
	inline uint DefaultFileArchive::getVersion() const
	{
		return m_nVersion;
	}
//----------------------------------------------------------------------
	inline void DefaultFileArchive::setFlags(uint _flags)
	{
		if(m_dwOpenMode & OPEN_WRITE)
			m_nFlags = _flags;
	}
//----------------------------------------------------------------------
}


#endif
