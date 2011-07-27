/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		FurWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_FURWRAPPER_H_
#define __INC_FURWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"
#include "NodeFur.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	using nGENE::Nature::NodeFur;

	
	/// Wrapper for a NodeFur class.
	ref class FurWrapper: public NodeVisibleWrapper
	{
	protected:
		NodeFur* m_pFur;
		static int s_nFurCount;

	public:
		FurWrapper(SurfaceWrapper^ _surface)
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pFur = new NodeFur();
			m_pFur->setCreator(sm);
			m_pFur->init();
			Surface surface(*_surface->getSurface());
			m_pFur->addSurface(surface.getName(), surface);

			m_pFur->setPosition(0.0f, 0.0f, 0.0f);

			m_Position = gcnew Vector3(&m_pFur->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pFur->getScale());
			m_Rotation = gcnew Quaternion(&m_pFur->getRotation());

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			m_pNode = m_pFur;

			wostringstream buffer;
			buffer << "Fur";
			buffer << s_nFurCount;
			wstring text = buffer.str();
			
			frmSceneGraph::graph->selectParent();
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pFur);

			this->Name = "Fur" + (s_nFurCount);
			this->Key = "Fur" + (s_nFurCount++);


			/// @todo Use singleton instead.
			frmSceneGraph::graph->addNode(this);

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pFur->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pFur->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pFur->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		FurWrapper(NodeFur* _fur, String^ _name):
			NodeVisibleWrapper(_fur, _name, 0)
		{
			m_pFur = _fur;
			s_nFurCount++;

			frmSceneGraph::graph->beginAdd();
			for(uint i = 0; i < m_pFur->getSurfacesNum(); ++i)
			{
				String^ str = gcnew String(m_pFur->getSurface(i)->getName().c_str());
				SurfaceWrapper^ surf = gcnew SurfaceWrapper(m_pFur->getSurface(i), str);
			}
			frmSceneGraph::graph->endAdd();
		}

		virtual ~FurWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Fur"),
		DescriptionAttribute("Specifies fur length")]
		property float Length
		{
			float get()
			{
				return m_pFur->getFurLength();
			}

			void set(float value)
			{
				m_pFur->setFurLength(value);
			}
		}

		[Browsable(true), CategoryAttribute("Fur"),
		DescriptionAttribute("Specifies number of fur layers")]
		property uint LayersNum
		{
			uint get()
			{
				return m_pFur->getLayersNum();
			}

			void set(uint value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pFur->setLayersNum(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Fur"),
		DescriptionAttribute("Specifies fur texture dimension")]
		property uint TextureSize
		{
			uint get()
			{
				return m_pFur->getTextureSize();
			}

			void set(uint value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pFur->setTextureSize(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Fur"),
		DescriptionAttribute("Specifies fur density")]
		property uint Density
		{
			uint get()
			{
				return m_pFur->getFurDensity();
			}

			void set(uint value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pFur->setFurDensity(value);

				frmRenderTarget::engine->releaseLock();
			}
		}
	};
}


#endif