/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderConstantTypes.h
Version:	0.01
Info:		This file contains various types definitions which
			are used by the shader constants.
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHADERCONSTANTTYPES_H_
#define __INC_SHADERCONSTANTTYPES_H_


namespace nGENE
{
	/** Class describing shader semantic function.
	*/
	class nGENEDLL ShaderSemanticFunction
	{
	public:
		virtual void operator()(void* _values, uint _count)=0;
	};

	/** Structure describing semantic.
		@remarks
			Semantic is the way in which shader constant is interpreted
			by the nGENE. It specifies some specific constant behaviour
			in which it is similar to DirectX semantic.
	*/
	typedef struct SShaderConstantSemantic
	{
		wstring stName;						///< Semantic's name
		wstring stDataType;					///< Which data-type is associated with
		void (*pFunction)(void*, uint);		///< Function called for semantic
		ShaderSemanticFunction* function;

		SShaderConstantSemantic():
		pFunction(NULL),
			function(NULL)
		{
		}
	} SEMANTIC_DESC;

	/// Structure describing data-type.
	typedef struct SShaderConstantType
	{
		wstring stName;						///< Type name
		uint nValuesCount;					///< How many values it contains

		///< Base type for this type
		enum BASE_TYPE
		{
			BT_FLOAT,
			BT_INT,
			BT_BOOL
		} baseType;
	} DATATYPE_DESC;
}


#endif