/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Camera2D.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Camera2D.h"
#include "CameraEnumerator.h"
#include "Frustum.h"
#include "ISerializer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Camera2D::Type(L"Camera2D", &Camera::Type);


	Camera2D::Camera2D():
		m_fVelocity(20.0f),
		m_fZoom(1.0f),
		m_fDistance(800.0f),
		m_Axes(CLA_X_Y)
	{
	}
//----------------------------------------------------------------------
	Camera2D::~Camera2D()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Camera2D::init()
	{
		Camera::init();
	}
//----------------------------------------------------------------------
	void Camera2D::cleanup()
	{
	}
//----------------------------------------------------------------------
	void Camera2D::updateAxes()
	{
		// Update the vectors
		m_vecRight = m_quatOrientation * Vector3::UNIT_X;
		m_vecUp = m_quatOrientation * Vector3::UNIT_Y;
		m_vecForward = m_quatOrientation * Vector3::UNIT_Z;

		Vector3 vecPosition = m_vecPosition;
		if(m_pParent)
			vecPosition += m_pParent->getPositionLocal();

		m_matView.lookAt(vecPosition - m_vecForward * m_fDistance,
						 vecPosition,
						 m_vecUp);

		Renderer::getSingleton().setViewMatrix(m_matView);
	}
//----------------------------------------------------------------------
	void Camera2D::updateCamera()
	{
		Camera::updateCamera();

		Renderer& renderer = Renderer::getSingleton();

		Matrix4x4 newProjection;
		float fWidth = renderer.getDisplayMode().width;
		float fHeight = renderer.getDisplayMode().height;
		newProjection.orthogonal(fWidth / m_fZoom,
								 fHeight / m_fZoom,
								 m_pFrustum->getNear(),
								 m_pFrustum->getFar());

		renderer.setProjectionMatrix(newProjection);
	}
//----------------------------------------------------------------------
	void Camera2D::scroll(const Vector2& _direction)
	{
		if(!_direction.x && !_direction.y)
			return;

		Vector2 dir = _direction;
		Maths::clamp <float> (dir.x, -1.0f, 1.0f);
		Maths::clamp <float> (dir.y, -1.0f, 1.0f);
		float fDiff = float(Engine::getSingleton().getTimer().getMilliseconds() - m_lPrevTime) * 0.001f;

		Vector3 vecMove;
		switch(m_Axes)
		{
		case CLA_X_Y: vecMove.x = dir.x;
					  vecMove.y = dir.y;
					  break;

		case CLA_X_Z: vecMove.x = dir.x;
					  vecMove.z = dir.y;
		
		case CLA_Y_Z: vecMove.y = dir.x;
					  vecMove.z = dir.y;
					  break;
		}

		m_vecPosition += (m_quatOrientation * (vecMove * m_fVelocity * fDiff));

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Camera2D::setZoom(Real _zoom)
	{
		m_fZoom = _zoom;

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Camera2D::setDistance(Real _distance)
	{
		m_fDistance = _distance;

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Camera2D::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);

			Camera::serialize(_serializer, false, _serializeChildren);
			
			Property <float> prVelocity(m_fVelocity);
			_serializer->addProperty("Velocity", prVelocity);
			Property <float> prZoom(m_fZoom);
			_serializer->addProperty("Zoom", prZoom);
			Property <float> prDistance(m_fDistance);
			_serializer->addProperty("Distance", prDistance);

			uint nTmp = static_cast<uint>(m_Axes);
			Property <uint> proper(nTmp);
			_serializer->addProperty("LockedAxes", proper);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void Camera2D::deserialize(ISerializer* _serializer)
	{
		Property <float> prVelocity(m_fVelocity);
		_serializer->getProperty("Velocity", prVelocity);
		Property <float> prZoom(m_fZoom);
		_serializer->getProperty("Zoom", prZoom);
		Property <float> prDistance(m_fDistance);
		_serializer->getProperty("Distance", prDistance);

		uint nTmp;
		Property <uint> proper(nTmp);
		_serializer->getProperty("LockedAxes", proper);

		Camera::deserialize(_serializer);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	Camera2DFactory::Camera2DFactory()
	{
	}
//----------------------------------------------------------------------
	Camera2DFactory::~Camera2DFactory()
	{
	}
//----------------------------------------------------------------------
}