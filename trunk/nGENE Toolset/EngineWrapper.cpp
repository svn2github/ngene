/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		EngineWrapper.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "stdafx.h"

#include "EngineWrapper.h"
#include "frmSceneGraph.h"

#include "CameraWrapper.h"
#include "CloudsWrapper.h"
#include "LightWrapper.h"
#include "LightningWrapper.h"
#include "MeshWrapper.h"
#include "ParticleSystemWrapper.h"
#include "PrefabBoxWrapper.h"
#include "PrefabCapsuleWrapper.h"
#include "PrefabCylinderWrapper.h"
#include "PrefabPlaneWrapper.h"
#include "PrefabPyramidWrapper.h"
#include "PrefabSphereWrapper.h"
#include "PrefabTeapotWrapper.h"
#include "PrefabTorusWrapper.h"
#include "SkyWrapper.h"
#include "SoundWrapper.h"
#include "TerrainWrapper.h"
#include "WaterWrapper.h"

#include "NodeClouds.h"
#include "NodeLightning.h"
#include "NodeSky.h"
#include "NodeWater.h"

using nGENE::Nature::NodeClouds;
using nGENE::Nature::NodeLightning;
using nGENE::Nature::NodeSky;
using nGENE::Nature::NodeWater;


namespace nGENEToolset
{
	void ToolsetListener::handleEvent(const MouseEvent& _evt)
	{
		// No camera so return
		if(!Engine::getSingleton().hasActiveCamera())
			return;

		const SDisplayMode& mode = Renderer::getSingleton().getDisplayMode();
		float x = (_evt.mousePosition.x - m_XMin) / (m_XMax - m_XMin) * mode.width;
		float y = (_evt.mousePosition.y - m_YMin) / (m_YMax - m_YMin) * mode.height - 100.0f;


		if(_evt.type == MET_BUTTON_LEFT)
		{
			if(_evt.nvalue & 0x80)
			{
				ICullable* pObj = m_pApp->pick(x, y, true);
				if(dynamic_cast<Surface*>(pObj))
				{
					Surface* pSurf = (Surface*)pObj;
					NodeVisible* pNode = pSurf->getNode();
					m_pApp->drawAABB(pSurf);
					m_pApp->drawAxisRods(pSurf);
					
					wstring& tmp = pNode->getName();
					String^ name = gcnew String(tmp.c_str());


					SceneNode^ node = frmSceneGraph::graph->findNode(name);
					frmSceneGraph::graph->selectNode(node->Key);
				}
				else if(dynamic_cast<NodeLight*>(pObj))
				{
					NodeLight* pNode = (NodeLight*)pObj;
					m_pApp->drawAABB(pNode);
					m_pApp->drawAxisRods(pNode);
				}

				m_LBMDown = true;
			}
			else
				m_LBMDown = false;
		}


		if(m_LAltDown)
		{
			// Rotate object
			if(_evt.type == MET_MOVE_X && m_LBMDown)
			{
				if(frmSceneGraph::graph->getActive())
				{
					Node* pNode = frmSceneGraph::graph->getActive()->getnGENENode();

					// Calculate movement axis
					Matrix4x4 matInvView = Engine::getSingleton().getActiveCamera()->getViewMatrix();
					matInvView.invert();
					matInvView.transpose();

					Vector4 axis = Vector4::UNIT_Y;
					axis = matInvView.multiply(axis);

					nGENE::Vector3 tmp(axis.x, axis.y, axis.z);
					if(frmRenderTarget::engine->LockX == false)
						tmp.x = 0.0f;
					if(frmRenderTarget::engine->LockY == false)
						tmp.y = 0.0f;
					if(frmRenderTarget::engine->LockZ == false)
						tmp.z = 0.0f;
					if(tmp.getLength() > 0.01f)
						tmp.normalize();
					else
						tmp.set(axis.x, axis.y, axis.z);
					nGENE::Quaternion quat(tmp, _evt.fvalue * 0.05f);
					quat = pNode->getRotation() * quat;
					pNode->setRotation(quat);

					if(dynamic_cast<ICullable*>(pNode))
					{
						m_pApp->drawAABB((ICullable*)pNode);
						m_pApp->drawAxisRods((ICullable*)pNode);
					}
					else if(dynamic_cast<NodeVisible*>(pNode))
					{
						m_pApp->drawAABB((NodeVisible*)pNode);
						m_pApp->drawAxisRods((NodeVisible*)pNode);
					}

					frmSceneGraph::graph->getActive()->Rotation->x = quat.x;
					frmSceneGraph::graph->getActive()->Rotation->y = quat.y;
					frmSceneGraph::graph->getActive()->Rotation->z = quat.z;
					frmSceneGraph::graph->getActive()->Rotation->w = quat.w;
				}
			}

			if(_evt.type == MET_MOVE_Y && m_LBMDown)
			{
				if(frmSceneGraph::graph->getActive())
				{
					Node* pNode = frmSceneGraph::graph->getActive()->getnGENENode();

					// Calculate movement axis
					Matrix4x4 matInvView = Engine::getSingleton().getActiveCamera()->getViewMatrix();
					matInvView.invert();
					matInvView.transpose();

					Vector4 axis = Vector4::UNIT_Z;
					axis = matInvView.multiply(axis);

					nGENE::Vector3 tmp(axis.x, axis.y, axis.z);
					if(frmRenderTarget::engine->LockX == false)
						tmp.x = 0.0f;
					if(frmRenderTarget::engine->LockY == false)
						tmp.y = 0.0f;
					if(frmRenderTarget::engine->LockZ == false)
						tmp.z = 0.0f;
					if(tmp.getLength() > 0.01f)
						tmp.normalize();
					else
						tmp.set(axis.x, axis.y, axis.z);
					nGENE::Quaternion quat(tmp, _evt.fvalue * 0.05f);
					quat = pNode->getRotation() * quat;
					pNode->setRotation(quat);

					if(dynamic_cast<ICullable*>(pNode))
					{
						m_pApp->drawAABB((ICullable*)pNode);
						m_pApp->drawAxisRods((ICullable*)pNode);
					}
					else if(dynamic_cast<NodeVisible*>(pNode))
					{
						m_pApp->drawAABB((NodeVisible*)pNode);
						m_pApp->drawAxisRods((NodeVisible*)pNode);
					}

					frmSceneGraph::graph->getActive()->Rotation->x = quat.x;
					frmSceneGraph::graph->getActive()->Rotation->y = quat.y;
					frmSceneGraph::graph->getActive()->Rotation->z = quat.z;
					frmSceneGraph::graph->getActive()->Rotation->w = quat.w;
				}
			}
		}
		else if(m_LShiftDown)
		{
			// Scale object
			if(_evt.type == MET_MOVE_X && m_LBMDown)
			{
				if(frmSceneGraph::graph->getActive())
				{
					Node* pNode = frmSceneGraph::graph->getActive()->getnGENENode();

					// Calculate scaling axis
					Matrix4x4 matInvView = Engine::getSingleton().getActiveCamera()->getViewMatrix();
					matInvView.invert();
					matInvView.transpose();

					Vector4 axis = Vector4::UNIT_X;
					axis = matInvView.multiply(axis);

					nGENE::Vector3 tmp(axis.x, axis.y, axis.z);
					if(frmRenderTarget::engine->LockX == false)
						tmp.x = 0.0f;
					if(frmRenderTarget::engine->LockY == false)
						tmp.y = 0.0f;
					if(frmRenderTarget::engine->LockZ == false)
						tmp.z = 0.0f;
					if(tmp.getLength() > 0.01f)
						tmp.normalize();
					nGENE::Vector3 scale = pNode->getScale() + tmp * _evt.fvalue * 0.05f;

					frmSceneGraph::graph->getActive()->Scale->x = scale.x;
					frmSceneGraph::graph->getActive()->Scale->y = scale.y;
					frmSceneGraph::graph->getActive()->Scale->z = scale.z;

					if(dynamic_cast<ICullable*>(pNode))
					{
						m_pApp->drawAABB((ICullable*)pNode);
						m_pApp->drawAxisRods((ICullable*)pNode);
					}
					else if(dynamic_cast<NodeVisible*>(pNode))
					{
						m_pApp->drawAABB((NodeVisible*)pNode);
						m_pApp->drawAxisRods((NodeVisible*)pNode);
					}
				}
			}

			if(_evt.type == MET_MOVE_Y && m_LBMDown)
			{
				if(frmSceneGraph::graph->getActive())
				{
					Node* pNode = frmSceneGraph::graph->getActive()->getnGENENode();

					// Calculate scaling axis
					Matrix4x4 matInvView = Engine::getSingleton().getActiveCamera()->getViewMatrix();
					matInvView.invert();
					matInvView.transpose();

					Vector4 axis = Vector4::UNIT_Y;
					axis = matInvView.multiply(axis);

					nGENE::Vector3 tmp(axis.x, axis.y, axis.z);
					if(frmRenderTarget::engine->LockX == false)
						tmp.x = 0.0f;
					if(frmRenderTarget::engine->LockY == false)
						tmp.y = 0.0f;
					if(frmRenderTarget::engine->LockZ == false)
						tmp.z = 0.0f;
					if(tmp.getLength() > 0.01f)
						tmp.normalize();
					nGENE::Vector3 scale = pNode->getScale() - tmp * _evt.fvalue * 0.05f;

					frmSceneGraph::graph->getActive()->Scale->x = scale.x;
					frmSceneGraph::graph->getActive()->Scale->y = scale.y;
					frmSceneGraph::graph->getActive()->Scale->z = scale.z;

					if(dynamic_cast<ICullable*>(pNode))
					{
						m_pApp->drawAABB((ICullable*)pNode);
						m_pApp->drawAxisRods((ICullable*)pNode);
					}
					else if(dynamic_cast<NodeVisible*>(pNode))
					{
						m_pApp->drawAABB((NodeVisible*)pNode);
						m_pApp->drawAxisRods((NodeVisible*)pNode);
					}
				}
			}
		}
		else
		{
			// Move object
			if(_evt.type == MET_MOVE_X && m_LBMDown)
			{
				if(frmSceneGraph::graph->getActive())
				{
					Node* pNode = frmSceneGraph::graph->getActive()->getnGENENode();

					// Calculate movement axis
					Matrix4x4 matInvView = Engine::getSingleton().getActiveCamera()->getViewMatrix();
					matInvView.invert();
					matInvView.transpose();

					Vector4 axis = Vector4::UNIT_X;
					axis = matInvView.multiply(axis);

					nGENE::Vector3 tmp(axis.x, axis.y, axis.z);
					if(frmRenderTarget::engine->LockX == false)
						tmp.x = 0.0f;
					if(frmRenderTarget::engine->LockY == false)
						tmp.y = 0.0f;
					if(frmRenderTarget::engine->LockZ == false)
						tmp.z = 0.0f;
					if(tmp.getLength() > 0.01f)
						tmp.normalize();
					nGENE::Vector3 pos = pNode->getPositionLocal() + tmp * _evt.fvalue * 0.05f;

					frmSceneGraph::graph->getActive()->Position->x = pos.x;
					frmSceneGraph::graph->getActive()->Position->y = pos.y;
					frmSceneGraph::graph->getActive()->Position->z = pos.z;

					if(dynamic_cast<ICullable*>(pNode))
					{
						m_pApp->drawAABB((ICullable*)pNode);
						m_pApp->drawAxisRods((ICullable*)pNode);
					}
					else if(dynamic_cast<NodeVisible*>(pNode))
					{
						m_pApp->drawAABB((NodeVisible*)pNode);
						m_pApp->drawAxisRods((NodeVisible*)pNode);
					}
				}
			}

			if(_evt.type == MET_MOVE_Y && m_LBMDown)
			{
				if(frmSceneGraph::graph->getActive())
				{
					Node* pNode = frmSceneGraph::graph->getActive()->getnGENENode();

					// Calculate movement axis
					Matrix4x4 matInvView = Engine::getSingleton().getActiveCamera()->getViewMatrix();
					matInvView.invert();
					matInvView.transpose();

					Vector4 axis = Vector4::UNIT_Y;
					axis = matInvView.multiply(axis);

					nGENE::Vector3 tmp(axis.x, axis.y, axis.z);
					if(frmRenderTarget::engine->LockX == false)
						tmp.x = 0.0f;
					if(frmRenderTarget::engine->LockY == false)
						tmp.y = 0.0f;
					if(frmRenderTarget::engine->LockZ == false)
						tmp.z = 0.0f;
					if(tmp.getLength() > 0.01f)
						tmp.normalize();
					nGENE::Vector3 pos = pNode->getPositionLocal() - tmp * _evt.fvalue * 0.05f;
					pNode->setPosition(pos);

					frmSceneGraph::graph->getActive()->Position->x = pos.x;
					frmSceneGraph::graph->getActive()->Position->y = pos.y;
					frmSceneGraph::graph->getActive()->Position->z = pos.z;

					if(dynamic_cast<ICullable*>(pNode))
					{
						m_pApp->drawAABB((ICullable*)pNode);
						m_pApp->drawAxisRods((ICullable*)pNode);
					}
					else if(dynamic_cast<NodeVisible*>(pNode))
					{
						m_pApp->drawAABB((NodeVisible*)pNode);
						m_pApp->drawAxisRods((NodeVisible*)pNode);
					}
				}
			}
		}
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	System::Void EngineWrapper::reset(uint _width, uint _height)
	{
		Monitor::Enter(lock);
			m_bLoading = true;
			m_bRun = false;

			// Remove all post processing materials
			for(uint i = 0; i < Engine::getSingleton().getSceneManagersNum(); ++i)
				Engine::getSingleton().getSceneManager(i)->cleanup();

			Engine::getSingleton().setActiveCamera(NULL);

			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			Node* pRoot = new Node();
			pRoot->setName(L"SceneRoot");
			pRoot->setCreator(sm);
			sm->init(pRoot);

			// Remove all nodes
			frmSceneGraph::graph->cleanup();

			NodeWrapper^ root = gcnew NodeWrapper(sm->getRootNode(), "Scene Root", 0);
			CameraWrapper^ camera = gcnew CameraWrapper();
			camera->Aspect = (float)_width / (float)_height;

			((nGENE::PostStage*)nGENE::Renderer::getSingleton().getRenderStage(L"PostProcess"))->removeAllMaterials();

			m_bRun = true;
			m_bLoading = false;

		Monitor::Exit(lock);

		m_pApp->drawAABB((NodeVisible*)NULL);
		m_pApp->drawAxisRods((NodeVisible*)NULL);
	}
//----------------------------------------------------------------------
	System::Void EngineWrapper::save()
	{
		SaveFileDialog^ dlgSaveFile = gcnew SaveFileDialog();
		dlgSaveFile->Title = "Save map file";
		dlgSaveFile->Filter = ".nmp (nGENE map files)|*.nmp";
		dlgSaveFile->FilterIndex = 1;
		dlgSaveFile->AddExtension = true;

		if(dlgSaveFile->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			pin_ptr <const wchar_t> str1 = PtrToStringChars(dlgSaveFile->FileName);
			wstring fileName(str1);

			Monitor::Enter(lock);
				m_bRun = false;
			Monitor::Exit(lock);

			// Write to file
			m_pApp->serialize(fileName);

			Monitor::Enter(lock);
				m_bRun = true;
			Monitor::Exit(lock);
		}
	}
//----------------------------------------------------------------------
	bool EngineWrapper::load()
	{
		OpenFileDialog^ dlgOpenFile = gcnew OpenFileDialog();
		dlgOpenFile->Title = "Open map file";
		dlgOpenFile->Filter = ".nmp (nGENE map files)|*.nmp|All files (*.*)|*.*";
		dlgOpenFile->FilterIndex = 1;

		if(dlgOpenFile->ShowDialog() != System::Windows::Forms::DialogResult::OK)
			return false;

		pin_ptr <const wchar_t> str1 = PtrToStringChars(dlgOpenFile->FileName);
		wstring fileName(str1);

		Monitor::Enter(lock);
			m_bLoading = true;
			m_bRun = false;

			// Read from file
			m_pApp->deserialize(fileName);

			// Remove all nodes
			frmSceneGraph::graph->cleanup();

			Node* pNode = Engine::getSingleton().getSceneManager(0)->getRootNode();
			Serialized^ ser = gcnew Serialized(pNode);
			ser->deserialize();

			m_bRun = true;
			m_bLoading = false;
		Monitor::Exit(lock);

		return true;
	}
//----------------------------------------------------------------------
	System::Void EngineWrapper::removeSelected()
	{
		Monitor::Enter(lock);
			m_bRun = false;

			Node* pNode = frmSceneGraph::graph->getActive()->getnGENENode();

			if(dynamic_cast<NodeLight*>(pNode))
			{
				Renderer::getSingleton().removeLight((NodeLight*)pNode);
			}

			frmSceneGraph::graph->remove(frmSceneGraph::graph->getActive());

			SceneManager* sm = pNode->getCreator();
			sm->removeFromSceneGraph(pNode->getName());

			pNode->getParent()->removeChild(pNode);

			m_bRun = true;

		Monitor::Exit(lock);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	System::Void Serialized::deserialize()
	{
		String^ name = gcnew String(m_pNode->getName().c_str());
		if(name == "")
			return;
		// The nodes which are not serializable should not be deserialized
		if(!m_pNode->isSerializable())
			return;

		if(dynamic_cast<NodeClouds*>(m_pNode))
			CloudsWrapper^ node = gcnew CloudsWrapper(dynamic_cast<NodeClouds*>(m_pNode), name);
		else if(dynamic_cast<Camera*>(m_pNode))
			CameraWrapper^ node = gcnew CameraWrapper(dynamic_cast<Camera*>(m_pNode), name);
		else if(dynamic_cast<NodeLight*>(m_pNode))
			LightWrapper^ node = gcnew LightWrapper(dynamic_cast<NodeLight*>(m_pNode), name);
		else if(dynamic_cast<PrefabBox*>(m_pNode))
			PrefabBoxWrapper^ node = gcnew PrefabBoxWrapper(dynamic_cast<PrefabBox*>(m_pNode), name);
		else if(dynamic_cast<PrefabCapsule*>(m_pNode))
			PrefabCapsuleWrapper^ node = gcnew PrefabCapsuleWrapper((PrefabCapsule*)m_pNode, name);
		else if(dynamic_cast<PrefabCylinder*>(m_pNode))
			PrefabCylinderWrapper^ node = gcnew PrefabCylinderWrapper((PrefabCylinder*)m_pNode, name);
		else if(dynamic_cast<PrefabPlane*>(m_pNode))
			PrefabPlaneWrapper^ node = gcnew PrefabPlaneWrapper(dynamic_cast<PrefabPlane*>(m_pNode), name);
		else if(dynamic_cast<PrefabPyramid*>(m_pNode))
			PrefabPyramidWrapper^ node = gcnew PrefabPyramidWrapper(dynamic_cast<PrefabPyramid*>(m_pNode), name);
		else if(dynamic_cast<PrefabSphere*>(m_pNode))
			PrefabSphereWrapper^ node = gcnew PrefabSphereWrapper(dynamic_cast<PrefabSphere*>(m_pNode), name);
		else if(dynamic_cast<PrefabTeapot*>(m_pNode))
			PrefabTeapotWrapper^ node = gcnew PrefabTeapotWrapper((PrefabTeapot*)m_pNode, name);
		else if(dynamic_cast<PrefabTorus*>(m_pNode))
			PrefabTorusWrapper^ node = gcnew PrefabTorusWrapper((PrefabTorus*)m_pNode, name);
		else if(dynamic_cast<NodeSky*>(m_pNode))
			SkyWrapper^ node = gcnew SkyWrapper(dynamic_cast<NodeSky*>(m_pNode), name);
		else if(dynamic_cast<NodeSound*>(m_pNode))
			SoundWrapper^ node = gcnew SoundWrapper((NodeSound*)m_pNode, name);
		else if(dynamic_cast<NodeTerrain*>(m_pNode))
			TerrainWrapper^ node = gcnew TerrainWrapper(dynamic_cast<NodeTerrain*>(m_pNode), name);
		else if(dynamic_cast<NodeWater*>(m_pNode))
			WaterWrapper^ node = gcnew WaterWrapper(dynamic_cast<NodeWater*>(m_pNode), name);
		else if(dynamic_cast<NodeMesh <MeshLoadPolicy3dsFile>* >(m_pNode))
			MeshWrapper3dsFile^ node = gcnew MeshWrapper3dsFile(dynamic_cast<NodeMesh <MeshLoadPolicy3dsFile>* >(m_pNode), name);
		else if(dynamic_cast<NodeMesh <MeshLoadPolicyXFile>* >(m_pNode))
			MeshWrapperXFile^ node = gcnew MeshWrapperXFile(dynamic_cast<NodeMesh <MeshLoadPolicyXFile>* >(m_pNode), name);
		else if(dynamic_cast<NodeMesh <MeshLoadPolicyCollada>* >(m_pNode))
			MeshWrapperColladaFile^ node = gcnew MeshWrapperColladaFile((NodeMesh <MeshLoadPolicyCollada>*)m_pNode, name);
		else if(dynamic_cast<NodeParticleSystem*>(m_pNode))
			ParticleSystemWrapper^ node = gcnew ParticleSystemWrapper((NodeParticleSystem*)m_pNode, name);
		else if(dynamic_cast<NodeVolumetric*>(m_pNode))
			VolumetricLightWrapper^ node = gcnew VolumetricLightWrapper((NodeVolumetric*)m_pNode, name);
		else if(dynamic_cast<NodeLightning*>(m_pNode))
			LightningWrapper^ node = gcnew LightningWrapper((NodeLightning*)m_pNode, name);
		else
			NodeWrapper^ node = gcnew NodeWrapper(m_pNode, name, 0);

		Node* pNode = m_pNode->getChild();
		while(pNode)
		{
			Serialized^ ser = gcnew Serialized(pNode);
			ser->deserialize();
			pNode = pNode->getNextSibling();
		}

		frmSceneGraph::graph->selectParent();
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	void ToolsetApplication::initLoggers()
	{
		m_LogConsole.setEnabled(false);
		m_LogXML.setEnabled(false);
	}
//----------------------------------------------------------------------
	void ToolsetApplication::loadResources()
	{
		// Mount '../media' path as current directory
		FileManager::getSingleton().changeDir("../media");

		loadShaderConfig(L"ShaderConfig.xml");
		MaterialManager::getSingleton().createLibraryFromFile(L"core.lib", L"core", false);
		MaterialManager::getSingleton().createLibraryFromFile(L"default.lib", L"default", false);
	}
//----------------------------------------------------------------------
}