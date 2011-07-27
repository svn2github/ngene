/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShaderTypes.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHADERTYPES_H_
#define __INC_SHADERTYPES_H_


namespace nGENE
{
	/** Type of the shader.
	*/
	enum SHADER_TYPE
	{
		SHADER_UNKNOWN,
		SHADER_VERTEX,
		SHADER_GEOMETRY,
		SHADER_PIXEL,
		SHADER_EFFECT						/**< Comprehensive effect file featuring
												 all shader types */
	};

	/// Description of the single shader.
	typedef struct nGENEDLL SShaderDescription
	{
		SHADER_TYPE type;					///< Type of the shader
		bool debugInfo;						/**< Should debug info be included in the compiled
												 version? */
		wstring fileName;					///< Name of the file containing shader code
		wstring name;						///< Identifier of the shader in nGENE Tech
		string profile;						///< Profile shader would be compiled against
		string function;					///< Entry point for the shader code

		SShaderDescription():
			type(SHADER_UNKNOWN),
			debugInfo(false),
			fileName(L""),
			name(L""),
			profile(""),
			function("")
		{
		}
	} SHADER_DESC;
}


#endif