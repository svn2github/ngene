/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ISerializer.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ISERIALIZER_H_
#define __INC_ISERIALIZER_H_


#include "Prerequisities.h"
#include "TypeInfo.h"
#include "SerializableProperty.h"

#include "HashTable.h"

#include <sstream>


namespace nGENE
{
	/// ISerializable factory.
	class ISerializableFactory
	{
	public:
		/// Creates new object.
		virtual ISerializable* create()=0;
		/// Returns TypeInfo object associated with this factory.
		virtual TypeInfo* getType()=0;
	};

	/// Generic ISerializable factory.
	template <typename Class>
	class SerializableFactory: public ISerializableFactory
	{
	public:
		ISerializable* create() {return new Class();}
		TypeInfo* getType() {return &Class::Type;}
	};


	/** Serializer abstract interface.
	*/
	class nGENEDLL ISerializer
	{
	protected:
		SceneManager* m_pSceneManager;
		ISerializable* m_pParent;

	public:
		ISerializer();

		/// Specifies beginning of object definition.
		virtual void addObject(const TypeInfo& _type)=0;
		/// Specifies end of object definition.
		virtual void endObject(const TypeInfo& _type)=0;

		/// Adds new attribute.
		virtual void addProperty(const string& _name, IProperty& _property)=0;
		/// Adds new attribute.
		virtual void addProperty(const string& _name, IProperty* _property)=0;

		/// Reads attribute with the given name.
		virtual void getProperty(const string& _name, IProperty& _property)=0;
		/// Reads attribute with the given name.
		virtual void getProperty(const string& _name, IProperty* _property)=0;

		/// Begins deserialization process.
		virtual void deserialize()=0;

		/// Moves to the next object.
		virtual void getNextObject()=0;

		/// Specifies scene manager.
		virtual void setSceneManager(SceneManager* _mgr);
		virtual SceneManager* getSceneManager() const;

		/// Specifies parent.
		virtual void setParent(ISerializable* _parent);

		virtual ISerializable* getParent() const;
	};



	inline void ISerializer::setSceneManager(SceneManager* _mgr)
	{
		m_pSceneManager = _mgr;
	}
//----------------------------------------------------------------------
	inline SceneManager* ISerializer::getSceneManager() const
	{
		return m_pSceneManager;
	}
//----------------------------------------------------------------------
	inline void ISerializer::setParent(ISerializable* _parent)
	{
		m_pParent = _parent;
	}
//----------------------------------------------------------------------
	inline ISerializable* ISerializer::getParent() const
	{
		return m_pParent;
	}
//----------------------------------------------------------------------
}


#endif