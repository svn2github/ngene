/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderManager.h
Version:	0.10
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHADERMANAGER_H_
#define __INC_SHADERMANAGER_H_


#include "BaseClass.h"
#include "HashTable.h"
#include "Prerequisities.h"
#include "Shader.h"
#include "Singleton.h"


namespace nGENE
{
	/** Manages shaders.
		@remarks
			All shaders, sharing same Shader interface are stored in the
			hash-table within the manager.
	*/
	class nGENEDLL ShaderManager: public BaseClass, public Singleton <ShaderManager>
	{
	private:
		/// HashTable storing all shaders
		HashTable <wstring, Shader*> m_pShaders;

		/// HashTable storing all semantics
		HashTable <wstring, SEMANTIC_DESC*> m_pSemantics;
		/// HashTable storing all constant data-type
		HashTable <wstring, DATATYPE_DESC*> m_pDataTypes;

		/// Active vertex shader.
		Shader* m_pActiveVS;
		/// Active geometry shader.
		Shader* m_pActiveGS;
		/// Active pixel shader.
		Shader* m_pActivePS;


		/// Debuggable shaders?
		bool m_bDebugEnabled;


		void bindVertexShader(Shader* _vertexShader);
		void bindGeometryShader(Shader* _geometryShader);
		void bindPixelShader(Shader* _pixelShader);

	public:
		ShaderManager();
		virtual ~ShaderManager();

		void init();
		void cleanup();

		/** Creates new shader.
			@param
				_desc description of the shader.
			@returns
				Shader* pointer to the newly created object or pointer to
				the existing Shader object if one with the same filename exists.
		*/
		Shader* createShader(const SHADER_DESC& _desc);

		/** Gets pointer to the shader with the filename specified.
			@param
				_fileName name of the shader file.
			@returns
				Shader* pointer existing shader object. If no matching shader is
				found, NULL is returned.
		*/
		Shader* getShader(const wstring& _fileName);
		/** Remove specified shader from manager.
			@param
				_name name of Shader to remove.
		*/
		void removeShader(const wstring& _name);

		/** Checks if specified shader already exists.
			@returns
				bool if the shader is found the function returns true and
				false otherwise.
		*/
		bool findShader(const wstring& _fileName);

		/** Activates shader specified.
			@param
				_name name of Shader to bind.
		*/
		void bindShader(const wstring& _name);

		/// Binds given shader.
		void bindShader(Shader* _shader);
		/// Unbinds shader of given type.
		void unbindShader(SHADER_TYPE _type);

		/** Returns number of shaders registered in the manager.
			@returns
				uint number of the shaders registered in the manager.
		*/
		uint getShadersNum() const;

		/// Creates new data type.
		DATATYPE_DESC* createDataType();

		/** Registers new data type for the shader constants.
			@param
				_type description of the newly registered type.
		*/
		void addDataType(DATATYPE_DESC* _type);

		/** Returns data type with the given name.
			@param
				_name name of the type to be returned.
			@returns
				DATATYPE_DESC* pointer to the type description.
		*/
		DATATYPE_DESC* getDataType(const wstring& _name);

		/// Creates new semantic.
		SEMANTIC_DESC* createSemantic();

		/** Registers new data semantic for the shader constants.
			@param
				_type description of the newly registered semantic.
		*/
		void addSemantic(SEMANTIC_DESC* _semantic);

		/** Returns semantic with the given name.
			@param
				_name name of the semantic to be returned.
			@returns
				SEMANTIC_DESC* pointer to the semantic description.
		*/
		SEMANTIC_DESC* getSemantic(const wstring& _name);

		/// Enables/disables shader debugging.
		void setDebugEnabled(bool _value);
		bool isDebugEnabled() const;
	};



	inline uint ShaderManager::getShadersNum() const
	{
		return m_pShaders.size();
	}
//----------------------------------------------------------------------
	inline void ShaderManager::setDebugEnabled(bool _value)
	{
		m_bDebugEnabled = _value;
	}
//----------------------------------------------------------------------
	inline bool ShaderManager::isDebugEnabled() const
	{
		return m_bDebugEnabled;
	}
//----------------------------------------------------------------------
}


#endif