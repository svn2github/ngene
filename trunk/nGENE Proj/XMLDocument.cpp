/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		XMLDocument.h
Version:	0.10
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "XMLDocument.h"


namespace nGENE
{
	XMLDocument::XMLDocument(): m_pNode(NULL)
	{
	}
//----------------------------------------------------------------------
	XMLDocument::XMLDocument(DOMNode* _pNode): m_pNode(_pNode)
	{
	}
//----------------------------------------------------------------------
	XMLDocument::~XMLDocument()
	{
		m_pNode = NULL;
	}
//----------------------------------------------------------------------
	string XMLDocument::getID() const
	{
		return getAttribute("id");
	}
//----------------------------------------------------------------------
	string XMLDocument::getAttribute(const string& _attribute) const
	{
		_TRY_BEGIN
			const DOMElement& element = static_cast<const DOMElement&>(*m_pNode);
			XMLCh* nameKey = XMLString::transcode(_attribute.c_str());
			char* temp = XMLString::transcode(element.getAttribute(nameKey));

			string result = temp;
			
			// Release resources
			XMLString::release(&nameKey);
			XMLString::release(&temp);

			return result;
		_CATCH(bad_cast)
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cast could not be performed!");
			return "";
		_CATCH_END
	}
//----------------------------------------------------------------------
	string XMLDocument::getTagName() const
	{
		_TRY_BEGIN
			short type = m_pNode->getNodeType();
			char* temp = NULL;
			if(type == DOMNode::ELEMENT_NODE)
			{
				const DOMElement& element = static_cast<const DOMElement&>(*m_pNode);
				temp = XMLString::transcode(element.getTagName());
			}
			else if(type == DOMNode::TEXT_NODE)
			{
				const DOMText& element = static_cast<const DOMText&>(*m_pNode);
				temp = XMLString::transcode(element.getData());
			}

			if(temp)
			{
				string result = temp;
				XMLString::release(&temp);

				return result;
			}
		_CATCH(bad_cast)
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cast could not be performed!");
		_CATCH_END

		return "";
	}
//----------------------------------------------------------------------
	string XMLDocument::getValue() const
	{
		_TRY_BEGIN
			short type = m_pNode->getNodeType();
			char* temp = NULL;

			if(type == DOMNode::ELEMENT_NODE)
			{
				const DOMElement& element = static_cast<const DOMElement&>(*m_pNode);
				temp = XMLString::transcode(element.getTextContent());
			}

			if(temp)
			{
				string result = temp;
				XMLString::release(&temp);

				return result;
			}
		_CATCH(bad_cast)
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cast could not be performed!");
		_CATCH_END

		return "";
	}
//----------------------------------------------------------------------
	XMLDocument* XMLDocument::getNextSibling() const
	{
		if(m_pNode->getNextSibling())
			return (new XMLDocument(m_pNode->getNextSibling()));
		else
			return NULL;
	}
//----------------------------------------------------------------------
	XMLDocument* XMLDocument::getFirstChild() const
	{
		if(m_pNode->hasChildNodes())
			return (new XMLDocument(m_pNode->getFirstChild()));
		else
			return NULL;
	}
//----------------------------------------------------------------------
	XMLDocument* XMLDocument::getChild(uint _index) const
	{
		if(m_pNode->getChildNodes()->getLength() > _index)
			return (new XMLDocument(m_pNode->getChildNodes()->item(_index)));
		else
			return NULL;
	}
//----------------------------------------------------------------------
	XMLDocument* XMLDocument::getParent() const
	{
		if(m_pNode->getParentNode())
			return (new XMLDocument(m_pNode->getParentNode()));
		else
			return NULL;
	}
//----------------------------------------------------------------------
	uint XMLDocument::getChildrenNum() const
	{
		return (m_pNode->getChildNodes()->getLength());
	}
//----------------------------------------------------------------------
}