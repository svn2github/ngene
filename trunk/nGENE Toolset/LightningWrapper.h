/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LightningWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/


#ifndef __INC_LIGHTNINGWRAPPER_H_
#define __INC_LIGHTNINGWRAPPER_H_


#include "nGENE.h"
#include "NodeLightning.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	using nGENE::Nature::NodeLightning;

	
	/// Wrapper for a NodeLightning class.
	ref class LightningWrapper: public NodeVisibleWrapper
	{
	protected:
		NodeLightning* m_pLightning;

		Color m_BoltColour;

		nGENEToolset::Vector3^ startPoint;
		nGENEToolset::Vector3^ endPoint;

		static int s_nLightningCount;

	public:
		LightningWrapper()
		{
			frmRenderTarget::engine->getLock();

			SceneManager* sm = Engine::getSingleton().getSceneManager(0);

			m_pLightning = new NodeLightning();
			m_pLightning->setCreator(sm);
			m_pLightning->init();
			m_pLightning->setPosition(0.0f, 0.0f, 0.0f);

			m_Position = gcnew Vector3(&m_pLightning->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pLightning->getScale());
			m_Rotation = gcnew Quaternion(&m_pLightning->getRotation());

			m_pNode = m_pLightning;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			nGENE::Colour& col = m_pLightning->getColour();
			m_BoltColour.FromArgb(col.getRed(), col.getGreen(), col.getBlue());

			wostringstream buffer;
			buffer << "Lightning";
			buffer << s_nLightningCount;
			wstring text = buffer.str();

			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pLightning);

			
			startPoint = gcnew nGENEToolset::Vector3(&m_pLightning->getStartPoint());
			endPoint = gcnew nGENEToolset::Vector3(&m_pLightning->getEndPoint());

			this->Name = "Lightning" + (s_nLightningCount);
			this->Key = "Lightning" + (s_nLightningCount++);


			frmSceneGraph::graph->addNode(this);

			frmRenderTarget::engine->releaseLock();
		}

		LightningWrapper(NodeLightning* _Lightning, String^ _name):
			NodeVisibleWrapper(_Lightning, _name, 0)
		{
			m_pLightning = _Lightning;
			s_nLightningCount++;

			startPoint = gcnew nGENEToolset::Vector3(&m_pLightning->getStartPoint());
			endPoint = gcnew nGENEToolset::Vector3(&m_pLightning->getEndPoint());
		}

		virtual ~LightningWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Where the lightning begins")]
		property nGENEToolset::Vector3^ StartPoint
		{
			nGENEToolset::Vector3^ get()
			{
				nGENE::Vector3 start(startPoint->x,
									 startPoint->y,
									 startPoint->z);
				m_pLightning->setStartPoint(start);

				return startPoint;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				startPoint->x = value->x;
				startPoint->y = value->y;
				startPoint->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Where the lightning ends")]
		property nGENEToolset::Vector3^ EndPoint
		{
			nGENEToolset::Vector3^ get()
			{
				nGENE::Vector3 endpoint(endPoint->x,
								 endPoint->y,
								 endPoint->z);
				m_pLightning->setEndPoint(endpoint);

				return endPoint;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				endPoint->x = value->x;
				endPoint->y = value->y;
				endPoint->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Number of algorithm iterations")]
		property uint GenerationsCount
		{
			uint get()
			{
				return m_pLightning->getSegmentsNum();
			}

			void set(uint value)
			{
				m_pLightning->setSegmentsNum(value);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("How often lightning will be updated (in milliseconds)")]
		property dword UpdateFrequency
		{
			dword get()
			{
				return m_pLightning->getUpdateFrequency();
			}

			void set(dword value)
			{
				m_pLightning->setUpdateFrequency(value);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("What is the pause between first and second hit (in milliseconds)")]
		property dword Delay
		{
			dword get()
			{
				return m_pLightning->getPause();
			}

			void set(dword value)
			{
				m_pLightning->setPause(value);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Number of lightning branches")]
		property uint BranchesCount
		{
			uint get()
			{
				return m_pLightning->getBranchesNum();
			}

			void set(uint value)
			{
				m_pLightning->setBranchesNum(value);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Maximum number of runs")]
		property uint RunsCount
		{
			uint get()
			{
				return m_pLightning->getNumberOfRuns();
			}

			void set(uint value)
			{
				m_pLightning->setNumberOfRuns(value);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Transparency of the second bolt")]
		property Real Alpha
		{
			Real get()
			{
				return m_pLightning->getBolt2Alpha();
			}

			void set(Real value)
			{
				m_pLightning->setBolt2Alpha(value);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Maximum offset")]
		property Real Offset
		{
			Real get()
			{
				return m_pLightning->getOffset();
			}

			void set(Real value)
			{
				m_pLightning->setOffset(value);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Maximum spread of branches")]
		property Real BranchSpread
		{
			Real get()
			{
				return m_pLightning->getBranchSpread();
			}

			void set(Real value)
			{
				m_pLightning->setBranchSpread(value);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Maximum flash strength. Ranges 0 to 1")]
		property Real FlashStrength
		{
			Real get()
			{
				return m_pLightning->getMaxFlash();
			}

			void set(Real value)
			{
				m_pLightning->setMaxFlash(value);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Colour of the lightning bolt")]
		property System::Drawing::Color Colour
		{
			Color get()
			{
				return m_BoltColour;
			}

			void set(Color value)
			{
				m_BoltColour = value;
				nGENE::Colour col((dword)m_BoltColour.ToArgb());
				m_pLightning->setColour(col);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Width of the main branch")]
		property Real Width
		{
			Real get()
			{
				return m_pLightning->getWidth();
			}

			void set(Real value)
			{
				m_pLightning->setWidth(value);
			}
		}

		[Browsable(true), CategoryAttribute("Lightning"),
		DescriptionAttribute("Minimum length of a single branch")]
		property Real MinBranchLength
		{
			Real get()
			{
				return m_pLightning->getMinBranchLength();
			}

			void set(Real value)
			{
				m_pLightning->setMinBranchLength(value);
			}
		}
	};
}


#endif