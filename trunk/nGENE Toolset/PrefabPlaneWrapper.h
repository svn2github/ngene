/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabPlaneWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_PREFABPLANEWRAPPER_H_
#define __INC_PREFABPLANEWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	
	/// Wrapper for a PrefabPlane class.
	ref class PrefabPlaneWrapper: public NodeVisibleWrapper
	{
	protected:
		PrefabPlane* m_pPlane;
		static int s_nPlaneCount;

		nGENEToolset::Vector2^  m_TexCoord;

	public:
		PrefabPlaneWrapper()
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			Plane pl(nGENE::Vector3::UNIT_Y, nGENE::Point(0.0f, 0.0f, 0.0f));
			m_pPlane = sm->createPlane(pl);

			m_pPlane->setPosition(0.0f, 0.0f, 0.0f);
			Surface* pSurface = m_pPlane->getSurface(L"Base");
			Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
			pSurface->setMaterial(pMaterial);

			m_Position = gcnew Vector3(&m_pPlane->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pPlane->getScale());
			m_Rotation = gcnew Quaternion(&m_pPlane->getRotation());
			m_TexCoord = gcnew Vector2(&m_pPlane->getTexCoordMult());

			m_pNode = m_pPlane;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Plane";
			buffer << s_nPlaneCount;
			wstring text = buffer.str();
			
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pPlane);

			this->Name = "Plane" + (s_nPlaneCount);
			this->Key = "Plane" + (s_nPlaneCount++);


			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pPlane->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pPlane->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pPlane->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		PrefabPlaneWrapper(PrefabPlane* _Plane, String^ _name):
			NodeVisibleWrapper(_Plane, _name, 0)
		{
			m_pPlane = _Plane;
			s_nPlaneCount++;

			m_TexCoord = gcnew Vector2(&m_pPlane->getTexCoordMult());

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pPlane->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pPlane->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pPlane->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		virtual ~PrefabPlaneWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Plane"),
		DescriptionAttribute("Texture coordinates multiplier")]
		property nGENEToolset::Vector2^ TexCoord
		{
			nGENEToolset::Vector2^ get()
			{
				if(!frmRenderTarget::engine->tryLock())
					return m_TexCoord;

				m_pPlane->setTexCoordMult(m_TexCoord->getVector());

				frmRenderTarget::engine->releaseLock();

				return m_TexCoord;
			}

			void set(nGENEToolset::Vector2^ value)
			{
				m_TexCoord->x = value->x;
				m_TexCoord->y = value->y;
			}
		}

		[Browsable(true), CategoryAttribute("Plane"),
		DescriptionAttribute("Plane's side length")]
		property float SideLength
		{
			float get()
			{
				return m_pPlane->getSideLength();
			}

			void set(float value)
			{
				m_pPlane->setSideLength(value);
			}
		}
	};
}


#endif