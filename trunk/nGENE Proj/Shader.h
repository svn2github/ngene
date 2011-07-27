/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Shader.h
Version:	0.14
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHADER_H_
#define __INC_SHADER_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "ShaderTypes.h"
#include "TypeInfo.h"
#include "XMLNode.h"


namespace nGENE
{
	/** Shader class.
		@par
			Shaders are mechanisms which give you direct access to
			the GPU functionality.
		@par
			nGENE is a shader-based engine, meaning fixed pipeline
			functionality has been reduced to minimum required for
			pre-DirectX 10-cards to run it.
	*/
	class nGENEDLL Shader: public XMLNode
	{
		EXPOSE_TYPE
	protected:
		/// Vector of shader constants.
		mutable vector <ShaderConstant*> m_pVariables;
		/// HashTable of pointers to shader constants.
		mutable HashTable <string, ShaderConstant*> m_pVariablesRef;

		/// Vector of defines
		vector <string> m_vDefines;

		/// Description of the shader.
		SHADER_DESC m_ShaderDesc;


		virtual void createVertexShader()=0;
		virtual void createGeometryShader()=0;
		virtual void createPixelShader()=0;
		virtual void createEffect()=0;

	public:
		Shader(const SHADER_DESC& _desc);
		virtual ~Shader();

		/// Creates the shader.
		void create();
		void update() const;

		/// Parses the shader.
		void parse(XMLDocument* _node);
		virtual void cleanup()=0;

		virtual void load() {};

		/// Binds shader to the rendering pipeline.
		virtual void bind() const;
		/// Unbinds shader.
		virtual void unbind() const=0;

		/// All constants are set to their default values.
		virtual void setDefaults() const;

		/** Sets floating point constant.
			@param
				_name name of the shader's constant.
			@param
				_value floating point value being passed to the shader.
		*/
		virtual void setFloat(const string& _name, const float& _value)=0;

		/** Sets floating point array constant.
			@param
				_name name of the shader's constant.
			@param
				_values array of values to be passed to the shader.
			@param
				_count number of floating point numbers count in the array.
		*/
		virtual void setFloatArray(const string& _name, const float* _values,
			const uint _count) const=0;
		virtual void setMatrix(const string& _name, const Matrix4x4& _mat)=0;
		virtual void setInteger(const string& _name, const int& _value)=0;
		virtual void setIntegerArray(const string& _name, const int* _values,
			const uint _count) const=0;
		virtual void setBoolean(const string& _name, const bool& _value)=0;
		virtual void setBooleanArray(const string& _name, const bool* _values,
			const uint _count) const=0;

		
		/** Returns shader constant.
			@returns
				ShaderConstant* pointer to the shader's constant if _index is valid
				or NULL otherwise.
		*/
		ShaderConstant* getConstant(uint _index) const;

		/** Returns shader constant by name.
			@returns
				ShaderConstant* pointer to the shader's constant if _index is valid
				or NULL otherwise.
		*/
		ShaderConstant* getConstant(const string& _name) const;
		/// Returns number of shader constants.
		uint getConstantsNum() const;

		/// Returns description of the shader.
		SHADER_DESC& getDesc();
	};



	inline ShaderConstant* Shader::getConstant(uint _index) const
	{
		if(_index < m_pVariables.size())
			return m_pVariables[_index];

		return NULL;
	}
//----------------------------------------------------------------------
	inline SHADER_DESC& Shader::getDesc()
	{
		return m_ShaderDesc;
	}
//----------------------------------------------------------------------
	inline uint Shader::getConstantsNum() const
	{
		return m_pVariables.size();
	}
//----------------------------------------------------------------------
}


#endif