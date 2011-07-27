/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabTorus.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABTORUS_H_
#define __INC_PREFABTORUS_H_


#include "NodePrefab.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Visualization of the torus.
		@par
			PrefabTorus is a donut-shaped mesh.
	*/
	class nGENEDLL PrefabTorus: public NodePrefab
	{
		EXPOSE_TYPE
	private:
		float m_fInnerRadius;			///< Inner radius
		float m_fOuterRadius;			///< Outer radius

		uint m_nRadialSlices;			///< Number of slices along radius
		uint m_nCircleSlices;			///< Number of slices along perimeter


		void updateGeometry();

	public:
		PrefabTorus(uint _radialSlices, uint _circleSlices, float _innerRadius,
			float _outerRadius, const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		PrefabTorus();
		~PrefabTorus();

		void init();
		void cleanup();

		/// Sets inner radius.
		void setInnerRadius(Real _radius);
		Real getInnerRadius() const;

		/// Sets outer radius.
		void setOuterRadius(Real _radius);
		Real getOuterRadius() const;

		/// Sets slices along perimeter.
		void setRadialSlices(uint _slices);
		uint getRadialSlices() const;

		/// Sets slices along radius.
		void setCircleSlices(uint _slices);
		uint getCircleSlices() const;

		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline Real PrefabTorus::getInnerRadius() const
	{
		return m_fInnerRadius;
	}
//----------------------------------------------------------------------
	inline Real PrefabTorus::getOuterRadius() const
	{
		return m_fOuterRadius;
	}
//----------------------------------------------------------------------
	inline uint PrefabTorus::getRadialSlices() const
	{
		return m_nRadialSlices;
	}
//----------------------------------------------------------------------
	inline uint PrefabTorus::getCircleSlices() const
	{
		return m_nCircleSlices;
	}
//----------------------------------------------------------------------
}


#endif