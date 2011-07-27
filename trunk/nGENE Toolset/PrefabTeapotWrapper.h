/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabTeapotWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_PREFABTEAPOTWRAPPER_H_
#define __INC_PREFABTEAPOTWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	
	/// Wrapper for a PrefabTeapot class.
	ref class PrefabTeapotWrapper: public NodeVisibleWrapper
	{
	protected:
		PrefabTeapot* m_pTeapot;
		static int s_nTeapotCount;

		nGENEToolset::Vector2^ m_TexCoord;

	public:
		PrefabTeapotWrapper()
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pTeapot = sm->createTeapot();

			m_pTeapot->setPosition(0.0f, 0.0f, 0.0f);
			Surface* pSurface = m_pTeapot->getSurface(L"Base");
			Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
			pSurface->setMaterial(pMaterial);

			m_Position = gcnew Vector3(&m_pTeapot->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pTeapot->getScale());
			m_Rotation = gcnew Quaternion(&m_pTeapot->getRotation());
			m_TexCoord = gcnew Vector2(&m_pTeapot->getTexCoordMult());

			m_pNode = m_pTeapot;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Teapot";
			buffer << s_nTeapotCount;
			wstring text = buffer.str();
			
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pTeapot);

			this->Name = "Teapot" + (s_nTeapotCount);
			this->Key = "Teapot" + (s_nTeapotCount++);

			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pTeapot->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pTeapot->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pTeapot->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		PrefabTeapotWrapper(PrefabTeapot* _teapot, String^ _name):
			NodeVisibleWrapper(_teapot, _name, 0)
		{
			m_pTeapot = _teapot;
			s_nTeapotCount++;

			m_TexCoord = gcnew Vector2(&m_pTeapot->getTexCoordMult());

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pTeapot->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pTeapot->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pTeapot->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		virtual ~PrefabTeapotWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Teapot"),
		DescriptionAttribute("Texture coordinates multiplier")]
		property nGENEToolset::Vector2^ TexCoord
		{
			nGENEToolset::Vector2^ get()
			{
				if(!frmRenderTarget::engine->tryLock())
					return m_TexCoord;

				m_pTeapot->setTexCoordMult(m_TexCoord->getVector());

				frmRenderTarget::engine->releaseLock();

				return m_TexCoord;
			}

			void set(nGENEToolset::Vector2^ value)
			{
				m_TexCoord->x = value->x;
				m_TexCoord->y = value->y;
			}
		}
	};
}


#endif