/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexDeclaration.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VERTEXDECLARATION_H_
#define __INC_VERTEXDECLARATION_H_


#include "Prerequisities.h"


namespace nGENE
{
	/// Usage of the vertex element
	enum VERTEX_ELEMENT_USAGE
	{
		VEU_POSITION,
		VEU_NORMAL,
		VEU_BINORMAL,
		VEU_TANGENT,
		VEU_TEXCOORD,
		VEU_COLOUR,
		VEU_FOG,
		VEU_DEPTH,
		VEU_SAMPLE,
		VEU_BLEND_WEIGHTS,
		VEU_BLEND_INDICES
	};

	/// Single vertex element
	typedef struct SVertexElement
	{
		uint stream;						///< ID of the stream
		uint offset;						///< Offset in bytes from the beginning
		uint elementIndex;					/**< Index of the element (applicable if there are multiple
												 with the same usage) */
		uint size;							///< Number of input elements
		VERTEX_ELEMENT_USAGE usage;			///< Usage of the element
	} VERTEX_ELEMENT;

	/** Vertex declaration.
		@par
			This class specifies the layout of the vertices in the
			vertex buffers.
	*/
	class nGENEDLL VertexDeclaration
	{
	protected:
		vector <SVertexElement> m_Elements;

	public:
		VertexDeclaration();
		virtual ~VertexDeclaration();

		/// Initializes vertex declaration.
		virtual void init()=0;

		/// Adds single vertex element.
		virtual void addVertexElement(const VERTEX_ELEMENT& _element);
		/** Adds array of vertex elements.
			@param
				_array array of vertex elements.
			@param
				_count number of vertex elements stored in the array.
		*/
		virtual void addVertexElement(SVertexElement* _array, uint _count);

		/// Binds vertex declaration.
		virtual void bind()=0;


		/** Returns element with a given index.
			@returns
				VertexElement* pointer to the vertex element or NULL
				if it wasn't found.
		*/
		VERTEX_ELEMENT* getVertexElement(uint _index);
		/** Returns first element with given index.
			@returns
				VertexElement* pointer to the vertex element or NULL
				if it wasn't found.
		*/
		VERTEX_ELEMENT* getVertexElement(VERTEX_ELEMENT_USAGE _usage);

		/// Returns number of elements.
		uint getCount() const;
	};



	inline VERTEX_ELEMENT* VertexDeclaration::getVertexElement(uint _index)
	{
		if(_index >= m_Elements.size())
			return NULL;

		return &m_Elements[_index];
	}
//----------------------------------------------------------------------
	inline uint VertexDeclaration::getCount() const
	{
		return m_Elements.size();
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------


	/// Factory to be used for VertexDeclaration class objects creation.
	class nGENEDLL VertexDeclarationFactory
	{
	public:
		virtual VertexDeclaration* create()=0;
	};
}


#endif
