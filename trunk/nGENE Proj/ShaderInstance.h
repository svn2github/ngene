/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderInstance.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHADERINSTANCE_H_
#define __INC_SHADERINSTANCE_H_


#include "Prerequisities.h"
#include "TypeInfo.h"
#include "XMLNode.h"


namespace nGENE
{
	/** ShaderInstance class.
		@remarks
			As different materials can share some of the shaders,
			there is need to modify their behaviour for all materials
			without affecting shader itself.
	*/
	class nGENEDLL ShaderInstance: public XMLNode
	{
		EXPOSE_TYPE
	protected:
		mutable vector <ShaderConstant*> m_pVariables;
		HashTable <string, ShaderConstant*> m_Variables;

		Shader* m_pShader;					///< Pointer to the existing shader object.


		void addNonInstanceVariables();

	public:
		ShaderInstance();
		ShaderInstance(const ShaderInstance& src);
		~ShaderInstance();

		/// Assignment operator.
		ShaderInstance& operator=(const ShaderInstance& rhs);

		/// Updates shader instance
		void update() const;

		void save(File* _file);
		void load() {}

		/// Parses shader instance.
		void parse(XMLDocument* _node);
		void cleanup();

		/// Binds shader instance to the rendering pipeline.
		void bind() const;
		/// Unbinds shader instance.
		void unbind() const;

		/// Sets shader object pointer.
		void setShader(Shader* _shader);
		/// Returns shader object pointer.
		Shader* getShader() const;

		/// Lets you add shader constant to this instance.
		void addShaderConstant(const ShaderConstant& _constant);

		/// Returns shader constant with a given name.
		ShaderConstant* getConstant(const string& _name);
		/// Returns shader constant with a given index.
		ShaderConstant* getConstant(uint _index);

		/// Returns shader constants count.
		uint getConstantsNum() const;
	};



	inline void ShaderInstance::setShader(Shader* _shader)
	{
		m_pShader = _shader;
	}
//----------------------------------------------------------------------
	inline Shader* ShaderInstance::getShader() const
	{
		return m_pShader;
	}
//----------------------------------------------------------------------
	inline uint ShaderInstance::getConstantsNum() const
	{
		return m_pVariables.size();
	}
//----------------------------------------------------------------------
}


#endif