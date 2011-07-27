/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexDeclaration.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "VertexDeclaration.h"


namespace nGENE
{
	VertexDeclaration::VertexDeclaration()
	{
	}
//----------------------------------------------------------------------
	VertexDeclaration::~VertexDeclaration()
	{
	}
//----------------------------------------------------------------------
	void VertexDeclaration::addVertexElement(const VERTEX_ELEMENT& _element)
	{
		m_Elements.push_back(_element);
	}
//----------------------------------------------------------------------
	void VertexDeclaration::addVertexElement(SVertexElement* _array, uint _count)
	{
		for(uint i = 0; i < _count; ++i)
			m_Elements.push_back(_array[i]);
	}
//----------------------------------------------------------------------
	VERTEX_ELEMENT* VertexDeclaration::getVertexElement(VERTEX_ELEMENT_USAGE _usage)
	{
		uint nCount = m_Elements.size();
		for(uint i = 0; i < nCount; ++i)
		{
			if(m_Elements[i].usage == _usage)
				return &m_Elements[i];
		}

		return NULL;
	}
//----------------------------------------------------------------------
}
