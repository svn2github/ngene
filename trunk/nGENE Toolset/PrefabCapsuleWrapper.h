/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabCapsuleWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_PREFABCAPSULEWRAPPER_H_
#define __INC_PREFABCAPSULEWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	
	/// Wrapper for a PrefabCapsule class.
	ref class PrefabCapsuleWrapper: public NodeVisibleWrapper
	{
	protected:
		PrefabCapsule* m_pCapsule;
		static int s_nCapsuleCount;

		nGENEToolset::Vector2^ m_TexCoord;

	public:
		PrefabCapsuleWrapper()
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pCapsule = sm->createCapsule(10, 10, 1.0f, 1.0f);
			m_pCapsule->setPosition(0.0f, 0.0f, 0.0f);
			Surface* pSurface = m_pCapsule->getSurface(L"Base");
			Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
			pSurface->setMaterial(pMaterial);

			m_Position = gcnew Vector3(&m_pCapsule->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pCapsule->getScale());
			m_Rotation = gcnew Quaternion(&m_pCapsule->getRotation());
			m_TexCoord = gcnew Vector2(&m_pCapsule->getTexCoordMult());

			m_pNode = m_pCapsule;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Capsule";
			buffer << s_nCapsuleCount;
			wstring text = buffer.str();
			
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pCapsule);

			this->Name = "Capsule" + (s_nCapsuleCount);
			this->Key = "Capsule" + (s_nCapsuleCount++);


			/// @todo Use singleton instead.
			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pCapsule->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pCapsule->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pCapsule->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		PrefabCapsuleWrapper(PrefabCapsule* _capsule, String^ _name):
			NodeVisibleWrapper(_capsule, _name, 0)
		{
			m_pCapsule = _capsule;
			s_nCapsuleCount++;

			m_TexCoord = gcnew Vector2(&m_pCapsule->getTexCoordMult());

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pCapsule->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pCapsule->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pCapsule->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		virtual ~PrefabCapsuleWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Capsule"),
		DescriptionAttribute("Specifies capsule radius")]
		property float Radius
		{
			float get()
			{
				return m_pCapsule->getRadius();
			}

			void set(float value)
			{
				m_pCapsule->setRadius(value);
			}
		}

		[Browsable(true), CategoryAttribute("Capsule"),
		DescriptionAttribute("Texture coordinates multiplier")]
		property nGENEToolset::Vector2^ TexCoord
		{
			nGENEToolset::Vector2^ get()
			{
				if(!frmRenderTarget::engine->tryLock())
					return m_TexCoord;

				m_pCapsule->setTexCoordMult(m_TexCoord->getVector());

				frmRenderTarget::engine->releaseLock();

				return m_TexCoord;
			}

			void set(nGENEToolset::Vector2^ value)
			{
				m_TexCoord->x = value->x;
				m_TexCoord->y = value->y;
			}
		}

		[Browsable(true), CategoryAttribute("Capsule"),
		DescriptionAttribute("Number of slices")]
		property uint Slices
		{
			uint get()
			{
				return m_pCapsule->getSlices();
			}

			void set(uint value)
			{
				m_pCapsule->setSlices(value);
			}
		}

		[Browsable(true), CategoryAttribute("Capsule"),
		DescriptionAttribute("Number of stacks")]
		property uint Stacks
		{
			uint get()
			{
				return m_pCapsule->getStacks();
			}

			void set(uint value)
			{
				m_pCapsule->setStacks(value);
			}
		}

		[Browsable(true), CategoryAttribute("Capsule"),
		DescriptionAttribute("Capsule height")]
		property Real Height
		{
			Real get()
			{
				return m_pCapsule->getHeight();
			}

			void set(Real value)
			{
				m_pCapsule->setHeight(value);
			}
		}
	};
}


#endif