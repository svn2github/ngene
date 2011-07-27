/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CameraThirdPerson.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "CameraEnumerator.h"
#include "CameraThirdPerson.h"
#include "ISerializer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo CameraThirdPerson::Type(L"CameraThirdPerson", &Camera3D::Type);


	CameraThirdPerson::CameraThirdPerson():
		m_fDistanceFromTarget(10.0f)
	{
	}
//----------------------------------------------------------------------
	CameraThirdPerson::~CameraThirdPerson()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void CameraThirdPerson::init()
	{
		Camera3D::init();
	}
//----------------------------------------------------------------------
	void CameraThirdPerson::cleanup()
	{
	}
//----------------------------------------------------------------------
	void CameraThirdPerson::updateAxes()
	{
		// Update the vectors
		m_vecRight = m_quatOrientation * Vector3::UNIT_X;
		m_vecUp = m_quatOrientation * Vector3::UNIT_Y;
		m_vecForward = m_quatOrientation * Vector3::UNIT_Z;

		// Normalize all vectors
		m_vecForward.normalize();
		m_vecRight.normalize();
		m_vecUp.normalize();

		m_vecPosition = m_vecTarget - m_fDistanceFromTarget * m_vecForward;

		m_matView.lookAt(m_vecPosition,
						 m_vecTarget,
						 m_vecUp);
		Renderer::getSingleton().setViewMatrix(m_matView);
	}
//----------------------------------------------------------------------
	void CameraThirdPerson::move(Real _direction)
	{
		if(!_direction)
			return;

		float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_lPrevTime) * 0.001f;

		m_vecTarget.z += _direction * m_fVelocity * fDiff;

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void CameraThirdPerson::strafe(Real _direction)
	{
		if(!_direction)
			return;

		float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_lPrevTime) * 0.001f;

		m_vecTarget.x += _direction * m_fVelocity * fDiff;

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void CameraThirdPerson::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			Camera3D::serialize(_serializer, false, _serializeChildren);

			Property <float> prDistance(m_fDistanceFromTarget);
			_serializer->addProperty("Distance", prDistance);
			Property <Vector3> prTarget(m_vecTarget);
			_serializer->addProperty("Target", prTarget);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void CameraThirdPerson::deserialize(ISerializer* _serializer)
	{
		Property <float> prDistance(m_fDistanceFromTarget);
		_serializer->getProperty("Distance", prDistance);
		Property <Vector3> prTarget(m_vecTarget);
		_serializer->getProperty("Target", prTarget);

		Camera3D::deserialize(_serializer);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	TPPCameraFactory::TPPCameraFactory()
	{
	}
//----------------------------------------------------------------------
	TPPCameraFactory::~TPPCameraFactory()
	{
	}
//----------------------------------------------------------------------
}