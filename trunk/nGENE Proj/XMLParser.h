/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		XMLParser.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_XMLPARSER_H_
#define __INC_XMLPARSER_H_


#ifdef _ENGINE_BUILD
	#define PLATFORM_IMPORT     __declspec(dllimport)
#else
	#define PLATFORM_EXPORT     __declspec(dllexport)
#endif


#include "Prerequisities.h"
#include "Singleton.h"
#include "XMLDocument.h"


#include <Xercesc/Util/PlatformUtils.hpp>
#include <Xercesc/Parsers/XercesDOMParser.hpp>
#include <Xercesc/Util/XMLString.hpp>
#include <Xercesc/Framework/MemBufInputSource.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>


XERCES_CPP_NAMESPACE_USE


namespace nGENE
{
	/** Wrapper for Xerces XML parser.
	*/
	class nGENEDLL XMLParser: public Singleton <XMLParser>
	{
	private:
		/// Error handler
		class nGENEDomErrorHandler: public DefaultHandler
		{
		public:
			void warning(const SAXParseException& e);
			void error(const SAXParseException& e);
			void fatalError(const SAXParseException& e);
		} m_ErrorHandler;

	private:
		XercesDOMParser* m_pParser;					///< Xerces DOM parser
		bool m_bValidation;							///< Is validation taken?


		void setValidation(bool _value);

	public:
		XMLParser();
		virtual ~XMLParser();

		/// Initializes XML parsing.
		void init();
		void cleanup();

		/** Parses the wide-string.
			@remarks
				DOM tree is being built internally.
			@param
				_str string to be parsed.
			@param
				_validate if set to true then validation against
				schema will be taken.
		*/
		XMLDocument* parse(const wstring& _str, bool _validate=false);
	};
}


#endif