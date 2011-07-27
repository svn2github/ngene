/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabArrow.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PrefabArrow.h"
#include "PrefabCylinder.h"
#include "PrefabPyramid.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo PrefabArrow::Type(L"PrefabArrow", &Node::Type);


	PrefabArrow::PrefabArrow(float _length, float _endLength):
		m_fLength(_length),
		m_fEndLength(_endLength)
	{
	}
//----------------------------------------------------------------------
	PrefabArrow::PrefabArrow():
		m_fLength(1.0f),
		m_fEndLength(0.1f)
	{
	}
//----------------------------------------------------------------------
	PrefabArrow::~PrefabArrow()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void PrefabArrow::init()
	{
		SceneManager* sm = getCreator();

		float lengthTwo = m_fEndLength * 2.0f;
		m_pEnd = sm->createPyramid(m_fEndLength, lengthTwo);
		m_pLine = sm->createCylinder(8, 4, lengthTwo * 0.1f, m_fLength);

		m_pEnd->setPosition(0.0f, m_fLength * 0.5f, 0.0f);

		wstringstream buffer;
		buffer << L"arrow_line_" << this;
		addChild(buffer.str(), m_pLine);

		buffer.flush();
		buffer << L"arrow_end_" << this;
		addChild(buffer.str(), m_pEnd);
	}
//----------------------------------------------------------------------
	void PrefabArrow::cleanup()
	{
	}
//----------------------------------------------------------------------
	void PrefabArrow::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
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
	void PrefabArrow::deserialize(ISerializer* _serializer)
	{
		Property <Real> prLength(m_fLength);
		_serializer->getProperty("Length", prLength);

		Property <Real> prEndLength(m_fEndLength);
		_serializer->getProperty("EndLength", prEndLength);

		Node::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	void PrefabArrow::setLength(Real _length)
	{
		m_fLength = _length;

		m_pLine->setHeight(_length);
	}
//----------------------------------------------------------------------
	Real PrefabArrow::getLength() const
	{
		return m_pLine->getHeight();
	}
//----------------------------------------------------------------------
	void PrefabArrow::setEndLength(Real _length)
	{
		m_fEndLength = _length;

		m_pEnd->setHeight(_length);
		m_pEnd->setSideLength(_length * 2.0f);
	}
//----------------------------------------------------------------------
	Real PrefabArrow::getEndLength() const
	{
		return m_pEnd->getHeight();
	}
//----------------------------------------------------------------------
	void PrefabArrow::setMaterial(Material* _mat)
	{
		m_pLine->getSurface(L"Base")->setMaterial(_mat);
		m_pEnd->getSurface(L"Base")->setMaterial(_mat);
	}
//----------------------------------------------------------------------
}