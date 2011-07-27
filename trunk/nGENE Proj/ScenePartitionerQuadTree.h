/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScenePartitionerQuadTree.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCENEPARTITIONERQUADTREE_H_
#define __INC_SCENEPARTITIONERQUADTREE_H_


#include "Prerequisities.h"
#include "Frustum.h"
#include "ScenePartitioner.h"
#include "UtilitiesTypes.h"


namespace nGENE
{
	/** A single node of the QuadTree.
	*/
	typedef struct SQuadTreeNode
	{
		static const float NodeY;

		vector <ICullable*> vNodes;
		RECT_FLOAT NodeBound;

		class ScenePartitionerQuadTree* pTree;

		SQuadTreeNode* pParent;

		SQuadTreeNode* pTopLeft;
		SQuadTreeNode* pTopRight;
		SQuadTreeNode* pBottomLeft;
		SQuadTreeNode* pBottomRight;

		CULLING_RESULT cull;

		SQuadTreeNode();

		void update(Frustum* _frustum, CULLING_RESULT _cull=CR_UNKNOWN);

		/** Updates hierarchy of the node.
			@remarks
				The necessity to fix node placement in the tree
				occurs when it is transformed (eg. translated).
		*/
		void updateHierarchy();

		/// Searches for a cullable in all child nodes.
		SQuadTreeNode* find(ICullable* _node);

		/// Performs intersection test.
		void intersect(Ray& _ray, bool _perTriangle);

		/// Frees memory.
		void cleanup();

	private:
		void intersectChild(SQuadTreeNode* _child, Ray& _ray, AABB& _box, bool _perTriangle);
	} QuadTreeNode;


	/// Scene partitioner descriptor.
	typedef struct SQuadTreeDesc
	{
		RECT_FLOAT bounds;						///< Boundaries of the tree
		float minimumNodeSize;					///< Minimal size of the node

		/// Checks if the description is valid.
		bool isValid()
		{
			return (minimumNodeSize > 0.0f && bounds.left < bounds.right &&
				bounds.top < bounds.bottom);
		}

		SQuadTreeDesc():
			minimumNodeSize(20.0f)
		{
		}
	} QUAD_TREE_DESC;


	/** ScenePartitioner for quadtrees.
		@remarks
			Quadtree is a basic and simple tree-structure which
			has been commonly used for 2D scenes optimizations
			and was also found useful for optimizing rendering
			of 3D terrain.
		@par
			When a node is added to the quadtree, it is traversed
			recursively (partitioned if necessary) to find relevant
			tree node to put scene-node. A scene-node is put in
			the smallest possible tree-node (such that whole scene-node
			can be put in it).
	*/
	class nGENEDLL ScenePartitionerQuadTree: public ScenePartitioner
	{
		EXPOSE_TYPE
	private:
		QuadTreeNode m_RootNode;					///< Root of the Quad Tree

		QUAD_TREE_DESC m_Description;				///< Description of the tree

		vector <pair<ICullable*, float> > m_vIntersections;

	public:
		ScenePartitionerQuadTree();
		ScenePartitionerQuadTree(const QUAD_TREE_DESC& _desc);
		virtual ~ScenePartitionerQuadTree();

		/// Initializes partitioner.
		virtual void init();

		/// Cleans up.
		virtual void cleanup();

		/// Updates partitioner.
		void update();
		/// Tests visibility.
		void testSceneVisibility();

		/// Adds node to the partitioning data structure used.
		void addNode(ICullable* _node);
		/// Removes node from the partitioning data structure.
		void removeNode(ICullable* _node);

		/** Performs intersection test with the scene.
			@par
				These intersection test checks only if a ray intersects
				bounding volume of a node. It means that in case of meshes
				to get intersection point you have to carry additional test.
			@param
				_ray ray to be tested against.
		*/
		void intersect(Ray& _ray, bool _perTriangle=false);

		/** Returns closest intersection.
			@returns
				ICullable* returns pointer to the ICullable object
				if intersection test passed and NULL otherwise.
		*/
		ICullable* getClosest();

		void intersects(ICullable* _cullable, float _t);

		/// Sets minimum node size.
		void setMinimumNodeSize(float _size);
		float getMinimumNodeSize() const;
	};



	inline void ScenePartitionerQuadTree::setMinimumNodeSize(float _size)
	{
		m_Description.minimumNodeSize = _size;
	}
//----------------------------------------------------------------------
	inline float ScenePartitionerQuadTree::getMinimumNodeSize() const
	{
		return m_Description.minimumNodeSize;
	}
//----------------------------------------------------------------------
}


#endif