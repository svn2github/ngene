/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TypeInfo.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TYPEINFO_H_
#define __INC_TYPEINFO_H_


#include <string>
#include "Prerequisities.h"


namespace nGENE
{
	/** Dynamic type information.
		@remarks
			To make use of dynamic type information classes have to declare
			static member of TypeInfo type which should be initialized with
			the proper class and its parent's name.
		@par
			If you want to find out object's type just call getTypeName() method.
	*/
	class nGENEDLL TypeInfo
	{
	protected:
		TypeInfo* m_pParentType;					///< Type of the parent
		wstring m_stTypeName;						///< Name of the class

		static const wstring s_NoParent;

	public:
		TypeInfo();
		TypeInfo(const wstring& _type, TypeInfo* _parent);
		virtual ~TypeInfo();

		/// Sets type name.
		void setTypeName(const wstring& _type);
		/// Sets parent's type name.
		void setParentType(TypeInfo* _type) {m_pParentType=_type;}

		/// Gets name of the class.
		const wstring& getTypeName() const {return m_stTypeName;}
		/// Gets name of the class.
		const wstring& Type() const {return m_stTypeName;}

		/// Gets name of the super-class.
		const wstring& getParentTypeName() const;
		/// Gets name of the super-class.
		const wstring& ParentType() const;

		/// Gets type of the parental class.
		TypeInfo* getParentType() const {return m_pParentType;}
		/// Gets type of the parental class.
		TypeInfo* Parent() const {return m_pParentType;}

		/// Checks if specified type belongs to hierarchy.
		bool isChild(TypeInfo* _type);
	};


	/// Use following macro to add type info support to the class.
	#define EXPOSE_TYPE public: static TypeInfo Type;
}


#endif