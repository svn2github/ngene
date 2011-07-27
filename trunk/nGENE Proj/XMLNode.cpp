/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		XMLNode.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "XMLNode.h"


namespace nGENE
{
	XMLNode::XMLNode():
		m_stName("unknown"),
		m_stValue(""),
		m_pParent(NULL)
	{
	}
//----------------------------------------------------------------------
	XMLNode::XMLNode(const string& _name):
		m_stName(_name),
		m_stValue(""),
		m_pParent(NULL)
	{
	}
//----------------------------------------------------------------------
	XMLNode::XMLNode(const string& _name, const string& _value):
		m_stName(_name),
		m_stValue(_value),
		m_pParent(NULL)
	{
	}
//----------------------------------------------------------------------
	XMLNode::~XMLNode()
	{
		if(!m_vChildren.empty())
			m_vChildren.clear();

		if(!m_Attributes.empty())
			m_Attributes.clear();
	}
//----------------------------------------------------------------------
	void XMLNode::cleanup()
	{
		for(uint i = 0; i < m_vChildren.size(); ++i)
		{
			m_vChildren[i]->cleanup();
			NGENE_DELETE(m_vChildren[i]);
		}

		m_vChildren.clear();
	}
//----------------------------------------------------------------------
	void XMLNode::addAttribute(const Attribute& _attribute)
	{
		m_Attributes[_attribute.first] = _attribute.second;
	}
//----------------------------------------------------------------------
	void XMLNode::addAttribute(const string& _name, const string& _value)
	{
		m_Attributes[_name] = _value;
	}
//----------------------------------------------------------------------
	void XMLNode::addChild(XMLNode* _child)
	{
		_child->setParent(this);
		m_vChildren.push_back(_child);
	}
//----------------------------------------------------------------------
	void XMLNode::parse(XMLDocument* _document)
	{
	}
//----------------------------------------------------------------------
	void XMLNode::load()
	{
	}
//----------------------------------------------------------------------
	void XMLNode::save(File* _file)
	{
		// Save the node itself
		_file->write("<%s", m_stName.c_str());
		HashTable <string, string>::iterator iter;
		for(iter = m_Attributes.begin(); iter != m_Attributes.end(); ++iter)
			_file->write(" %s=\"%s\"", iter->first.c_str(), iter->second.c_str());

		if(m_stValue.compare("") == 0)
		{
			if(!m_vChildren.size())
			{
				_file->write(" />\n");
			}
			else
			{
				_file->write(">\n");

				// Save all child nodes
				vector <XMLNode*>::iterator iter;
				for(iter = m_vChildren.begin(); iter != m_vChildren.end(); ++iter)
					(*iter)->save(_file);
				_file->write("</%s>\n", m_stName.c_str());
			}
		}
		else
		{
			_file->write(">%s</%s>\n", m_stValue.c_str(), m_stName.c_str());
		}
	}
//----------------------------------------------------------------------
}