/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		XMLSerializer.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_XMLSERIALIZER_H_
#define __INC_XMLSERIALIZER_H_


#include "ISerializer.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Serializer for XML file.
	*/
	class nGENEDLL XMLSerializer: public ISerializer
	{
	private:
		static HashTable <wstring, ISerializableFactory*> s_Factories;

		XMLNode* m_pXML;
		XMLDocument* m_pCurrentNode;

		wstring m_stFileName;


		void addAttribute(const string& _name, const string& _value);
		bool getAttribute(const string& _name, string& _value);

	public:
		XMLSerializer();
		~XMLSerializer();

		void setFileName(const wstring& _fileName);

		void addObject(const TypeInfo& _type);
		void endObject(const TypeInfo& _type);

		void getNextObject();

		void addProperty(const string& _name, IProperty& _property);
		void addProperty(const string& _name, IProperty* _property);

		void getProperty(const string& _name, IProperty& _property);
		void getProperty(const string& _name, IProperty* _property);

		void setCurrentNode(XMLDocument* _document);
		XMLDocument* getCurrentNode() const;

		void deserialize();

		static void addFactory(const wstring& _name, ISerializableFactory* _factory);
		static ISerializableFactory* getFactory(const wstring& _name);
		static void removeFactories();
	};

	/// Single node of XML data file.
	struct XMLSerializedNode
	{
		XMLDocument* document;
		XMLSerializer* serializer;
		ISerializable* node;
		wstring type;


		void deserialize(XMLSerializedNode* _parent);
	};
	


	inline void XMLSerializer::addProperty(const string& _name, IProperty& _property)
	{
		addAttribute(_name, _property.toString());
	}
//----------------------------------------------------------------------
	inline void XMLSerializer::addProperty(const string& _name, IProperty* _property)
	{
		addAttribute(_name, _property->toString());
	}
//----------------------------------------------------------------------
	inline void XMLSerializer::getProperty(const string& _name, IProperty& _property)
	{
		string value;
		if(getAttribute(_name, value))
			_property.fromString(value);
	}
//----------------------------------------------------------------------
	inline void XMLSerializer::getProperty(const string& _name, IProperty* _property)
	{
		string value;
		if(getAttribute(_name, value))
			_property->fromString(value);
	}
//----------------------------------------------------------------------
	inline void XMLSerializer::setCurrentNode(XMLDocument* _document)
	{
		m_pCurrentNode = _document;
	}
//----------------------------------------------------------------------
	inline XMLDocument* XMLSerializer::getCurrentNode() const
	{
		return m_pCurrentNode;
	}
//----------------------------------------------------------------------
}


#endif