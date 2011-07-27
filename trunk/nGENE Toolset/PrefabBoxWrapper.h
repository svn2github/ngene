/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabBoxWrapper.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABBOXWRAPPER_H_
#define __INC_PREFABBOXWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	
	/// Wrapper for a PrefabBox class.
	ref class PrefabBoxWrapper: public NodeVisibleWrapper
	{
	protected:
		PrefabBox* m_pBox;
		static int s_nBoxCount;

		nGENEToolset::Vector2^ m_TexCoord;

	public:
		PrefabBoxWrapper()
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pBox = sm->createBox(1.0f, 1.0f, 1.0f);
			m_pBox->setPosition(0.0f, 0.0f, 0.0f);
			Surface* pSurface = m_pBox->getSurface(L"Base");
			Material* pMaterial = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"normalMap");
			pSurface->setMaterial(pMaterial);

			m_Position = gcnew Vector3(&m_pBox->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pBox->getScale());
			m_Rotation = gcnew Quaternion(&m_pBox->getRotation());
			m_TexCoord = gcnew Vector2(&m_pBox->getTexCoordMult());

			m_pNode = m_pBox;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Box";
			buffer << s_nBoxCount;
			wstring text = buffer.str();
			
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pBox);

			this->Name = "Box" + (s_nBoxCount);
			this->Key = "Box" + (s_nBoxCount++);


			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pBox->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pBox->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pBox->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		PrefabBoxWrapper(PrefabBox* _box, String^ _name):
			NodeVisibleWrapper(_box, _name, 0)
		{
			m_pBox = _box;
			s_nBoxCount++;

			m_TexCoord = gcnew Vector2(&m_pBox->getTexCoordMult());

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pBox->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pBox->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pBox->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		virtual ~PrefabBoxWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Box"),
		DescriptionAttribute("Texture coordinates multiplier")]
		property nGENEToolset::Vector2^ TexCoord
		{
			nGENEToolset::Vector2^ get()
			{
				if(!frmRenderTarget::engine->tryLock())
					return m_TexCoord;

				m_pBox->setTexCoordMult(m_TexCoord->getVector());

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