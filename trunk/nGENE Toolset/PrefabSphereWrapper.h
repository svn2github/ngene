/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabSphereWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_PREFABSPHEREWRAPPER_H_
#define __INC_PREFABSPHEREWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	
	/// Wrapper for a PrefabSphere class.
	ref class PrefabSphereWrapper: public NodeVisibleWrapper
	{
	protected:
		PrefabSphere* m_pSphere;
		static int s_nSphereCount;

		nGENEToolset::Vector2^ m_TexCoord;
		
	public:
		PrefabSphereWrapper()
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pSphere = sm->createSphere();
			m_pSphere->setPosition(0.0f, 0.0f, 0.0f);
			Surface* pSurface = m_pSphere->getSurface(L"Base");
			Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
			pSurface->setMaterial(pMaterial);

			m_Position = gcnew Vector3(&m_pSphere->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pSphere->getScale());
			m_Rotation = gcnew Quaternion(&m_pSphere->getRotation());
			m_TexCoord = gcnew Vector2(&m_pSphere->getTexCoordMult());

			m_pNode = m_pSphere;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Sphere";
			buffer << s_nSphereCount;
			wstring text = buffer.str();
			
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pSphere);

			this->Name = "Sphere" + (s_nSphereCount);
			this->Key = "Sphere" + (s_nSphereCount++);


			/// @todo Use singleton instead.
			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pSphere->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pSphere->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pSphere->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		PrefabSphereWrapper(PrefabSphere* _sphere, String^ _name):
			NodeVisibleWrapper(_sphere, _name, 0)
		{
			m_pSphere = _sphere;
			s_nSphereCount++;

			m_TexCoord = gcnew Vector2(&m_pSphere->getTexCoordMult());

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pSphere->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pSphere->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pSphere->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		virtual ~PrefabSphereWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Sphere"),
		DescriptionAttribute("Specifies sphere radius")]
		property float Radius
		{
			float get()
			{
				return m_pSphere->getRadius();
			}

			void set(float value)
			{
				m_pSphere->setRadius(value);
			}
		}

		[Browsable(true), CategoryAttribute("Sphere"),
		DescriptionAttribute("Texture coordinates multiplier")]
		property nGENEToolset::Vector2^ TexCoord
		{
			nGENEToolset::Vector2^ get()
			{
				if(!frmRenderTarget::engine->tryLock())
					return m_TexCoord;

				m_pSphere->setTexCoordMult(m_TexCoord->getVector());

				frmRenderTarget::engine->releaseLock();

				return m_TexCoord;
			}

			void set(nGENEToolset::Vector2^ value)
			{
				m_TexCoord->x = value->x;
				m_TexCoord->y = value->y;
			}
		}

		[Browsable(true), CategoryAttribute("Sphere"),
		DescriptionAttribute("Number of slices")]
		property uint Slices
		{
			uint get()
			{
				return m_pSphere->getSlices();
			}

			void set(uint value)
			{
				m_pSphere->setSlices(value);
			}
		}

		[Browsable(true), CategoryAttribute("Sphere"),
		DescriptionAttribute("Number of stacks")]
		property uint Stacks
		{
			uint get()
			{
				return m_pSphere->getStacks();
			}

			void set(uint value)
			{
				m_pSphere->setStacks(value);
			}
		}

		[Browsable(true), CategoryAttribute("Sphere"),
		DescriptionAttribute("Phi angle")]
		property Real Phi
		{
			Real get()
			{
				return m_pSphere->getPhi();
			}

			void set(Real value)
			{
				m_pSphere->setPhi(value);
			}
		}
	};
}


#endif