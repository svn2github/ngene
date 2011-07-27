/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		XMLSerializer.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ISerializer.h"
#include "FileManager.h"
#include "XMLDocument.h"
#include "XMLNode.h"
#include "XMLParser.h"
#include "XMLSerializer.h"


namespace nGENE
{
	// Initialize static members
	HashTable <wstring, ISerializableFactory*> XMLSerializer::s_Factories;


	XMLSerializer::XMLSerializer():
		m_stFileName(L"save.nmp"),
		m_pXML(NULL),
		m_pCurrentNode(NULL)
	{
	}
//----------------------------------------------------------------------
	XMLSerializer::~XMLSerializer()
	{
	}
//----------------------------------------------------------------------
	void XMLSerializer::addFactory(const wstring& _name, ISerializableFactory* _factory)
	{
		if(s_Factories.find(_name) != s_Factories.end())
			return;

		s_Factories[_name] = _factory;
	}
//----------------------------------------------------------------------
	ISerializableFactory* XMLSerializer::getFactory(const wstring& _name)
	{
		if(s_Factories.find(_name) == s_Factories.end())
			return NULL;

		return s_Factories[_name];
	}
//----------------------------------------------------------------------
	void XMLSerializer::removeFactories()
	{
		if(s_Factories.size())
		{
			HashTable <wstring, ISerializableFactory*>::iterator iter;
			for(iter = s_Factories.begin(); iter != s_Factories.end(); ++iter)
				NGENE_DELETE(iter->second);
			s_Factories.clear();
		}
	}
//----------------------------------------------------------------------
	void XMLSerializer::addAttribute(const string& _name, const string& _value)
	{
		m_pXML->addAttribute(_name, _value);
	}
//----------------------------------------------------------------------
	bool XMLSerializer::getAttribute(const string& _name, string& _value)
	{
		_value = m_pCurrentNode->getAttribute(_name);
		if(_value == "")
			return false;
		return true;
	}
//----------------------------------------------------------------------
	void XMLSerializer::addObject(const TypeInfo& _type)
	{
		XMLNode* pNewNode = new XMLNode();

		string name(_type.Type().begin(), _type.Type().end());
		pNewNode->setName(name);

		if(m_pXML)
			m_pXML->addChild(pNewNode);
		m_pXML = pNewNode;
	}
//----------------------------------------------------------------------
	void XMLSerializer::endObject(const TypeInfo& _type)
	{
		if(m_pXML->getParent())
			m_pXML = m_pXML->getParent();
		else
		{
			File file(m_stFileName, OPEN_WRITE);
			m_pXML->save(&file);
			file.close();

			m_pXML->cleanup();
			NGENE_DELETE(m_pXML);
		}
	}
//----------------------------------------------------------------------
	void XMLSerializer::getNextObject()
	{
		if(m_pCurrentNode->getChildrenNum())
			m_pCurrentNode = m_pCurrentNode->getFirstChild();
		else
			m_pCurrentNode = m_pCurrentNode->getNextSibling();

		while(m_pCurrentNode->getTagName() == "\n")
			m_pCurrentNode = m_pCurrentNode->getNextSibling();
	}
//----------------------------------------------------------------------
	void XMLSerializer::setFileName(const wstring& _fileName)
	{
		m_stFileName = _fileName;
	}
//----------------------------------------------------------------------
	void XMLSerializer::deserialize()
	{
		File file(m_stFileName, OPEN_READ);
		wstring buffer = file.getBuffer();
		file.close();
		AutoPointer <XMLDocument> pDocument(XMLParser::getSingleton().parse(buffer, false));
		XMLDocument* pChild = pDocument->getFirstChild();
		m_pCurrentNode = pChild;

		string temp = pChild->getTagName();
		wstring type(temp.begin(), temp.end());
		while(temp == "\n")
		{
			pChild = pChild->getNextSibling();
			temp = pChild->getTagName();
		}

		m_pParent = NULL;

		XMLSerializedNode node;
		node.document = pChild;
		node.serializer = this;
		node.deserialize(NULL);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	void XMLSerializedNode::deserialize(XMLSerializedNode* _parent)
	{
		string temp = document->getTagName();
		type.assign(temp.begin(), temp.end());
		if(temp != "\n")
		{
			if(serializer->getFactory(type))
			{
				serializer->setCurrentNode(document);

				// Create node
				ISerializable* pNode = serializer->getFactory(type)->create();
				pNode->deserialize(serializer);

				this->node = pNode;
			}
			else
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Unknown type in .xml file: %ls", type.c_str());
			}
		}

		// Deserialize child nodes
		AutoPointer <XMLDocument> pChild;
		if(document == serializer->getCurrentNode())
			pChild = document->getFirstChild();
		else
			pChild = serializer->getCurrentNode()->getNextSibling();

		while(pChild)
		{
			serializer->setParent(this->node);

			string temp = pChild->getTagName();
			while(temp == "\n")
			{
				pChild = pChild->getNextSibling();
				if(!pChild)
					break;
				temp = pChild->getTagName();
			}

			if(!pChild)
				break;

			XMLSerializedNode node;
			node.document = pChild;
			node.serializer = serializer;
			node.deserialize(this);
			
			pChild = pChild->getNextSibling();
		}
	}
//----------------------------------------------------------------------
}