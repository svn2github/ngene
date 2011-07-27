/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrefabAxisRods.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREFABAXISRODS_H_
#define __INC_PREFABAXISRODS_H_


#include "Node.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Visualization of the AxisRods.
		@par
			Axis rods are 3 orthogonal to each other arrows
			pointing in the directions of 3D Cartesian coordinate
			space.
		@remarks
			PrefabAxisRods although has Prefab in its name is not
			of NodeVisible type but rather Node. Therefore, to
			assign materials to it you have to either call
			its setMaterial() method or get it three components,
			namely axisX (getAxisX()), axisY (getAxisY()) and axisZ
			(getAxisZ()), then obtain from them "Base" surface and
			assign material	to it.
	*/
	class nGENEDLL PrefabAxisRods: public Node
	{
		EXPOSE_TYPE
	private:
		PrefabArrow* m_pAxisX;
		PrefabArrow* m_pAxisY;
		PrefabArrow* m_pAxisZ;

		float m_fLength;
		float m_fEndLength;

	public:
		PrefabAxisRods(float _length, float _endLength);
		PrefabAxisRods();
		virtual ~PrefabAxisRods();

		void init();
		void cleanup();

		/// Sets length of the AxisRods (i.e. length of arrows).
		void setLength(Real _length);
		/// Returns length of the AxisRods.
		Real getLength() const;

		/// Sets length of AxisRods end.
		void setEndLength(Real _length);
		/// Returns length of AxisRods end.
		Real getEndLength() const;


		/// Returns x-axis arrow.
		PrefabArrow* getAxisX() const;
		/// Returns y-axis arrow.
		PrefabArrow* getAxisY() const;
		/// Returns z-axis arrow.
		PrefabArrow* getAxisZ() const;

		/// Sets material for all components.
		void setMaterial(Material* _mat);


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline PrefabArrow* PrefabAxisRods::getAxisX() const
	{
		return m_pAxisX;
	}
//----------------------------------------------------------------------
	inline PrefabArrow* PrefabAxisRods::getAxisY() const
	{
		return m_pAxisY;
	}
//----------------------------------------------------------------------
	inline PrefabArrow* PrefabAxisRods::getAxisZ() const
	{
		return m_pAxisZ;
	}
//----------------------------------------------------------------------
}


#endif