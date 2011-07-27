/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeMesh.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEMESH_H_
#define __INC_NODEMESH_H_


#include "NodeVisible.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** 3D mesh (probably loaded from file).
		@remarks
			This class is built around policy-based design. The policies
			classes	describe the functionality. To load specific file format,
			you do not have to derive from this class - it is sufficient
			to just provide policy class for this format.
	*/
	template <typename LoaderPolicy>
	class NodeMesh: public NodeVisible, public LoaderPolicy
	{
		EXPOSE_TYPE
	private:
		/// File name containing mesh data
		wstring m_stFileName;

	public:
		NodeMesh();
		explicit NodeMesh(const wstring& _fileName);
		explicit NodeMesh(const wstring& _fileName, const Matrix4x4& _transform);
		explicit NodeMesh(const wstring& _fileName, const Matrix4x4& _transform,
			bool _moveToCenter);
		~NodeMesh();

		/// Loads given file.
		void load(const wstring& _fileName, const Matrix4x4& _transform=Matrix4x4::IDENTITY,
			bool _moveToCenter=false);

		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	template <typename LoaderPolicy>
	NodeMesh <typename LoaderPolicy>::NodeMesh()
	{
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	NodeMesh <typename LoaderPolicy>::NodeMesh(const wstring& _fileName):
		m_stFileName(_fileName)
	{
		this->load(m_stFileName);
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	NodeMesh <typename LoaderPolicy>::NodeMesh(const wstring& _fileName,
		const Matrix4x4& _transform):
		m_stFileName(_fileName)
	{
		this->load(m_stFileName, _transform);
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	NodeMesh <typename LoaderPolicy>::NodeMesh(const wstring& _fileName,
		const Matrix4x4& _transform, bool _moveToCenter):
		m_stFileName(_fileName)
	{
		this->load(m_stFileName, _transform, _moveToCenter);
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	NodeMesh <typename LoaderPolicy>::~NodeMesh()
	{
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void NodeMesh <typename LoaderPolicy>::load(const wstring& _fileName, const Matrix4x4& _transform,
		bool _moveToCenter)
	{
		m_stFileName = _fileName;

		// Retrieve surfaces from the loader
		vector <Surface> vSurfaces = LoaderPolicy::loadMesh(_fileName, _transform,
			_moveToCenter);
		Surface* pSurface = NULL;
		for(uint i = 0; i < vSurfaces.size(); ++i)
		{
			pSurface = &vSurfaces[i];
			NodeVisible::addSurface(pSurface->getName(), *pSurface);
		}


		calculateBoundingBox();
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void NodeMesh <typename LoaderPolicy>::serialize(ISerializer* _serializer,
												     bool _serializeType,
													 bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			NodeVisible::serialize(_serializer, false, _serializeChildren);
			
			Property <wstring> proper(m_stFileName);
			_serializer->addProperty("FileName", proper);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void NodeMesh <typename LoaderPolicy>::deserialize(ISerializer* _serializer)
	{
		Property <wstring> proper(m_stFileName);
		_serializer->getProperty("FileName", proper);
		
		// Load mesh
		load(m_stFileName);

		NodeVisible::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	// Initialize static members.
	template <typename LoaderPolicy>
	TypeInfo NodeMesh <typename LoaderPolicy>::Type(L"NodeMesh" + LoaderPolicy::Type.getTypeName(),
													&NodeVisible::Type);
//----------------------------------------------------------------------
}


#endif