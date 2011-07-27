/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TypeInfo.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "TypeInfo.h"


namespace nGENE
{
	// Initialize static members
	const wstring TypeInfo::s_NoParent = L"NULL";


	TypeInfo::TypeInfo(): m_pParentType(NULL)
	{
	}
//----------------------------------------------------------------------
	TypeInfo::TypeInfo(const std::wstring& _type, TypeInfo* _parent):
		m_pParentType(_parent),
		m_stTypeName(_type)
	{
	}
//----------------------------------------------------------------------
	TypeInfo::~TypeInfo()
	{
	}
//----------------------------------------------------------------------
	const wstring& TypeInfo::getParentTypeName() const
	{
		if(m_pParentType)
			return m_pParentType->getTypeName();
		
		return s_NoParent;
	}
//----------------------------------------------------------------------
	const wstring& TypeInfo::ParentType() const
	{
		if(m_pParentType)
			return m_pParentType->getTypeName();
		
		return s_NoParent;
	}
//----------------------------------------------------------------------
	bool TypeInfo::isChild(TypeInfo* _type)
	{
		TypeInfo* pCurrent = this;
		while(pCurrent)
		{
			if(pCurrent == _type)
				return true;
			pCurrent = pCurrent->Parent();
		}

		return false;
	}
//----------------------------------------------------------------------
	void TypeInfo::setTypeName(const wstring& _type)
	{
		m_stTypeName = _type;
	}
//----------------------------------------------------------------------
}