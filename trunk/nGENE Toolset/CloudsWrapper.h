/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CloudsWrapper.h
Version:	0.03
---------------------------------------------------------------------------
*/


#ifndef __INC_CLOUDSWRAPPER_H_
#define __INC_CLOUDSWRAPPER_H_


#include "nGENE.h"
#include "NodeClouds.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	using nGENE::Nature::NodeClouds;
	using nGENE::Nature::CLOUDS_DESC;

	
	/// Wrapper for a NodeClouds class.
	ref class CloudsWrapper: public NodeVisibleWrapper
	{
	protected:
		NodeClouds* m_pClouds;

		nGENEToolset::TimeDate^ time;
		nGENEToolset::TimeDate^ sunrise;
		nGENEToolset::TimeDate^ sunset;

		static int s_nCloudsCount;

	public:
		CloudsWrapper()
		{
			frmRenderTarget::engine->getLock();

			SceneManager* sm = Engine::getSingleton().getSceneManager(0);

			CLOUDS_DESC cloudsDesc;
			cloudsDesc.domeRadius = 900.0f;
			cloudsDesc.domeHeight = 0.0004375f * 0.3;
			m_pClouds = new NodeClouds(cloudsDesc);
			m_pClouds->setCreator(sm);
			m_pClouds->init();
			m_pClouds->setSimulated(false);
			m_pClouds->setPosition(0.0f, 15.0f, 0.0f);
			m_pClouds->setDayLength(60.0f);

			m_Position = gcnew Vector3(&m_pClouds->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pClouds->getScale());
			m_Rotation = gcnew Quaternion(&m_pClouds->getRotation());

			m_pNode = m_pClouds;

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			wostringstream buffer;
			buffer << "Clouds";
			buffer << s_nCloudsCount;
			wstring text = buffer.str();

			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pClouds);

			time = gcnew TimeDate();
			sunrise = gcnew TimeDate(&m_pClouds->getSunRiseTime());
			sunset = gcnew TimeDate(&m_pClouds->getSunsetTime());

			this->Name = "Clouds" + (s_nCloudsCount);
			this->Key = "Clouds" + (s_nCloudsCount++);


			frmSceneGraph::graph->addNode(this);

			frmRenderTarget::engine->releaseLock();
		}

		CloudsWrapper(NodeClouds* _clouds, String^ _name):
			NodeVisibleWrapper(_clouds, _name, 0)
		{
			m_pClouds = _clouds;
			s_nCloudsCount++;

			time = gcnew TimeDate(&m_pClouds->getDayTime());
			sunrise = gcnew TimeDate(&m_pClouds->getSunRiseTime());
			sunset = gcnew TimeDate(&m_pClouds->getSunsetTime());
		}

		virtual ~CloudsWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Clouds"),
		DescriptionAttribute("Specifies whether clouds node is fixed or not. If not it will be moving with camera")]
		property bool Fixed
		{
			bool get()
			{
				return m_pClouds->isFixed();
			}

			void set(bool value)
			{
				m_pClouds->setFixed(value);
			}
		}

		[Browsable(true), CategoryAttribute("Clouds"),
		DescriptionAttribute("Time of day. Valid only when Simulated is false")]
		property nGENEToolset::TimeDate^ TimeOfDay
		{
			nGENEToolset::TimeDate^ get()
			{
				nGENE::TimeDate tm;
				tm.seconds = time->seconds;
				tm.minutes = time->minutes;
				tm.hours = time->hours;

				if(!frmRenderTarget::engine->tryLock())
					return time;

				m_pClouds->setDayTime(tm);

				frmRenderTarget::engine->releaseLock();

				return time;
			}

			void set(nGENEToolset::TimeDate^ value)
			{
				time->seconds = value->seconds;
				time->minutes = value->minutes;
				time->hours = value->hours;

				nGENE::TimeDate tm;
				tm.seconds = time->seconds;
				tm.minutes = time->minutes;
				tm.hours = time->hours;

				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pClouds->setDayTime(tm);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Clouds"),
		DescriptionAttribute("Time of sun rise. Valid only when Simulated is false")]
		property nGENEToolset::TimeDate^ TimeOfSunRise
		{
			nGENEToolset::TimeDate^ get()
			{
				nGENE::TimeDate tm;
				tm.seconds = sunrise->seconds;
				tm.minutes = sunrise->minutes;
				tm.hours = sunrise->hours;

				if(!frmRenderTarget::engine->tryLock())
					return sunrise;

				m_pClouds->setSunRiseTime(tm);

				frmRenderTarget::engine->releaseLock();

				return sunrise;
			}

			void set(nGENEToolset::TimeDate^ value)
			{
				sunrise->seconds = value->seconds;
				sunrise->minutes = value->minutes;
				sunrise->hours = value->hours;
			}
		}

		[Browsable(true), CategoryAttribute("Clouds"),
		DescriptionAttribute("Time of sun set. Valid only when Simulated is false")]
		property nGENEToolset::TimeDate^ TimeOfSunset
		{
			nGENEToolset::TimeDate^ get()
			{
				nGENE::TimeDate tm;
				tm.seconds = sunset->seconds;
				tm.minutes = sunset->minutes;
				tm.hours = sunset->hours;

				if(!frmRenderTarget::engine->tryLock())
					return sunset;

				m_pClouds->setSunsetTime(tm);

				frmRenderTarget::engine->releaseLock();

				return sunset;
			}

			void set(nGENEToolset::TimeDate^ value)
			{
				sunset->seconds = value->seconds;
				sunset->minutes = value->minutes;
				sunset->hours = value->hours;
			}
		}

		[Browsable(true), CategoryAttribute("Clouds"),
		DescriptionAttribute("Specifies whether day light simulation is taking place")]
		property bool Simulated
		{
			bool get()
			{
				return m_pClouds->isSimulated();
			}

			void set(bool value)
			{
				m_pClouds->setSimulated(value);
			}
		}

		[Browsable(true), CategoryAttribute("Clouds"),
		DescriptionAttribute("Update frequency in milliseconds")]
		property dword UpdateFrequency
		{
			dword get()
			{
				return m_pClouds->getUpdateFrequency();
			}

			void set(dword value)
			{
				m_pClouds->setUpdateFrequency(value);
			}
		}

		[Browsable(true), CategoryAttribute("Clouds"),
		DescriptionAttribute("Length of a day in seconds. Only valid if Simulated is true")]
		property float DayLength
		{
			float get()
			{
				return m_pClouds->getDayLength();
			}

			void set(float value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pClouds->setDayLength(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Clouds shape"),
		DescriptionAttribute("Sharpness of clouds")]
		property float Sharpness
		{
			float get()
			{
				return m_pClouds->getCloudSharpness();
			}

			void set(float value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pClouds->setCloudSharpness(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Clouds shape"),
		DescriptionAttribute("Clouds cover level")]
		property float Cover
		{
			float get()
			{
				return m_pClouds->getCloudCover();
			}

			void set(float value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pClouds->setCloudCover(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Clouds shape"),
		DescriptionAttribute("Clouds density level")]
		property float Density
		{
			float get()
			{
				return m_pClouds->getCloudDensity();
			}

			void set(float value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pClouds->setCloudDensity(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Clouds shape"),
		DescriptionAttribute("Size of the clouds. Smaller values increase size of clouds")]
		property float Size
		{
			float get()
			{
				return m_pClouds->getTextureScaling();
			}

			void set(float value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pClouds->setTextureScaling(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Horizon"),
		DescriptionAttribute("Level of the horizon")]
		property float HorizonLevel
		{
			float get()
			{
				return m_pClouds->getHorizonLevel();
			}

			void set(float value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pClouds->setHorizonLevel(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Horizon"),
		DescriptionAttribute("Height of the horizon")]
		property float HorizonHeight
		{
			float get()
			{
				return m_pClouds->getHorizonHeight();
			}

			void set(float value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pClouds->setHorizonHeight(value);

				frmRenderTarget::engine->releaseLock();
			}
		}
	};
}


#endif