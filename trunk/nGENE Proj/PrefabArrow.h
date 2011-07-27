/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabArrow.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABARROW_H_
#define __INC_PREFABARROW_H_


#include "Node.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Visualization of the arrow.
		@remarks
			PrefabArrow although has Prefab in its name is not
			of NodeVisible type but rather Node. Therefore, to
			assign materials to it you have to either call
			its setMaterial() method or get it two components,
			namely line (getLine()) and ending (getEnd()), then
			obtain from them "Base" surface and assign material
			to it.
	*/
	class nGENEDLL PrefabArrow: public Node
	{
		EXPOSE_TYPE
	private:
		PrefabCylinder* m_pLine;
		PrefabPyramid* m_pEnd;

		float m_fLength;
		float m_fEndLength;

	public:
		PrefabArrow(float _length, float _endLength);
		PrefabArrow();
		virtual ~PrefabArrow();

		void init();
		void cleanup();

		/// Sets length of the arrow.
		void setLength(Real _length);
		/// Returns length of the arrow.
		Real getLength() const;

		/// Sets length of arrow end.
		void setEndLength(Real _length);
		/// Returns length of arrow end.
		Real getEndLength() const;

		/// Sets material for both components.
		void setMaterial(Material* _mat);


		PrefabCylinder* getLine() const;
		PrefabPyramid* getEnd() const;


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline PrefabCylinder* PrefabArrow::getLine() const
	{
		return m_pLine;
	}
//----------------------------------------------------------------------
	inline PrefabPyramid* PrefabArrow::getEnd() const
	{
		return m_pEnd;
	}
//----------------------------------------------------------------------
}


#endif