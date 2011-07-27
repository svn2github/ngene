/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScenePartitionerOctTree.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCENEPARTITIONEROCTTREE_H_
#define __INC_SCENEPARTITIONEROCTTREE_H_


#include "Prerequisities.h"
#include "AABB.h"
#include "Frustum.h"
#include "ScenePartitioner.h"
#include "UtilitiesTypes.h"


namespace nGENE
{
	/** A single node of a OctTree.
	*/
	typedef struct SOctTreeNode
	{
		vector <ICullable*> vNodes;
		AABB NodeBound;

		class ScenePartitionerOctTree* pTree;

		SOctTreeNode* pParent;

		SOctTreeNode* pChildren[8];

		CULLING_RESULT cull;

		SOctTreeNode();

		void update(Frustum* _frustum, CULLING_RESULT _cull=CR_UNKNOWN);

		/** Updates hierarchy of the node.
			@remarks
				The necessity to fix node placement in the tree
				occurs when it is transformed (eg. translated).
		*/
		void updateHierarchy();

		/// Searches for a cullable in all child nodes.
		SOctTreeNode* find(ICullable* _node);

		/// Checks whether AABB fits in this node.
		bool fits(AABB& _aabb);

		/// Performs intersection test.
		void intersect(Ray& _ray, bool _perTriangle);

		/// Frees memory.
		void cleanup();

	private:
		void intersectChild(SOctTreeNode* _child, Ray& _ray, AABB& _box, bool _perTriangle);
	} OctTreeNode;


	/// Scene partitioner descriptor.
	typedef struct SOctTreeDesc
	{
		AABB bounds;							///< Boundaries of the tree
		float minimumNodeSize;					///< Minimal size of the node

		/// Checks if the description is valid.
		bool isValid()
		{
			return (minimumNodeSize > 0.0f &&
				bounds.getMinimum().x < bounds.getMaximum().x &&
				bounds.getMinimum().y < bounds.getMaximum().y &&
				bounds.getMinimum().z < bounds.getMaximum().z);
		}

		SOctTreeDesc():
			minimumNodeSize(20.0f)
		{
		}
	} OCT_TREE_DESC;


	/** ScenePartitioner for octtrees.
		@par
			Octtree is a basic and simple tree-structure which
			has been commonly used for 3D scenes optimizations.
		@par
			When a node is added to the octtree, it is traversed
			recursively (partitioned if necessary) to find relevant
			tree node to put scene-node. A scene-node is put in
			the smallest possible tree-node (such that whole scene-node
			can be put in it).
	*/
	class nGENEDLL ScenePartitionerOctTree: public ScenePartitioner
	{
		EXPOSE_TYPE
	private:
		OctTreeNode m_RootNode;					///< Root of the Quad Tree

		OCT_TREE_DESC m_Description;				///< Description of the tree

		vector <pair<ICullable*, float> > m_vIntersections;

	public:
		ScenePartitionerOctTree();
		ScenePartitionerOctTree(const OCT_TREE_DESC& _desc);
		virtual ~ScenePartitionerOctTree();

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



	inline void ScenePartitionerOctTree::setMinimumNodeSize(float _size)
	{
		m_Description.minimumNodeSize = _size;
	}
//----------------------------------------------------------------------
	inline float ScenePartitionerOctTree::getMinimumNodeSize() const
	{
		return m_Description.minimumNodeSize;
	}
//----------------------------------------------------------------------
}


#endif