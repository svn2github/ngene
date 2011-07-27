/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		World.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_WORLD_H_
#define __INC_WORLD_H_


#include "Prerequisities.h"
#include "XMLNode.h"


namespace nGENE
{
	/** Class for loading world into the engine.
	*/
	class nGENEDLL World: public XMLNode
	{
	private:
		wstring m_stName;					///< World's name
		wstring m_stAuthor;					///< World's author

		float m_fVersion;					///< World's version number

	public:
		World();
		virtual ~World();

		void parse(XMLDocument* _document);
		void load() {}
		void save(File* _file);
	};
}


#endif