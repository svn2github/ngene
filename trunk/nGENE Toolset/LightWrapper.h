/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LightWrapper.h
Version:	0.05
---------------------------------------------------------------------------
*/

#ifndef __INC_LIGHTWRAPPER_H_
#define __INC_LIGHTWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"
#include "NodeVolumetric.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	using nGENE::Nature::NodeVolumetric;
	
	enum class LightType
	{
		Point,
		Directional,
		Spot
	};


	/// Wrapper for the nGENE NodeLight class.
	ref class LightWrapper: public NodeWrapper
	{
	private:
		NodeLight* m_pLight;				///< Pointer to the nGENE Light Node
		Color m_LightColour;
		nGENEToolset::Vector3^ m_Direction;

		Real m_FOV;							///< FOV for spot-light

		static int s_nLightCount;

	public:
		LightWrapper()
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pNode = sm->createLight(LT_POINT);
			m_pLight = static_cast<NodeLight*>(m_pNode);
			m_pLight->setPickable(false);
			wostringstream buffer;
			buffer << "Light";
			buffer << s_nLightCount;
			wstring text = buffer.str();
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pLight);

			m_Position = gcnew Vector3(&m_pNode->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pNode->getScale());
			m_Direction = gcnew Vector3(&((NodeLight*)m_pNode)->getDirection());
			m_Rotation = gcnew Quaternion(&m_pLight->getRotation());

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());

			this->Name = "Light" + (s_nLightCount);
			this->Key = "Light" + (s_nLightCount++);


			ImageIndex = 3;


			/// @todo Use singleton instead.
			frmSceneGraph::graph->addNode(this);
			Renderer::getSingleton().addLight(m_pLight);

			m_FOV = Maths::radToDeg(m_pLight->getFOV());
		}

		LightWrapper(NodeLight* _light, String^ _name):
			NodeWrapper(_light, _name, 3)
		{
			m_pLight = _light;
			s_nLightCount++;

			m_pLight->setPickable(false);
			
			m_Direction = gcnew Vector3(&m_pLight->getDirection());
			m_FOV = Maths::radToDeg(m_pLight->getFOV());
		}

		~LightWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Light"),
		DescriptionAttribute("Colour of the light")]
		property System::Drawing::Color Colour
		{
			Color get()
			{
				return m_LightColour;
			}

			void set(Color value)
			{
				m_LightColour = value;
				m_pLight->setColour(value.R, value.G, value.B);
			}
		}

		[Browsable(true), CategoryAttribute("Shadow"),
		DescriptionAttribute("Specifies resolution of shadow map texture for this light")]
		property uint ShadowMapResolution
		{
			uint get()
			{
				return m_pLight->getShadowResolution();
			}

			void set(uint _value)
			{
				m_pLight->setShadowResolution(_value);
			}
		}

		[Browsable(true), CategoryAttribute("Shadow"),
		DescriptionAttribute("Density of shadow for this light")]
		property Real ShadowDensity
		{
			Real get()
			{
				return m_pLight->getShadowDensity();
			}

			void set(Real _value)
			{
				m_pLight->setShadowDensity(_value);
			}
		}

		[Browsable(true), CategoryAttribute("Shadow"),
		DescriptionAttribute("Specifies whether light casts shadows")]
		property bool Shadow
		{
			bool get()
			{
				return m_pLight->isCastingShadows();
			}

			void set(bool value)
			{
				m_pLight->setCastShadows(value);
			}
		}

		[Browsable(true), CategoryAttribute("Shadow"),
		DescriptionAttribute("Specifies whether shadows are dynamic")]
		property bool Static
		{
			bool get()
			{
				return m_pLight->hasStaticShadow();
			}

			void set(bool value)
			{
				m_pLight->setStaticShadow(value);
			}
		}

		[Browsable(true), CategoryAttribute("Shadow"),
		DescriptionAttribute("Specifies whether shadow-map belongs only to this light")]
		property bool IndividualShadow
		{
			bool get()
			{
				return m_pLight->hasIndividualShadow();
			}

			void set(bool value)
			{
				m_pLight->setIndividualShadow(value);
			}
		}

		[Browsable(true), CategoryAttribute("Spot"),
		DescriptionAttribute("Spot exponent - only for spot lights")]
		property float SpotExponent
		{
			float get()
			{
				return m_pLight->getSpotExponent();
			}

			void set(float value)
			{
				m_pLight->setSpotExponent(value);
			}
		}

		[Browsable(true), CategoryAttribute("Spot"),
		DescriptionAttribute("Constant attenuation coefficient")]
		property float ConstantAttenuation
		{
			float get()
			{
				return m_pLight->getConstantAttenuation();
			}

			void set(float value)
			{
				m_pLight->setConstantAttenuation(value);
			}
		}

		[Browsable(true), CategoryAttribute("Spot"),
		DescriptionAttribute("Linear attenuation coefficient")]
		property float LinearAttenuation
		{
			float get()
			{
				return m_pLight->getLinearAttenuation();
			}

			void set(float value)
			{
				m_pLight->setLinearAttenuation(value);
			}
		}

		[Browsable(true), CategoryAttribute("Spot"),
		DescriptionAttribute("Quadratic attenuation coefficient")]
		property float QuadraticAttenuation
		{
			float get()
			{
				return m_pLight->getQuadraticAttenuation();
			}

			void set(float value)
			{
				m_pLight->setQuadraticAttenuation(value);
			}
		}

		[Browsable(true), CategoryAttribute("Spot"),
		DescriptionAttribute("Field of View - only for spot lights")]
		property float FOV
		{
			float get()
			{
				return m_FOV;
			}

			void set(float value)
			{
				m_FOV = value;
				m_pLight->setFOV(Maths::degToRad(value));
			}
		}

		[Browsable(true), CategoryAttribute("Shadow"),
		DescriptionAttribute("Shadow bias")]
		property float ShadowBias
		{
			float get()
			{
				return m_pLight->getShadowBias();
			}

			void set(float value)
			{
				m_pLight->setShadowBias(value);
			}
		}

		[Browsable(true), CategoryAttribute("Light"),
		DescriptionAttribute("Direction of the light. Not valid for point lights")]
		property nGENEToolset::Vector3^ Direction
		{
			nGENEToolset::Vector3^ get()
			{
				m_pLight->setDirection(m_Direction->x,
									   m_Direction->y,
									   m_Direction->z);

				return m_Direction;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Direction->x = value->x;
				m_Direction->y = value->y;
				m_Direction->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Light"),
		DescriptionAttribute("Type of the light")]
		property LightType Type
		{
			LightType get()
			{
				return (LightType)m_pLight->getLightType();
			}

			void set(LightType value)
			{
				m_pLight->setLightType((nGENE::LIGHT_TYPE)value);
			}
		}

		[Browsable(true), CategoryAttribute("Light"),
		DescriptionAttribute("Radius of the light")]
		property float Radius
		{
			float get()
			{
				return m_pLight->getRadius().x;
			}

			void set(float value)
			{
				m_pLight->setRadius(value);
			}
		}

		[Browsable(true), CategoryAttribute("Light"),
		DescriptionAttribute("Intensity of the light")]
		property float Intensity
		{
			float get()
			{
				return m_pLight->getIntensity();
			}

			void set(float value)
			{
				m_pLight->setIntensity(value);
			}
		}

		NodeLight* getLight()
		{
			return m_pLight;
		}
	};

	ref class VolumetricLightWrapper: public NodeWrapper
	{
	private:
		NodeVolumetric* m_pVolume;				///< Pointer to the nGENE Light Node

		Color m_LightColour;

		static int s_nVolumesCount;

	public:
		VolumetricLightWrapper(NodeLight* _light)
		{
			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pNode = new NodeVolumetric(_light);
			m_pNode->setCreator(sm);
			m_pNode->init();
			
			m_pVolume = static_cast<NodeVolumetric*>(m_pNode);

			wostringstream buffer;
			buffer << "LightVolume";
			buffer << s_nVolumesCount;
			wstring text = buffer.str();
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pVolume);

			m_Position = gcnew Vector3(&m_pNode->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pNode->getScale());

			this->Name = "LightShaft" + (s_nVolumesCount);
			this->Key = "LightShaft" + (s_nVolumesCount++);


			ImageIndex = 3;

			frmSceneGraph::graph->addNode(this);
		}

		VolumetricLightWrapper(NodeVolumetric* _shaft, String^ _name):
			NodeWrapper(_shaft, _name, 3)
		{
			m_pVolume = _shaft;
			s_nVolumesCount++;
		}

		~VolumetricLightWrapper()
		{
		}

		[Browsable(true), CategoryAttribute("Volumetric"),
		DescriptionAttribute("Range of the effect")]
		property Real Distance
		{
			Real get()
			{
				return m_pVolume->getFar();
			}

			void set(Real value)
			{
				m_pVolume->setFar(value);
			}
		}

		[Browsable(true), CategoryAttribute("Volumetric"),
		DescriptionAttribute("Is shadow texture used?")]
		property bool UseShadow
		{
			bool get()
			{
				return m_pVolume->isShadowUsed();
			}

			void set(bool value)
			{
				m_pVolume->setShadowUsed(value);
			}
		}

		[Browsable(true), CategoryAttribute("Volumetric"),
		DescriptionAttribute("Is cookie texture used?")]
		property bool UseCookie
		{
			bool get()
			{
				return m_pVolume->isCookieUsed();
			}

			void set(bool value)
			{
				m_pVolume->setCookieUsed(value);
			}
		}

		[Browsable(true), CategoryAttribute("Light"),
		DescriptionAttribute("Colour of the light shaft")]
		property System::Drawing::Color Colour
		{
			Color get()
			{
				nGENE::Colour& clr = m_pVolume->getColour();
				return Color::FromArgb(clr.getRed(), clr.getGreen(), clr.getBlue());
			}

			void set(Color value)
			{
				//m_LightColour = value;
				m_pVolume->setColour(nGENE::Colour(value.R, value.G, value.B));
			}
		}

		[Browsable(true), CategoryAttribute("Volumetric"),
		DescriptionAttribute("Is noise texture used?")]
		property bool UseNoise
		{
			bool get()
			{
				return m_pVolume->isNoiseUsed();
			}

			void set(bool value)
			{
				m_pVolume->setNoiseUsed(value);
			}
		}
	};
}


#endif