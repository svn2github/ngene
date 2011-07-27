/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeWrapper.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "NodeWrapper.h"

#include "frmSceneGraph.h"


namespace nGENEToolset
{
	SurfaceWrapper::SurfaceWrapper(Surface* _surf, String^ _name)
	{
		m_pSurface = _surf;

		this->Name = _name;
		frmSceneGraph::graph->add(this);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	NodeWrapper::NodeWrapper(Node* _node, String^ _name, int _imageIndex)
	{
		m_pNode = _node;

		m_Position = gcnew Vector3(&m_pNode->getPositionLocal());
		m_Dimensions = gcnew Vector3(&m_pNode->getScale());
		m_Rotation = gcnew Quaternion(&m_pNode->getRotation());
		
		//m_PositionWorld = gcnew Vector3(NULL);
		//m_PositionWorld->setVector(m_pNode->getPositionWorld());

		this->Name = _name;

		ImageIndex = _imageIndex;

		frmSceneGraph::graph->addNode(this);
	}
//----------------------------------------------------------------------
}