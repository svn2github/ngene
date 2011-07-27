/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabCylinder.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABCYLINDER_H_
#define __INC_PREFABCYLINDER_H_


#include "NodePrefab.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Visualization of the cylinder.
		@par
			Cylinder can be closed or not. Closed cylinder
			has top and bottom caps wheres opened - not.
	*/
	class nGENEDLL PrefabCylinder: public NodePrefab
	{
		EXPOSE_TYPE
	private:
		float m_fRadius;			///< Cylinder radius
		float m_fHeight;			///< Cylinder height

		uint m_nSlices;				///< Number of slices
		uint m_nStacks;				///< Number of stacks

		bool m_bClosed;				///< Is cylinder closed?


		void updateGeometry();

	public:
		PrefabCylinder(uint _slices, uint _stacks, float _radius,
			float _height, bool closed=true,
			const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		PrefabCylinder();
		~PrefabCylinder();

		void init();
		void cleanup();

		/// Sets radius.
		void setRadius(Real _radius);
		/// Returns radius.
		Real getRadius() const;

		/// Sets height.
		void setHeight(Real _radius);
		/// Returns height.
		Real getHeight() const;

		/// Sets slices number.
		void setSlices(uint _slices);
		/// Returns number of slices.
		uint getSlices() const;

		/// Sets stacks number.
		void setStacks(uint _slices);
		/// Returns number of stacks.
		uint getStacks() const;

		/// Sets if cylinder should be closed or not.
		void setClosed(bool _value);
		/// Is cylinder closed?
		bool isClosed() const;


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline Real PrefabCylinder::getRadius() const
	{
		return m_fRadius;
	}
//----------------------------------------------------------------------
	inline Real PrefabCylinder::getHeight() const
	{
		return m_fHeight;
	}
//----------------------------------------------------------------------
	inline uint PrefabCylinder::getSlices() const
	{
		return m_nSlices;
	}
//----------------------------------------------------------------------
	inline uint PrefabCylinder::getStacks() const
	{
		return m_nStacks;
	}
//----------------------------------------------------------------------
	inline void PrefabCylinder::setClosed(bool _value)
	{
		m_bClosed = _value;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline bool PrefabCylinder::isClosed() const
	{
		return m_bClosed;
	}
//----------------------------------------------------------------------
}


#endif