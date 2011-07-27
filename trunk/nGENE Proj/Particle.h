/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Particle.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PARTICLE_H_
#define __INC_PARTICLE_H_


#include "Prerequisities.h"
#include "TypeInfo.h"
#include "Vector3.h"
#include "Colour.h"


namespace nGENE
{
	/** Base class for particles objects.
		@remarks
			Particles are medium playing important role in our
			daily life. They can make image more vivid, add depth
			to it. They could also enhance visual quality as they
			are great for special FX.
		@par
			nGENE Tech makes use of two well known technologies:
			Soft Particles (primary one), and Soft Mega Particles
			both of which can be run on DirectX 9.0 compatible hardware.
			Besides due to use of these techniques particle systems
			are slower in comparison to regular ones, but they
			also need much less particles to provide satisfactory
			results.
	*/
	class nGENEDLL Particle
	{
		EXPOSE_TYPE
	private:
		Vector3 m_vecVelocity;						///< Particle velocity
		Vector3 m_vecAcceleration;					///< Particle acceleration
		Vector3 m_vecPosition;						///< Particle position
		Vector3 m_vecOldPosition;					///< Previous particle position
		float m_fAngularVelocity;					///< Particle angular velocity
		float m_fRotation;							///< Current rotation angle

		Real m_fSize;								///< Particle size
		Real m_fGrowth;								///< Particle growth amount
		Real m_fMass;								///< Particle mass (@todo not used yet)

		Colour m_Colour;							///< Current particle colour

		// Starting colour
		float m_SR;
		float m_SG;
		float m_SB;
		float m_SA;

		// Target colour
		float m_TR;
		float m_TG;
		float m_TB;
		float m_TA;

		Real m_fLifeTime;							///< Particle life-time
		Real m_fPassedTime;							///< Time passed since particle birth

		bool m_bFadeEnabled;						///< Can particle change its colour?
		Real m_fFadeSpeed;							///< Colour fading speed

		// Beginning and ending of colour fade represented as particle life-time fractions
		float m_fFadeStart;
		float m_fFadeEnd;

	public:
		Particle();
		virtual ~Particle();

		/** Updates particle.
			@param
				_delta time which passed since last update.
		*/
		void update(Real _delta);
		void updateColour(Real _delta);

		/// Resets the particle.
		void reset();


		// Below are many accessors


		void setLifeTime(Real _lifeTime);
		Real getLifeTime() const;
		Real getPassedTime() const;

		void setVelocity(const Vector3& _velocity);
		Vector3& getVelocity();
		void setAcceleration(const Vector3& _acceleration);

		void setPosition(const Vector3& _position);
		const Vector3& getPosition() const;

		const Vector3& getOldPosition() const;

		void setAngularVelocity(Real _velocity);
		Real getAngularVelocity() const;
		Real getRotation() const;

		void setSize(Real _size);
		float getSize() const;

		void setGrowth(Real _growth);
		float getGrowth() const;

		void setMass(Real _mass);

		void setStartColour(const Colour& _colour);
		void setTargetColour(const Colour& _colour);
		Colour getStartColour();
		Colour getTargetColour();

		Colour& getColour();

		void setFadeEnabled(bool _value);
		void setFadeSpeed(Real _speed);
		void setFadeStart(Real _start);
		void setFadeEnd(Real _start);
	};



