/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderDX9.h
Version:	0.12
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHADERDX9_H_
#define __INC_SHADERDX9_H_


#include <D3DX9Shader.h>
#include "Prerequisities.h"
#include "Shader.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Shader DirectX 9 class.
	*/
	class nGENEDLL ShaderDX9: public Shader
	{
		EXPOSE_TYPE
	private:
		// Type of the shader used
		union
		{
			IDirect3DVertexShader9* m_pVertexShader;	///< Vertex Shader
			IDirect3DPixelShader9* m_pPixelShader;		///< Pixel Shader
			ID3DXEffect* m_pEffect;						///< Effect
		};


		D3DXMACRO* getMacros();

		D3DXHANDLE getVariableByName(const string& _name) const;

		void loadConstants();
		void handleErrors(ID3DXBuffer* _errors);

	protected:
		ID3DXConstantTable* m_pConstants;				///< Table of shader constants

		void createVertexShader();
		void createGeometryShader();
		void createPixelShader();
		/// @todo Implement this.
		void createEffect();

	public:
		ShaderDX9(const SHADER_DESC& _desc);
		~ShaderDX9();

		void cleanup();

		void bind() const;
		void unbind() const;

		void setFloat(const string& _name, const float& _value);
		void setFloatArray(const string& _name, const float* _values,
			const uint _count) const;
		void setMatrix(const string& _name, const Matrix4x4& _mat);
		void setInteger(const string& _name, const int& _value);
		void setIntegerArray(const string& _name, const int* _values,
			const uint _count) const;
		void setBoolean(const string& _name, const bool& _value);
		void setBooleanArray(const string& _name, const bool* _values,
			const uint _count) const;
	};
}


#endif