/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabSphere.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABSPHERE_H_
#define __INC_PREFABSPHERE_H_


#include "AABB.h"
#include "NodePrefab.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Visualization of the sphere.
		@par
			On creation you can specify the sphere level of detail
			through stacks and slices parameters. Besides if necessary
			maxPhi parameter can be used to limit sphere to eg.
			semi-sphere.
	*/
	class nGENEDLL PrefabSphere: public NodePrefab
	{
		EXPOSE_TYPE
	private:
		uint m_nSlices;					///< Number of slices
		uint m_nStacks;					///< Number of stacks

		Real m_fRadius;					///< Radius of a sphere
		Real m_fPhi;					///< Angle


		void updateGeometry();

	public:
		/** Prefab sphere constructor.
			@param _slices number of slices.
			@param _stacks number of stacks.
			@param _radius sphere radius.
			@param
				_texCoordMult value by which texture coordinates
				will be multiplied.
			@param
				_maxPhi maximum angle.
				
		*/
		PrefabSphere(uint _slices=10, uint _stacks=10, Real _radius=1.0f,
			const Vector2& _texCoordMul=Vector2::ONE_VECTOR,
			Real _maxPhi=Maths::PI);
		~PrefabSphere();

		void init();
		void cleanup();

		/// Sets new sphere radius.
		void setRadius(Real _radius);
		/// Returns sphere radius.
		Real getRadius() const;

		/// Sets number of slices.
		void setSlices(uint _slices);
		/// Returns number of slices.
		uint getSlices() const;

		/// Sets number of stacks.
		void setStacks(uint _stacks);
		/// Returns number of stacks.
		uint getStacks() const;

		/// Sets phi.
		void setPhi(Real _phi);
		/// Returns phi.
		Real getPhi() const;


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline void PrefabSphere::setRadius(Real _radius)
	{
		m_fRadius = _radius;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline Real PrefabSphere::getRadius() const
	{
		return m_fRadius;
	}
//----------------------------------------------------------------------
	inline void PrefabSphere::setSlices(uint _slices)
	{
		m_nSlices = _slices;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline uint PrefabSphere::getSlices() const
	{
		return m_nSlices;
	}
//----------------------------------------------------------------------
	inline void PrefabSphere::setStacks(uint _stacks)
	{
		m_nStacks = _stacks;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline uint PrefabSphere::getStacks() const
	{
		return m_nStacks;
	}
//----------------------------------------------------------------------
	inline void PrefabSphere::setPhi(Real _phi)
	{
		m_fPhi = _phi;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline Real PrefabSphere::getPhi() const
	{
		return m_fPhi;
	}
//----------------------------------------------------------------------
}


#endif