/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabTorusWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_PREFABTORUSWRAPPER_H_
#define __INC_PREFABTORUSWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	
	/// Wrapper for a PrefabTorus class.
	ref class PrefabTorusWrapper: public NodeVisibleWrapper
	{
	protected:
		PrefabTorus* m_pTorus;
		static int s_nTorusCount;

		nGENEToolset::Vector2^ m_TexCoord;

	public:
		PrefabTorusWrapper()
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pTorus = sm->createTorus(10, 10, 0.25f, 1.0f);
			m_pTorus->setPosition(0.0f, 0.0f, 0.0f);
			Surface* pSurface = m_pTorus->getSurface(L"Base");
			Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
			pSurface->setMaterial(pMaterial);

			m_Position = gcnew Vector3(&m_pTorus->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pTorus->getScale());
			m_Rotation = gcnew Quaternion(&m_pTorus->getRotation());
			m_TexCoord = gcnew Vector2(&m_pTorus->getTexCoordMult());

			m_pNode = m_pTorus;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Torus";
			buffer << s_nTorusCount;
			wstring text = buffer.str();
			
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pTorus);

			this->Name = "Torus" + (s_nTorusCount);
			this->Key = "Torus" + (s_nTorusCount++);


			/// @todo Use singleton instead.
			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pTorus->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pTorus->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pTorus->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		PrefabTorusWrapper(PrefabTorus* _Torus, String^ _name):
			NodeVisibleWrapper(_Torus, _name, 0)
		{
			m_pTorus = _Torus;
			s_nTorusCount++;

			m_TexCoord = gcnew Vector2(&m_pTorus->getTexCoordMult());

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pTorus->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pTorus->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pTorus->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		virtual ~PrefabTorusWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Torus"),
		DescriptionAttribute("Specifies torus inner radius")]
		property float InnerRadius
		{
			float get()
			{
				return m_pTorus->getInnerRadius();
			}

			void set(float value)
			{
				m_pTorus->setInnerRadius(value);
			}
		}

		[Browsable(true), CategoryAttribute("Torus"),
		DescriptionAttribute("Specifies torus outer radius")]
		property float OuterRadius
		{
			float get()
			{
				return m_pTorus->getOuterRadius();
			}

			void set(float value)
			{
				m_pTorus->setOuterRadius(value);
			}
		}

		[Browsable(true), CategoryAttribute("Torus"),
		DescriptionAttribute("Specifies torus slices along perimeter")]
		property uint PerimeterSlices
		{
			uint get()
			{
				return m_pTorus->getCircleSlices();
			}

			void set(uint value)
			{
				m_pTorus->setCircleSlices(value);
			}
		}

		[Browsable(true), CategoryAttribute("Torus"),
		DescriptionAttribute("Specifies torus slices along radius")]
		property uint RadialSlices
		{
			uint get()
			{
				return m_pTorus->getRadialSlices();
			}

			void set(uint value)
			{
				m_pTorus->setRadialSlices(value);
			}
		}

		[Browsable(true), CategoryAttribute("Torus"),
		DescriptionAttribute("Texture coordinates multiplier")]
		property nGENEToolset::Vector2^ TexCoord
		{
			nGENEToolset::Vector2^ get()
			{
				if(!frmRenderTarget::engine->tryLock())
					return m_TexCoord;

				m_pTorus->setTexCoordMult(m_TexCoord->getVector());

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