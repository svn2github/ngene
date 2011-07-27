/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		XMLParser.cpp
Version:	0.05
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "XMLParser.h"


namespace nGENE
{
	XMLParser::XMLParser()
	{
		init();
	}
//----------------------------------------------------------------------
	XMLParser::~XMLParser()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void XMLParser::init()
	{
		XMLPlatformUtils::Initialize();
		m_pParser = new XercesDOMParser();

		// Configure parser
		setValidation(false);

		m_pParser->cacheGrammarFromParse(true);
		m_pParser->setErrorHandler(&m_ErrorHandler);
	}
//----------------------------------------------------------------------
	void XMLParser::cleanup()
	{
		m_pParser->resetCachedGrammarPool();

		NGENE_DELETE(m_pParser);
		XMLPlatformUtils::Terminate();
	}
//----------------------------------------------------------------------
	XMLDocument* XMLParser::parse(const wstring& _str, bool _validate)
	{
		// Parse the input
		_TRY_BEGIN
			const char* bufID = "buffer";
			char* temp = XMLString::transcode(_str.c_str());
			MemBufInputSource src((const XMLByte*)temp, _str.length(), bufID);
			// Prepare for validation
			if(m_bValidation != _validate)
				setValidation(_validate);
			m_pParser->parse(src);

			DOMNode* pNode = m_pParser->getDocument();
			XERCES_CPP_NAMESPACE::DOMDocument* pDocument = static_cast<XERCES_CPP_NAMESPACE::DOMDocument*>(pNode);
			if(!pNode)
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Data cannot be parsed correctly");
				return NULL;
			}

			if(temp)
				XMLString::release(&temp);

			return (new XMLDocument(pDocument->getDocumentElement()));
		_CATCH(bad_cast)
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cast could not be performed!");
			return NULL;
		_CATCH_END
	}
//----------------------------------------------------------------------
	void XMLParser::setValidation(bool _value)
	{
		m_pParser->setDoSchema(_value);
		m_pParser->setDoNamespaces(_value);
		if(_value)
			m_pParser->setValidationScheme(XercesDOMParser::Val_Always);
		else
			m_pParser->setValidationScheme(XercesDOMParser::Val_Never);

		m_bValidation = _value;
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	void XMLParser::nGENEDomErrorHandler::warning(const SAXParseException& e)
	{
		Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__,
			__LINE__, L"%ls on line %d", e.getMessage(), e.getLineNumber());
	}
//----------------------------------------------------------------------
	void XMLParser::nGENEDomErrorHandler::error(const SAXParseException& e)
	{
		Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
			__LINE__, L"%ls on line %d", e.getMessage(), e.getLineNumber());
	}
//----------------------------------------------------------------------
	void XMLParser::nGENEDomErrorHandler::fatalError(const SAXParseException& e)
	{
		Log::log(LET_FATAL_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
			__LINE__, L"%ls on line %d", e.getMessage(), e.getLineNumber());
	}
//----------------------------------------------------------------------
}