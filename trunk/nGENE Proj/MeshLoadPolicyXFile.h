/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MeshLoadPolicyXFile.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_MESHLOADPOLICYXFILE_H_
#define __INC_MESHLOADPOLICYXFILE_H_


#include "Prerequisities.h"
#include "TypeInfo.h"
#include "Matrix4x4.h"

#include <D3DX9Mesh.h>


namespace nGENE
{
	/** Policy for loading X files.
	*/
	class nGENEDLL MeshLoadPolicyXFile
	{
		EXPOSE_TYPE
	private:
		Matrix4x4 m_matTransform;
		bool m_bMoveToCentre;


		/// Optimizes mesh so that attributes table can be accessed.
		void optimizeMesh(ID3DXMesh* _mesh);
		/// Returns attributes table info.
		void getAttributesTable(ID3DXMesh* _mesh, dword& _numAttrs,
			D3DXATTRIBUTERANGE** _table);

		/// Processes attributes table to form new surfaces.
		void processMesh(ID3DXMesh* _mesh, dword _numAttributes, D3DXATTRIBUTERANGE* _table,
			vector <Surface>& _surfaces);

		/// Clones mesh to match correct vertex declaration.
		ID3DXMesh* cloneMesh(ID3DXMesh* _mesh, IDirect3DDevice9* _device);

	protected:
		~MeshLoadPolicyXFile() {}
	
	public:
		MeshLoadPolicyXFile();

		/** Loads contents of the file.
			@remarks
				Simply call it on host application to obtain
				all mesh data contained in the file.
		*/
		vector <Surface> loadMesh(const wstring& _fileName, const Matrix4x4& _transform,
			bool _moveToCentre);
	};
}


#endif
