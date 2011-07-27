/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderConstant.h
Version:	0.13
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHADERCONSTANT_H_
#define __INC_SHADERCONSTANT_H_


#include "Prerequisities.h"
#include "ShaderConstantTypes.h"
#include "TypeInfo.h"
#include "XMLNode.h"


namespace nGENE
{
	/** Constant used by a shader program.
		@remarks
			Shader constants have multiple parameters, but only these
			with semantic specified will be automatically updated by
			the nGENE Tech.
	*/
	class nGENEDLL ShaderConstant: public XMLNode
	{
		EXPOSE_TYPE
	private:
		void* m_pValues;				///< Current value of the constant
		void* m_pDefaultValues;			///< Default value of the constant

		uint m_nValuesCount;			///< Number of values
		uint m_nOffset;					///< Offset from beginning of a parent structure

		bool m_bArray;

		SEMANTIC_DESC* m_pSemantic;		///< Pointer to constant's semantic
		DATATYPE_DESC* m_pDataType;		///< Pointer to constant's data-type

		/// Base type of the constant
		SShaderConstantType::BASE_TYPE m_Type;

		string m_stConstantName;		///< Name of the constant

		bool m_bDynamic;				///< Does it require to be updated per object
	
	protected:
		void init(const string& _type, const string& _semantic);
		void parseValues();

	public:
		ShaderConstant();
		ShaderConstant(const ShaderConstant& src);
		~ShaderConstant();

		/// Assignment operator.
		ShaderConstant& operator=(const ShaderConstant& rhs);

		void parse(XMLDocument* _node);
		void save(File* _file);
		void load() {}

		/** Updates the constant.
			@remarks
				Constants with the semantic specified are updated automatically,
				so passing values to the shader is not necessary, eg. specifying
				constant's semantic as 'world' would cause engine to pass world
				transform to the shader whenever shader is used.
		*/
		void update();

		/// Gets the values of the constant.
		void* getValues() const;
		/// Sets the values of the constant.
		void setValues(void* _values);

		/// Sets name of this constant.
		void setName(const string& _name);
		/// Returns name of this constant.
		const string& getName() const;

		/// Checks if this constant has semantic specified?
		bool hasSemantic() const;
		/// Checks if this is dynamic variable.
		bool isDynamic() const;

		void setValuesCount(uint _count);
		uint getValuesCount() const;

		void setOffset(uint _offset);
		uint getOffset() const;

		/// Sets shader constant default value.
		void setDefaultValue(void* _value);
		/** Sets shader constant default value for specified number of
			array elements.
		*/
		void setDefaultValue(void* _value, uint _count);
		/// Returns default value of this shader constant.
		void* getDefaultValues() const;

		/// Sets data base type.
		void setType(SShaderConstantType::BASE_TYPE _type);
		/// Returns data base type.
		SShaderConstantType::BASE_TYPE getType() const;

		/// Is constant an array?
		bool isArray() const;
	};



	inline void* ShaderConstant::getValues() const
	{
		return m_pValues;
	}
//----------------------------------------------------------------------
	inline void* ShaderConstant::getDefaultValues() const
	{
		return m_pDefaultValues;
	}
//----------------------------------------------------------------------
	inline const string& ShaderConstant::getName() const
	{
		return m_stConstantName;
	}
//----------------------------------------------------------------------
	inline bool ShaderConstant::isDynamic() const
	{
		return m_bDynamic;
	}
//----------------------------------------------------------------------
	inline void ShaderConstant::setName(const string& _name)
	{
		m_stConstantName = _name;
	}
//----------------------------------------------------------------------
	inline void ShaderConstant::setOffset(uint _offset)
	{
		m_nOffset = _offset;
	}
//----------------------------------------------------------------------
	inline uint ShaderConstant::getOffset() const
	{
		return m_nOffset;
	}
//----------------------------------------------------------------------
	inline void ShaderConstant::setType(SShaderConstantType::BASE_TYPE _type)
	{
		m_Type = _type;
	}
//----------------------------------------------------------------------
	inline SShaderConstantType::BASE_TYPE ShaderConstant::getType() const
	{
		return m_Type;
	}
//----------------------------------------------------------------------
	inline bool ShaderConstant::isArray() const
	{
		return m_bArray;
	}
//----------------------------------------------------------------------
}


#endif