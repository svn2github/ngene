/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabCylinderWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_PREFABCYLINDERWRAPPER_H_
#define __INC_PREFABCYLINDERWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	
	/// Wrapper for a PrefabCylinder class.
	ref class PrefabCylinderWrapper: public NodeVisibleWrapper
	{
	protected:
		PrefabCylinder* m_pCylinder;
		static int s_nCylinderCount;

		nGENEToolset::Vector2^ m_TexCoord;
		
	public:
		PrefabCylinderWrapper()
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pCylinder = sm->createCylinder();
			m_pCylinder->setPosition(0.0f, 0.0f, 0.0f);
			Surface* pSurface = m_pCylinder->getSurface(L"Base");
			Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
			pSurface->setMaterial(pMaterial);

			m_Position = gcnew Vector3(&m_pCylinder->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pCylinder->getScale());
			m_Rotation = gcnew Quaternion(&m_pCylinder->getRotation());
			m_TexCoord = gcnew Vector2(&m_pCylinder->getTexCoordMult());

			m_pNode = m_pCylinder;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Cylinder";
			buffer << s_nCylinderCount;
			wstring text = buffer.str();
			
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pCylinder);

			this->Name = "Cylinder" + (s_nCylinderCount);
			this->Key = "Cylinder" + (s_nCylinderCount++);

			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pCylinder->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pCylinder->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pCylinder->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		PrefabCylinderWrapper(PrefabCylinder* _Cylinder, String^ _name):
			NodeVisibleWrapper(_Cylinder, _name, 0)
		{
			m_pCylinder = _Cylinder;
			s_nCylinderCount++;

			m_TexCoord = gcnew Vector2(&m_pCylinder->getTexCoordMult());

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pCylinder->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pCylinder->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pCylinder->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		virtual ~PrefabCylinderWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Cylinder"),
		DescriptionAttribute("Specifies cylinder radius")]
		property float Radius
		{
			float get()
			{
				return m_pCylinder->getRadius();
			}

			void set(float value)
			{
				m_pCylinder->setRadius(value);
			}
		}

		[Browsable(true), CategoryAttribute("Cylinder"),
		DescriptionAttribute("Specifies cylinder height")]
		property float Height
		{
			float get()
			{
				return m_pCylinder->getHeight();
			}

			void set(float value)
			{
				m_pCylinder->setHeight(value);
			}
		}

		[Browsable(true), CategoryAttribute("Cylinder"),
		DescriptionAttribute("Texture coordinates multiplier")]
		property nGENEToolset::Vector2^ TexCoord
		{
			nGENEToolset::Vector2^ get()
			{
				if(!frmRenderTarget::engine->tryLock())
					return m_TexCoord;

				m_pCylinder->setTexCoordMult(m_TexCoord->getVector());

				frmRenderTarget::engine->releaseLock();

				return m_TexCoord;
			}

			void set(nGENEToolset::Vector2^ value)
			{
				m_TexCoord->x = value->x;
				m_TexCoord->y = value->y;
			}
		}

		[Browsable(true), CategoryAttribute("Cylinder"),
		DescriptionAttribute("Number of slices")]
		property uint Slices
		{
			uint get()
			{
				return m_pCylinder->getSlices();
			}

			void set(uint value)
			{
				m_pCylinder->setSlices(value);
			}
		}

		[Browsable(true), CategoryAttribute("Cylinder"),
		DescriptionAttribute("Number of stacks")]
		property uint Stacks
		{
			uint get()
			{
				return m_pCylinder->getStacks();
			}

			void set(uint value)
			{
				m_pCylinder->setStacks(value);
			}
		}

		[Browsable(true), CategoryAttribute("Cylinder"),
		DescriptionAttribute("Specifies if cylinder is a tube (when false) or not (when true)")]
		property bool Closed
		{
			bool get()
			{
				return m_pCylinder->isClosed();
			}

			void set(bool value)
			{
				m_pCylinder->setClosed(value);
			}
		}
	};
}


#endif