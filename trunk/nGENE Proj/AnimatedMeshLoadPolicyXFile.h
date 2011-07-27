/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AnimatedMeshLoadPolicyXFile.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ANIMATEDMESHLOADPOLICYXFILE_H_
#define __INC_ANIMATEDMESHLOADPOLICYXFILE_H_


#include "Prerequisities.h"
#include "TypeInfo.h"
#include "SkeletalAnimationController.h"
#include "Matrix4x4.h"

#include <D3DX9Mesh.h>


namespace nGENE
{
	struct D3DXFRAME_DERIVED;


	/** Policy for loading X files.
	*/
	class nGENEDLL AnimatedMeshLoadPolicyXFile
	{
		EXPOSE_TYPE
	protected:
		int m_nStartIndex;

		Skeleton m_Skeleton;					///< This mesh skeleton

		vector <KeyFrameAnimationSet> m_Sets;	///< Animations sets

		LPD3DXFRAME pRoot;

		Matrix4x4 m_matTransform;
		bool m_bMoveToCentre;

		HashTable <D3DXFRAME_DERIVED*, BONE*> m_Bones;

	private:
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

		/// Processes frames in the hierarchy recursively.
		void processFrame(D3DXFRAME_DERIVED* _frame, BONE* _parent, vector <Surface>& _surfaces);

		/// Processes animation sets.
		void processAnimation(ID3DXAnimationController* _controller);

	protected:
		~AnimatedMeshLoadPolicyXFile() {}
	
	public:
		AnimatedMeshLoadPolicyXFile();

		/** Loads contents of the file.
			@remarks
				Simply call it on host application to obtain
				all mesh data contained in the file.
		*/
		vector <Surface> loadMesh(const wstring& _fileName, const Matrix4x4& _transform,
			bool _moveToCentre);

		/// Returns reference to the underlying skeleton object.
		Skeleton& getSkeleton();

		/// Returns number of animations sets.
		uint getAnimationSetsNum() const;

		/// Returns animation set with the given index.
		KeyFrameAnimationSet& getAnimationSet(uint _index);
	};



	inline Skeleton& AnimatedMeshLoadPolicyXFile::getSkeleton()
	{
		return m_Skeleton;
	}
//----------------------------------------------------------------------
	inline uint AnimatedMeshLoadPolicyXFile::getAnimationSetsNum() const
	{
		return m_Sets.size();
	}
//----------------------------------------------------------------------
	inline KeyFrameAnimationSet& AnimatedMeshLoadPolicyXFile::getAnimationSet(uint _index)
	{
		return m_Sets[_index];
	}
//----------------------------------------------------------------------
}


#endif
