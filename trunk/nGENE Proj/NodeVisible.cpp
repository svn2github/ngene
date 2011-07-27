/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeVisible.cpp
Version:	0.15
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "NodeVisible.h"
#include "ScenePartitioner.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo NodeVisible::Type(L"NodeVisible", &Node::Type);


	NodeVisible::NodeVisible():
		m_bLightable(true)
	{
	}
//----------------------------------------------------------------------
	NodeVisible::NodeVisible(const NodeVisible& src):
		Node(src)
	{
		m_bLightable = src.m_bLightable;

		HashTable <wstring, Surface>::iterator iter;
		for(iter = src.m_Surfaces.begin(); iter != src.m_Surfaces.end(); ++iter)
			addSurface(iter->first, iter->second);
	}
//----------------------------------------------------------------------
	NodeVisible& NodeVisible::operator=(const NodeVisible& rhs)
	{
		if(this == &rhs)
			return (*this);

		Node::operator=(rhs);

		m_bLightable = rhs.m_bLightable;

		HashTable <wstring, Surface>::iterator iter;
		for(iter = rhs.m_Surfaces.begin(); iter != rhs.m_Surfaces.end(); ++iter)
			addSurface(iter->first, iter->second);

		return (*this);
	}
//----------------------------------------------------------------------
	NodeVisible::~NodeVisible()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void NodeVisible::init()
	{
		calculateBoundingBox();
	}
//----------------------------------------------------------------------
	void NodeVisible::cleanup()
	{
		if(!m_Surfaces.empty())
			m_Surfaces.clear();
	}
//----------------------------------------------------------------------
	void NodeVisible::onUpdate()
	{
		Node::onUpdate();

		HashTable <wstring, Surface>::iterator iter;
		Renderer& renderer = Renderer::getSingleton();
		Surface* pSurface = NULL;
		for(iter = m_Surfaces.begin(); iter != m_Surfaces.end(); ++iter)
		{
			pSurface = &iter->second;
			if(pSurface->isEnabled())
			{
				// Add it to an appropriate render queue
				Material* pMaterial = pSurface->getMaterialPtr();
				renderer.addToRender(pSurface, pMaterial);

				if(m_bHasChanged)
					pSurface->setChanged(true);
			}
		}
	}
//----------------------------------------------------------------------
	void NodeVisible::addSurface(const wstring& _name, Surface& _surf)
	{
		// If surface already exists, do not add it
		if(m_Surfaces.find(_name) != m_Surfaces.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Surface with the name: %ls already exists", _name.c_str());
			return;
		}

		m_Surfaces[_name] = _surf;
		m_Surfaces[_name].setNode(this);
		m_Surfaces[_name].setName(_name);

		m_vSurfaces.push_back(&m_Surfaces[_name]);
	}
//----------------------------------------------------------------------
	Surface* NodeVisible::getSurface(const wstring& _name)
	{
		// If surface doesn't exist, return
		if(m_Surfaces.find(_name) == m_Surfaces.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Surface with the name: %ls does not exist", _name.c_str());
			return NULL;
		}

		return &m_Surfaces[_name];
	}
//----------------------------------------------------------------------
	bool NodeVisible::findSurface(const wstring& _name)
	{
		if(m_Surfaces.find(_name) == m_Surfaces.end())
			return false;

		return true;
	}
//----------------------------------------------------------------------
	void NodeVisible::calculateBoundingBox()
	{
		if(m_Surfaces.size())
		{
			HashTable <wstring, Surface>::iterator iter;
			for(iter = m_Surfaces.begin(); iter != m_Surfaces.end(); ++iter)
			{
				iter->second.calculateBoundingBox();
			}
		}
	}
//----------------------------------------------------------------------
	void NodeVisible::addToPartitioner(ScenePartitioner* _partitioner)
	{
		HashTable <wstring, Surface>::iterator iter;
		for(iter = m_Surfaces.begin(); iter != m_Surfaces.end(); ++iter)
		{
			_partitioner->addNode(&iter->second);
		}
	}
//----------------------------------------------------------------------
	void NodeVisible::removeFromPartitioner(ScenePartitioner* _partitioner)
	{
		HashTable <wstring, Surface>::iterator iter;
		for(iter = m_Surfaces.begin(); iter != m_Surfaces.end(); ++iter)
		{
			_partitioner->removeNode(&iter->second);
		}
	}
//---------------------------------------------------------------------
	void NodeVisible::setPickable(bool _value)
	{
		HashTable <wstring, Surface>::iterator iter;
		for(iter = m_Surfaces.begin(); iter != m_Surfaces.end(); ++iter)
		{
			iter->second.setPickable(_value);
		}
	}
//---------------------------------------------------------------------
	void NodeVisible::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			
			Property <bool> prLightable(m_bLightable);
			_serializer->addProperty("Lightable", prLightable);
			
			uint count = m_vSurfaces.size();
			Property <uint> prCount(count);
			_serializer->addProperty("SurfacesCount", prCount);

			// Save all surfaces
			if(_serializeChildren)
			{
				for(uint i = 0; i < m_vSurfaces.size(); ++i)
					m_vSurfaces[i]->serialize(_serializer, true, _serializeChildren);
			}

			Node::serialize(_serializer, false, _serializeChildren);
		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeVisible::deserialize(ISerializer* _serializer)
	{
		Property <bool> prLightable(m_bLightable);
		_serializer->getProperty("Lightable", prLightable);

		uint count;
		Property <uint> prCount(count);
		_serializer->getProperty("SurfacesCount", prCount);

		Node::deserialize(_serializer);

		// Deserialize surface data
		for(uint i = 0; i < m_vSurfaces.size(); ++i)
		{
			_serializer->getNextObject();
			m_vSurfaces[i]->deserialize(_serializer);
		}

		if(count > m_vSurfaces.size())
		{
			for(uint i = m_vSurfaces.size(); i < count; ++i)
			{
				Surface surf;
				_serializer->getNextObject();
				surf.deserialize(_serializer);

				addSurface(surf.getName(), surf);
			}
		}
	}
//----------------------------------------------------------------------
}