/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabPyramid.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABPYRAMID_H_
#define __INC_PREFABPYRAMID_H_


#include "NodePrefab.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Visualization of the pyramid.
		@par
			Pyramid is defined by height and side length of the
			base.
	*/
	class nGENEDLL PrefabPyramid: public NodePrefab
	{
		EXPOSE_TYPE
	private:
		float m_fHeight;				///< Pyramid height
		float m_fSideLength;			///< Base side length


		void updateGeometry();

	public:
		PrefabPyramid(float _height, float _side,
			const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		PrefabPyramid();
		~PrefabPyramid();

		void init();
		void cleanup();

		/// Sets pyramid height.
		void setHeight(Real _height);
		/// Returns pyramid height.
		Real getHeight() const;

		/// Sets length of the base side.
		void setSideLength(Real _radius);
		/// Returns pyramid length of the base side.
		Real getSideLength() const;


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline Real PrefabPyramid::getHeight() const
	{
		return m_fHeight;
	}
//----------------------------------------------------------------------
	inline Real PrefabPyramid::getSideLength() const
	{
		return m_fSideLength;
	}
//----------------------------------------------------------------------
}


#endif