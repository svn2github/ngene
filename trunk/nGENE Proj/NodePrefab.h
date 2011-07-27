/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodePrefab.h
Version:	0.08
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEPREFAB_H_
#define __INC_NODEPREFAB_H_


#include "NodeVisible.h"


namespace nGENE
{
	/** Geometry prefabricate (prefab for short) object.
		@remarks
			Prefabs are simple geometry objects like sphere,
			box or cylinder provided to speed up prototypes
			or simple visualizations creation.
		@par
			Their vertices are fully described so that these
			objects could be used in any kind of 3D scene.
	*/
	class nGENEDLL NodePrefab: public NodeVisible
	{
	protected:
		Vector2 m_vecTexCoordMul;			///< Texture coordinates multiplier


		/** Updates prefab geometry.
			@remarks
				This method is used to update geometry when any of the prefab
				parameters changes its value. It is also used on prefab
				creation to initialize this geometry.
		*/
		virtual void updateGeometry()=0;
		/// Updates or creates single surface.
		Surface* updateSurface(const wstring& _name, VertexBuffer* _vb);

	public:
		NodePrefab(const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		virtual ~NodePrefab();

		/// Sets texture coordinate multiplier.
		void setTexCoordMult(const Vector2& _mult);
		/// Sets texture coordinate multiplier.
		void setTexCoordMult(Real _x, Real _y);
		/// Returns texture coordinate multiplier.
		Vector2& getTexCoordMult();

		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};



	inline void NodePrefab::setTexCoordMult(const Vector2& _mult)
	{
		m_vecTexCoordMul = _mult;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline void NodePrefab::setTexCoordMult(Real _x, Real _y)
	{
		m_vecTexCoordMul.set(_x, _y);
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline Vector2& NodePrefab::getTexCoordMult()
	{
		return m_vecTexCoordMul;
	}
//----------------------------------------------------------------------
}


#endif