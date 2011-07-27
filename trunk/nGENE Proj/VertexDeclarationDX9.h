/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexDeclarationDX9.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VERTEXDECLARATIONDX9_H_
#define __INC_VERTEXDECLARATIONDX9_H_


#include "Prerequisities.h"
#include "VertexDeclaration.h"


namespace nGENE
{
	/** Vertex declaration for DirectX 9.0c.
	*/
	class nGENEDLL VertexDeclarationDX9: public VertexDeclaration
	{
	protected:
		IDirect3DVertexDeclaration9* m_pVertexDecl;			///< Vertex declaration


		void addDummyElement(D3DVERTEXELEMENT9* _element);

	public:
		VertexDeclarationDX9();
		~VertexDeclarationDX9();

		/// Inits vertex declaration.
		void init();

		/// Adds single vertex element.
		void addVertexElement(const VERTEX_ELEMENT& _element);
		/** Adds array of vertex elements.
			@param
				_array array of vertex elements.
			@param
				_count number of vertex elements stored in the array.
		*/
		void addVertexElement(SVertexElement* _array, uint _count);

		/// Binds vertex declaration.
		void bind();
	};


	/// Factory to be used for DirectX9 VertexDeclaration class objects creation.
	class nGENEDLL VertexDeclarationDX9Factory: public VertexDeclarationFactory
	{
	public:
		VertexDeclaration* create() {return new VertexDeclarationDX9();}
	};
}


#endif
