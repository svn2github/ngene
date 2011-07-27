/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Camera.cpp
Version:	0.15
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Camera.h"
#include "CameraEnumerator.h"
#include "Frustum.h"
#include "ISerializer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Camera::Type(L"Camera", &Node::Type);


	Camera::Camera():
		m_pFrustum(new Frustum()),
		m_vecForward(Vector3::UNIT_Z),
		m_vecUp(Vector3::UNIT_Y),
		m_vecRight(Vector3::UNIT_X),
		m_bChanged(true)
	{
	}
//----------------------------------------------------------------------
	Camera::~Camera()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Camera::init()
	{
		updateFrustum();		
		updateCamera();
	}
//----------------------------------------------------------------------
	void Camera::cleanup()
	{
	}
//----------------------------------------------------------------------
	void Camera::updateFrustum()
	{
		m_pFrustum->update();

		// Update projection matrix of the renderer
		if(Engine::getSingleton().getActiveCamera() == this)
			Renderer::getSingleton().setProjectionMatrix(m_pFrustum->getProjectionMatrix());
	}
//----------------------------------------------------------------------
	void Camera::updateCamera()
	{
		// Has camera changed
		if(m_bChanged || m_bHasChanged)
		{
			updateAxes();

			m_pFrustum->setVectors(m_vecPosition,
								   m_vecForward,
								   m_vecRight,
								   m_vecUp);

			updateFrustum();

			m_bChanged = false;
		}

		if(m_pFrustum->hasChanged())
			updateFrustum();

		// Update time
		m_lPrevTime = Engine::getSingleton().getTimer().getMilliseconds();
	}
//----------------------------------------------------------------------
	void Camera::updateAxes()
	{
		m_matView.lookAt(m_vecPosition, m_vecPosition + m_vecForward, m_vecUp);
		Renderer::getSingleton().setViewMatrix(m_matView);

		m_vecRight.x = m_matView._m11;
		m_vecRight.y = m_matView._m21;
		m_vecRight.z = m_matView._m31;

		m_vecForward.x = m_matView._m13;
		m_vecForward.y = m_matView._m23;
		m_vecForward.z = m_matView._m33;

		// Normalize all vectors
		m_vecForward.normalize();
		m_vecRight.normalize();
		m_vecUp.normalize();
	}
//----------------------------------------------------------------------
	void Camera::setRotation(const Quaternion& _quat)
	{
		if((_quat.x == 0.0f) && (_quat.y == 0.0f) &&
		   (_quat.z == 0.0f) && (_quat.w == 0.0f))
		{
			return;
		}

		Node::setRotation(_quat);

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Camera::setRotation(const Vector3& _axis, const Real _angle)
	{
		if(!_angle)
			return;

		Node::setRotation(_axis, _angle);

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Camera::setRotation(const Vector3& _rot)
	{
		if((!_rot.x) && (!_rot.y) && (!_rot.z))
			return;

		Node::setRotation(_rot);

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Camera::setForward(const Vector3& _forward)
	{
		m_vecForward = _forward;

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Camera::setUp(const Vector3& _up)
	{
		m_vecUp = _up;

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Camera::setPosition(const Vector3& _pos)
	{
		Node::setPosition(_pos);

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Camera::setPosition(Real _x, Real _y, Real _z)
	{
		Node::setPosition(_x, _y, _z);

		m_bChanged = true;
		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Camera::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			Node::serialize(_serializer, false, _serializeChildren);

			// Serialize frustum data
			float fTmp = m_pFrustum->getAspect();
			Property <float> prAspect(fTmp);
			_serializer->addProperty("Aspect", prAspect);

			fTmp = m_pFrustum->getFOV();
			Property <float> prFOV(fTmp);
			_serializer->addProperty("FOV", prFOV);

			fTmp = m_pFrustum->getNear();
			Property <float> prNear(fTmp);
			_serializer->addProperty("Near", prNear);

			fTmp = m_pFrustum->getFar();
			Property <float> prFar(fTmp);
			_serializer->addProperty("Far", prFar);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void Camera::deserialize(ISerializer* _serializer)
	{		
		Node::deserialize(_serializer);

		// Deserialize frustum data
		float fTmp;
		Property <float> prAspect(fTmp);
		_serializer->getProperty("Aspect", prAspect);
		m_pFrustum->setAspect(fTmp);

		Property <float> prFOV(fTmp);
		_serializer->getProperty("FOV", prFOV);
		m_pFrustum->setFOV(fTmp);

		Property <float> prNear(fTmp);
		_serializer->getProperty("Near", prNear);
		m_pFrustum->setNear(fTmp);

		Property <float> prFar(fTmp);
		_serializer->getProperty("Far", prFar);
		m_pFrustum->setFar(fTmp);

		// Set this camera as active one
		Engine::getSingleton().setActiveCamera(this);

		m_bChanged = true;
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	DefaultCameraFactory::DefaultCameraFactory()
	{
	}
//----------------------------------------------------------------------
	DefaultCameraFactory::~DefaultCameraFactory()
	{
	}
//----------------------------------------------------------------------
}