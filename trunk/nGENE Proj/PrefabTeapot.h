/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabTeapot.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABTEAPOT_H_
#define __INC_PREFABTEAPOT_H_


#include "NodePrefab.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Visualization of the teapot.
		@par
			It is classical CG teapot. Useful for testing effects
			and shaders.
	*/
	class nGENEDLL PrefabTeapot: public NodePrefab
	{
		EXPOSE_TYPE
	protected:
		void updateGeometry();

	public:
		PrefabTeapot(const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		~PrefabTeapot();

		void init();
		void cleanup();

		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};
}


#endif