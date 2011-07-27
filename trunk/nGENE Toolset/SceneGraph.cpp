/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SceneGraph.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "SceneGraph.h"

#include "frmSceneGraph.h"


namespace nGENEToolset
{
	void SceneGraph::addNode(SceneNode^ node)
	{
		vector->Add(node);
		if(node->Key != nullptr && node->Key != "")
			frmGraph->addNode(node->Name, node->Key, node->ImageIndex);
	}
//----------------------------------------------------------------------
	void SceneGraph::selectParent()
	{
		frmGraph->selectParent();
	}
//----------------------------------------------------------------------
	SceneNode^ SceneGraph::findNode(String^ name)
	{
		SceneNode^ node = nullptr;
		for(int i = 0; i < vector->Count; ++i)
		{
			try {
				node = safe_cast<SceneNode^>(vector[i]);
				if(node->Key->Equals(name) || node->Name->Equals(name))
					return node;
			} catch(InvalidCastException^) {}
		}

		return node;
	}
//----------------------------------------------------------------------
	void SceneGraph::selectNode(String^ name)
	{
		try
		{
			SceneNode^ node = findNode(name);
			if(node != nullptr)
			{
				frmGraph->selectNode(name);
			}
		}
		catch(Exception^ e)
		{
			System::Console::WriteLine(e->Message);
		}
	}
//----------------------------------------------------------------------
	NodeWrapper^ SceneGraph::getActive()
	{
		if(frmGraph->getSelected() == nullptr)
			return nullptr;

		NodeWrapper^ node = nullptr;
		try {
			node = safe_cast<NodeWrapper^>(frmGraph->getSelected());
		} catch(InvalidCastException^) {
		}

		return node;
	}
//----------------------------------------------------------------------
	SceneNode^ SceneGraph::getActiveSceneNode()
	{
		return frmGraph->getSelected();
	}
//----------------------------------------------------------------------
	void SceneGraph::add(SceneNode^ node)
	{
		vector->Add(node);
		frmGraph->add(node->Name, node->Key);
	}
//----------------------------------------------------------------------
	void SceneGraph::remove(SceneNode^ node)
	{
		vector->Remove(node);
		frmGraph->remove(node->Key);
	}
//----------------------------------------------------------------------
	void SceneGraph::beginAdd()
	{
		frmGraph->beginAdd();
	}
//----------------------------------------------------------------------
	void SceneGraph::endAdd()
	{
		frmGraph->endAdd();
	}
//----------------------------------------------------------------------
	void SceneGraph::cleanup()
	{
		frmGraph->cleanup();
	}
//----------------------------------------------------------------------
}