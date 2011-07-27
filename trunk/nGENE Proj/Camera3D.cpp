/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Camera3D.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Camera3D.h"
#include "ISerializer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Camera3D::Type(L"Camera3D", &Camera::Type);


	Camera3D::Camera3D():
		m_fVelocity(20.0f)
	{
	}
//----------------------------------------------------------------------
	Camera3D::~Camera3D()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Camera3D::init()
	{
		Camera::init();
	}
//----------------------------------------------------------------------
	void Camera3D::cleanup()
	{
	}
//----------------------------------------------------------------------
	void Camera3D::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			Camera::serialize(_serializer, false, _serializeChildren);
			
			Property <float> prVelocity(m_fVelocity);
			_serializer->addProperty("Velocity", prVelocity);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void Camera3D::deserialize(ISerializer* _serializer)
	{
		Property <float> prVelocity(m_fVelocity);
		_serializer->getProperty("Velocity", prVelocity);

		Camera::deserialize(_serializer);
	}
//----------------------------------------------------------------------
}