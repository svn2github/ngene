/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ParticleSystemWrapper.h
Version:	0.02
---------------------------------------------------------------------------
*/


#ifndef __INC_PARTICLESYSTEMWRAPPER_H_
#define __INC_PARTICLESYSTEMWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include "frmRenderTarget.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;
	using namespace System::Drawing;

	using nGENE::PARTICLE_COLOUR;
	using nGENE::SParticleForce;
	using nGENE::SParticleDeflector;
	using nGENE::NodeParticleSystem;
	using nGENE::ParticleEmitter;
	using nGENE::SceneManager;
	using nGENE::Engine;
	using nGENE::Real;

	ref class ParticleEmitterWrapper;


	/// Wrapper for particle deflector structure.
	ref class ParticleDeflectorWrapper: public SceneNode
	{
	protected:
		SParticleDeflector* m_pDeflector;

		bool m_bOnHeap;

		nGENEToolset::Vector3^ m_Position;
		nGENEToolset::Vector3^ m_Normal;

		static int s_nDeflectorsCount;

	public:
		ParticleDeflectorWrapper()
		{
			m_pDeflector = new SParticleDeflector();
			m_bOnHeap = true;

			m_Position = gcnew nGENEToolset::Vector3(NULL);
			m_Normal = gcnew nGENEToolset::Vector3(NULL);

			m_Position->x = m_pDeflector->plane.getPlanePoint().x;
			m_Position->y = m_pDeflector->plane.getPlanePoint().y;
			m_Position->z = m_pDeflector->plane.getPlanePoint().z;

			m_Normal->x = m_pDeflector->plane.getPlaneNormal().x;
			m_Normal->y = m_pDeflector->plane.getPlaneNormal().y;
			m_Normal->z = m_pDeflector->plane.getPlaneNormal().z;

			this->Name = "ParticleDeflector" + (s_nDeflectorsCount);
			this->Key = "ParticleDeflector" + (s_nDeflectorsCount++);
		}

		ParticleDeflectorWrapper(SParticleDeflector* _deflector)
		{
			m_pDeflector = _deflector;
			m_bOnHeap = false;

			m_Position = gcnew nGENEToolset::Vector3(NULL);
			m_Normal = gcnew nGENEToolset::Vector3(NULL);

			m_Position->x = m_pDeflector->plane.getPlanePoint().x;
			m_Position->y = m_pDeflector->plane.getPlanePoint().y;
			m_Position->z = m_pDeflector->plane.getPlanePoint().z;

			m_Normal->x = m_pDeflector->plane.getPlaneNormal().x;
			m_Normal->y = m_pDeflector->plane.getPlaneNormal().y;
			m_Normal->z = m_pDeflector->plane.getPlaneNormal().z;

			this->Name = "ParticleDeflector" + (s_nDeflectorsCount);
			this->Key = "ParticleDeflector" + (s_nDeflectorsCount++);
		}

		virtual ~ParticleDeflectorWrapper()
		{
			if(m_bOnHeap)
				NGENE_DELETE(m_pDeflector);
		}

		[Browsable(true), CategoryAttribute("Deflector"),
		DescriptionAttribute("Deflector bounciness")]
		property Real Bounciness
		{
			Real get()
			{
				return m_pDeflector->restitution;
			}

			void set(Real value)
			{
				m_pDeflector->restitution = value;
			}
		}

		[Browsable(true), CategoryAttribute("Deflector"),
		DescriptionAttribute("Position of the deflector")]
		property nGENEToolset::Vector3^ Position
		{
			nGENEToolset::Vector3^ get()
			{
				m_pDeflector->plane.setPlane(m_pDeflector->plane.getPlaneNormal(),
					nGENE::Point(m_Position->x,
								 m_Position->y,
								 m_Position->z));

				return m_Position;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Position->x = value->x;
				m_Position->y = value->y;
				m_Position->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Deflector"),
		DescriptionAttribute("Normal vector of the deflector plane")]
		property nGENEToolset::Vector3^ Normal
		{
			nGENEToolset::Vector3^ get()
			{
				m_pDeflector->plane.setPlane(m_Normal->getVector(),
											 m_pDeflector->plane.getPlanePoint());

				return m_Normal;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Normal->x = value->x;
				m_Normal->y = value->y;
				m_Normal->z = value->z;
			}
		}

		SParticleDeflector* getDeflector()
		{
			return m_pDeflector;
		}

		void setDeflector(SParticleDeflector* _deflector)
		{
			if(m_bOnHeap)
			{
				NGENE_DELETE(m_pDeflector);
				m_bOnHeap = false;
			}
			m_pDeflector = _deflector;
		}
	};

	/// Wrapper for particle force structure.
	ref class ParticleForceWrapper: public SceneNode
	{
	protected:
		SParticleForce* m_pForce;

		nGENEToolset::Vector3^ m_Position;

		bool m_bOnHeap;

		static int s_nForcesCount;

	public:
		ParticleForceWrapper()
		{
			m_pForce = new SParticleForce();
			m_bOnHeap = true;

			m_Position = gcnew nGENEToolset::Vector3(&m_pForce->vecPosition);

			this->Name = "ParticleForce" + (s_nForcesCount);
			this->Key = "ParticleForce" + (s_nForcesCount++);
		}

		ParticleForceWrapper(SParticleForce* _force)
		{
			m_pForce = _force;
			m_bOnHeap = false;

			m_Position = gcnew nGENEToolset::Vector3(&m_pForce->vecPosition);
			
			this->Name = "ParticleForce" + (s_nForcesCount);
			this->Key = "ParticleForce" + (s_nForcesCount++);
		}

		virtual ~ParticleForceWrapper()
		{
			if(m_bOnHeap)
				NGENE_DELETE(m_pForce);
		}

		[Browsable(true), CategoryAttribute("Force"),
		DescriptionAttribute("Position of the force")]
		property nGENEToolset::Vector3^ Position
		{
			nGENEToolset::Vector3^ get()
			{
				m_pForce->vecPosition = nGENE::Vector3(m_Position->x,
													   m_Position->y,
													   m_Position->z);

				return m_Position;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Position->x = value->x;
				m_Position->y = value->y;
				m_Position->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Force"),
		DescriptionAttribute("Strength of the force")]
		property Real Strength
		{
			Real get()
			{
				return m_pForce->fStrength;
			}

			void set(Real value)
			{
				m_pForce->fStrength = value;
			}
		}

		[Browsable(true), CategoryAttribute("Force"),
		DescriptionAttribute("Linear attenuation with distance")]
		property Real LinearAttenuation
		{
			Real get()
			{
				return m_pForce->fLinearAtt;
			}

			void set(Real value)
			{
				m_pForce->fLinearAtt = value;
			}
		}

		[Browsable(true), CategoryAttribute("Force"),
		DescriptionAttribute("Quadratic attenuation with distance")]
		property Real QuadraticAttenuation
		{
			Real get()
			{
				return m_pForce->fQuadraticAtt;
			}

			void set(Real value)
			{
				m_pForce->fQuadraticAtt = value;
			}
		}

		SParticleForce* getForce()
		{
			return m_pForce;
		}

		void setForce(SParticleForce* _force)
		{
			if(m_bOnHeap)
			{
				NGENE_DELETE(m_pForce);
				m_bOnHeap = false;
			}
			m_pForce = _force;
			m_Position->setPointer(&m_pForce->vecPosition);
		}
	};

	/// Wrapper for particle colour structure.
	ref class ParticleColourWrapper: public SceneNode
	{
	protected:
		PARTICLE_COLOUR* m_pColour;
		ParticleEmitter* m_pEmitter;
		ParticleEmitterWrapper^ m_Emitter;
		Color m_Colour;

		bool m_bOnHeap;

		static int s_nColoursCount;

	public:
		ParticleColourWrapper()
		{
			m_pColour = new PARTICLE_COLOUR();
			m_bOnHeap = true;

			this->Name = "ParticleColour" + (s_nColoursCount);
			this->Key = "ParticleColour" + (s_nColoursCount++);

			m_pEmitter = NULL;
		}

		ParticleColourWrapper(PARTICLE_COLOUR* _colour)
		{
			m_pColour = _colour;
			m_bOnHeap = false;

			this->Name = "ParticleColour" + (s_nColoursCount);
			this->Key = "ParticleColour" + (s_nColoursCount++);

			m_pEmitter = NULL;
		}

		virtual ~ParticleColourWrapper()
		{
			if(m_bOnHeap)
				NGENE_DELETE(m_pColour);
		}


		[Browsable(true), CategoryAttribute("Colour"),
		DescriptionAttribute("Colour of the particle")]
		property System::Drawing::Color Colour
		{
			Color get()
			{
				return m_Colour;
			}

			void set(Color value)
			{
				m_Colour = value;

				m_pColour->colour.setRed(value.R);
				m_pColour->colour.setGreen(value.G);
				m_pColour->colour.setBlue(value.B);
			}
		}

		[Browsable(true), CategoryAttribute("Animation"),
		DescriptionAttribute("Time when colour should reach this value")]
		property Real Stage
		{
			Real get()
			{
				return m_pColour->fLifeTimeLevel;
			}

			void set(Real value)
			{
				if(value >= 0.0f && value <= 1.0f)
				{
					refreshEmitter(value);
				}
				else
				{
					System::Windows::Forms::MessageBox::Show("Invalid value. Values are limited to 0 to 1 range.",
						"Invalid value",
						System::Windows::Forms::MessageBoxButtons::OK,
						System::Windows::Forms::MessageBoxIcon::Warning);
				}
			}
		}

		[Browsable(true), CategoryAttribute("Colour"),
		DescriptionAttribute("Transparency level")]
		property nGENE::byte Transparency
		{
			nGENE::byte get()
			{
				return m_pColour->colour.getAlpha();
			}

			void set(nGENE::byte value)
			{
				m_pColour->colour.setAlpha(value);
			}
		}

		PARTICLE_COLOUR* getColour()
		{
			return m_pColour;
		}

		void setColour(PARTICLE_COLOUR* _colour)
		{
			if(m_bOnHeap)
			{
				NGENE_DELETE(m_pColour);
				m_bOnHeap = false;
			}
			m_pColour = _colour;
		}

		void setEmitter(ParticleEmitterWrapper^ _emitter)
		{
			m_Emitter = _emitter;
		}

		private: void refreshEmitter(float value);
	};


	/// Wrapper for a ParticleEmitter class
	ref class ParticleEmitterWrapper: public SceneNode
	{
	protected:
		ParticleEmitter* m_pEmitter;

		bool m_bOnHeap;

		static int s_nEmittersCount;

		nGENEToolset::Vector3^ m_Position;
		nGENEToolset::Vector3^ m_Velocity;
		nGENEToolset::Vector3^ m_VelocitySpread;
		nGENEToolset::Vector3^ m_Accel;
		nGENEToolset::Vector3^ m_AccelSpread;

		System::Collections::ArrayList^ m_ColoursList;

	public:
		ParticleEmitterWrapper();
		ParticleEmitterWrapper(ParticleEmitter* _emitter);

		virtual ~ParticleEmitterWrapper()
		{
			if(m_bOnHeap)
				NGENE_DELETE(m_pEmitter);
		}

		ParticleEmitter* getEmitter()
		{
			return m_pEmitter;
		}

		[Browsable(true), CategoryAttribute("Emission"),
		DescriptionAttribute("Maximum number of particles emitted")]
		property uint MaxParticlesCount
		{
			uint get()
			{
				return m_pEmitter->getParticlesMaxCount();
			}

			void set(uint value)
			{
				if(!frmRenderTarget::engine->tryLock())
					return;

				m_pEmitter->setParticlesMaxCount(value);

				frmRenderTarget::engine->releaseLock();
			}
		}

		[Browsable(true), CategoryAttribute("Emission"),
		DescriptionAttribute("Number of particles spawned per second")]
		property uint SpawnRate
		{
			uint get()
			{
				return m_pEmitter->getSpawnRate();
			}

			void set(uint value)
			{
				m_pEmitter->setSpawnRate(value);
			}
		}

		[Browsable(true), CategoryAttribute("Emission"),
		DescriptionAttribute("Time in milliseconds without emitting particles (warm-up time)")]
		property uint WarmUpTime
		{
			uint get()
			{
				return m_pEmitter->getWarmUpTime();
			}

			void set(uint value)
			{
				m_pEmitter->setWarmUpTime(value);
			}
		}

		[Browsable(true), CategoryAttribute("Particles"),
		DescriptionAttribute("Average size of a particle")]
		property float Size
		{
			float get()
			{
				return m_pEmitter->getSize();
			}

			void set(float value)
			{
				m_pEmitter->setSize(value);
			}
		}

		[Browsable(true), CategoryAttribute("Particles"),
		DescriptionAttribute("Deviation of size of a particle")]
		property float SizeSpread
		{
			float get()
			{
				return m_pEmitter->getSizeSpread();
			}

			void set(float value)
			{
				m_pEmitter->setSizeSpread(value);
			}
		}

		[Browsable(true), CategoryAttribute("Particles"),
		DescriptionAttribute("Average particle growth per second")]
		property float Growth
		{
			float get()
			{
				return m_pEmitter->getGrowth();
			}

			void set(float value)
			{
				m_pEmitter->setGrowth(value);
			}
		}

		[Browsable(true), CategoryAttribute("Particles"),
		DescriptionAttribute("Deviation of growth of a particle")]
		property float GrowthSpread
		{
			float get()
			{
				return m_pEmitter->getGrowthSpread();
			}

			void set(float value)
			{
				m_pEmitter->setGrowthSpread(value);
			}
		}

		[Browsable(true), CategoryAttribute("Particles"),
		DescriptionAttribute("Average particle life time")]
		property float LifeTime
		{
			float get()
			{
				return m_pEmitter->getLifeTime();
			}

			void set(float value)
			{
				m_pEmitter->setLifeTime(value);
			}
		}

		[Browsable(true), CategoryAttribute("Particles"),
		DescriptionAttribute("Deviation of life time of a particle")]
		property float LifeTimeSpread
		{
			float get()
			{
				return m_pEmitter->getLifeTimeSpread();
			}

			void set(float value)
			{
				m_pEmitter->setLifeTimeSpread(value);
			}
		}

		[Browsable(true), CategoryAttribute("Particles"),
		DescriptionAttribute("Modifies fading factor of a particle")]
		property float FadeSpeed
		{
			float get()
			{
				return m_pEmitter->getFadeSpeed();
			}

			void set(float value)
			{
				m_pEmitter->setFadeSpeed(value);
			}
		}

		[Browsable(true), CategoryAttribute("Movement"),
		DescriptionAttribute("Average angular velocity of a particle")]
		property float AngularVelocity
		{
			float get()
			{
				return m_pEmitter->getAngularVelocity();
			}

			void set(float value)
			{
				m_pEmitter->setAngularVelocity(value);
			}
		}

		[Browsable(true), CategoryAttribute("Movement"),
		DescriptionAttribute("Deviation of angular velocity of a particle")]
		property float AngularVelocitySpread
		{
			float get()
			{
				return m_pEmitter->getAngularVelocitySpread();
			}

			void set(float value)
			{
				m_pEmitter->setAngularVelocitySpread(value);
			}
		}

		[Browsable(true), CategoryAttribute("Movement"),
		DescriptionAttribute("Starting position of a particle in regard to particle system position")]
		property nGENEToolset::Vector3^ ParticlePosition
		{
			nGENEToolset::Vector3^ get()
			{
				m_pEmitter->setPosition(m_Position->x,
										m_Position->y,
										m_Position->z);

				return m_Position;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Position->x = value->x;
				m_Position->y = value->y;
				m_Position->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Movement"),
		DescriptionAttribute("Starting average velocity of a particle")]
		property nGENEToolset::Vector3^ Velocity
		{
			nGENEToolset::Vector3^ get()
			{
				m_pEmitter->setVelocity(m_Velocity->x,
										m_Velocity->y,
										m_Velocity->z);

				return m_Velocity;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Velocity->x = value->x;
				m_Velocity->y = value->y;
				m_Velocity->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Movement"),
		DescriptionAttribute("Deviation of velocity of a particle")]
		property nGENEToolset::Vector3^ VelocitySpread
		{
			nGENEToolset::Vector3^ get()
			{
				m_pEmitter->setVelocitySpread(m_VelocitySpread->x,
											  m_VelocitySpread->y,
											  m_VelocitySpread->z);

				return m_VelocitySpread;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_VelocitySpread->x = value->x;
				m_VelocitySpread->y = value->y;
				m_VelocitySpread->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Movement"),
		DescriptionAttribute("Starting average acceleration of a particle")]
		property nGENEToolset::Vector3^ Acceleration
		{
			nGENEToolset::Vector3^ get()
			{
				m_pEmitter->setAcceleration(m_Accel->x,
											m_Accel->y,
											m_Accel->z);

				return m_Accel;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_Accel->x = value->x;
				m_Accel->y = value->y;
				m_Accel->z = value->z;
			}
		}

		[Browsable(true), CategoryAttribute("Movement"),
		DescriptionAttribute("Deviation of acceleration of a particle")]
		property nGENEToolset::Vector3^ AccelerationSpread
		{
			nGENEToolset::Vector3^ get()
			{
				m_pEmitter->setAccelerationSpread(m_AccelSpread->x,
												  m_AccelSpread->y,
												  m_AccelSpread->z);

				return m_AccelSpread;
			}

			void set(nGENEToolset::Vector3^ value)
			{
				m_AccelSpread->x = value->x;
				m_AccelSpread->y = value->y;
				m_AccelSpread->z = value->z;
			}
		}


		/// Adds particle colour wrapper to this particle emitter (also to a wrapper ;) )
		System::Void addColour(ParticleColourWrapper^ colour);

		System::Void addForce(ParticleForceWrapper^ force);

		System::Void addDeflector(ParticleDeflectorWrapper^ deflector);

		System::Void sortColours()
		{
			if(!frmRenderTarget::engine->tryLock())
				return;

			m_pEmitter->sortColours();
			for(unsigned int i = 0; i < m_pEmitter->getParticleColoursNum(); ++i)
			{
				((ParticleColourWrapper^)m_ColoursList[i])->setColour(m_pEmitter->getParticleColour(i));
				((ParticleColourWrapper^)m_ColoursList[i])->Colour = Color::FromArgb(255,
						 m_pEmitter->getParticleColour(i)->colour.getRed(),
						 m_pEmitter->getParticleColour(i)->colour.getGreen(),
						 m_pEmitter->getParticleColour(i)->colour.getBlue());
			}

			frmRenderTarget::engine->releaseLock();
		}

		public:
			void parseColours();
			void parseForces();
			void parseDeflectors();
	};

	
	/// Wrapper for a NodeParticleSystem class.
	ref class ParticleSystemWrapper: public NodeVisibleWrapper
	{
	protected:
		NodeParticleSystem* m_pPS;
		static int s_nPSCount;

	public:
		ParticleSystemWrapper();
		ParticleSystemWrapper(NodeParticleSystem* _ps, String^ _name);

		virtual ~ParticleSystemWrapper()
		{
		}

		/// Adds particle emitter wrapper to this particle system (also to a wrapper ;) )
		System::Void addEmitter(ParticleEmitterWrapper^ emitter);
	};
}


#endif