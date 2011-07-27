/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		WaterWrapper.h
Version:	0.02
---------------------------------------------------------------------------
*/


#ifndef __INC_WATERWRAPPER_H_
#define __INC_WATERWRAPPER_H_


#include "nGENE.h"
#include "NodeWater.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	using nGENE::Nature::NodeWater;

	
	/// Wrapper for a NodeWater class.
	ref class WaterWrapper: public NodeWrapper
	{
	protected:
		NodeWater* m_pWater;

		nGENEToolset::TimeDate^ time;
		nGENEToolset::TimeDate^ sunrise;
		nGENEToolset::TimeDate^ sunset;

		nGENEToolset::Vector2^ m_Wind;
		nGENEToolset::Vector2^ m_WavesScale;

		nGENEToolset::Vector3^ m_Foam;
		nGENEToolset::Vector3^ m_Extinction;

		Color m_DepthColor;
		Color m_BigDepthColor;

	public:
		WaterWrapper()
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);

			m_pWater = new NodeWater();
			m_pWater->setCreator(sm);
			m_pWater->init();
			m_pWater->setSimulated(false);
			m_pWater->setPosition(0.0f, 0.0f, 0.0f);
			m_pWater->setDayLength(60.0f);

			m_pNode = m_pWater;

			m_Position = gcnew Vector3(&m_pWater->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pWater->getScale());

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());


			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(L"Water", m_pWater);

			this->Name = "Water";
			this->Key = "Water";

			time = gcnew TimeDate();
			sunrise = gcnew TimeDate(&m_pWater->getSunRiseTime());
			sunset = gcnew TimeDate(&m_pWater->getSunsetTime());

			m_Wind = gcnew Vector2(&m_pWater->getWind());
			m_WavesScale = gcnew Vector2(&m_pWater->getWavesScale());

			m_Foam = gcnew Vector3(&m_pWater->getFoam());
			m_Extinction = gcnew Vector3(&m_pWater->getExtinction());


			frmSceneGraph::graph->addNode(this);
		}

		WaterWrapper(NodeWater* _water, String^ _name):
			NodeWrapper(_water, _name, 0)
		{
			m_pWater = _water;

			time = gcnew TimeDate(&m_pWater->getDayTime());
			sunrise = gcnew TimeDate(&m_pWater->getSunRiseTime());
			sunset = gcnew TimeDate(&m_pWater->getSunsetTime());

			m_Wind = gcnew Vector2(&m_pWater->getWind());
			m_WavesScale = gcnew Vector2(&m_pWater->getWavesScale());

			m_Foam = gcnew Vector3(&m_pWater->getFoam());
			m_Extinction = gcnew Vector3(&m_pWater->getExtinction());
		}

		virtual ~WaterWrapper()
		{
			NGENE_DELETE(m_pWater);
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Specifies whether day light simulation is taking place")]
		property bool Simulated
		{
			bool get()
			{
				return m_pWater->isSimulated();
			}

			void set(bool value)
			{
				m_pWater->setSimulated(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Length of a day in seconds. Only valid if Simulated is true")]
		property float DayLength
		{
			float get()
			{
				return m_pWater->getDayLength();
			}

			void set(float value)
			{
				m_pWater->setDayLength(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Horizontal water transparency")]
		property float HorizontalTransparency
		{
			float get()
			{
				return m_pWater->getHorizontalVisibility();
			}

			void set(float value)
			{
				m_pWater->setHorizontalVisibility(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Maximum waves height")]
		property float WavesAmplitude
		{
			float get()
			{
				return m_pWater->getWavesAmplitude();
			}

			void set(float value)
			{
				m_pWater->setWavesAmplitude(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Reflection displacement along normal vector")]
		property float Displacement
		{
			float get()
			{
				return m_pWater->getDisplacement();
			}

			void set(float value)
			{
				m_pWater->setDisplacement(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Index of refraction")]
		property float RefractionIndex
		{
			float get()
			{
				return m_pWater->getRefractionIndex();
			}

			void set(float value)
			{
				m_pWater->setRefractionIndex(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Muddiness of water")]
		property float Muddiness
		{
			float get()
			{
				return m_pWater->getMuddiness();
			}

			void set(float value)
			{
				m_pWater->setMuddiness(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Specify larger values to empasize refraction over reflection")]
		property float RefractionStrength
		{
			float get()
			{
				return m_pWater->getRefractionStrength();
			}

			void set(float value)
			{
				m_pWater->setRefractionStrength(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Refraction displacement strength")]
		property float RefractionScale
		{
			float get()
			{
				return m_pWater->getRefractionScale();
			}

			void set(float value)
			{
				m_pWater->setRefractionScale(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Specify larger values to have sharper water edges, and smaller for softer")]
		property float ShoreHardness
		{
			float get()
			{
				return m_pWater->getShoreHardness();
			}

			void set(float value)
			{
				m_pWater->setShoreHardness(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Water shininess")]
		property float Shininess
		{
			float get()
			{
				return m_pWater->getShininess();
			}

			void set(float value)
			{
				m_pWater->setShininess(value);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Wind strength")]
		property nGENEToolset::Vector2^ Wind
		{
			nGENEToolset::Vector2^ get()
			{
				m_pWater->setWind(nGENE::Vector2(m_Wind->x, m_Wind->y));

				return m_Wind;
			}

			void set(nGENEToolset::Vector2^ value)
			{
				m_Wind->x = value->x;
				m_Wind->y = value->y;
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Time of day. Valid only when Simulated is false")]
		property nGENEToolset::TimeDate^ TimeOfDay
		{
			nGENEToolset::TimeDate^ get()
			{
				nGENE::TimeDate tm;
				tm.seconds = time->seconds;
				tm.minutes = time->minutes;
				tm.hours = time->hours;
				m_pWater->setDayTime(tm);

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
				m_pWater->setDayTime(tm);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Time of sun rise. Valid only when Simulated is false")]
		property nGENEToolset::TimeDate^ TimeOfSunRise
		{
			nGENEToolset::TimeDate^ get()
			{
				nGENE::TimeDate tm;
				tm.seconds = sunrise->seconds;
				tm.minutes = sunrise->minutes;
				tm.hours = sunrise->hours;
				m_pWater->setSunRiseTime(tm);

				return sunrise;
			}

			void set(nGENEToolset::TimeDate^ value)
			{
				sunrise->seconds = value->seconds;
				sunrise->minutes = value->minutes;
				sunrise->hours = value->hours;
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Time of sun set. Valid only when Simulated is false")]
		property nGENEToolset::TimeDate^ TimeOfSunset
		{
			nGENEToolset::TimeDate^ get()
			{
				nGENE::TimeDate tm;
				tm.seconds = sunset->seconds;
				tm.minutes = sunset->minutes;
				tm.hours = sunset->hours;
				m_pWater->setSunsetTime(tm);

				return sunset;
			}

			void set(nGENEToolset::TimeDate^ value)
			{
				sunset->seconds = value->seconds;
				sunset->minutes = value->minutes;
				sunset->hours = value->hours;
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Scale of waves, set to smaller values to have larger waves")]
		property nGENEToolset::Vector2^ WavesScale
		{
			nGENEToolset::Vector2^ get()
			{
				m_pWater->setWavesScale(nGENE::Vector2(m_WavesScale->x, m_WavesScale->y));

				return m_WavesScale;
			}

			void set(nGENEToolset::Vector2^ value)
			{
				m_WavesScale->x = value->x;
				m_WavesScale->y = value->y;
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Existence of foam")]
		property nGENEToolset::Vector3^ Foam
		{
			nGENEToolset::Vector3^ get()
			{
				m_pWater->setFoam(nGENE::Vector3(m_Foam->x,
								  m_Foam->y,
								  m_Foam->z));

				return m_Foam;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Foam->x = value->x;
				m_Foam->y = value->y;
				m_Foam->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Specifies how fast each component of RGB colour will fade out")]
		property nGENEToolset::Vector3^ Extinction
		{
			nGENEToolset::Vector3^ get()
			{
				m_pWater->setExtinction(nGENE::Vector3(m_Extinction->x,
								  m_Extinction->y,
								  m_Extinction->z));

				return m_Extinction;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Extinction->x = value->x;
				m_Extinction->y = value->y;
				m_Extinction->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Colour of the water surface")]
		property System::Drawing::Color SurfaceColour
		{
			Color get()
			{
				return m_DepthColor;
			}

			void set(Color value)
			{
				m_DepthColor = value;
				nGENE::Colour col((dword)m_DepthColor.ToArgb());
				m_pWater->setSurfaceColour(col);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Colour of the depth")]
		property System::Drawing::Color BigDepthColour
		{
			Color get()
			{
				return m_BigDepthColor;
			}

			void set(Color value)
			{
				m_BigDepthColor = value;
				nGENE::Colour col((dword)m_BigDepthColor.ToArgb());
				m_pWater->setDepthColour(col);
			}
		}

		[Browsable(true), CategoryAttribute("Water"),
		DescriptionAttribute("Normal modifier")]
		property Real NormalScale
		{
			Real get()
			{
				return m_pWater->getNormalScale();
			}

			void set(Real value)
			{
				m_pWater->setNormalScale(value);
			}
		}
	};
}


#endif