/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Particle.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Particle.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Particle::Type(L"Particle", NULL);


	Particle::Particle():
		m_vecVelocity(Vector3::ZERO_VECTOR),
		m_vecAcceleration(Vector3::ZERO_VECTOR),
		m_vecPosition(Vector3::ZERO_VECTOR),
		m_fAngularVelocity(0.0f),
		m_fRotation(0.0f),
		m_fSize(0.125f),
		m_fGrowth(0.0f),
		m_fPassedTime(0.0f),
		m_fLifeTime(4000.0f),
		m_bFadeEnabled(true),
		m_fFadeSpeed(1.0f),
		m_fFadeStart(0.0f),
		m_fFadeEnd(1.0f)
	{
	}
//----------------------------------------------------------------------
	Particle::~Particle()
	{
	}
//----------------------------------------------------------------------
	void Particle::update(Real _delta)
	{
		m_vecOldPosition = m_vecPosition;
		m_vecPosition += m_vecVelocity * _delta * 0.001f;
		m_vecVelocity += m_vecAcceleration * _delta * 0.001f;
		m_fRotation += m_fAngularVelocity * _delta * 0.001f;
		m_fSize += m_fGrowth * _delta * 0.001f;

		m_fPassedTime += _delta;

		if(m_bFadeEnabled)
			updateColour(_delta);
	}
//----------------------------------------------------------------------
	void Particle::updateColour(Real _delta)
	{
		float fFadeLife = m_fLifeTime * (m_fFadeEnd - m_fFadeStart);
		float fFadePassed = m_fPassedTime - m_fFadeStart * m_fLifeTime;
		float time = (fFadePassed / fFadeLife) * m_fFadeSpeed;

		int nValue = FastFloat::floatToInt(Maths::lerp <float>(m_SA, m_TA, time));
		Maths::clamp <int>(nValue, 0, 255);
		m_Colour.setAlpha(nValue);

		nValue = FastFloat::floatToInt(Maths::lerp <float>(m_SR, m_TR, time));
		Maths::clamp <int>(nValue, 0, 255);
		m_Colour.setRed(nValue);

		nValue = FastFloat::floatToInt(Maths::lerp <float>(m_SG, m_TG, time));
		Maths::clamp <int>(nValue, 0, 255);
		m_Colour.setGreen(nValue);

		nValue = FastFloat::floatToInt(Maths::lerp <float>(m_SB, m_TB, time));
		Maths::clamp <int>(nValue, 0, 255);
		m_Colour.setBlue(nValue);
	}
//----------------------------------------------------------------------
}