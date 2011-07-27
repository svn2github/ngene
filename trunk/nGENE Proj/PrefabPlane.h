/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabPlane.h
Version:	0.08
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABPLANE_H_
#define __INC_PREFABPLANE_H_


#include "NodePrefab.h"
#include "Plane.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Visualization of the plane.
		@remarks
			Although the plane in geometry is infinite,
			in nGENE Tech it could be limited.
	*/
	class nGENEDLL PrefabPlane: public NodePrefab
	{
		EXPOSE_TYPE
	private:
		/// Plane object
		Plane m_Plane;

		/// Length of the single side
		Real m_fSideLength;


		void updateGeometry();

	public:
		/** Constructs the plane primitive.
			@param
				_plane reference to the plane object to
				be visualized. Whatever the plane is, vertex
				data is always the same. Rotated plane effect
				is achieved due to node's rotation.
		*/
		PrefabPlane(Plane& _plane, Real _sideLength=400.0f,
			const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		PrefabPlane();
		virtual ~PrefabPlane();

		void init();
		void cleanup();

		/// Sets length of the plane's side.
		void setSideLength(Real _length);
		/// Returns length of the plane's side.
		Real getSideLength() const;

		/// Sets plane description.
		void setPlane(const Plane& _plane);
		/// Returns plane description.
		Plane& getPlane();


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline void PrefabPlane::setSideLength(Real _length)
	{
		m_fSideLength = _length;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline Real PrefabPlane::getSideLength() const
	{
		return m_fSideLength;
	}
//----------------------------------------------------------------------
	inline void PrefabPlane::setPlane(const Plane& _plane)
	{
		m_Plane = _plane;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline Plane& PrefabPlane::getPlane()
	{
		return m_Plane;
	}
//----------------------------------------------------------------------
}


#endif