/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MeshLoadPolicy3dsFile.h
Version:	0.09
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MESHLOADPOLICY3DSFILE_H_
#define __INC_MESHLOADPOLICY3DSFILE_H_


#include "Prerequisities.h"
#include "Surface.h"
#include "TypeInfo.h"
#include "Matrix4x4.h"


// Here goes the chunks' names
#define ROOT		  0x4d4d

// Main Chunks
#define VERSION       0x0002
#define OBJECTINFO    0x3D3D

// Sub defines of OBJECTINFO
#define MATERIAL	  0xAFFF
#define OBJECT		  0x4000

#define MESH		  0x4100

// Sub defines of OBJECT_MESH
#define VERTICES      0x4110
#define FACES		  0x4120
#define UV			  0x4140


namespace nGENE
{
	/// Chunk of data for 3ds file loader
	struct SChunk
	{
		ushort ID;						///< Chunk ID
		uint length;					///< Chunk length in bytes
	};

	/** Policy for loading 3ds files.
		@par
			3ds file format is somewhat old fashioned and not
			very flexible. But as it is standard and quite simple
			to parse, it is implemented.
		@par
			Each 3ds file consists of so called chunks described
			by the ID no. and length in bytes. Some of chunks can
			have children. Note that only some of all available chunks
			are of any use in a real-time 3D application - these are
			parsed.
	*/
	class nGENEDLL MeshLoadPolicy3dsFile
	{
		EXPOSE_TYPE
	private:
		/// Currently parsed surface/mesh.
		struct SModel
		{
			SVertex* pVertices;					///< Model vertices
			ushort* pIndices;					///< Model triangles (indices)
			uint nVerticesCount;				///< Vertex count;
			uint nIndicesCount;					///< Indices count
		};


		IFile* m_pFile;							///< Mesh file

		Matrix4x4 m_matTransform;
		bool m_bMoveToCentre;

	private:

		void unknownChunk(const SChunk& _chunk);
		void computeNormals(SModel* _model);

		/// Processes chunk data.
		SChunk processChunk();

		void processVertices(SModel* _model);
		void processIndices(SModel* _model);
		void processTexCoords(SModel* _model);

		void processObject(vector <Surface>& _surfaces);
		void processMesh(vector <Surface>& _surfaces);
		
		/// Creates mesh.
		void createMesh(vector <Surface>& _surfaces, Surface& _surface, SModel* _model);

		/// Adds surface to the mesh.
		void addSurface(vector <Surface>& _surfaces, const wstring& _name);

	protected:
		~MeshLoadPolicy3dsFile() {}
	
	public:
		MeshLoadPolicy3dsFile();

		/** Loads contents of the file.
			@remarks
				Simply call it on host application to obtain
				all mesh data contained in the file.
		*/
		vector <Surface> loadMesh(const wstring& _fileName, const Matrix4x4& _transform,
			bool _moveToCenter);
	};
}


#endif
