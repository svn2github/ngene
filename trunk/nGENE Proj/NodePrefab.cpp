/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodePrefab.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "NodePrefab.h"


namespace nGENE
{
	NodePrefab::NodePrefab(const Vector2& _texCoordMul):
		m_vecTexCoordMul(_texCoordMul)
	{
	}
//----------------------------------------------------------------------
	NodePrefab::~NodePrefab()
	{
	}
//----------------------------------------------------------------------
	void NodePrefab::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			NodeVisible::serialize(_serializer, false, _serializeChildren);
			
			Property <Vector2> proper(m_vecTexCoordMul);
			_serializer->addProperty("TexCoordMult", proper);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodePrefab::deserialize(ISerializer* _serializer)
	{
		Property <Vector2> proper(m_vecTexCoordMul);
		_serializer->getProperty("TexCoordMult", proper);

		NodeVisible::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	Surface* NodePrefab::updateSurface(const wstring& _name, VertexBuffer* _vb)
	{
		if(findSurface(_name))
		{
			Surface* pSurface = getSurface(_name);
			pSurface->setVertexBuffer(_vb);

			return pSurface;
		}
		else
		{
			Surface temp;
			temp.setVertexBuffer(_vb);
			addSurface(_name, temp);

			return getSurface(_name);
		}
	}
//----------------------------------------------------------------------
}