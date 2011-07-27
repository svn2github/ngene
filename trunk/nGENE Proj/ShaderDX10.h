/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderDX10.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHADERDX10_H_
#define __INC_SHADERDX10_H_


#include <D3D10_1.h>
#include <D3DX10Async.h>
#include "Prerequisities.h"
#include "Shader.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Shader DirectX 10 class.
	*/
	class nGENEDLL ShaderDX10: public Shader
	{
		EXPOSE_TYPE
	private:
		// Type of the shader used
		union
		{
			ID3D10VertexShader* m_pVertexShader;	///< Vertex Shader
			ID3D10GeometryShader* m_pGeometryShader;///< Geometry shader
			ID3D10PixelShader* m_pPixelShader;		///< Pixel Shader
		};
		ID3D10Blob* m_pCode;						///< Pointer to compiled shader file

//		D3DXHANDLE getVariableByName(const string& _name) const;
		void loadConstants();
		void handleErrors(ID3D10Blob* _errors);

	protected:
		ID3D10Buffer* m_pConstants;				///< Table of shader constants

		void createVertexShader();
		void createGeometryShader();
		void createPixelShader();
		/// @todo Implement this.
		void createEffect();

	public:
		ShaderDX10(const SHADER_DESC& _desc);
		~ShaderDX10();

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