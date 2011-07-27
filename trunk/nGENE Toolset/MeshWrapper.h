/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		MeshWrapper.h
Version:	0.02
---------------------------------------------------------------------------
*/


#ifndef __INC_MESHWRAPPER_H_
#define __INC_MESHWRAPPER_H_


#include <vcclr.h>
#include "nGENE.h"


namespace nGENEToolset
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Drawing;
	using namespace System::Text;

	using namespace nGENE;


	/// Base for meshes wrapper.
	ref class MeshWrapper: public NodeVisibleWrapper
	{
	protected:
		static int s_nMeshCount;

		MeshWrapper() {}

		MeshWrapper(NodeVisible* _node, String^ _name):
			NodeVisibleWrapper(_node, _name, 0)
		{
			s_nMeshCount++;
		}
	};


	/// Wrapper for .xml file meshes (Collada format).
	ref class MeshWrapperColladaFile: public MeshWrapper
	{
	protected:
		NodeMesh <MeshLoadPolicyCollada>* m_pMesh;

	public:
		MeshWrapperColladaFile(String^ fileName)
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);

			pin_ptr <const wchar_t> str1 = PtrToStringChars(fileName);
			wstring name(str1);

			m_pMesh = sm->createMesh <MeshLoadPolicyCollada>(name);

			Material* matTemp = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"stone");
			Surface* surf = NULL;
			for(uint i = 0; i < m_pMesh->getSurfacesNum(); ++i)
			{
				surf = m_pMesh->getSurface(i);
				surf->setMaterial(matTemp);
			}

			m_Position = gcnew Vector3(&m_pMesh->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pMesh->getScale());
			m_Rotation = gcnew Quaternion(&m_pMesh->getRotation());

			m_pNode = m_pMesh;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Mesh";
			buffer << s_nMeshCount;
			wstring text = buffer.str();

			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pMesh);

			this->Name = "Mesh" + (s_nMeshCount);
			this->Key = "Mesh" + (s_nMeshCount++);


			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pMesh->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pMesh->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pMesh->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		MeshWrapperColladaFile(NodeMesh <MeshLoadPolicyCollada>* _mesh, String^ _name):
			MeshWrapper(_mesh, _name)
		{
			m_pMesh = _mesh;
			m_pNode = m_pMesh;


			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pMesh->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pMesh->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pMesh->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}
	};

	
	/// Wrapper for .x file meshes.
	ref class MeshWrapperXFile: public MeshWrapper
	{
	protected:
		NodeMesh <MeshLoadPolicyXFile>* m_pMesh;

	public:
		MeshWrapperXFile(String^ fileName)
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);

			pin_ptr <const wchar_t> str1 = PtrToStringChars(fileName);
			wstring name(str1);

			m_pMesh = sm->createMesh <MeshLoadPolicyXFile>(name);

			Material* matTemp = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"stone");
			Surface* surf = NULL;
			for(uint i = 0; i < m_pMesh->getSurfacesNum(); ++i)
			{
				surf = m_pMesh->getSurface(i);
				surf->setMaterial(matTemp);
			}

			m_Position = gcnew Vector3(&m_pMesh->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pMesh->getScale());
			m_Rotation = gcnew Quaternion(&m_pMesh->getRotation());

			m_pNode = m_pMesh;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Mesh";
			buffer << s_nMeshCount;
			wstring text = buffer.str();

			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pMesh);

			this->Name = "Mesh" + (s_nMeshCount);
			this->Key = "Mesh" + (s_nMeshCount++);


			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pMesh->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pMesh->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pMesh->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		MeshWrapperXFile(NodeMesh <MeshLoadPolicyXFile>* _mesh, String^ _name):
			MeshWrapper(_mesh, _name)
		{
			m_pMesh = _mesh;
			m_pNode = m_pMesh;


			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pMesh->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pMesh->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pMesh->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}
	};

	/// Wrapper for .3ds file meshes.
	ref class MeshWrapper3dsFile: public MeshWrapper
	{
	protected:
		NodeMesh <MeshLoadPolicy3dsFile>* m_pMesh;

	public:
		MeshWrapper3dsFile(String^ fileName)
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);

			pin_ptr <const wchar_t> str1 = PtrToStringChars(fileName);
			wstring name(str1);

			m_pMesh = sm->createMesh <MeshLoadPolicy3dsFile>(name);

			Material* matTemp = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"stone");
			Surface* surf = NULL;
			for(uint i = 0; i < m_pMesh->getSurfacesNum(); ++i)
			{
				surf = m_pMesh->getSurface(i);
				surf->setMaterial(matTemp);
			}

			m_Position = gcnew Vector3(&m_pMesh->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pMesh->getScale());
			m_Rotation = gcnew Quaternion(&m_pMesh->getRotation());

			m_pNode = m_pMesh;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Mesh";
			buffer << s_nMeshCount;
			wstring text = buffer.str();

			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pMesh);

			this->Name = "Mesh" + (s_nMeshCount);
			this->Key = "Mesh" + (s_nMeshCount++);


			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pMesh->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pMesh->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pMesh->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		MeshWrapper3dsFile(NodeMesh <MeshLoadPolicy3dsFile>* _mesh, String^ _name):
			MeshWrapper(_mesh, _name)
		{
			m_pMesh = _mesh;
			m_pNode = m_pMesh;


			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pMesh->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pMesh->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pMesh->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}
	};
}


#endif