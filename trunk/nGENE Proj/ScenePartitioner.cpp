/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScenePartitioner.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ScenePartitioner.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ScenePartitioner::Type(L"ScenePartitioner", NULL);


	ScenePartitioner::ScenePartitioner():
		m_pFrustum(NULL),
		m_nNodesCount(0),
		m_nNodesVisibleCount(0)
	{
	}
//----------------------------------------------------------------------
	ScenePartitioner::~ScenePartitioner()
	{
	}
//----------------------------------------------------------------------
}