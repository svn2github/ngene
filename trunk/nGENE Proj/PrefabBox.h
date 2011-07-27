/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabBox.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABBOX_H_
#define __INC_PREFABBOX_H_


#include "AABB.h"
#include "NodePrefab.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Visualization of the box (not specifically a cube).
	*/
	class nGENEDLL PrefabBox: public NodePrefab
	{
		EXPOSE_TYPE
	private:
		AABB m_Box;				///< AABB is enough to visualize box


		void setPositions(SVertex* _vertices);
		void setNormals(SVertex* _vertices);
		void setTexCoords(SVertex* _vertices);


		void updateGeometry();

	public:
		PrefabBox(AABB& aabb, const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		PrefabBox();
		~PrefabBox();

		void init();
		void cleanup();


		/// Sets box AABB.
		void setAABB(const AABB& _aabb);
		/// Returns box AABB.
		AABB& getAABB();


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline void PrefabBox::setAABB(const AABB& _aabb)
	{
		m_Box = _aabb;
		updateGeometry();
	}
//----------------------------------------------------------------------
	inline AABB& PrefabBox::getAABB()
	{
		return m_Box;
	}
//----------------------------------------------------------------------
}


#endif