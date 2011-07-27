/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SceneGraph.h
Version:	0.04
---------------------------------------------------------------------------
*/

#ifndef __INC_ASCENEGRAPH_H_
#define __INC_ASCENEGRAPH_H_


#include "NodeWrapper.h"


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Collections;

	ref class frmSceneGraph;


	public ref class SceneGraph: public System::Object
	{
	private:
		ArrayList^ vector;
		frmSceneGraph^ frmGraph;

	public:
		SceneGraph(frmSceneGraph^ form)
		{
			vector = gcnew ArrayList();
			frmGraph = form;
		}

		void cleanup();

		NodeWrapper^ getActive();
		SceneNode^ getActiveSceneNode();

		/// Adds node to the scene graph
		void addNode(SceneNode^ node);

		/// Finds node in the scene graph
		SceneNode^ findNode(String^ name);
		void selectNode(String^ name);

		/// Selects parent
		void selectParent();

		void beginAdd();
		void add(SceneNode^ node);
		void endAdd();

		void remove(SceneNode^ node);
	};
}


#endif