/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CameraFirstPersonSliding.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "CameraEnumerator.h"
#include "CameraFirstPersonSliding.h"
#include "ISerializer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo CameraFirstPersonSliding::Type(L"CameraFirstPersonSliding", &CameraFirstPerson::Type);


	CameraFirstPersonSliding::CameraFirstPersonSliding():
		m_fSlowDownRatio(2.0f)
	{
	}
//----------------------------------------------------------------------
	CameraFirstPersonSliding::~CameraFirstPersonSliding()
	{
	}
//----------------------------------------------------------------------
	void CameraFirstPersonSliding::updateCamera()
	{
		float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_lPrevTime) * 0.001f;

		m_vecPosition += (m_quatOrientation * (m_vecMovementDir * (m_fCurrentVelocity * fDiff)));

		m_fCurrentVelocity -= m_fSlowDownRatio * fDiff;
		if(m_fCurrentVelocity < 0.0f)
			m_fCurrentVelocity = 0.0f;

		m_bChanged = true;

		CameraFirstPerson::updateCamera();
	}
//----------------------------------------------------------------------
	void CameraFirstPersonSliding::move(Real _direction)
	{
		Maths::clamp <float> (_direction, -1.0f, 1.0f);

		m_vecMovementDir.z = _direction;
		m_fCurrentVelocity = m_fVelocity;

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void CameraFirstPersonSliding::strafe(Real _direction)
	{
		Maths::clamp <float> (_direction, -1.0f, 1.0f);

		m_vecMovementDir.x = _direction;
		m_fCurrentVelocity = m_fVelocity;

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void CameraFirstPersonSliding::setSlowDownSpeed(Real _value)
	{
		m_fSlowDownRatio = _value;
	}
//----------------------------------------------------------------------
	void CameraFirstPersonSliding::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			CameraFirstPerson::serialize(_serializer, false, _serializeChildren);

			Property <float> prSlowDown(m_fSlowDownRatio);
			_serializer->addProperty("SlowDown", prSlowDown);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void CameraFirstPersonSliding::deserialize(ISerializer* _serializer)
	{
		CameraFirstPerson::deserialize(_serializer);

		Property <float> prSlowDown(m_fSlowDownRatio);
		_serializer->getProperty("SlowDown", prSlowDown);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	SlidingCameraFactory::SlidingCameraFactory()
	{
		CameraEnumerator::getSingleton().addFactory(*this, L"CameraFirstPersonSliding");
	}
//----------------------------------------------------------------------
	SlidingCameraFactory::~SlidingCameraFactory()
	{
	}
//----------------------------------------------------------------------
}