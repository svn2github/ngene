/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		XMLNode.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_XMLNODE_H_
#define __INC_XMLNODE_H_


#include "HashTable.h"
#include "ISerializable.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** XML node is the smallest entity in the XML language.
	*/
	class nGENEDLL XMLNode
	{
	public:
		typedef pair <string, string> Attribute;

	protected:
		string m_stName;							///< Node's name
		string m_stValue;							///< Node's value

		HashTable <string, string> m_Attributes;	///< Node's attributes
		vector <XMLNode*> m_vChildren;				///< Node's children

		XMLNode* m_pParent;							///< Node's parent

	public:
		XMLNode();
		XMLNode(const string& _name);
		XMLNode(const string& _name, const string& _value);
		virtual ~XMLNode();

		void cleanup();

		void setName(const string& _name);
		void setName(const char* _name);
		string& getName() {return m_stName;}

		/** Sets the node's value.
			@remarks
				The node which has its value cannot have any child
				nodes, so setting value overwrites all information
				about them.
		*/
		void setValue(const string& _value) {m_stValue=_value;}

		/// Adds node's attribute.
		void addAttribute(const Attribute& _attribute);
		/// Adds node's attribute.
		void addAttribute(const string& _name, const string& _value);
		/// Adds child node to the node.
		void addChild(XMLNode* _child);

		/// Sets parent of the node.
		void setParent(XMLNode* _parent);
		/// Returns parent of the node.
		XMLNode* getParent() const;

		/// Parses the XML document.
		virtual void parse(XMLDocument* _document);
		virtual void load();
		/// Saves XML node to the file.
		virtual void save(File* _file);
	};



	inline void XMLNode::setName(const string& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
	inline void XMLNode::setName(const char* _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
	inline void XMLNode::setParent(XMLNode* _parent)
	{
		m_pParent = _parent;
	}
//----------------------------------------------------------------------
	inline XMLNode* XMLNode::getParent() const
	{
		return m_pParent;
	}
//----------------------------------------------------------------------
}


#endif
