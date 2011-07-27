/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		OBB.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_OBB_H_
#define __INC_OBB_H_


#include "BoundingVolume.h"
#include "Matrix4x4.h"
#include "Prerequisities.h"
#include "Vector3.h"


namespace nGENE
{
	/** OBB stands for Object Bounding Box.
		@remarks
			In contrary to AABB, OBB is oriented in the model
			space meaning it rotates with the model. Although
			for that reason it is not the best choice for
			frustum culling it is used to find proper AABB.
	*/
	class nGENEDLL OBB: public BoundingVolume
	{
		EXPOSE_TYPE
	private:
		Vector3 m_vecMinimum;							///< Minimum corner of the OBB
		Vector3 m_vecMaximum;							///< Maximum corner of the OBB
		Vector3 m_vecCentre;							///< Centre point of the OBB

		Matrix4x4 m_matTransform;						///< OBB transformation matrix

		void computeCentre();

	public:
		OBB();
		OBB(const Vector3& _min, const Vector3& _max);
		explicit OBB(VertexBuffer& _vb);
		virtual ~OBB();

		/// Creates OBB around given set of vertices.
		void construct(VertexBuffer& _vb);
		/// Creates OBB around given set of vertices.
		void construct(VertexBuffer* _vb);
		/// Creates OBB around given set of vertices.
		void construct(void* _vertices, uint _count, uint _size, uint _offset);

		const Vector3& getMinimum() const {return m_vecMinimum;}
		const Vector3& getMaximum() const {return m_vecMaximum;}
		const Vector3& getCentre() const {return m_vecCentre;}

		/// Sets parameters of OBB
		void setOBB(const Vector3& _min, const Vector3& _max);

		/// Applies transformation to the OBB.
		void setTransform(const Matrix4x4& _mat);
		Matrix4x4& getTransform();

	};
}


#endif