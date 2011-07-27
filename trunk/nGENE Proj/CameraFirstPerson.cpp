/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CameraFirstPerson.cpp
Version:	0.14
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "CameraEnumerator.h"
#include "CameraFirstPerson.h"
#include "ISerializer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo CameraFirstPerson::Type(L"CameraFirstPerson", &Camera3D::Type);


	CameraFirstPerson::CameraFirstPerson()
	{
	}
//----------------------------------------------------------------------
	CameraFirstPerson::~CameraFirstPerson()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void CameraFirstPerson::init()
	{
		Camera3D::init();
	}
//----------------------------------------------------------------------
	void CameraFirstPerson::cleanup()
	{
	}
//----------------------------------------------------------------------
	void CameraFirstPerson::updateAxes()
	{
		// Update the vectors
		m_vecRight = m_quatOrientation * Vector3::UNIT_X;
		m_vecUp = m_quatOrientation * Vector3::UNIT_Y;
		m_vecForward = m_quatOrientation * Vector3::UNIT_Z;

		// Normalize all vectors
		m_vecForward.normalize();
		m_vecRight.normalize();
		m_vecUp.normalize();

		Vector3 vecPosition = m_vecPosition;
		if(m_pParent)
			vecPosition += m_pParent->getPositionLocal();

		m_matView.lookAt(vecPosition, vecPosition + m_vecForward, m_vecUp);
		Renderer::getSingleton().setViewMatrix(m_matView);
	}
//----------------------------------------------------------------------
	void CameraFirstPerson::move(Real _direction)
	{
		if(!_direction)
			return;

		float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_lPrevTime) * 0.001f;

		m_vecPosition += (m_quatOrientation * Vector3(0.0f, 0.0f, _direction * m_fVelocity * fDiff));
		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void CameraFirstPerson::strafe(Real _direction)
	{
		if(!_direction)
			return;

		float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_lPrevTime) * 0.001f;

		m_vecPosition += (m_quatOrientation * Vector3(_direction * m_fVelocity * fDiff, 0.0f, 0.0f));
		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void CameraFirstPerson::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			Camera3D::serialize(_serializer, false, _serializeChildren);
		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void CameraFirstPerson::deserialize(ISerializer* _serializer)
	{
		Camera3D::deserialize(_serializer);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	FPPCameraFactory::FPPCameraFactory()
	{
	}
//----------------------------------------------------------------------
	FPPCameraFactory::~FPPCameraFactory()
	{
	}
//----------------------------------------------------------------------
}