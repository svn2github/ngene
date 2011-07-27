/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SkyWrapper.h
Version:	0.04
---------------------------------------------------------------------------
*/


#ifndef __INC_SKYWRAPPER_H_
#define __INC_SKYWRAPPER_H_


#include "nGENE.h"
#include "NodeSky.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	using nGENE::Nature::NodeSky;

	
	/// Wrapper for a NodeSky class.
	ref class SkyWrapper: public NodeWrapper
	{
	protected:
		NodeSky* m_pSky;
		nGENEToolset::TimeDate^ time;
		nGENEToolset::TimeDate^ sunrise;
		nGENEToolset::TimeDate^ sunset;


	public:
		SkyWrapper()
		{
			frmRenderTarget::engine->getLock();

			SceneManager* sm = Engine::getSingleton().getSceneManager(0);

			m_pSky = new NodeSky(3500.0f);
			m_pSky->setCreator(sm);
			m_pSky->init();
			m_pSky->setSimulated(false);
			m_pSky->setPosition(0.0f, -13.0f, 0.0f);
			m_pSky->setDayLength(60.0f);

			m_pNode = m_pSky;

			m_Position = gcnew Vector3(&m_pSky->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pSky->getScale());
			m_Rotation = gcnew Quaternion(&m_pSky->getRotation());

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());


			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(L"Sky", m_pSky);

			time = gcnew TimeDate();
			sunrise = gcnew TimeDate(&m_pSky->getSunRiseTime());
			sunset = gcnew TimeDate(&m_pSky->getSunsetTime());

			this->Name = "Sky";
			this->Key = "Sky";


			/// @todo Use singleton instead.
			frmSceneGraph::graph->addNode(this);


			frmRenderTarget::engine->releaseLock();
		}

		SkyWrapper(NodeSky* _sky, String^ _name):
			NodeWrapper(_sky, _name, 0)
		{
			m_pSky = _sky;

			time = gcnew TimeDate(&m_pSky->getDayTime());
			sunrise = gcnew TimeDate(&m_pSky->getSunRiseTime());
			sunset = gcnew TimeDate(&m_pSky->getSunsetTime());
		}

		virtual ~SkyWrapper()
		{
			NGENE_DELETE(m_pSky);
		}

		[Browsable(true), CategoryAttribute("Sky"),
		DescriptionAttribute("Specifies whether day light simulation is taking place")]
		property bool Simulated
		{
			bool get()
			{
				return m_pSky->isSimulated();
			}

			void set(bool value)
			{
				m_pSky->setSimulated(value);
			}
		}

		[Browsable(true), CategoryAttribute("Sky"),
		DescriptionAttribute("Length of a day in seconds. Only valid if Simulated is true")]
		property float DayLength
		{
			float get()
			{
				return m_pSky->getDayLength();
			}

			void set(float value)
			{
				frmRenderTarget::engine->getLock();

				m_pSky->setDayLength(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Sky"),
		DescriptionAttribute("Time of day. Valid only when Simulated is false")]
		property nGENEToolset::TimeDate^ TimeOfDay
		{
			nGENEToolset::TimeDate^ get()
			{
				nGENE::TimeDate tm;
				tm.seconds = time->seconds;
				tm.minutes = time->minutes;
				tm.hours = time->hours;

				frmRenderTarget::engine->getLock();

				m_pSky->setDayTime(tm);

				frmRenderTarget::engine->releaseLock();

				return time;
			}

			void set(nGENEToolset::TimeDate^ value)
			{
				frmRenderTarget::engine->getLock();

				time->seconds = value->seconds;
				time->minutes = value->minutes;
				time->hours = value->hours;

				nGENE::TimeDate tm;
				tm.seconds = time->seconds;
				tm.minutes = time->minutes;
				tm.hours = time->hours;
				m_pSky->setDayTime(tm);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Sky"),
		DescriptionAttribute("Time of sun rise. Valid only when Simulated is false")]
		property nGENEToolset::TimeDate^ TimeOfSunRise
		{
			nGENEToolset::TimeDate^ get()
			{
				nGENE::TimeDate tm;
				tm.seconds = sunrise->seconds;
				tm.minutes = sunrise->minutes;
				tm.hours = sunrise->hours;

				frmRenderTarget::engine->getLock();

				m_pSky->setSunRiseTime(tm);

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

		[Browsable(true), CategoryAttribute("Sky"),
		DescriptionAttribute("Time of sun set. Valid only when Simulated is false")]
		property nGENEToolset::TimeDate^ TimeOfSunset
		{
			nGENEToolset::TimeDate^ get()
			{
				nGENE::TimeDate tm;
				tm.seconds = sunset->seconds;
				tm.minutes = sunset->minutes;
				tm.hours = sunset->hours;

				frmRenderTarget::engine->getLock();

				m_pSky->setSunsetTime(tm);

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

		[Browsable(true), CategoryAttribute("Sky"),
		DescriptionAttribute("Specifies whether sky is fixed or not. If not it will be moving with camera")]
		property bool Fixed
		{
			bool get()
			{
				return m_pSky->isFixed();
			}

			void set(bool value)
			{
				m_pSky->setFixed(value);
			}
		}

		[Browsable(true), CategoryAttribute("Scattering"),
		DescriptionAttribute("Specifies atmosphere turbidity")]
		property float Turbidity
		{
			float get()
			{
				frmRenderTarget::engine->getLock();

				m_pSky->getScattering()->setTurbidity(m_pSky->getScattering()->getTurbidity());

				frmRenderTarget::engine->releaseLock();

				return m_pSky->getScattering()->getTurbidity();
			}

			void set(float value)
			{
				frmRenderTarget::engine->getLock();

				m_pSky->getScattering()->setTurbidity(value);
				m_pSky->requestUpdate();

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Scattering"),
		DescriptionAttribute("Specifies exposure")]
		property float Exposure
		{
			float get()
			{
				frmRenderTarget::engine->getLock();

				m_pSky->getScattering()->setExposure(m_pSky->getScattering()->getExposure());

				frmRenderTarget::engine->releaseLock();

				return m_pSky->getScattering()->getExposure();
			}

			void set(float value)
			{
				frmRenderTarget::engine->getLock();

				m_pSky->getScattering()->setExposure(value);
				m_pSky->requestUpdate();

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Sun"),
		DescriptionAttribute("Specifies sun size. Keep it relatively small.")]
		property float SunSize
		{
			float get()
			{
				return m_pSky->getSunSize();
			}

			void set(float value)
			{
				frmRenderTarget::engine->getLock();

				m_pSky->setSunSize(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Sun"),
		DescriptionAttribute("Specifies sun brightness")]
		property float SunBrightness
		{
			float get()
			{
				return m_pSky->getSunBrightness();
			}

			void set(float value)
			{
				frmRenderTarget::engine->getLock();

				m_pSky->setSunBrightness(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Scattering"),
		DescriptionAttribute("Specifies in-scattering")]
		property float Inscattering
		{
			float get()
			{
				frmRenderTarget::engine->getLock();

				m_pSky->getScattering()->setInscattering(m_pSky->getScattering()->getInscattering());

				frmRenderTarget::engine->releaseLock();

				return m_pSky->getScattering()->getInscattering();
			}

			void set(float value)
			{
				frmRenderTarget::engine->getLock();

				m_pSky->getScattering()->setInscattering(value);
				m_pSky->requestUpdate();

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Shadow"),
		DescriptionAttribute("Specifies resolution of shadow map texture for this light")]
		property uint ShadowMapResolution
		{
			uint get()
			{
				return m_pSky->getLight()->getShadowResolution();
			}

			void set(uint _value)
			{
				m_pSky->getLight()->setShadowResolution(_value);
			}
		}

		[Browsable(true), CategoryAttribute("Shadow"),
		DescriptionAttribute("Specifies whether shadows are enabled from sun light")]
		property bool CastShadows
		{
			bool get()
			{
				return m_pSky->isCastingShadows();
			}

			void set(bool _value)
			{
				m_pSky->setCastShadows(_value);
			}
		}
	};
}


#endif