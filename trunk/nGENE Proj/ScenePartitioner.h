/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScenePartitioner.h
Version:	0.08
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCENEPARTITIONER_H_
#define __INC_SCENEPARTITIONER_H_


#include "Prerequisities.h"
#include "Task.h"


namespace nGENE
{
	/** ScenePartitioner partitions scene to optimize common
		operations on it.
		@par
			ScenePartitioner could be for example quad- or oct-tree,
			Portals & Sectors, BSP etc.
		@par
			Before using ScenePartitioner you have to set frustum to
			be used by it as frustum is used in the culling	process
			extensively.
		@par
			It is part of SceneManager.
	*/
	class nGENEDLL ScenePartitioner: public Task
	{
		EXPOSE_TYPE
	protected:
		Frustum* m_pFrustum;				///< Pointer to the frustum object.

		uint m_nNodesCount;					///< Total number of ICullable nodes
		uint m_nNodesVisibleCount;			///< Number of visible nodes

	public:
		ScenePartitioner();
		virtual ~ScenePartitioner();

		/// Initializes partitioner.
		virtual void init()=0;

		/// Cleans up.
		virtual void cleanup()=0;

		/// Updates partitioning data.
		virtual void update()=0;
		/** Tests scene visibility.
			@par
				For the majority of cases visibility will be probably
				tested against some view frustum.
		*/
		virtual void testSceneVisibility()=0;

		/// Adds node to the partitioning data structure used.
		virtual void addNode(ICullable* _node)=0;
		/// Removes node from the partitioning data structure.
		virtual void removeNode(ICullable* _node)=0;

		/** Performs intersection test with the scene.
			@param
				_ray ray to be tested against.
			@param
				_perTriangle specifies in picking is tested at the
				triangles level. Specify false to test only AABB of
				the object.
		*/
		virtual void intersect(Ray& _ray, bool _perTriangle=false)=0;

		/** Returns closest intersection.
			@returns
				ICullable* returns pointer to the ICullable object
				if intersection test passed and NULL otherwise.
		*/
		virtual ICullable* getClosest()=0;

		void setFrustum(Frustum* _frustum);
		Frustum* getFrustum() const;

		/// Returns total number of ICullable nodes in this partitioner.
		uint getNodesCount() const;
		/// Returns number of visible ICullable nodes in this partitioner.
		uint getNodesVisibleCount() const;
		void increaseNodeVisibleCount();
	};



	inline void ScenePartitioner::setFrustum(Frustum* _frustum)
	{
		m_pFrustum = _frustum;
	}
//----------------------------------------------------------------------
	inline Frustum* ScenePartitioner::getFrustum() const
	{
		return m_pFrustum;
	}
//----------------------------------------------------------------------
	inline uint ScenePartitioner::getNodesCount() const
	{
		return m_nNodesCount;
	}
//----------------------------------------------------------------------
	inline uint ScenePartitioner::getNodesVisibleCount() const
	{
		return m_nNodesVisibleCount;
	}
//----------------------------------------------------------------------
	inline void ScenePartitioner::increaseNodeVisibleCount()
	{
		++m_nNodesVisibleCount;
	}
//----------------------------------------------------------------------
}


#endif