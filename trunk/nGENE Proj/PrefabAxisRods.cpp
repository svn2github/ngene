/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabAxisRods.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PrefabAxisRods.h"
#include "ISerializer.h"
#include "PrefabArrow.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo PrefabAxisRods::Type(L"PrefabAxisRods", &Node::Type);


	PrefabAxisRods::PrefabAxisRods(float _length, float _endLength):
		m_fLength(_length),
		m_fEndLength(_endLength)
	{
	}
//----------------------------------------------------------------------
	PrefabAxisRods::PrefabAxisRods():
		m_fLength(1.0f),
		m_fEndLength(0.1f)
	{
	}
//----------------------------------------------------------------------
	PrefabAxisRods::~PrefabAxisRods()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PrefabAxisRods::init()
	{
		SceneManager* sm = getCreator();

		m_pAxisX = sm->createArrow();
		m_pAxisY = sm->createArrow();
		m_pAxisZ = sm->createArrow();

		m_pAxisX->setRotation(Vector3::UNIT_Z, -Maths::PI_HALF);
		m_pAxisZ->setRotation(Vector3::UNIT_X, Maths::PI_HALF);

		setLength(1.0f);
		
		addChild(L"AxisX", m_pAxisX);
		addChild(L"AxisY", m_pAxisY);
		addChild(L"AxisZ", m_pAxisZ);
	}
//----------------------------------------------------------------------
	void PrefabAxisRods::cleanup()
	{
	}
//----------------------------------------------------------------------
	void PrefabAxisRods::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			Node::serialize(_serializer, false, _serializeChildren);

			Property <Real> prLength(m_fLength);
			_serializer->addProperty("Length", prLength);

			Property <Real> prEndLength(m_fEndLength);
			_serializer->addProperty("EndLength", prEndLength);
			
		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void PrefabAxisRods::deserialize(ISerializer* _serializer)
	{
		Property <Real> prLength(m_fLength);
		_serializer->getProperty("Length", prLength);

		Property <Real> prEndLength(m_fEndLength);
		_serializer->getProperty("EndLength", prEndLength);

		Node::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	void PrefabAxisRods::setLength(Real _length)
	{
		m_fLength = _length;

		m_pAxisX->setLength(_length);
		m_pAxisY->setLength(_length);
		m_pAxisZ->setLength(_length);

		m_pAxisX->setPosition(_length * 0.5f, 0.0f, 0.0f);
		m_pAxisY->setPosition(0.0f, _length * 0.5f, 0.0f);
		m_pAxisZ->setPosition(0.0f, 0.0f, _length * 0.5f);
	}
//----------------------------------------------------------------------
	Real PrefabAxisRods::getLength() const
	{
		return m_pAxisX->getLength();
	}
//----------------------------------------------------------------------
	void PrefabAxisRods::setEndLength(Real _length)
	{
		m_fEndLength = _length;

		m_pAxisX->setEndLength(_length);
		m_pAxisY->setEndLength(_length);
		m_pAxisZ->setEndLength(_length);
	}
//----------------------------------------------------------------------
	Real PrefabAxisRods::getEndLength() const
	{
		return m_pAxisX->getEndLength();
	}
//----------------------------------------------------------------------
	void PrefabAxisRods::setMaterial(Material* _mat)
	{
		m_pAxisX->setMaterial(_mat);
		m_pAxisY->setMaterial(_mat);
		m_pAxisZ->setMaterial(_mat);
	}
//----------------------------------------------------------------------
}