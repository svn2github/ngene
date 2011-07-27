/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		XMLDocument.h
Version:	0.08
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_XMLDOCUMENT_H_
#define __INC_XMLDOCUMENT_H_


#include "Prerequisities.h"

#include <Xercesc/DOM/DOM.hpp>


XERCES_CPP_NAMESPACE_USE


namespace nGENE
{
	/** Wrapper fox Xerces DOM document.
	*/
	class nGENEDLL XMLDocument
	{
	private:
		/// Xerces DOM Document/Node
		DOMNode* m_pNode;

	public:
		XMLDocument();
		XMLDocument(DOMNode* _pNode);
		virtual ~XMLDocument();

		void setNode(XERCES_CPP_NAMESPACE::DOMDocument* _node) {m_pNode=_node;}

		/// Helper method returning value of id attribute.
		string getID() const;

		/// Returns value of the attribute specified.
		string getAttribute(const string& _attribute) const;
		/// Returns name of the currently node (tag).
		string getTagName() const;
		/// Returns value of the node.
		string getValue() const;

		/// Returns sibling of the node or NULL if it does not exist.
		XMLDocument* getNextSibling() const;
		/// Returns first child of the node or NULL if it does not exist.
		XMLDocument* getFirstChild() const;
		/// Returns _index-th child of the node or NULL if it does not exist.
		XMLDocument* getChild(uint _index) const;
		/** Returns parent of the node or NULL if it does not exist.
			@remarks
				If function returns NULL it in the most cases mean that the
				node is a root node of the XML document. It can also, in some
				rare cases, signal error situation.
		*/
		XMLDocument* getParent() const;

		/// Returns number of node's children.
		uint getChildrenNum() const;
	};
}


#endif