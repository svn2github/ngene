/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabCapsule.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABCAPSULE_H_
#define __INC_PREFABCAPSULE_H_


#include "NodePrefab.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Visualization of the capsule.
	*/
	class nGENEDLL PrefabCapsule: public NodePrefab
	{
		EXPOSE_TYPE
	private:
		float m_fHeight;				///< Length of segment between hemiCapsules
		float m_fRadius;				///< HemiCapsule radius

		uint m_nSlices;					///< Number of slices
		uint m_nStacks;					///< Number of stacks


		void updateGeometry();

	public:
		PrefabCapsule(uint _slices, uint _stacks, float _height, float _radius,
			const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		PrefabCapsule();
		~PrefabCapsule();

		void init();
		void cleanup();

		/// Sets capsule height.
		void setHeight(Real _height);
		/// Returns capsule height.
		Real getHeight() const;

		/// Sets new capsule radius.
		void setRadius(Real _radius);
		/// Returns capsule radius.
		Real getRadius() const;

		/// Sets number of slices.
		void setSlices(uint _slices);
		/// Returns number of slices.
		uint getSlices() const;

		/// Sets number of stacks.
		void setStacks(uint _stacks);
		/// Returns number of stacks.
		uint getStacks() const;


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline void PrefabCapsule::setHeight(Real _height)
	{
		m_fHeight = _height;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline Real PrefabCapsule::getHeight() const
	{
		return m_fHeight;
	}
//----------------------------------------------------------------------
	inline void PrefabCapsule::setRadius(Real _radius)
	{
		m_fRadius = _radius;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline Real PrefabCapsule::getRadius() const
	{
		return m_fRadius;
	}
//----------------------------------------------------------------------
	inline void PrefabCapsule::setSlices(uint _slices)
	{
		m_nSlices = _slices;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline uint PrefabCapsule::getSlices() const
	{
		return m_nSlices;
	}
//----------------------------------------------------------------------
	inline void PrefabCapsule::setStacks(uint _stacks)
	{
		m_nStacks = _stacks;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline uint PrefabCapsule::getStacks() const
	{
		return m_nStacks;
	}
//----------------------------------------------------------------------
}


#endif