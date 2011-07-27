/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabPyramidWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_PREFABPYRAMIDWRAPPER_H_
#define __INC_PREFABPYRAMIDWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	
	/// Wrapper for a PrefabPyramid class.
	ref class PrefabPyramidWrapper: public NodeVisibleWrapper
	{
	protected:
		PrefabPyramid* m_pPyramid;
		static int s_nPyramidCount;

		nGENEToolset::Vector2^ m_TexCoord;
		
	public:
		PrefabPyramidWrapper()
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pPyramid = sm->createPyramid();
			m_pPyramid->setPosition(0.0f, 0.0f, 0.0f);
			Surface* pSurface = m_pPyramid->getSurface(L"Base");
			Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
			pSurface->setMaterial(pMaterial);

			m_Position = gcnew Vector3(&m_pPyramid->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pPyramid->getScale());
			m_Rotation = gcnew Quaternion(&m_pPyramid->getRotation());
			m_TexCoord = gcnew Vector2(&m_pPyramid->getTexCoordMult());

			m_pNode = m_pPyramid;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Pyramid";
			buffer << s_nPyramidCount;
			wstring text = buffer.str();
			
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pPyramid);

			this->Name = "Pyramid" + (s_nPyramidCount);
			this->Key = "Pyramid" + (s_nPyramidCount++);

			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pPyramid->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pPyramid->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pPyramid->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		PrefabPyramidWrapper(PrefabPyramid* _Pyramid, String^ _name):
			NodeVisibleWrapper(_Pyramid, _name, 0)
		{
			m_pPyramid = _Pyramid;
			s_nPyramidCount++;

			m_TexCoord = gcnew Vector2(&m_pPyramid->getTexCoordMult());

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pPyramid->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pPyramid->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pPyramid->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		virtual ~PrefabPyramidWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Pyramid"),
		DescriptionAttribute("Specifies pyramid height")]
		property float Height
		{
			float get()
			{
				return m_pPyramid->getHeight();
			}

			void set(float value)
			{
				m_pPyramid->setHeight(value);
			}
		}

		[Browsable(true), CategoryAttribute("Pyramid"),
		DescriptionAttribute("Specifies length of pyramid base side")]
		property float SideLength
		{
			float get()
			{
				return m_pPyramid->getSideLength();
			}

			void set(float value)
			{
				m_pPyramid->setSideLength(value);
			}
		}

		[Browsable(true), CategoryAttribute("Pyramid"),
		DescriptionAttribute("Texture coordinates multiplier")]
		property nGENEToolset::Vector2^ TexCoord
		{
			nGENEToolset::Vector2^ get()
			{
				if(!frmRenderTarget::engine->tryLock())
					return m_TexCoord;

				m_pPyramid->setTexCoordMult(m_TexCoord->getVector());

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