	inline void Particle::setLifeTime(Real _lifeTime)
	{
		m_fLifeTime = _lifeTime;
	}
//----------------------------------------------------------------------
	inline Real Particle::getLifeTime() const
	{
		return m_fLifeTime;
	}
//----------------------------------------------------------------------
	inline Real Particle::getPassedTime() const
	{
		return m_fPassedTime;
	}
//----------------------------------------------------------------------
	inline void Particle::setVelocity(const Vector3& _velocity)
	{
		m_vecVelocity = _velocity;
	}
//----------------------------------------------------------------------
	inline Vector3& Particle::getVelocity()
	{
		return m_vecVelocity;
	}
//----------------------------------------------------------------------
	inline void Particle::setAcceleration(const Vector3& _acceleration)
	{
		m_vecAcceleration = _acceleration;
	}
//----------------------------------------------------------------------
	inline void Particle::setPosition(const Vector3& _position)
	{
		m_vecOldPosition = m_vecPosition;
		m_vecPosition = _position;
	}
//----------------------------------------------------------------------
	inline const Vector3& Particle::getPosition() const
	{
		return m_vecPosition;
	}
//----------------------------------------------------------------------
	inline const Vector3& Particle::getOldPosition() const
	{
		return m_vecOldPosition;
	}
//----------------------------------------------------------------------
	inline void Particle::setAngularVelocity(Real _velocity)
	{
		m_fAngularVelocity = _velocity;
	}
//----------------------------------------------------------------------
	inline Real Particle::getAngularVelocity() const
	{
		return m_fAngularVelocity;
	}
//----------------------------------------------------------------------
	inline Real Particle::getRotation() const
	{
		return m_fRotation;
	}
//----------------------------------------------------------------------
	inline void Particle::setSize(Real _size)
	{
		m_fSize = _size;
	}
//----------------------------------------------------------------------
	inline float Particle::getSize() const
	{
		return m_fSize;
	}
//----------------------------------------------------------------------
	inline void Particle::setGrowth(Real _growth)
	{
		m_fGrowth = _growth;
	}
//----------------------------------------------------------------------
	inline float Particle::getGrowth() const
	{
		return m_fGrowth;
	}
//----------------------------------------------------------------------
	inline void Particle::setMass(Real _mass)
	{
		m_fMass = _mass;
	}
//----------------------------------------------------------------------
	inline void Particle::setStartColour(const Colour& _colour)
	{
		m_SR = static_cast<float>(_colour.getRed());
		m_SG = static_cast<float>(_colour.getGreen());
		m_SB = static_cast<float>(_colour.getBlue());
		m_SA = static_cast<float>(_colour.getAlpha());
	}
//----------------------------------------------------------------------
	inline void Particle::setTargetColour(const Colour& _colour)
	{
		m_TR = static_cast<float>(_colour.getRed());
		m_TG = static_cast<float>(_colour.getGreen());
		m_TB = static_cast<float>(_colour.getBlue());
		m_TA = static_cast<float>(_colour.getAlpha());
	}
//----------------------------------------------------------------------
	inline Colour Particle::getStartColour()
	{
		return Colour(static_cast<byte>(FastFloat::floatToInt(m_SR) & 0xff),
					  static_cast<byte>(FastFloat::floatToInt(m_SG) & 0xff),
					  static_cast<byte>(FastFloat::floatToInt(m_SB) & 0xff),
					  static_cast<byte>(FastFloat::floatToInt(m_SA) & 0xff));
	}
//----------------------------------------------------------------------
	inline Colour Particle::getTargetColour()
	{
		return Colour(static_cast<byte>(FastFloat::floatToInt(m_TR) & 0xff),
					  static_cast<byte>(FastFloat::floatToInt(m_TG) & 0xff),
					  static_cast<byte>(FastFloat::floatToInt(m_TB) & 0xff),
					  static_cast<byte>(FastFloat::floatToInt(m_TA) & 0xff));
	}
//----------------------------------------------------------------------
	inline Colour& Particle::getColour()
	{
		return m_Colour;
	}
//----------------------------------------------------------------------
	inline void Particle::reset()
	{
		m_fPassedTime = 0.0f;
		m_Colour = Colour(static_cast<byte>(FastFloat::floatToInt(m_SR) & 0xff),
						  static_cast<byte>(FastFloat::floatToInt(m_SG) & 0xff),
						  static_cast<byte>(FastFloat::floatToInt(m_SB) & 0xff),
						  static_cast<byte>(FastFloat::floatToInt(m_SA) & 0xff));
	}
//----------------------------------------------------------------------
	inline void Particle::setFadeEnabled(bool _value)
	{
		m_bFadeEnabled = _value;
	}
//----------------------------------------------------------------------
	inline void Particle::setFadeSpeed(Real _speed)
	{
		m_fFadeSpeed = _speed;
	}
//----------------------------------------------------------------------
	inline void Particle::setFadeStart(Real _start)
	{
		m_fFadeStart = _start;
	}
//----------------------------------------------------------------------
	inline void Particle::setFadeEnd(Real _end)
	{
		m_fFadeEnd = _end;
	}
//----------------------------------------------------------------------
}


#endif